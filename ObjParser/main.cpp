#include "DisplayWindow.h"
#include "Parser.h"
#include "Object3D.h"
#include <iostream>


int main() {   

    std::string path = "E:\\BSUIR\\c++\\ParserPreviusVersion - Copy\\resources\\plain2.obj";

    Object3D object3D;
    Parser::parseObjFile(path, object3D);

    DisplayWindow window(1080, 720);
    window.render(object3D);

    return 0;
}