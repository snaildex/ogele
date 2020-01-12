#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	const string ScreenQuadUVSource = R"(
#version 400 core
layout(location = 0) in vec2 vUV;
layout(location = 1) in vec3 vNearPos;
out vec2 UV;
uniform dmat4 MVP;
void main(){gl_Position = vec4(MVP*vec4(vNearPos.xy,0,1));UV=vUV;}
)";
	const string DrawTexFragSource = R"(
#version 400 core
uniform sampler2D Tex;
in vec2 UV;
layout(location = 0) out vec4 Result;
void main(){Result=texture(Tex,UV);}
)";
	const string CoordBasisVertSource = R"(
#version 400 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vCol;
out vec3 Col;
uniform dmat4 MVP;
void main(){gl_Position = vec4(MVP*dvec4(vPos,1));Col=vCol;}
)";
	const string CoordBasisFragSource = R"(
#version 400 core
in vec3 Col;
layout(location = 0) out vec3 Result;
void main(){Result=Col;}
)";
	const string DrawLinesVertSource = R"(
#version 410 core
layout(location = 0) in dvec3 vPos;
layout(location = 1) in vec3 vCol;
out vec3 Col;
uniform dmat4 MVP;
void main(){gl_Position = vec4(MVP*dvec4(vPos,1));Col=vCol;}
)";
	const string DrawLinesFragSource = R"(
#version 410 core
in vec3 Col;
layout(location = 0) out vec4 Result;
void main(){Result=vec4(Col,1);}
)";


	void Application::CreateInternal() {
		m_drawTex.reset(CreateShaderProgram("ScreenDraw", ScreenQuadUVSource, DrawTexFragSource));
		m_drawBasis.reset(CreateShaderProgram("DrawBasis", CoordBasisVertSource, CoordBasisFragSource));
		m_drawLines.reset(CreateShaderProgram("DrawLines", DrawLinesVertSource, DrawLinesFragSource));
		m_screenQuad.reset(new ScreenQuadMesh());
		m_screenQuad->SetName("ScreenQuad");
		m_screenQuad->AddTag("Internal");
		m_coordBasis.reset(new CoordBasisMesh());
		m_coordBasis->SetName("CoordBasis");
		m_coordBasis->AddTag("Internal");
	}
}
