#include "DisplayWindow.h"
#include <chrono>
#include <iostream>


// Implement the constructor
DisplayWindow::DisplayWindow(int width, int height, const std::string& title, int framerateLimit) {
    this->width = width;
    this->height = height;
    window.create(sf::VideoMode(width, height), title);
    window.setFramerateLimit(framerateLimit);
    pixelBuffer = new sf::Uint8[width * height * PIXEL_SIZE_BYTES];
    zBuffer = new float[width * height];
}

DisplayWindow::~DisplayWindow()
{
    delete[] pixelBuffer;
    delete[] zBuffer;
}


void DisplayWindow::DrawDDALine(int x1, int y1, int x2, int y2, float z1, float z2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xIncrement = static_cast<float>(dx) / steps;
    float yIncrement = static_cast<float>(dy) / steps;

    float x = static_cast<float>(x1);
    float y = static_cast<float>(y1);
    float z = z1;

    for (int i = 0; i <= steps; i++) {
        int ix = static_cast<int>(x);
        int iy = static_cast<int>(y);

        if (ix >= 0 && ix < width && iy >= 0 && iy < height) {
            int index = (iy * width + ix) * PIXEL_SIZE_BYTES;

            if (z < zBuffer[iy * width + ix]) {
                pixelBuffer[index] = 48;       // R
                pixelBuffer[index + 1] = 255;  // G
                pixelBuffer[index + 2] = 55;   // B
                pixelBuffer[index + 3] = 255;  // A (Alpha)
                zBuffer[iy * width + ix] = z;
            }
        }

        x += xIncrement;
        y += yIncrement;
        z += (z2 - z1) / steps;
    }
}



void DisplayWindow::drawObjModel(Object3D& obj3D, float fElapsedTime)
{
    std::fill(pixelBuffer, pixelBuffer + width * height * PIXEL_SIZE_BYTES, 0);  // Initialize to black

    vec3d cameraPosition = { 0.f, 0.f, 100.0f, 1.0f };
    vec3d cameraTarget = { 0, 0.f, 0.f, 1.f };
    vec3d cameraUp = { 0.f, 1.f, 0.f, 1.f };

    // Create Matrices   
    mat4x4 matRotate = mat4x4::MatrixMakeRotation(obj3D.rotateX, obj3D.rotateY, obj3D.rotateZ);
    mat4x4 matScale = mat4x4::MatrixMakeScale(obj3D.scaleX, obj3D.scaleY, obj3D.scaleZ);
    mat4x4 matMove = mat4x4::MatrixMakeTranslation(obj3D.offsetX, obj3D.offsetY, obj3D.offsetZ);
    mat4x4 matProjection = mat4x4::MatrixMakeProjection(45, (float)width / (float)height, 0.1, 100);
    mat4x4 viewMatrix = mat4x4::MatrixPointAt(cameraPosition, cameraTarget, cameraUp);
    mat4x4 viewPort = mat4x4::MatrixMakeViewPort((float)width, (float)height);

    // Transpose Matrices
    matRotate = mat4x4::Transposition(matRotate);
    matMove = mat4x4::Transposition(matMove);
    matProjection = mat4x4::Transposition(matProjection);
    viewMatrix = mat4x4::Transposition(viewMatrix);
    viewPort = mat4x4::Transposition(viewPort);

    // Create a single transformation matrix
    mat4x4 projectionModel = matScale * matRotate * matMove * viewMatrix * matProjection;

    // Draw a triangle
    for (triangle& tri : obj3D.triangles) {
        triangle triProjected, triNormal;

        vec3d averageNormal;
        averageNormal.x = (tri.normal[0].x + tri.normal[1].x + tri.normal[2].x);
        averageNormal.y = (tri.normal[0].y + tri.normal[1].y + tri.normal[2].y);
        averageNormal.z = (tri.normal[0].z + tri.normal[1].z + tri.normal[2].z);

        averageNormal = mat4x4::MatrixMultiplyByVector(matRotate, averageNormal);
        if (mat4x4::Vector_DotProduct(cameraPosition, averageNormal) >= 0) {

            // Multiply the transformation matrix by each vertex of the triangle.
            triProjected.vertex[0] = mat4x4::MatrixMultiplyByVector(projectionModel, tri.vertex[0]);
            triProjected.vertex[1] = mat4x4::MatrixMultiplyByVector(projectionModel, tri.vertex[1]);
            triProjected.vertex[2] = mat4x4::MatrixMultiplyByVector(projectionModel, tri.vertex[2]);

            // Convert coordinates to viewport space.
            triProjected.vertex[0] = mat4x4::MatrixMultiplyByVector(viewPort, triProjected.vertex[0]);
            triProjected.vertex[1] = mat4x4::MatrixMultiplyByVector(viewPort, triProjected.vertex[1]);
            triProjected.vertex[2] = mat4x4::MatrixMultiplyByVector(viewPort, triProjected.vertex[2]);

            // Scanline
            int y0 = triProjected.vertex[0].y;
            int x0 = triProjected.vertex[0].x;
            float z0 = triProjected.vertex[0].z;

            int y1 = triProjected.vertex[1].y;
            int x1 = triProjected.vertex[1].x;
            float z1 = triProjected.vertex[1].z;

            int y2 = triProjected.vertex[2].y;
            int x2 = triProjected.vertex[2].x;
            float z2 = triProjected.vertex[2].z;


            int tmp;
            if (y0 > y1) {
                tmp = y0;
                y0 = y1;
                y1 = tmp;
                tmp = x0;
                x0 = x1;
                x1 = tmp;

                float temp = z0;
                z0 = z1;
                z1 = temp;
            }

            if (y0 > y2) {
                tmp = y0;
                y0 = y2;
                y2 = tmp;
                tmp = x0;
                x0 = x2;
                x2 = tmp;

                float temp = z0;
                z0 = z2;
                z2 = temp;
            }

            if (y1 > y2) {
                tmp = y1;
                y1 = y2;
                y2 = tmp;
                tmp = x1;
                x1 = x2;
                x2 = tmp;

                float temp = z1;
                z1 = z2;
                z2 = temp;
            }

            int cross_x1;
            int cross_x2;
            int dx1 = x1 - x0;
            int dy1 = y1 - y0;
            int dx2 = x2 - x0;
            int dy2 = y2 - y0;

            int top_y = y0;

            while (top_y < y1) {
                cross_x1 = x0 + dx1 * (top_y - y0) / dy1;
                cross_x2 = x0 + dx2 * (top_y - y0) / dy2;

                DrawDDALine(cross_x1, top_y, cross_x2, top_y, z0, z1);
                top_y++;
            }

            dx1 = x2 - x1;
            dy1 = y2 - y1;
            while (top_y < y2) {
                cross_x1 = x1 + dx1 * (top_y - y1) / dy1;
                cross_x2 = x0 + dx2 * (top_y - y0) / dy2;

                DrawDDALine(cross_x1, top_y, cross_x2, top_y, z1, z2);
                top_y++;
            }
        }
    }

    // Convert pixel buffer to Sprite
    sf::Texture texture;
    texture.create(width, height);
    texture.update(pixelBuffer);
    sf::Sprite sprite(texture);

    // Draw
    window.draw(sprite);
}

void DisplayWindow::resetZBuffer()
{
    for (int i = 0; i < width * height; i++) {
        zBuffer[i] = std::numeric_limits<float>::max();
    }
}

void DisplayWindow::fillTriangle(const Point& p1, const Point& p2, const Point& p3)
{

}

void DisplayWindow::DrawFilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
}

void DisplayWindow::drawDislayInfo()
{
    sf::Font font;
    if (!font.loadFromFile("../resources//SaarSPDemo.otf")) {
        return;
    }

    std::vector<sf::Text> texts(4);
    std::vector<sf::Vector2f> positions = { sf::Vector2f(10, 10), sf::Vector2f(10, 30), sf::Vector2f(10, 50) };

    for (int i = 0; i < 3; i++) {
        texts[i].setFont(font);
        texts[i].setCharacterSize(16);
        texts[i].setFillColor(sf::Color::White);
        texts[i].setPosition(positions[i]);
    }

    texts[0].setString("A, W, S, D - for moving");
    texts[1].setString("J, I, L, K, U, O - for rotating");
    texts[2].setString("plus, minus - for scaling");

    for (int i = 0; i < 3; i++) {
        window.draw(texts[i]);
    }
}

std::vector<int> DisplayWindow::findIntersections(const Point& p1, const Point& p2, int y)
{
    std::vector<int> intersections;
    if ((p1.y <= y && p2.y >= y) || (p1.y >= y && p2.y <= y)) {
        if (p1.y != p2.y) {
            int x = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
            intersections.push_back(x);
        }
    }
    return intersections;
}


void DisplayWindow::Render(Object3D& obj3D) {

    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = std::chrono::system_clock::now();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Equal) {
                    obj3D.offsetZ += OFFSET_Z;
                }
                if (event.key.code == sf::Keyboard::Hyphen) {
                    obj3D.offsetZ -= OFFSET_Z;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    obj3D.offsetX -= OFFSET_X;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    obj3D.offsetX += OFFSET_X;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    obj3D.offsetY += OFFSET_Y;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    obj3D.offsetY -= OFFSET_Y;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
                    obj3D.rotateX -= ROTATE_X;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
                    obj3D.rotateX += ROTATE_X;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
                    obj3D.rotateY += ROTATE_Y;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
                    obj3D.rotateY -= ROTATE_Y;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
                    obj3D.rotateZ += ROTATE_Z;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
                    obj3D.rotateZ -= ROTATE_Z;
                }
                if (event.type == sf::Event::Resized) {
                    sf::Vector2u newSize = window.getSize();
                    width = newSize.x; height = newSize.y;
                    sf::View view(sf::FloatRect(0, 0, newSize.x, newSize.y));
                    window.setView(view);
                }
            }
        }

        window.clear();
        tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();

        resetZBuffer();
        drawObjModel(obj3D, 0);
        drawDislayInfo();

        window.display();
    }
}

