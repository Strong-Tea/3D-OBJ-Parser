#include "DisplayWindow.h"
#include "Parser.h"
#include "Object3D.h"
#include <iostream>
#include <chrono>

int main()
{   
    std::string path = "../resources//hollowknight.obj";

    Object3D object3D;
    Parser::ParseObjFile(path, object3D);

    DisplayWindow window(1080, 720);
    window.Render(object3D);

    return 0;
}