#include <cstring>
#include <iostream>

#include "ArgParser.h"
#include "Renderer.h"

const double PI = 3.1415926;

int
main(int argc, const char *argv[])
{
    // Report help usage if no args specified.
    if (argc == 1) {
        std::cout << "Usage: a5 <args>\n"
            << "\n"
            << "Args:\n"
            << "\t-input <scene>\n"
            << "\t-size <width> <height>\n"
            << "\t-output <image.png>\n"
            << "\t[-depth <depth_min> <depth_max> <depth_image.png>\n]"
            << "\t[-normals <normals_image.png>]\n"
            << "\t[-bounces <max_bounces>\n]"
            << "\t[-shadows\n]"
            << "\n"
            ;
        return 1;
    }

    ArgParser argsParser(argc, argv);
    Renderer renderer(argsParser);
    // Render at different angles for zooming
    // for (int i = 0; i < 11; i++) {
    //     float percentAngle = 0.93 + i * 0.001;
    //     renderer.Render(percentAngle);
    // }

    // Render rotations
    for (int i = 0; i < 360; i++) {
        float rotateAngle = 2 * PI * i / 360; 
        renderer.Render(.90, rotateAngle);
    }

    // renderer.Render(.95);
    return 0;
}
