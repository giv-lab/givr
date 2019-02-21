#include "phong.h"

using phong = givr::style::PhongBase;
using prc = givr::style::PhongRenderContext;
using pirc = givr::style::PhongInstancedRenderContext;
using namespace givr::style;

void phong::setUniforms(std::unique_ptr<givr::Program> const &p) const {
    using namespace givr::style;
    p->setVec3("colour", value<Colour>());
    p->setVec3("lightPosition", value<LightPosition>());
    p->setFloat("ambientFactor", value<AmbientFactor>());
    p->setFloat("specularFactor",value<SpecularFactor>());
    p->setFloat("phongExponent", value<PhongExponent>());
    p->setBool("perVertexColour", value<PerVertexColour>());
    p->setBool("showWireFrame", value<ShowWireFrame>());
    p->setVec3("wireFrameColour", value<WireFrameColour>());
    p->setFloat("wireFramePercent", value<WireFramePercent>());
}

std::string phongVertexSource(std::string modelSource) {
    return
        "#version 330 core\n" + modelSource + std::string(R"shader( mat4 model;
        in vec3 position;
        in vec3 normal;
        in vec2 uvs;
        in vec3 colour;

        uniform mat4 view;
        uniform mat4 projection;

        out vec3 geomNormal;
        out vec3 geomOriginalPosition;
        out vec2 geomUv;
        out vec3 geomColour;

        void main(){
            mat4 mv = view * model;
            mat4 mvp = projection * mv;
            gl_Position = mvp * vec4(position, 1.0);
            geomOriginalPosition = vec3(model * vec4(position, 1.0));
            geomNormal = vec3(model*vec4(normal, 0));
            geomUv = uvs;
            geomColour = colour;
        }

        )shader"
    );
}
std::string phongGeometrySource() {
    return std::string(R"shader(
        #version 330 core
        layout (triangles) in;
        layout (triangle_strip, max_vertices = 3) out;

        uniform bool showWireFrame;
        uniform vec3 wireFrameColour;

        in vec3 geomNormal[];
        in vec3 geomOriginalPosition[];
        in vec2 geomUv[];
        in vec3 geomColour[];

        out vec3 fragNormal;
        out vec3 originalPosition;
        out vec2 fragUv;
        out vec3 fragColour;
        out vec3 fragBarycentricCoords;

        void main() {
            gl_Position = gl_in[0].gl_Position;
            fragNormal = geomNormal[0];
            originalPosition = geomOriginalPosition[0];
            fragUv = geomUv[0];
            fragColour = geomColour[0];
            fragBarycentricCoords = vec3(1.0, 0.0, 0.0);
            EmitVertex();

            gl_Position = gl_in[1].gl_Position;
            fragNormal = geomNormal[1];
            originalPosition = geomOriginalPosition[1];
            fragUv = geomUv[1];
            fragColour = geomColour[1];
            fragBarycentricCoords = vec3(0.0, 1.0, 0.0);
            EmitVertex();

            gl_Position = gl_in[2].gl_Position;
            fragNormal = geomNormal[2];
            originalPosition = geomOriginalPosition[2];
            fragUv = geomUv[2];
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
std::string phongFragmentSource() {
    return std::string(R"shader(#version 330 core
        #define M_PI 3.1415926535897932384626433832795

        float edgeFactor(vec3 vBC){
            vec3 d = fwidth(vBC);
            vec3 a3 = smoothstep(vec3(0.0), d*1.5, vBC);
            return min(min(a3.x, a3.y), a3.z);
        }

        uniform vec3 colour;
        uniform bool perVertexColour;
        uniform vec3 lightPosition;
        uniform float ambientFactor;
        uniform float specularFactor;
        uniform float phongExponent;
        uniform vec3 viewPosition;
        uniform bool showWireFrame;
        uniform vec3 wireFrameColour;
        uniform float wireFramePercent;

        in vec3 fragNormal;
        in vec3 originalPosition;
        in vec2 fragUv;
        in vec3 fragColour;
        in vec3 fragBarycentricCoords;

        out vec4 outColour;

        void main()
        {
            vec3 finalColour = colour;
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

            if(showWireFrame && any(lessThan(fragBarycentricCoords, vec3(wireFramePercent)))){
                outColour = vec4(mix(vec3(0.0), wireFrameColour, edgeFactor(fragBarycentricCoords)), 1.0);
            } else{
                outColour = vec4(ambient + diffuse + specular, 1.0);
            }
        }


        )shader"
    );
}

// TODO: these shaders are near duplicates of each other, we need to deal
//       with this.
std::string pirc::getVertexShaderSource() const {
    return phongVertexSource("in");
}

std::string pirc::getGeometryShaderSource() const {
    return phongGeometrySource();
}

std::string pirc::getFragmentShaderSource() const {
    return phongFragmentSource();
}

std::string prc::getVertexShaderSource() const {
    return phongVertexSource("uniform");
}

std::string prc::getGeometryShaderSource() const {
    return phongGeometrySource();
}

std::string prc::getFragmentShaderSource() const {
    return phongFragmentSource();
}
