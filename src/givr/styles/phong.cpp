#include "phong.h"
#include <iostream>

template<typename ColorSrc>
using prc = givr::style::T_PhongRenderContext<ColorSrc>;
template<typename ColorSrc>
using pirc = givr::style::T_PhongInstancedRenderContext<ColorSrc>;
using namespace givr::style;

std::string givr::style::phongVertexSource(std::string modelSource, bool usingTexture, bool hasNormals) {
    return
        "#version 330 core\n" +
        std::string(usingTexture ? "#define USING_TEXTURE\n" : "") +
        std::string(hasNormals ? "#define HAS_NORMALS\n" : "") +
        modelSource +
        std::string(R"shader( mat4 model;
        in vec3 position;
        #ifdef HAS_NORMALS
            in vec3 normal;
        #endif
        #ifdef USING_TEXTURE
            in vec2 uvs;
        #endif
        in vec3 colour;

        uniform mat4 view;
        uniform mat4 projection;

        #ifdef HAS_NORMALS
            out vec3 geomNormal;
        #endif
        out vec3 geomOriginalPosition;
        #ifdef USING_TEXTURE
            out vec2 geomUv;
        #endif
        out vec3 geomColour;

        void main(){
            mat4 mv = view * model;
            mat4 mvp = projection * mv;
            gl_Position = mvp * vec4(position, 1.0);
            geomOriginalPosition = vec3(model * vec4(position, 1.0));
            #ifdef HAS_NORMALS
                geomNormal = vec3(model*vec4(normal, 0));
            #endif
            geomColour = colour;
            #ifdef USING_TEXTURE
                geomUv = uvs;
            #endif
        }

        )shader"
    );
}
std::string givr::style::phongGeometrySource(bool usingTexture, bool hasNormals) {
    return
        "#version 330 core\n" +
        std::string(usingTexture ? "#define USING_TEXTURE\n" : "") +
        std::string(hasNormals ? "#define HAS_NORMALS\n" : "") +
        std::string(R"shader(
        layout (triangles) in;
        layout (triangle_strip, max_vertices = 3) out;

        uniform bool generateNormals;

        #ifdef HAS_NORMALS
            in vec3 geomNormal[];
        #endif
        in vec3 geomOriginalPosition[];
        #ifdef USING_TEXTURE
            in vec2 geomUv[];
        #endif
        in vec3 geomColour[];

        out vec3 fragNormal;
        out vec3 originalPosition;
        out vec2 fragUv;
        out vec3 fragColour;
        out vec3 fragBarycentricCoords;

        void main() {
            vec3 normal;
            if (generateNormals) {
                normal =
                    cross(
                        vec3(gl_in[1].gl_Position - gl_in[0].gl_Position),
                        vec3(gl_in[2].gl_Position - gl_in[0].gl_Position)
                    );
            }


            gl_Position = gl_in[0].gl_Position;
            if (!generateNormals) {
                #ifdef HAS_NORMALS
                    fragNormal = geomNormal[0];
                #endif
            } else {
                fragNormal = normal;
            }
            originalPosition = geomOriginalPosition[0];
            #ifdef USING_TEXTURE
                fragUv = geomUv[0];
            #endif
            fragColour = geomColour[0];
            fragBarycentricCoords = vec3(1.0, 0.0, 0.0);
            EmitVertex();

            gl_Position = gl_in[1].gl_Position;
            if (!generateNormals) {
                #ifdef HAS_NORMALS
                    fragNormal = geomNormal[1];
                #endif
            } else {
                fragNormal = normal;
            }
            originalPosition = geomOriginalPosition[1];
            #ifdef USING_TEXTURE
                fragUv = geomUv[1];
            #endif
            fragColour = geomColour[1];
            fragBarycentricCoords = vec3(0.0, 1.0, 0.0);
            EmitVertex();

            gl_Position = gl_in[2].gl_Position;
            if (!generateNormals) {
                #ifdef HAS_NORMALS
                    fragNormal = geomNormal[2];
                #endif
            } else {
                fragNormal = normal;
            }
            originalPosition = geomOriginalPosition[2];
            #ifdef USING_TEXTURE
                fragUv = geomUv[2];
            #endif
            fragColour = geomColour[2];
            fragBarycentricCoords = vec3(0.0, 0.0, 1.0);
            EmitVertex();

            EndPrimitive();
        }

        )shader"
        );
}

// Using wireframe technique from:
// http://codeflow.org/entries/2012/aug/02/easy-wireframe-display-with-barycentric-coordinates/
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
        uniform bool showWireFrame;
        uniform vec3 wireFrameColour;
        uniform float wireFrameWidth;


        float edgeFactor(vec3 vBC){
            vec3 d = fwidth(vBC);
            vec3 a3 = smoothstep(vec3(0.0), d*wireFrameWidth, vBC);
            return min(min(a3.x, a3.y), a3.z);
        }

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
        in vec3 fragBarycentricCoords;

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
            if (!gl_FrontFacing) normal = -normal;
            float diff = max(dot(lightDirection, normal), 0.0);
            vec3 diffuse = diff * finalColour;

            // specular
            vec3 viewDirection = normalize(viewPosition - originalPosition);
            vec3 reflectDirection = normalize(2.0*dot(lightDirection, normal)*normal - lightDirection);
            float normalization = (phongExponent+2.0)/(2.0*M_PI);
            float spec = normalization*diff*pow(max(dot(viewDirection, reflectDirection), 0.0), phongExponent);
            vec3 specular = vec3(specularFactor) * spec; // assuming bright white light colour

            vec3 shadedColour = ambient + diffuse + specular;
            if(showWireFrame) {
                shadedColour = mix(wireFrameColour, shadedColour, edgeFactor(fragBarycentricCoords));
            }
            outColour = vec4(shadedColour, 1.0);
        }


        )shader"
        );
}
