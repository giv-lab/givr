#include "lines.h"

using Colour = givr::style::Colour;

std::string givr::style::linesVertexSource(std::string modelSource) {
    std::cout << "modelSource: " << modelSource << std::endl;
    return "#version 330 core\n" + modelSource + std::string(R"shader( mat4 model;
        layout(location=4) in vec3 position;

        uniform mat4 view;
        uniform mat4 projection;

        void main(){
            mat4 mvp = projection * view * model;
            gl_Position = mvp * vec4(position, 1.0);
        }

        )shader"
    );
}

std::string givr::style::linesFragmentSource() {
    return std::string(R"shader(#version 330 core
        uniform vec3 colour;

        out vec4 outColour;

        void main()
        {
            outColour = vec4(colour, 1.);
        }


        )shader"
    );
}

