//
// Created by ??????? on 19.07.2018.
//
#include "../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

    const string ScreenQuadUVSource = R"(
#version 330 core
layout(location = 0) in vec2 vUV;
layout(location = 1) in vec3 vNearPos;
out vec2 UV;
void main(){gl_Position = vec4(vNearPos.xy,0,1);UV=vUV;}
)";
    const string DrawTexFragSource = R"(
#version 330 core
uniform sampler2D Tex;
in vec2 UV;
layout(location = 0) out vec3 Result;
void main(){Result=texture(Tex,UV).rgb;}
)";
    const string CoordBasisVertSource = R"(
#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vCol;
out vec3 Col;
uniform mat4 MVP;
void main(){gl_Position = MVP*vec4(vPos,1);Col=vCol;}
)";
    const string CoordBasisFragSource = R"(
#version 330 core
in vec3 Col;
layout(location = 0) out vec3 Result;
void main(){Result=Col;}
)";

    void Application::CreateInternal() {
        try {
            m_drawTex = CreateShaderProgram(ScreenQuadUVSource, DrawTexFragSource);
            m_drawTex->SetName("ScreenDraw");
            m_drawTex->AddTag("Internal");
            m_res->AddResource(m_drawTex);
            m_drawBasis = CreateShaderProgram(CoordBasisVertSource, CoordBasisFragSource);
            m_drawBasis->SetName("DrawBasis");
            m_drawBasis->AddTag("Internal");
            m_res->AddResource(m_drawBasis);
            m_screenQuad = new ScreenQuadMesh();
            m_screenQuad->SetName("ScreenQuad");
            m_screenQuad->AddTag("Internal");
            m_res->AddResource(m_screenQuad);
            m_coordBasis = new CoordBasisMesh();
            m_coordBasis->SetName("CoordBasis");
            m_coordBasis->AddTag("Internal");
            m_res->AddResource(m_coordBasis);
        }
        catch (const ShaderCompileException &ex) {
            cout << ex.what();
        }
    }

}
