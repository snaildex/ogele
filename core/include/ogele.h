#ifndef OGELE_LIBRARY_H
#define OGELE_LIBRARY_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <tchar.h>
#include <string>
#include <sstream>
#include <memory>
#include <type_traits>
#include <stdexcept>
#include <limits>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <list>
#include <map>
#include <experimental/filesystem>
#define _USE_MATH_DEFINES
#include <math.h>
#include <functional>
#include <chrono>

#define GLEW_STATIC
#define GLM_FORCE_CXX03

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/wrap.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <jzon/Jzon.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#ifdef DOUBLE_PRECISION
#define trscalar double
#define trvec3 dvec3
#define trvec4 dvec4
#define trquat dquat
#define trmat4 dmat4
#else
#define trscalar float
#define trvec3 vec3
#define trvec4 vec4
#define trquat quat
#define trmat4 mat4

#endif

#include "helpers/helpers.h"
#include "buffer/vectors.h"
#include "other/enums.h"
#include "application/window.h"
#include "application/application.h"
#include "other/glerror.h"
#include "object/object.h"
#include "object/resource.h"
#include "other/glstatic.h"
#include "buffer/vertexBuffer.h"
#include "buffer/vertexArray.h"
#include "shader/shaderCompileException.h"
#include "shader/shaderStage.h"
#include "shader/shaderProgram.h"
#include "shader/shaderLoader.h"
#include "query/query.h"
#include "query/gpuStopwatch.h"
#include "helpers/guiHelper.h"
#include "texture/textureBase.h"
#include "texture/textureLoaderHelpers.h"
#include "texture/texture2D/texture2D.h"
#include "texture/texture2D/texture2DLoader.h"
#include "texture/textureCube/textureCube.h"
#include "texture/textureCube/textureCubeLoader.h"
#include "texture/texture3D/texture3D.h"
#include "shader/material.h"
#include "framebuffer/framebuffer.h"
#include "framebuffer/renderTarget.h"
#include "helpers/math3D.h"
#include "components/transform/transform.h"
#include "components/camera/camera.h"
#include "mesh/meshBase.h"
#include "mesh/modelMesh.h"
#include "mesh/screenQuadMesh.h"
#include "mesh/coordBasisMesh.h"
#include "mesh/terrainMesh.h"
#include "components/terrain/terrain.h"
#include "components/clouds/clouds.h"
#include "pipelines/deferredPBRPipeline.h"

#endif