#include "DisplayWindow.h"
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>

#define EPSILON 1e-8
#define EPSILONRAY 1e-3

DisplayWindow::DisplayWindow(int width, int height, const std::string& title, int framerateLimit) {
    this->width = width;
    this->height = height;
    window.create(sf::VideoMode(width, height), title);
    window.setFramerateLimit(framerateLimit);
    pixelBuffer = new sf::Uint8[width * height * PIXEL_SIZE_BYTES];

    zBuffer = new float[width * height];

    ambient = 0.2f;
    shininess = 100.f;
    ambientIntensity = 0.2f;
    diffuseReflectanceCoefficient = 0.8f;
    specularReflectionCoefficient = 0.8f;

    modelColor = { 0, 0, 255, 1 };
    diffuseColor = { 255,  0,   0,      255 };
    specularColor = { 255, 255, 255, 255 };
    lightPosition = { 20.f, 0.f, 1.5f, 1.f };//{ 1.f, 0.f, -0.1f,  1.f };



    diffuseMap = readMap("../resources//textures//diffuse.png", diffuseMapWidth, diffuseMapHeight);
    normalMap = readMap("../resources//textures//normal.png", normalMapWidth, normalMapHeight);
    specularMap = readMap("../resources//textures//specular.png", specularMapWidth, specularMapHeight);
}

DisplayWindow::~DisplayWindow() {
    delete[] pixelBuffer;
    delete[] zBuffer;
}


void DisplayWindow::drawDDALine(int x1, int y1, int x2, int y2, float z1, float z2, float intensity, triangle& tri, mat4x4& invViewPort, triangle& tri1, mat4x4& matRotate, Object3D& obj3D, mat4x4& matproj) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    float dz = z2 - z1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xIncrement = static_cast<float>(dx) / steps;
    float yIncrement = static_cast<float>(dy) / steps;
    float zIncrement = dz / steps;

    float x = static_cast<float>(x1);
    float y = static_cast<float>(y1);
    float iz = z1;

    for (int i = 0; i <= steps; i++) {
        int ix = static_cast<int>(x);
        int iy = static_cast<int>(y);

        if (ix >= 0 && ix < width && iy >= 0 && iy < height) {
            int index = (iy * width + ix) * PIXEL_SIZE_BYTES;
            if (iz < zBuffer[iy * width + ix]) {
                vec3d point = { ix, iy, 0 };

                vec3d barycentric = vec3d::calculateBarycentricCoordinates(tri.vertex[0], tri.vertex[1], tri.vertex[2], point);
                vec3d interUV = (tri.texture[0] * barycentric.x / tri1.vertex[0].w +
                    tri.texture[1] * barycentric.y / tri1.vertex[1].w +
                    tri.texture[2] * barycentric.z / tri1.vertex[2].w) /
                    (barycentric.x / tri1.vertex[0].w + barycentric.y / tri1.vertex[1].w + barycentric.z / tri1.vertex[2].w);
                int textureX = static_cast<int>(interUV.x * (diffuseMapWidth - 1)) & (diffuseMapWidth - 1);
                int textureY = static_cast<int>((1 - interUV.y) * (diffuseMapHeight - 1)) & (diffuseMapHeight - 1);

                // Getting a diffuse color from a normal map
                vec3d textureDiffuseColor = { diffuseMap[textureY][textureX].r,
                    diffuseMap[textureY][textureX].g,
                    diffuseMap[textureY][textureX].b,
                    255 };

                // Getting a normal from a normal map
                int n0x = static_cast<int>(tri.texture[0].x * (normalMapWidth - 1)) & (normalMapWidth - 1);
                int n0y = static_cast<int>((1 - tri.texture[0].y) * (normalMapHeight - 1)) & (normalMapHeight - 1);
                int n1x = static_cast<int>(tri.texture[1].x * (normalMapWidth - 1)) & (normalMapWidth - 1);
                int n1y = static_cast<int>((1 - tri.texture[1].y) * (normalMapHeight - 1)) & (normalMapHeight - 1);
                int n2x = static_cast<int>(tri.texture[2].x * (normalMapWidth - 1)) & (normalMapWidth - 1);
                int n2y = static_cast<int>((1 - tri.texture[2].y) * (normalMapHeight - 1)) & (normalMapHeight - 1);

                vec3d Normal1 = { 2 * (normalMap[n0y][n0x].r) / 255.0f - 1,
                    2 * (normalMap[n0y][n0x].g) / 255.0f - 1,
                    2 * (normalMap[n0y][n0x].b) / 255.0f - 1 };
                vec3d Normal2 = { 2 * (normalMap[n1y][n1x].r) / 255.0f - 1,
                  2 * (normalMap[n1y][n1x].g) / 255.0f - 1,
                  2 * (normalMap[n1y][n1x].b) / 255.0f - 1 };
                vec3d Normal3 = { 2 * (normalMap[n2y][n2x].r) / 255.0f - 1,
                  2 * (normalMap[n2y][n2x].g) / 255.0f - 1,
                  2 * (normalMap[n2y][n2x].b) / 255.0f - 1 };


                vec3d textureNormal = (Normal1 * barycentric.x / tri.vertex[0].w +
                    Normal2 * barycentric.y / tri.vertex[1].w +
                    Normal3 * barycentric.z / tri.vertex[2].w) /
                    (barycentric.x / tri.vertex[0].w + barycentric.y / tri.vertex[1].w + barycentric.z / tri.vertex[0].w);

                textureNormal = vec3d::matrixMultiplyByVector(matRotate, textureNormal);

                //Getting a specular coefficient from a specular map
                vec3d textureSpecularColor = { specularMap[textureY][textureX].r,
                    specularMap[textureY][textureX].g,
                    specularMap[textureY][textureX].b };


                // For palin
                vec3d interNormals = (tri1.normal[0] * barycentric.x / tri1.vertex[0].w +
                    tri1.normal[1] * barycentric.y / tri1.vertex[1].w +
                    tri1.normal[2] * barycentric.z / tri1.vertex[2].w) /
                    (barycentric.x / tri1.vertex[0].w + barycentric.y / tri1.vertex[1].w + barycentric.z / tri1.vertex[2].w);


                vec3d interPoint = vec3d::interpolateVertexes(tri1.vertex[0], tri1.vertex[1], tri1.vertex[2], barycentric);

                vec3d phongShading = calculatePhongShading(interPoint, interNormals, lightPosition, camera.position, textureDiffuseColor, diffuseColor, textureSpecularColor, obj3D, matproj);

                pixelBuffer[index] = int(phongShading.x) > 255 ? 255 : int(phongShading.x);     // R
                pixelBuffer[index + 1] = int(phongShading.y) > 255 ? 255 : int(phongShading.y); // G
                pixelBuffer[index + 2] = int(phongShading.z) > 255 ? 255 : int(phongShading.z); // B
                pixelBuffer[index + 3] = 255;                                                   // A (Alpha)
                zBuffer[iy * width + ix] = iz;
            }
        }
        else break;

        x += xIncrement;
        y += yIncrement;
        iz += zIncrement;
    }
}

void displayMatrix(mat4x4 matrix) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << matrix.m[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void DisplayWindow::drawObjModel(Object3D& obj3D) {
    std::fill(pixelBuffer, pixelBuffer + width * height * PIXEL_SIZE_BYTES, 0);  // Initialize to black

    // Create Matrices   
    mat4x4 matRotate = mat4x4::matrixMakeRotation(obj3D.rotateX, obj3D.rotateY, obj3D.rotateZ);
    mat4x4 matScale = mat4x4::matrixMakeScale(obj3D.scaleX, obj3D.scaleY, obj3D.scaleZ);
    mat4x4 matMove = mat4x4::matrixMakeTranslation(obj3D.offsetX, obj3D.offsetY, obj3D.offsetZ);
    mat4x4 matProjection = mat4x4::matrixMakeProjection(camera.fov, (float)width / (float)height, camera.fNear, camera.fFar);
    mat4x4 viewMatrix = mat4x4::matrixPointAt(camera.position, camera.target, camera.up);
    mat4x4 viewPort = mat4x4::matrixMakeViewPort((float)width, (float)height);

    mat4x4 transpositionViewPort = mat4x4::transposition(viewPort);
    float det = mat4x4::determinant4x4(transpositionViewPort);
    mat4x4 invViewPort = mat4x4::inverse(transpositionViewPort, det);

    // Create a single transformation matrix
    mat4x4 projectionModel = matScale * matRotate * matMove * viewMatrix;

    // Draw a triangle
    for (triangle& tri : obj3D.triangles) {
        triangle triProjected;
        vec3d normal;
        vec3d eyeVector = camera.position - tri.vertex[0];
        vec3d vec1 = tri.vertex[1] - tri.vertex[0];
        vec3d vec2 = tri.vertex[2] - tri.vertex[0];

        normal = vec3d::vectorCrossProduct(vec1, vec2);
        normal = vec3d::vectorNormalise(normal);
        normal = vec3d::matrixMultiplyByVector(matRotate, normal);

        if (vec3d::vectorDotProduct(eyeVector, normal) >= 0) {
            // Multiply the transformation matrix by each vertex of the triangle.
            triProjected.texture[0] = tri.texture[0];
            triProjected.texture[1] = tri.texture[1];
            triProjected.texture[2] = tri.texture[2];
            triProjected.vertex[0] = vec3d::matrixMultiplyByVector(projectionModel, tri.vertex[0]);
            triProjected.vertex[1] = vec3d::matrixMultiplyByVector(projectionModel, tri.vertex[1]);
            triProjected.vertex[2] = vec3d::matrixMultiplyByVector(projectionModel, tri.vertex[2]);


            // Multiply the transformation matrix by each vertex of the triangle.
            triProjected.normal[0] = vec3d::matrixMultiplyByVector(matRotate, tri.normal[0]);
            triProjected.normal[1] = vec3d::matrixMultiplyByVector(matRotate, tri.normal[1]);
            triProjected.normal[2] = vec3d::matrixMultiplyByVector(matRotate, tri.normal[2]);

            triangle tricopy1 = triProjected;
            triProjected.vertex[0] = vec3d::matrixMultiplyByVector(matProjection, triProjected.vertex[0]);
            triProjected.vertex[1] = vec3d::matrixMultiplyByVector(matProjection, triProjected.vertex[1]);
            triProjected.vertex[2] = vec3d::matrixMultiplyByVector(matProjection, triProjected.vertex[2]);


            triangle tricopy;

            // Convert coordinates to viewport space.
            triProjected.vertex[0] = vec3d::matrixMultiplyByVector(viewPort, triProjected.vertex[0]);
            triProjected.vertex[1] = vec3d::matrixMultiplyByVector(viewPort, triProjected.vertex[1]);
            triProjected.vertex[2] = vec3d::matrixMultiplyByVector(viewPort, triProjected.vertex[2]);

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
            float cross_z1;
            float cross_z2;

            int dx1 = x1 - x0;
            int dy1 = y1 - y0;
            int dx2 = x2 - x0;
            int dy2 = y2 - y0;
            float dz1 = z1 - z0;
            float dz2 = z2 - z0;

            int top_y = y0;

            // Lambert
            vec3d lightDir = lightPosition - tri.vertex[0];
            float intensity = calculateLambertLighting(normal, lightDir, ambient);

            while (top_y < y1) {
                cross_x1 = x0 + dx1 * (top_y - y0) / dy1;
                cross_x2 = x0 + dx2 * (top_y - y0) / dy2;
                cross_z1 = z1 + dz1 * (top_y - y1) / dy1;
                cross_z2 = z0 + dz2 * (top_y - y0) / dy2;
                drawDDALine(cross_x1, top_y, cross_x2, top_y, cross_z1, cross_z2, intensity, triProjected, invViewPort, tricopy1, matRotate, obj3D, projectionModel);
                top_y++;
            }

            dx1 = x2 - x1;
            dy1 = y2 - y1;
            dz1 = z2 - z1;
            while (top_y < y2) {
                cross_x1 = x1 + dx1 * (top_y - y1) / dy1;
                cross_x2 = x0 + dx2 * (top_y - y0) / dy2;
                cross_z1 = z1 + dz1 * (top_y - y1) / dy1;
                cross_z2 = z0 + dz2 * (top_y - y0) / dy2;
                drawDDALine(cross_x1, top_y, cross_x2, top_y, cross_z1, cross_z2, intensity, triProjected, invViewPort, tricopy1, matRotate, obj3D, projectionModel);
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

vec3d DisplayWindow::calculatePhongShading(vec3d& point, vec3d& interpolatedNormal, vec3d& lightPosition, vec3d& cameraPosition, vec3d& modelColor, vec3d& diffuseColor, vec3d& specularColor, Object3D& obj, mat4x4& matproj) {
    vec3d lightDirection = lightPosition - point;
    vec3d cameraDirection = point - cameraPosition;
    vec3d L = vec3d::vectorNormalise(lightDirection);
    vec3d N = vec3d::vectorNormalise(interpolatedNormal);
    vec3d V = vec3d::vectorNormalise(cameraDirection);

    vec3d ambient = modelColor * ambientIntensity;

    // Check if the point is in shadow
    bool isInShadow = isPointInShadow(point, lightPosition, obj, matproj);

    if (isInShadow) {
        return ambient; // Point is in shadow, only ambient light is considered
    }

    float cosineAngle = std::max(0.0f, vec3d::vectorDotProduct(N, L));
    vec3d diffuse = modelColor * cosineAngle * diffuseReflectanceCoefficient;

    vec3d R = L - (N * vec3d::vectorDotProduct(L, N) * 2);
    R = vec3d::vectorNormalise(R);

    float intenseShine = std::pow(std::max(0.0f, vec3d::vectorDotProduct(R, V)), shininess);
    vec3d specular = specularColor * specularReflectionCoefficient * intenseShine;

    return ambient + diffuse + specular;
}

float DisplayWindow::calculateLambertLighting(vec3d& normal, vec3d& lightDirection, float ambient) {
    normal = vec3d::vectorNormalise(normal);
    lightDirection = vec3d::vectorNormalise(lightDirection);
    float dotProduct = vec3d::vectorDotProduct(normal, lightDirection);
    float lambertian = std::max(0.0f, dotProduct);
    float intensity = ambient + (1.0f - ambient) * lambertian;
    return intensity;
}


void DisplayWindow::resetZBuffer() {
    for (int i = 0; i < width * height; i++) {
        zBuffer[i] = std::numeric_limits<float>::max();
    }
}



void DisplayWindow::drawDislayInfo() {
    sf::Font font;
    if (!font.loadFromFile("../resources//caviar_dreams.ttf")) {
        return;
    }

    std::vector<sf::Text> texts(4);
    std::vector<sf::Vector2f> positions = { sf::Vector2f(10, 10), sf::Vector2f(10, 30), sf::Vector2f(10, 50),sf::Vector2f(10, 70) };

    for (int i = 0; i < 4; i++) {
        texts[i].setFont(font);
        texts[i].setCharacterSize(16);
        texts[i].setFillColor(sf::Color::White);
        texts[i].setPosition(positions[i]);
    }
    
    texts[0].setString("A, W, S, D - for moving");
    texts[1].setString("J, I, L, K, U, O - for rotating");
    texts[2].setString("plus, minus - for scaling");
   

    for (int i = 0; i < 4; i++) {
        window.draw(texts[i]);
    }
}

std::vector<std::vector<sf::Color>> DisplayWindow::readMap(const std::string& filepath, unsigned int& width, unsigned int& height) {
    sf::Image image;

    if (!image.loadFromFile(filepath)) {
        std::cerr << "Error loading image: " << filepath << std::endl;
        return {};
    }

    width = image.getSize().x;
    height = image.getSize().y;

    std::vector<std::vector<sf::Color>> diffuseMap(height, std::vector<sf::Color>(width));

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            diffuseMap[y][x] = image.getPixel(x, y);
        }
    }

    return diffuseMap;
}



bool DisplayWindow::isPointInShadow(const vec3d& point, const vec3d& lightPosition, Object3D& obj, mat4x4& matproj) {
    vec3d RayDir = lightPosition - point;
    RayDir = vec3d::vectorNormalise(RayDir);
    vec3d shiftedPoint = point + RayDir * EPSILONRAY; //fix self-shadowing
    for (triangle& tri : obj.triangles) {
        triangle triProjected;
        triProjected.vertex[0] = vec3d::matrixMultiplyByVector(matproj, tri.vertex[0]);
        triProjected.vertex[1] = vec3d::matrixMultiplyByVector(matproj, tri.vertex[1]);
        triProjected.vertex[2] = vec3d::matrixMultiplyByVector(matproj, tri.vertex[2]);
        // Check if the point is behind the triangle
        vec3d edge1 = triProjected.vertex[1] - triProjected.vertex[0];
        vec3d edge2 = triProjected.vertex[2] - triProjected.vertex[0];
        vec3d normal = vec3d::vectorCrossProduct(edge1, edge2);
        normal = vec3d::vectorNormalise(normal);

        if (vec3d::vectorDotProduct(RayDir, normal) > 0) {
            // Point is behind the triangle, skip
            continue;
        }

        vec3d pvec = vec3d::vectorCrossProduct(RayDir, edge2);
        float det = vec3d::vectorDotProduct(edge1, pvec);

        if (det < EPSILON && det > -EPSILON) {
            continue;
        }

        float inv_det = 1 / det;
        vec3d tvec = shiftedPoint - triProjected.vertex[0];
        float u = vec3d::vectorDotProduct(tvec, pvec) * inv_det;
        if (u < 0.f || u > 1.f) {
            continue;
        }

        vec3d qvec = vec3d::vectorCrossProduct(tvec, edge1);
        float v = vec3d::vectorDotProduct(RayDir, qvec) * inv_det;
        if (v < 0.f || u + v > 1.f) {
            continue;
        }
        float t = vec3d::vectorDotProduct(edge2, qvec) * inv_det;
        if (t > EPSILON)
            return true;
    }
    return false;
}



void DisplayWindow::render(Object3D& obj3D) {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Equal) {
                    obj3D.offsetZ += SPEED_MOVE * elapsed.asSeconds();
                }
                if (event.key.code == sf::Keyboard::Hyphen) {
                    obj3D.offsetZ -= SPEED_MOVE * elapsed.asSeconds();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    obj3D.offsetX -= SPEED_MOVE * elapsed.asSeconds();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    obj3D.offsetX += SPEED_MOVE * elapsed.asSeconds();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    obj3D.offsetY += SPEED_MOVE * elapsed.asSeconds();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    obj3D.offsetY -= SPEED_MOVE * elapsed.asSeconds();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
                    obj3D.rotateX -= SPEED_ROTATE * elapsed.asSeconds();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
                    obj3D.rotateX += SPEED_ROTATE * elapsed.asSeconds();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
                    obj3D.rotateY += SPEED_ROTATE * elapsed.asSeconds();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
                    obj3D.rotateY -= SPEED_ROTATE * elapsed.asSeconds();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
                    obj3D.rotateZ += SPEED_ROTATE * elapsed.asSeconds();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
                    obj3D.rotateZ -= SPEED_ROTATE * elapsed.asSeconds();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    camera.position.y += SPEED_MOVE * elapsed.asSeconds();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    camera.position.y -= SPEED_MOVE * elapsed.asSeconds();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    camera.position.x += SPEED_MOVE * elapsed.asSeconds();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    camera.position.x -= SPEED_MOVE * elapsed.asSeconds();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
                    camera.position.z += SPEED_MOVE * elapsed.asSeconds();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
                    camera.position.z -= SPEED_MOVE * elapsed.asSeconds();
                }

            }

            if (event.type == sf::Event::Resized) {
                sf::Vector2u newSize = window.getSize();
                width = newSize.x; height = newSize.y;
                sf::View view(sf::FloatRect(0, 0, newSize.x, newSize.y));
                window.setView(view);
                delete[] zBuffer;
                delete[] pixelBuffer;
                zBuffer = new float[height * width];
                pixelBuffer = new sf::Uint8[width * height * PIXEL_SIZE_BYTES];
            }
        }

        window.clear();

        resetZBuffer();
        drawObjModel(obj3D);
        drawDislayInfo();

        window.display();
    }
}

