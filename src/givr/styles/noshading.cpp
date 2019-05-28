#include "noshading.h"

std::string givr::style::noShadingVertexSource(std::string modelSource) {
    return "#version 330 core\n" + modelSource + std::string(R"shader( mat4 model;
        layout(location=4) in vec3 position;

        uniform mat4 view;
        uniform mat4 projection;
        uniform vec3 colour;

        void main()
        {
            mat4 mvp = projection * view * model;
            gl_Position = mvp * vec4(position, 1.0);
        }
        )shader"
    );
}

std::string givr::style::noShadingFragmentSource() {
    return std::string(R"shader(#version 330 core
        uniform vec3 colour;

        out vec4 outColour;

        void main()
        {
            outColour = vec4(colour, 1.0);
        }

        )shader"
    );
}
