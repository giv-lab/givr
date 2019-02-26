#include "phong.h"

template<typename ColorSrc>
using prc = givr::style::T_PhongRenderContext<ColorSrc>;
template<typename ColorSrc>
using pirc = givr::style::T_PhongInstancedRenderContext<ColorSrc>;
using namespace givr::style;

std::string givr::style::phongVertexSource(std::string modelSource, bool usingTexture) {
    return
        "#version 330 core\n" +
        std::string(usingTexture ? "#define USING_TEXTURE\n" : "") +
        modelSource +
        std::string(R"shader( mat4 model;
        in vec3 position;
        in vec3 normal;
        #ifdef USING_TEXTURE
            in vec2 uvs;
            out vec2 fragUv;
        #endif
        in vec3 colour;

        uniform mat4 view;
        uniform mat4 projection;

        out vec3 fragNormal;
        out vec3 originalPosition;
        out vec3 fragColour;

        void main(){
            mat4 mv = view * model;
            mat4 mvp = projection * mv;
            gl_Position = mvp * vec4(position, 1.0);
            originalPosition = vec3(model * vec4(position, 1.0));
            fragNormal = vec3(model*vec4(normal, 0));
            fragColour = colour;
            #ifdef USING_TEXTURE
                fragUv = uvs;
            #endif
        }

        )shader"
        );
}

std::string givr::style::phongFragmentSource(bool usingTexture) {
    return
        "#version 330 core\n" +
        std::string(usingTexture ? "#define USING_TEXTURE\n" : "") +
        std::string(R"shader(
        #define M_PI 3.1415926535897932384626433832795

        uniform vec3 colour;
        uniform bool perVertexColour;
        uniform vec3 lightPosition;
        uniform float ambientFactor;
        uniform float specularFactor;
        uniform float phongExponent;
        uniform vec3 viewPosition;
        #ifdef USING_TEXTURE
            uniform sampler2D colorTexture;
            in vec2 fragUv;
    
            vec3 getColor(){ return texture(colorTexture, fragUv).rgb; }
        #else
            vec3 getColor(){ return colour; }
        #endif

        in vec3 fragNormal;
        in vec3 originalPosition;
        in vec3 fragColour;

        out vec4 outColour;

        void main()
        {
            vec3 finalColour = getColor();
            if (perVertexColour) {
                finalColour = fragColour;
            }
            // ambient
            vec3 ambient = ambientFactor * finalColour;

            // diffuse
            vec3 lightDirection = normalize(lightPosition - originalPosition);
            vec3 normal = normalize(fragNormal);
            float diff = max(dot(lightDirection, normal), 0.0);
            vec3 diffuse = diff * finalColour;

            // specular
            vec3 viewDirection = normalize(viewPosition - originalPosition);
            vec3 reflectDirection = normalize(2.0*dot(lightDirection, normal)*normal - lightDirection);
            float normalization = (phongExponent+2.0)/(2.0*M_PI);
            float spec = normalization*diff*pow(max(dot(viewDirection, reflectDirection), 0.0), phongExponent);
            vec3 specular = vec3(specularFactor) * spec; // assuming bright white light colour

            outColour = vec4(ambient + diffuse + specular, 1.0);
        }


        )shader"
        );
}
/*
// TODO: these shaders are near duplicates of each other, we need to deal
//       with this.
std::string pirc::getVertexShaderSource(bool usingTexture) const {
return texturedPhongVertexSource("in");
}

std::string pirc::getFragmentShaderSource(bool usingTexture) const {
return texturedPhongFragmentSource();
}

std::string prc::getVertexShaderSource(bool usingTexture) const {
return texturedPhongVertexSource("uniform");
}

std::string prc::getFragmentShaderSource(bool usingTexture) const {
return texturedPhongFragmentSource();
}
*/
