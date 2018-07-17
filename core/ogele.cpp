#include "ogele.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

namespace ogele {
//region Helpers

    list<fs::path> ScanFiles(const string &extension) {
        list<fs::path> files;
        for (const auto &p : fs::recursive_directory_iterator(fs::current_path())) {
            const string &ext = p.path().extension().string();
            if (ext == extension)
                files.push_back(p.path());
        }
        return files;
    }
//endregion

//region Application

    Application *Application::m_instance;

    Application::Application() : Window(glm::ivec2(800, 600), "Shader runner", true) {
        m_instance = this;
        MakeContextCurrent();
        m_res = make_unique<ResourceContainer>();
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        m_screenSize.x = mode->width;
        m_screenSize.y = mode->height;
        m_lastFrameTimePoint = chrono::high_resolution_clock::now();
        CreateInternal();

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(m_handle, true);
    }

    void Application::DrawTex(TextureBase *tex) {
        m_drawTex->Bind();
        m_drawTex->SetTexture("Tex", tex);
        m_screenQuad->Draw();
        m_drawTex->Unbind();
    }

    void Application::Run() {
        m_res->LoadFromFilesystem();
        Start();
        while (!ShouldClose()) {
            ClearColor(glm::vec4(0.6f, 0.8f, 1.f, 0.f));
            Clear(BufferBit::Color | BufferBit::Depth);

            auto ctime = chrono::high_resolution_clock::now();
            m_timeDelta = chrono::duration_cast<chrono::duration<double>>(ctime - m_lastFrameTimePoint).count();
            m_time = chrono::duration_cast<chrono::duration<double>>(ctime.time_since_epoch()).count();
            m_lastFrameTimePoint = ctime;

            ImGui_ImplGlfwGL3_NewFrame();
            Update();
            ImGui::Render();
            SwapBuffers();
            PollEvents();
        }
        Shutdown();
        m_res.reset();
    }

    Application::~Application() {
        ImGui_ImplGlfwGL3_Shutdown();
        glfwTerminate();
    }
//endregion

//region InternalResources

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

    void Application::CreateInternal() {
        try {
            m_drawTex = CreateShaderProgram(ScreenQuadUVSource, DrawTexFragSource);
            m_drawTex->SetName("ScreenDraw");
            m_drawTex->AddTag("Internal");
            m_res->AddResource(m_drawTex);
            m_screenQuad = new ScreenQuadMesh();
            m_screenQuad->SetName("ScreenQuad");
            m_screenQuad->AddTag("Internal");
            m_res->AddResource(m_screenQuad);
        }
        catch (const ShaderCompileException &ex) {
            cout << ex.what();
        }
    }
//endregion

//region VertexBuffer

    BufferBase::BufferBase(BufferDataType dataType, size_t structSize, int size) {
        switch (dataType) {
            case BufferDataType::Byte:
                m_elemSize = sizeof(signed char);
                break;
            case BufferDataType::Short:
                m_elemSize = sizeof(signed short);
                break;
            case BufferDataType::Int:
                m_elemSize = sizeof(signed int);
                break;
            case BufferDataType::UnsignedByte:
                m_elemSize = sizeof(unsigned char);
                break;
            case BufferDataType::UnsignedShort:
                m_elemSize = sizeof(unsigned short);
                break;
            case BufferDataType::UnsignedInt:
                m_elemSize = sizeof(unsigned int);
                break;
            case BufferDataType::Float:
                m_elemSize = sizeof(float);
                break;
            case BufferDataType::Double:
                m_elemSize = sizeof(double);
                break;
            default:
                throw runtime_error("Unsupported vertex buffer data type");
        }
        m_size = size;
        m_dataType = dataType;
        m_structSize = structSize;
        m_handle = 0;
        glGenBuffers(1, &m_handle);
        GLErr();
    }

    BufferBase::~BufferBase() {
        glDeleteBuffers(1, &m_handle);
        GLErr();
    }
//endregion

//region GLError

    GLError::GLError(GLenum code) noexcept : std::runtime_error("OpenGL runtime error") {
        m_code = code;
    }

    const char *GLError::what() const noexcept {
        switch (m_code) {
            case GL_INVALID_OPERATION:
                return "Invalid operation";
            case GL_INVALID_ENUM:
                return "Invalid enum";
            case GL_INVALID_VALUE:
                return "Invalid value";
            case GL_OUT_OF_MEMORY:
                return "Out of memory";
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                return "Invalid frame buffer operation";
            case GL_STACK_UNDERFLOW:
                return "Stack underflow";
            case GL_STACK_OVERFLOW:
                return "Stack overflow";
            default:
                return "Code " + m_code;
        }
    }

    const wchar_t *GLError::whatW() const {
        switch (m_code) {
            case GL_INVALID_OPERATION:
                return L"Invalid operation";
            case GL_INVALID_ENUM:
                return L"Invalid enum";
            case GL_INVALID_VALUE:
                return L"Invalid value";
            case GL_OUT_OF_MEMORY:
                return L"Out of memory";
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                return L"Invalid frame buffer operation";
            case GL_STACK_UNDERFLOW:
                return L"Stack underflow";
            case GL_STACK_OVERFLOW:
                return L"Stack overflow";
            default:
                return L"Code " + m_code;
        }
    }

    void CheckGLError() {
        auto err = glGetError();
        if (err == GL_NO_ERROR) return;
        throw GLError(err);
    }

//endregion
//region GLStatic
    void ClearColor(const glm::vec4 &color) {
        glClearColor(color.r, color.g, color.b, color.a);
        GLErr();
    }

    void Clear(BufferBit buffers) {
        glClear(static_cast<GLenum>(buffers));
        GLErr();
    }

    void Viewport(const glm::ivec2 &offset, const glm::ivec2 &size) {
        glViewport(offset.x, offset.y, size.x, size.y);
        GLErr();
    }

    void Enable(Feature feature) {
        glEnable(static_cast<GLenum>(feature));
        GLErr();
    }

    void Disable(Feature feature) {
        glDisable(static_cast<GLenum>(feature));
        GLErr();
    }

    void Barrier(MemoryBarriers barriers) {
        glMemoryBarrier(static_cast<GLenum>(barriers));
        GLErr();
    }

//endregion
//region IsSamplerIsImage
    bool ShaderProgram::IsSampler(UniformType type) {
        return (
                type == UniformType::Sampler1D ||
                type == UniformType::Sampler1DArray ||
                type == UniformType::Sampler1DArrayShadow ||
                type == UniformType::Sampler1DShadow ||
                type == UniformType::Sampler2D ||
                type == UniformType::Sampler2DArray ||
                type == UniformType::Sampler2DArrayShadow ||
                type == UniformType::Sampler2DMultisample ||
                type == UniformType::Sampler2DMultisampleArray ||
                type == UniformType::Sampler2DRect ||
                type == UniformType::Sampler2DRectShadow ||
                type == UniformType::Sampler2DShadow ||
                type == UniformType::Sampler3D ||
                type == UniformType::SamplerCube ||
                type == UniformType::SamplerCubeArray ||
                type == UniformType::SamplerCubeShadow ||
                type == UniformType::SamplerCubeArrayShadow ||
                type == UniformType::SamplerBuffer ||

                type == UniformType::IntSampler1D ||
                type == UniformType::IntSampler1DArray ||
                type == UniformType::IntSampler2D ||
                type == UniformType::IntSampler2DArray ||
                type == UniformType::IntSampler2DMultisample ||
                type == UniformType::IntSampler2DMultisampleArray ||
                type == UniformType::IntSampler2DRect ||
                type == UniformType::IntSampler3D ||
                type == UniformType::IntSamplerCube ||
                type == UniformType::IntSamplerCubeArray ||
                type == UniformType::IntSamplerBuffer ||

                type == UniformType::UnsignedIntSampler1D ||
                type == UniformType::UnsignedIntSampler1DArray ||
                type == UniformType::UnsignedIntSampler2D ||
                type == UniformType::UnsignedIntSampler2DArray ||
                type == UniformType::UnsignedIntSampler2DMultisample ||
                type == UniformType::UnsignedIntSampler2DMultisampleArray ||
                type == UniformType::UnsignedIntSampler2DRect ||
                type == UniformType::UnsignedIntSampler3D ||
                type == UniformType::UnsignedIntSamplerCube ||
                type == UniformType::UnsignedIntSamplerCubeArray ||
                type == UniformType::UnsignedIntSamplerBuffer
        );
    }

    bool ShaderProgram::IsImage(UniformType type) {
        return (
                type == UniformType::Image1D ||
                type == UniformType::Image1DArray ||
                type == UniformType::Image2D ||
                type == UniformType::Image2DArray ||
                type == UniformType::Image2DMultisample ||
                type == UniformType::Image2DMultisampleArray ||
                type == UniformType::Image2DRect ||
                type == UniformType::Image3D ||
                type == UniformType::ImageCube ||
                type == UniformType::ImageCubeArray ||

                type == UniformType::IntImage1D ||
                type == UniformType::IntImage1DArray ||
                type == UniformType::IntImage2D ||
                type == UniformType::IntImage2DArray ||
                type == UniformType::IntImage2DMultisample ||
                type == UniformType::IntImage2DMultisampleArray ||
                type == UniformType::IntImage2DRect ||
                type == UniformType::IntImage3D ||
                type == UniformType::UnsignedIntImage1D ||
                type == UniformType::UnsignedIntImage1DArray ||
                type == UniformType::UnsignedIntImage2D ||
                type == UniformType::UnsignedIntImage2DArray ||
                type == UniformType::UnsignedIntImage2DMultisample ||
                type == UniformType::UnsignedIntImage2DMultisampleArray ||
                type == UniformType::UnsignedIntImage2DRect ||
                type == UniformType::UnsignedIntImage3D ||
                type == UniformType::UnsignedIntImageCube ||
                type == UniformType::UnsignedIntImageCubeArray
        );
    }

//endregion
//region Object
    string Object::PrintTags() {
        stringstream s;
        s << '(';
        for (const auto &tag : m_tags)
            s << tag << ", ";
        //s.seekp(-2, std::ios_base::end);
        s << ')';
        return s.str();
    }
//endregion

//region ResourceContainer

    ResourceContainer::ResourceContainer() {
        m_loaders["Shader"] = make_unique<ShaderLoader>();
        m_loaders["Texture2D"] = make_unique<Texture2DLoader>();
    }

    template<typename Out>
    void split(const std::string &s, char delim, Out result) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            *(result++) = item;
        }
    }

    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }

    void ResourceLoader::LoadNameTags(const tinyxml2::XMLElement *reader, Resource *res) const {
        res->SetName(reader->FindAttribute("name")->Value());
        auto tags = reader->FirstChildElement("Tags");
        if (tags != nullptr) {
            auto t = split(tags->GetText(), ',');
            for (auto s : t) {
                std::string::iterator end_pos = std::remove(s.begin(), s.end(), ' ');
                s.erase(end_pos, s.end());
                if (!s.empty())
                    res->AddTag(s);
            }
        }
    }

    void ResourceContainer::LoadFromFilesystem() {
        fs::path cpath = fs::current_path();
        list<fs::path> configs = ScanFiles(".xml");
        for (const auto &p : configs) {
            tinyxml2::XMLDocument doc;
            if (doc.LoadFile(p.string().c_str()) == 0) {
                fs::current_path(p.parent_path());
                auto node = doc.RootElement()->FirstChildElement();
                auto txt = doc.RootElement()->Name();
                while (node != nullptr) {
                    AddResource(m_loaders[node->Name()]->Load(node));
                    node = node->NextSiblingElement();
                }
            }

        }
        fs::current_path(cpath);
    }

    void ResourceContainer::GUI() {
        ImGui::Begin("Resources");
        for (const auto &res : m_data)
            ImGui::Text((res->GetName() + res->PrintTags()).c_str());
        ImGui::End();
    }
//endregion

//region ShaderCompileException

    ShaderCompileException::ShaderCompileException(const std::string &message) : std::runtime_error(
            "Shader compile exception") {
        m_message = message;
    }

    const char *ShaderCompileException::what() const noexcept {
        return m_message.data();
    }

    ShaderCompileException::~ShaderCompileException() {
    }
//endregion

//region ShaderLoader

    map<string, ShaderType> ShaderLoader::m_shaderTypeMap = {
            {"Compute",        ShaderType::Compute},
            {"Fragment",       ShaderType::Fragment},
            {"Geometry",       ShaderType::Geometry},
            {"TessControl",    ShaderType::TessControl},
            {"TessEvaluation", ShaderType::TessEvaluation},
            {"Vertex",         ShaderType::Vertex}
    };

    ShaderProgram *CreateShaderProgram(const string &vertex, const string &fragment) {
        ShaderProgram *res = new ShaderProgram();
        auto vertexStage = make_unique<ShaderStage>(ShaderType::Vertex, vertex);
        auto fragmentStage = make_unique<ShaderStage>(ShaderType::Fragment, fragment);
        res->AttachStage(vertexStage.get());
        res->AttachStage(fragmentStage.get());
        res->Link();
        res->DetachStage(vertexStage.get());
        res->DetachStage(fragmentStage.get());
        return res;
    }

    ShaderProgram *CreateShaderProgram(const string &compute) {
        ShaderProgram *res = new ShaderProgram();
        auto computeStage = make_unique<ShaderStage>(ShaderType::Compute, compute);
        res->AttachStage(computeStage.get());
        res->Link();
        res->DetachStage(computeStage.get());
        return res;
    }

    ogele::Resource *ShaderLoader::Load(const tinyxml2::XMLElement *reader) const {
        list<unique_ptr<ShaderStage>> stages;
        ShaderProgram *res = new ShaderProgram();
        LoadNameTags(reader, res);
        auto stage = reader->FirstChildElement("Stages")->FirstChildElement();
        while (stage != nullptr) {
            ifstream srcFile(stage->FindAttribute("path")->Value());
            stringstream buffer;
            buffer << srcFile.rdbuf();
            stages.emplace_back(make_unique<ShaderStage>(m_shaderTypeMap[stage->Name()], buffer.str()));
            stage = stage->NextSiblingElement();
        }
        for (const auto &s : stages)
            res->AttachStage(s.get());
        res->Link();
        for (const auto &s : stages)
            res->DetachStage(s.get());
        return res;
    }
//endregion

//region ShaderProgram

    ShaderProgram::ShaderProgram() {
        m_handle = glCreateProgram();
        GLErr();
    }

    void ShaderProgram::Link() {
        glLinkProgram(m_handle);
        GLErr();
        GLint Result = GL_FALSE;
        int InfoLogLength;
        glGetProgramiv(m_handle, GL_LINK_STATUS, &Result);
        GLErr();
        if (Result == GL_FALSE) {
            glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &InfoLogLength);
            GLErr();
            char *message = new char[InfoLogLength + 1];
            glGetProgramInfoLog(m_handle, InfoLogLength, nullptr, &message[0]);
            GLErr();
            throw ShaderCompileException(message);
        }

        GLint numUnifBlocks = 0;
        glGetProgramInterfaceiv(m_handle, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numUnifBlocks);
        GLErr();
        const GLenum blockProperties[] = {GL_NAME_LENGTH};
        const GLenum activeUnifProp[] = {GL_ACTIVE_VARIABLES};
        const GLenum unifProperties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION};
        GLint numStorBlocks = 0;
        glGetProgramInterfaceiv(m_handle, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &numStorBlocks);
        GLErr();
        Bind();
        for (int blockIx = 0; blockIx < numUnifBlocks; ++blockIx) {
            GLint bufProp[1];
            glGetProgramResourceiv(m_handle, GL_UNIFORM_BLOCK, blockIx, 1, blockProperties, 1, NULL, bufProp);
            GLErr();
            std::vector<char> s(bufProp[0]);
            glGetProgramResourceName(m_handle, GL_UNIFORM_BLOCK, blockIx, bufProp[0], NULL, s.data());
            GLErr();
            glUniformBlockBinding(m_handle, blockIx, blockIx);
            GLErr();
            m_buffers.emplace_back(blockIx, string(s.data()), BufferTarget::Uniform);
        }
        for (int blockIx = 0; blockIx < numStorBlocks; ++blockIx) {
            GLint bufProp[1];
            glGetProgramResourceiv(m_handle, GL_SHADER_STORAGE_BLOCK, blockIx, 1, blockProperties, 1, NULL, bufProp);
            GLErr();
            std::vector<char> s(bufProp[0]);
            glGetProgramResourceName(m_handle, GL_SHADER_STORAGE_BLOCK, blockIx, bufProp[0], NULL, s.data());
            GLErr();
            glShaderStorageBlockBinding(m_handle, blockIx, blockIx + numUnifBlocks);
            GLErr();
            m_buffers.emplace_back(blockIx + numUnifBlocks, string(s.data()), BufferTarget::ShaderStorage);
        }
        GLint numUniforms = 0;
        glGetProgramInterfaceiv(m_handle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
        const GLenum properties[4] = {GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH, GL_LOCATION};
        int samplerNum = 0;
        int imageNum = 0;

        for (int unif = 0; unif < numUniforms; ++unif) {
            GLint values[4];
            glGetProgramResourceiv(m_handle, GL_UNIFORM, unif, 4, properties, 4, NULL, values);
            GLErr();
            std::vector<char> nameData(values[2]);
            glGetProgramResourceName(m_handle, GL_UNIFORM, unif, nameData.size(), NULL, &nameData[0]);
            GLErr();
            int pos;
            auto type = static_cast<UniformType>(values[1]);
            GLErr();
            if (IsSampler(type)) {
                glUniform1i(values[3], samplerNum);
                GLErr();
                pos = samplerNum;
                samplerNum++;
            } else if (IsImage(type)) {
                glUniform1i(values[3], imageNum);
                GLErr();
                pos = imageNum;
                imageNum++;
            } else
                pos = values[3];
            m_uniforms.emplace_back(pos, string(nameData.data()), static_cast<UniformType>(values[1]),
                                    (values[0] >= 0) ? &m_buffers[values[0]] : nullptr);
        }
        Unbind();
    }

    int ShaderProgram::GetUniformLocation(const string &name) const noexcept {
        int res = -1;
        for (int i = 0; i < m_uniforms.size(); i++)
            if (m_uniforms[i].GetName() == name)
                res = m_uniforms[i].GetPos();
        return res;
    }

    template<>
    void ShaderProgram::Set(const string &name, const float &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform1f(pos, value); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const vec2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform2f(pos, value.x, value.y); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const vec3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform3f(pos, value.x, value.y, value.z); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const vec4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform4f(pos, value.x, value.y, value.z, value.w); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const double &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform1d(pos, value); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dvec2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform2d(pos, value.x, value.y); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dvec3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform3d(pos, value.x, value.y, value.z); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dvec4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform4d(pos, value.x, value.y, value.z, value.w); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const int &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform1i(pos, value); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const ivec2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform2i(pos, value.x, value.y); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const ivec3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform3i(pos, value.x, value.y, value.z); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const ivec4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform4i(pos, value.x, value.y, value.z, value.w); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const unsigned int &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform1ui(pos, value); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const uvec2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform2ui(pos, value.x, value.y); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const uvec3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform3ui(pos, value.x, value.y, value.z); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const uvec4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform4ui(pos, value.x, value.y, value.z, value.w); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix2fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat2x3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix2x3fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat2x4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix2x4fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat3x2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix3x2fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix3fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat3x4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix3x4fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat4x2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix4x2fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat4x3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix4x3fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix4fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix2dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat2x3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix2x3dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat2x4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix2x4dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat3x2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix3x2dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix3dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat3x4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix3x4dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat4x2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix4x2dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat4x3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix4x3dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix4dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    void ShaderProgram::SetTexture(const string &name, TextureBase *value) {
        for (int i = 0; i < m_uniforms.size(); i++)
            if (m_uniforms[i].GetName() == name) {
                if (m_uniforms[i].IsSampler())
                    value->SetActive(m_uniforms[i].GetPos());
                if (m_uniforms[i].IsImage())
                    value->BindImage(m_uniforms[i].GetPos());
                return;
            }
    }

    void ShaderProgram::SetBuffer(const string &name, BufferBase *value) {
        for (int i = 0; i < m_buffers.size(); i++)
            if (m_buffers[i].GetName() == name) {
                value->BindBase(m_buffers[i].GetBufferTarget(), m_buffers[i].GetPos());
                return;
            }
    }

    Material *ShaderProgram::CreateMaterial() const {
        Material *res = new Material();
        for (const auto &u : m_uniforms)
            res->AddUniform(u.GetName(), u.GetType());
        return res;
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(m_handle);
        GLErr();
    }
//endregion

//region Query

    Query::Query(QueryType type) {
        m_type = type;
        glGenQueries(1, &m_handle);
        GLErr();
    }

    Query::~Query() {
        glDeleteQueries(1, &m_handle);
        GLErr();
    }
//endregion

//region GUIHelper

    template<>
    void PropertyGUI(const string &name, float *value) { ImGui::InputFloat(name.c_str(), value); }

    template<>
    void PropertyGUI(const string &name, vec2 *value) { ImGui::InputFloat2(name.c_str(), &value->x); }

    template<>
    void PropertyGUI(const string &name, vec3 *value) { ImGui::InputFloat3(name.c_str(), &value->x); }

    template<>
    void PropertyGUI(const string &name, vec4 *value) { ImGui::InputFloat4(name.c_str(), &value->x); }

    template<>
    void PropertyGUI(const string &name, int *value) { ImGui::InputInt(name.c_str(), value); }

    template<>
    void PropertyGUI(const string &name, ivec2 *value) { ImGui::InputInt2(name.c_str(), &value->x); }

    template<>
    void PropertyGUI(const string &name, ivec3 *value) { ImGui::InputInt3(name.c_str(), &value->x); }

    template<>
    void PropertyGUI(const string &name, ivec4 *value) { ImGui::InputInt4(name.c_str(), &value->x); }

    template<>
    void PropertyGUI(const string &name, unsigned int *value) {
        ImGui::LabelText(name.c_str(), to_string(*value).c_str());
    }

    template<>
    void PropertyGUI(const string &name, uvec2 *value) { ImGui::LabelText(name.c_str(), to_string(*value).c_str()); }

    template<>
    void PropertyGUI(const string &name, uvec3 *value) { ImGui::LabelText(name.c_str(), to_string(*value).c_str()); }

    template<>
    void PropertyGUI(const string &name, uvec4 *value) { ImGui::LabelText(name.c_str(), to_string(*value).c_str()); }
//endregion

//region Material

    void Material::AddUniform(const string &name, UniformType type) {
        switch (type) {
#define MaterialTypeCase(EnumName, TypeName) case UniformType:: EnumName : m_data[name] = unique_ptr<UniformBase>(new UniformValue< TypeName >()); break
            MaterialTypeCase(Float, float);
            MaterialTypeCase(FloatVec2, vec2);
            MaterialTypeCase(FloatVec3, vec3);
            MaterialTypeCase(FloatVec4, vec4);

            MaterialTypeCase(Double, double);
            MaterialTypeCase(DoubleVec2, dvec2);
            MaterialTypeCase(DoubleVec3, dvec3);
            MaterialTypeCase(DoubleVec4, dvec4);

            MaterialTypeCase(Int, int);
            MaterialTypeCase(IntVec2, ivec2);
            MaterialTypeCase(IntVec3, ivec3);
            MaterialTypeCase(IntVec4, ivec4);

            MaterialTypeCase(UnsignedInt, unsigned int);
            MaterialTypeCase(UnsignedIntVec2, uvec2);
            MaterialTypeCase(UnsignedIntVec3, uvec3);
            MaterialTypeCase(UnsignedIntVec4, uvec4);

            MaterialTypeCase(FloatMat2, mat2);
            MaterialTypeCase(FloatMat2x3, mat2x3);
            MaterialTypeCase(FloatMat2x4, mat2x4);
            MaterialTypeCase(FloatMat3x2, mat3x2);
            MaterialTypeCase(FloatMat3, mat3);
            MaterialTypeCase(FloatMat3x4, mat3x4);
            MaterialTypeCase(FloatMat4x2, mat4x2);
            MaterialTypeCase(FloatMat4x3, mat4x3);
            MaterialTypeCase(FloatMat4, mat4);

            MaterialTypeCase(DoubleMat2, dmat2);
            MaterialTypeCase(DoubleMat2x3, dmat2x3);
            MaterialTypeCase(DoubleMat2x4, dmat2x4);
            MaterialTypeCase(DoubleMat3x2, dmat3x2);
            MaterialTypeCase(DoubleMat3, dmat3);
            MaterialTypeCase(DoubleMat3x4, dmat3x4);
            MaterialTypeCase(DoubleMat4x2, dmat4x2);
            MaterialTypeCase(DoubleMat4x3, dmat4x3);
            MaterialTypeCase(DoubleMat4, dmat4);
        }
    }

    void Material::GUI() {
        for (const auto &u : m_data)
            u.second->GUI(u.first);
    }

    void Material::Apply(ShaderProgram *sh) {
        for (const auto &u : m_data)
            u.second->Apply(u.first, sh);
    }

    void Material::FetchValues(Material *mat) {
        for (const auto &mu : mat->m_data)
            for (const auto &u : m_data)
                if (u.first == mu.first)
                    u.second->TryCopyFrom(mu.second.get());
    }
//endregion

//region ShaderStage

    ShaderStage::ShaderStage(ShaderType type, const std::string &source) {
        m_handle = glCreateShader(static_cast<GLenum>(type));
        GLErr();
        auto p = source.data();
        glShaderSource(m_handle, 1, &p, nullptr);
        GLErr();
        glCompileShader(m_handle);
        GLErr();

        GLint Result = GL_FALSE;
        int InfoLogLength;
        glGetShaderiv(m_handle, GL_COMPILE_STATUS, &Result);
        GLErr();
        if (Result == GL_FALSE) {
            glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &InfoLogLength);
            GLErr();
            char *message = new char[InfoLogLength + 1];
            glGetShaderInfoLog(m_handle, InfoLogLength, nullptr, &message[0]);
            GLErr();
            throw ShaderCompileException(message);
        }
    }

    ShaderStage::~ShaderStage() {
        glDeleteShader(m_handle);
        GLErr();
    }
//endregion

//region VertexArray

    VertexArray::VertexArray(Topology topology) {
        m_topology = topology;
        m_indexed = false;
        glGenVertexArrays(1, &m_handle);
        GLErr();
    }

    void VertexArray::bAttachBuffer(int index, int vectorLength, const BufferBase *buffer, int offset) {
        bEnableVertexAttribArray(index);
        auto type = buffer->GetDataType();
        buffer->Bind(BufferTarget::Array);
        if (
                type == BufferDataType::Byte ||
                type == BufferDataType::Short ||
                type == BufferDataType::Int ||
                type == BufferDataType::UnsignedByte ||
                type == BufferDataType::UnsignedShort ||
                type == BufferDataType::UnsignedInt
                )
            glVertexAttribIPointer(index, vectorLength, static_cast<GLenum>(type), buffer->GetStructSize(),
                                   (const void *) (buffer->GetElementSize() * offset));
        else if (type == BufferDataType::Double)
            glVertexAttribLPointer(index, vectorLength, static_cast<GLenum>(type), buffer->GetStructSize(),
                                   (const void *) (buffer->GetElementSize() * offset));
        else
            glVertexAttribPointer(index, vectorLength, static_cast<GLenum>(type), false, buffer->GetStructSize(),
                                  (const void *) (buffer->GetElementSize() * offset));
        GLErr();
        buffer->Unbind(BufferTarget::Array);
    }

    void VertexArray::bAttachElementBuffer(const UnsignedIntBuffer<unsigned int> *buffer) {
        if (buffer == nullptr) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            GLErr();
            m_indexed = false;
        } else {
            buffer->Bind(BufferTarget::ElementArray);
            m_indexed = true;
        }
    }

    void VertexArray::bDraw(int first, int count) const {
        if (m_indexed) {
            glDrawElements(static_cast<GLenum>(m_topology), count, GL_UNSIGNED_INT,
                           (void *) (sizeof(unsigned int) * first));
            GLErr();
        } else {
            glDrawArrays(static_cast<GLenum>(m_topology), first, count);
            GLErr();

        }
    }

    void VertexArray::bDraw(int first, int count, int instanceCount) const {
        if (m_indexed) {
            glDrawElementsInstanced(static_cast<GLenum>(m_topology), count, GL_UNSIGNED_INT,
                                    (void *) (sizeof(unsigned int) * first), instanceCount);
            GLErr();
        } else {
            glDrawArraysInstanced(static_cast<GLenum>(m_topology), first, count, instanceCount);
            GLErr();
        }
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &m_handle);
        GLErr();
    }
//endregion

//region Window

    void Window::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        Window *wnd = (Window *) glfwGetWindowUserPointer(window);
        wnd->OnKey(static_cast<Key>(key), scancode, static_cast<KeyState>(action), static_cast<KeyModifiers>(mods));
    }

    void Window::CharCallback(GLFWwindow *window, unsigned int character) {
        Window *wnd = (Window *) glfwGetWindowUserPointer(window);
        wnd->OnChar(static_cast<wchar_t>(character));
    }

    void Window::MouseBtnCallback(GLFWwindow *window, int button, int action, int mods) {
        Window *wnd = (Window *) glfwGetWindowUserPointer(window);
        wnd->OnMouseBtn(static_cast<MouseButton>(button), static_cast<KeyState>(action),
                        static_cast<KeyModifiers>(mods));
    }

    void Window::CursorPosCallback(GLFWwindow *window, double x, double y) {
        dvec2 pos = dvec2(x, y);
        Window *wnd = (Window *) glfwGetWindowUserPointer(window);
        wnd->OnCursorPos(pos, pos - wnd->m_oldCursorPos);
        wnd->m_oldCursorPos = pos;
    }

    void Window::CursorEnterCallback(GLFWwindow *window, int entered) {
        Window *wnd = (Window *) glfwGetWindowUserPointer(window);
        wnd->OnCursorEnter(static_cast<bool>(entered));
    }

    void Window::ScrollCallback(GLFWwindow *window, double x, double y) {
        Window *wnd = (Window *) glfwGetWindowUserPointer(window);
        wnd->OnScroll(glm::dvec2(x, y));
    }

    void Window::FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
        Window *wnd = (Window *) glfwGetWindowUserPointer(window);
        Viewport(ivec2(0, 0), ivec2(width, height));
        wnd->OnResize(ivec2(width, height));
    }

    Window::Window(const glm::ivec2 &size, const std::string &caption, bool firstInit) {
        if (firstInit)
            glfwInit();
        m_handle = glfwCreateWindow(size.x, size.y, caption.data(), NULL, NULL);
        glfwSetWindowUserPointer(m_handle, this);

        if (firstInit) {
            MakeContextCurrent();
            glewInit();
        }
        glfwSetKeyCallback(m_handle, KeyCallback);
        glfwSetCharCallback(m_handle, CharCallback);
        glfwSetMouseButtonCallback(m_handle, MouseBtnCallback);
        glfwSetCursorPosCallback(m_handle, CursorPosCallback);
        glfwSetCursorEnterCallback(m_handle, CursorEnterCallback);
        glfwSetFramebufferSizeCallback(m_handle, FramebufferSizeCallback);
        glfwSetScrollCallback(m_handle, ScrollCallback);
        glfwGetCursorPos(m_handle, &m_oldCursorPos.x, &m_oldCursorPos.y);
    }

    Window::~Window() {
        glfwDestroyWindow(m_handle);
    }
//endregion

//region TextureBase

    bool IsTexFormatInteger(TextureFormat format) {
        switch (format) {
            case TextureFormat::R8I:
                return true;
            case TextureFormat::R8UI:
                return true;
            case TextureFormat::R16I:
                return true;
            case TextureFormat::R16UI:
                return true;
            case TextureFormat::R32I:
                return true;
            case TextureFormat::R32UI:
                return true;
            case TextureFormat::RG8I:
                return true;
            case TextureFormat::RG8UI:
                return true;
            case TextureFormat::RG16I:
                return true;
            case TextureFormat::RG16UI:
                return true;
            case TextureFormat::RG32I:
                return true;
            case TextureFormat::RG32UI:
                return true;
            case TextureFormat::RGB8I:
                return true;
            case TextureFormat::RGB8UI:
                return true;
            case TextureFormat::RGB16I:
                return true;
            case TextureFormat::RGB16UI:
                return true;
            case TextureFormat::RGB32I:
                return true;
            case TextureFormat::RGB32UI:
                return true;
            case TextureFormat::RGBA8I:
                return true;
            case TextureFormat::RGBA8UI:
                return true;
            case TextureFormat::RGBA16I:
                return true;
            case TextureFormat::RGBA16UI:
                return true;
            case TextureFormat::RGBA32I:
                return true;
            case TextureFormat::RGBA32UI:
                return true;
            default:
                return false;
        }
    }

    TextureBase::TextureBase(TextureType type, TextureFormat format, bool mipmaps) {
        m_type = type;
        m_format = format;
        m_mipmaps = mipmaps;
        m_bindAccess = AccessMode::ReadWrite;
        m_bindLevel = 0;
        m_bindLayered = true;
        m_bindLayer = 0;
        glGenTextures(1, &m_handle);
        GLErr();
    }

    bool TextureBase::IsInteger() const noexcept { return IsTexFormatInteger(GetFormat()); }

    void TextureBase::BindImage(int unitNum) const {
        glBindImageTexture(unitNum, m_handle, m_bindLevel, m_bindLayered, m_bindLayer,
                           static_cast<GLenum>(m_bindAccess), static_cast<GLenum>(m_format));
        GLErr();
    }

    ogele::TextureDepthStencilMode TextureBase::bGetDepthStencilMode() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_DEPTH_STENCIL_TEXTURE_MODE, &res);
        GLErr();
        return static_cast<TextureDepthStencilMode>(res);
    }

    void TextureBase::bSetDepthStencilMode(TextureDepthStencilMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_DEPTH_STENCIL_TEXTURE_MODE, static_cast<int>(value));
        GLErr();
    }

    glm::vec4 TextureBase::bGetBorderColor() const {
        vec4 res;
        glGetTexParameterfv(static_cast<GLenum>(m_type), GL_TEXTURE_BORDER_COLOR, &res.x);
        GLErr();
        return res;
    }

    void TextureBase::bSetBorderColor(vec4 value) {
        glTexParameterfv(static_cast<GLenum>(m_type), GL_TEXTURE_BORDER_COLOR, &value.x);
        GLErr();
    }

    ogele::DepthFuncMode TextureBase::bGetCompareFunc() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_COMPARE_FUNC, &res);
        GLErr();
        return static_cast<DepthFuncMode>(res);
    }

    void TextureBase::bSetCompareFunc(DepthFuncMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_COMPARE_FUNC, static_cast<int>(value));
        GLErr();
    }

    ogele::TextureCompareMode TextureBase::bGetCompareMode() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_COMPARE_MODE, &res);
        GLErr();
        return static_cast<TextureCompareMode>(res);
    }

    void TextureBase::bSetCompareMode(TextureCompareMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_COMPARE_MODE, static_cast<int>(value));
        GLErr();
    }

    ogele::TextureFilterMode TextureBase::bGetMinFilter() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_MIN_FILTER, &res);
        GLErr();
        return static_cast<TextureFilterMode>(res);
    }

    void TextureBase::bSetMinFilter(TextureFilterMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_MIN_FILTER, static_cast<int>(value));
        GLErr();
    }

    ogele::TextureFilterMode TextureBase::bGetMagFilter() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_MAG_FILTER, &res);
        GLErr();
        return static_cast<TextureFilterMode>(res);
    }

    void TextureBase::bSetMagFilter(TextureFilterMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_MAG_FILTER, static_cast<int>(value));
        GLErr();
    }

    int TextureBase::bGetBaseLevel() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_BASE_LEVEL, &res);
        GLErr();
        return res;
    }

    void TextureBase::bSetBaseLevel(int value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_BASE_LEVEL, value);
        GLErr();
    }

    int TextureBase::bGetMaxLevel() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_MAX_LEVEL, &res);
        GLErr();
        return res;
    }

    void TextureBase::bSetMaxLevel(int value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_MAX_LEVEL, value);
        GLErr();
    }

    float TextureBase::bGetLODBias() const {
        float res;
        glGetTexParameterfv(static_cast<GLenum>(m_type), GL_TEXTURE_LOD_BIAS, &res);
        GLErr();
        return res;
    }

    void TextureBase::bSetLODBias(float value) {
        glTexParameterf(static_cast<GLenum>(m_type), GL_TEXTURE_LOD_BIAS, value);
        GLErr();
    }

    float TextureBase::bGetMinLOD() const {
        float res;
        glGetTexParameterfv(static_cast<GLenum>(m_type), GL_TEXTURE_MIN_LOD, &res);
        GLErr();
        return res;
    }

    void TextureBase::bSetMinLOD(float value) {
        glTexParameterf(static_cast<GLenum>(m_type), GL_TEXTURE_MIN_LOD, value);
        GLErr();
    }

    float TextureBase::bGetMaxLOD() const {
        float res;
        glGetTexParameterfv(static_cast<GLenum>(m_type), GL_TEXTURE_MAX_LOD, &res);
        GLErr();
        return res;
    }

    void TextureBase::bSetMaxLOD(float value) {
        glTexParameterf(static_cast<GLenum>(m_type), GL_TEXTURE_MAX_LOD, value);
        GLErr();
    }

    ogele::ChannelSwizzleValue TextureBase::bGetSwizzleR() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_SWIZZLE_R, &res);
        GLErr();
        return static_cast<ChannelSwizzleValue>(res);
    }

    void TextureBase::bSetSwizzleR(ChannelSwizzleValue value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_SWIZZLE_R, static_cast<int>(value));
        GLErr();
    }

    ogele::ChannelSwizzleValue TextureBase::bGetSwizzleG() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_SWIZZLE_G, &res);
        GLErr();
        return static_cast<ChannelSwizzleValue>(res);
    }

    void TextureBase::bSetSwizzleG(ChannelSwizzleValue value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_SWIZZLE_G, static_cast<int>(value));
        GLErr();
    }

    ogele::ChannelSwizzleValue TextureBase::bGetSwizzleB() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_SWIZZLE_B, &res);
        GLErr();
        return static_cast<ChannelSwizzleValue>(res);
    }

    void TextureBase::bSetSwizzleB(ChannelSwizzleValue value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_SWIZZLE_B, static_cast<int>(value));
        GLErr();
    }

    ogele::ChannelSwizzleValue TextureBase::bGetSwizzleA() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_SWIZZLE_A, &res);
        GLErr();
        return static_cast<ChannelSwizzleValue>(res);
    }

    void TextureBase::bSetSwizzleA(ChannelSwizzleValue value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_SWIZZLE_A, static_cast<int>(value));
        GLErr();
    }

    ogele::TextureWrapMode TextureBase::bGetWrapS() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_S, &res);
        GLErr();
        return static_cast<TextureWrapMode>(res);
    }

    void TextureBase::bSetWrapS(TextureWrapMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_S, static_cast<int>(value));
        GLErr();
    }

    ogele::TextureWrapMode TextureBase::bGetWrapT() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_T, &res);
        GLErr();
        return static_cast<TextureWrapMode>(res);
    }

    void TextureBase::bSetWrapT(TextureWrapMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_T, static_cast<int>(value));
        GLErr();
    }

    ogele::TextureWrapMode TextureBase::bGetWrapR() const {
        int res;
        glGetTexParameteriv(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_R, &res);
        GLErr();
        return static_cast<TextureWrapMode>(res);
    }

    void TextureBase::bSetWrapR(TextureWrapMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_R, static_cast<int>(value));
        GLErr();
    }

    void TextureBase::bSetWrap(TextureWrapMode value) {
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_R, static_cast<int>(value));
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_S, static_cast<int>(value));
        glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_T, static_cast<int>(value));
        GLErr();
    }

    void TextureBase::bGenerateMipMap() {
        glGenerateMipmap(static_cast<GLenum>(m_type));
        GLErr();
    }

    void TextureBase::SetActive(int num) {
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + num));
        Bind(); GLErr();
    }

    TextureBase::~TextureBase() {
        glDeleteTextures(1, &m_handle);
        GLErr();
    }
//endregion

//region Texture2D

    void Texture2D::Allocate(const ivec2 &newSize) {
        m_size = newSize;
        Bind();
        if (HasMipmaps()) {
            int levels = std::log2(std::min(m_size.x, m_size.y));
            glTexStorage2D(static_cast<GLenum>(GetType()), levels, static_cast<GLenum>(GetFormat()), m_size.x,
                           m_size.y);
            GLErr();
            bGenerateMipMap();
        } else {
            glTexStorage2D(static_cast<GLenum>(GetType()), 1, static_cast<GLenum>(GetFormat()), m_size.x, m_size.y);
            GLErr();
        }
        Unbind();
    }

    Texture2D::Texture2D(const ivec2 &size, bool mipmaps, TextureFormat format) :
            TextureBase(TextureType::Single2D, format, mipmaps) {
        Allocate(size);
    }

#define Tex2DDef(dataType, format, pixelDataType) \
    template<> void Texture2D::bSetData< dataType >(int level, const ivec2& offset, const ivec2& size, const dataType * data)\
    {glTexSubImage2D(static_cast<GLenum>(GetType()), level, offset.x, offset.y, size.x, size.y,\
    static_cast<GLenum>(IsInteger()? PixelDataFormat::format ## Integer : PixelDataFormat::format),\
    static_cast<GLenum>(PixelDataType::pixelDataType ), data);GLErr();\
    if(HasMipmaps()) bGenerateMipMap();}\
    template<> void Texture2D::bGetData< dataType >(int level, dataType * data) const\
    {glGetTexImage(static_cast<GLenum>(GetType()), level,\
    static_cast<GLenum>(IsInteger()? PixelDataFormat::format ## Integer : PixelDataFormat::format),\
    static_cast<GLenum>(PixelDataType::pixelDataType ), data);GLErr();}

    Tex2DDef(char, RGBA, Byte);

    Tex2DDef(cvec2, RG, Byte);

    Tex2DDef(cvec3, RGB, Byte);

    Tex2DDef(cvec4, RGBA, Byte);

    Tex2DDef(unsigned char, RGBA, UnsignedByte);

    Tex2DDef(ucvec2, RG, UnsignedByte);

    Tex2DDef(ucvec3, RGB, UnsignedByte);

    Tex2DDef(ucvec4, RGBA, UnsignedByte);

    Tex2DDef(short, RGBA, Short);

    Tex2DDef(svec2, RG, Short);

    Tex2DDef(svec3, RGB, Short);

    Tex2DDef(svec4, RGBA, Short);

    Tex2DDef(unsigned short, RGBA, UnsignedShort);

    Tex2DDef(usvec2, RG, UnsignedShort);

    Tex2DDef(usvec3, RGB, UnsignedShort);

    Tex2DDef(usvec4, RGBA, UnsignedShort);

    Tex2DDef(unsigned int, Red, UnsignedInt);

    Tex2DDef(uvec2, RG, UnsignedInt);

    Tex2DDef(uvec3, RGB, UnsignedInt);

    Tex2DDef(uvec4, RGBA, UnsignedInt);

    Tex2DDef(int, Red, Int);

    Tex2DDef(ivec2, RG, Int);

    Tex2DDef(ivec3, RGB, Int);

    Tex2DDef(ivec4, RGBA, Int);

    Tex2DDef(float, Red, Float);

    Tex2DDef(vec2, RG, Float);

    Tex2DDef(vec3, RGB, Float);

    Tex2DDef(vec4, RGBA, Float);
//endregion

//region TextureLoaderHelpers

    string ReadXMLProperty(const tinyxml2::XMLElement *reader, const string &propName, const string &defaultValue) {
        auto elem = reader->FirstChildElement(propName.data());
        if (elem == nullptr)
            return defaultValue;
        else
            return elem->GetText();
    }

    map<const string, TextureWrapMode> StrToTexWrapMode = {
            {"ClampToEdge",       TextureWrapMode::ClampToEdge},
            {"ClampToBorder",     TextureWrapMode::ClampToBorder},
            {"MirroredRepeat",    TextureWrapMode::MirroredRepeat},
            {"Repeat",            TextureWrapMode::Repeat},
            {"MirrorClampToEdge", TextureWrapMode::MirrorClampToEdge}
    };
    map<const string, TextureFilterMode> StrToTexFilterMode = {
            {"Linear",               TextureFilterMode::Linear},
            {"LinearMipMapLinear",   TextureFilterMode::LinearMipMapLinear},
            {"LinearMipMapNearest",  TextureFilterMode::LinearMipMapNearest},
            {"Nearest",              TextureFilterMode::Nearest},
            {"NearestMipMapLinear",  TextureFilterMode::NearestMipMapLinear},
            {"NearestMipMapNearest", TextureFilterMode::NearestMipMapNearest}
    };
//endregion

//region Texture2DLoader

    Resource *Texture2DLoader::Load(const tinyxml2::XMLElement *reader) const {
        auto filePath = reader->FirstChildElement("File")->FindAttribute("path")->Value();
        int x, y, n;
        ucvec4 *data = reinterpret_cast<ucvec4 *>(stbi_load(filePath, &x, &y, &n, 4));
        ivec2 size = {x, y};
        Texture2D *tex = new Texture2D(size, true, TextureFormat::RGBA8);
        LoadNameTags(reader, tex);
        tex->Bind();
        tex->bSetWrap(StrToTexWrapMode[ReadXMLProperty(reader, "Wrap", "Repeat")]);
        tex->bSetMinFilter(StrToTexFilterMode[ReadXMLProperty(reader, "MinFilter", "LinearMipMapNearest")]);
        tex->bSetMagFilter(StrToTexFilterMode[ReadXMLProperty(reader, "MagFilter", "Linear")]);
        tex->bSetData(0, {0, 0}, size, data);
        tex->Unbind();
        stbi_image_free(data);
        return tex;
    }
//endregion

//region FrameBuffer

    RenderBuffer::RenderBuffer(RenderBufferFormat format, const ivec2 &size) {
        glGenRenderbuffers(1, &m_handle);
        GLErr();
        Bind();
        glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(format), size.x, size.y);
        Unbind();
    }

    RenderBuffer::~RenderBuffer() {
        glDeleteRenderbuffers(1, &m_handle);
        GLErr();
    }

    Framebuffer::Framebuffer() {
        glGenFramebuffers(1, &m_handle);
        GLErr();
    }

    void Framebuffer::bDrawBuffers(const vector<int> &attachments) {
        GLenum *bufs = new GLenum[attachments.size()];
        for (int i = 0; i < attachments.size(); i++)
            bufs[i] = attachments[i] + GL_COLOR_ATTACHMENT0;
        glDrawBuffers(attachments.size(), bufs);
        delete[] bufs;
        GLErr();
    }

    void Framebuffer::bDrawBuffers(const initializer_list<int> &attachments) {
        GLenum *bufs = new GLenum[attachments.size()];
        int i = 0;
        for (const auto at : attachments)
            bufs[i++] = at + GL_COLOR_ATTACHMENT0;
        glDrawBuffers(attachments.size(), bufs);
        delete[] bufs;
        GLErr();
    }

    inline FrameBufferCompletion Framebuffer::bGetCompletionStatus() const {
        GLenum res = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        GLErr();
        return static_cast<FrameBufferCompletion>(res);
    }

    void Framebuffer::Copy(Framebuffer *destination, ivec2 uv0, ivec2 uv1, BufferBit buffers, int sourceColorAttachment,
                           int destColorAttachment) const {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_handle);
        GLErr();
        glReadBuffer(GL_COLOR_ATTACHMENT0 + sourceColorAttachment);
        GLErr();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination->m_handle);
        GLErr();
        glDrawBuffer(GL_COLOR_ATTACHMENT0 + destColorAttachment);
        GLErr();
        glBlitFramebuffer(uv0.x, uv0.y, uv1.x, uv1.y, uv0.x, uv0.y, uv1.x, uv1.y, static_cast<GLbitfield>(buffers),
                          GL_NEAREST);
        GLErr();
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        GLErr();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        GLErr();
    }

    void Framebuffer::Copy(ivec2 uv0, ivec2 uv1, BufferBit buffers, int sourceColorAttachment,
                           DefaultColorBuffers destColorBuf) const {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_handle);
        GLErr();
        glReadBuffer(GL_COLOR_ATTACHMENT0 + sourceColorAttachment);
        GLErr();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        GLErr();
        glDrawBuffer(static_cast<GLenum>(destColorBuf));
        GLErr();
        glBlitFramebuffer(uv0.x, uv0.y, uv1.x, uv1.y, uv0.x, uv0.y, uv1.x, uv1.y, static_cast<GLbitfield>(buffers),
                          GL_NEAREST);
        GLErr();
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        GLErr();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        GLErr();
    }

    Framebuffer::~Framebuffer() {
        glDeleteFramebuffers(1, &m_handle);
        GLErr();
    }
//endregion

//region RenderTarget

    RenderTarget::RenderTarget(const ivec2 &size, int colorBufNum, TextureFormat format, bool mipmaps, bool depthBuffer,
                               bool stencilBuffer) {
        m_frameBuf = make_unique<Framebuffer>();
        m_frameBuf->Bind();
        vector<int> indecies;
        for (int i = 0; i < colorBufNum; i++) {
            indecies.push_back(i);
            m_colorBufs.emplace_back(new Texture2D(size, mipmaps, format));
            m_frameBuf->bAttachColorBuffer(i, m_colorBufs[i].get(), 0);
        }
        if (depthBuffer) {
            m_depthBuffer = make_unique<RenderBuffer>(RenderBufferFormat::DepthComponent32, size);
            m_frameBuf->bAttachDepthBuffer(m_depthBuffer.get());
        }
        if (stencilBuffer) {
            m_stencilBuffer = make_unique<RenderBuffer>(RenderBufferFormat::StencilIndex16, size);
            m_frameBuf->bAttachStencilBuffer(m_stencilBuffer.get());
        }
        m_frameBuf->bDrawBuffers(indecies);
        if (!m_frameBuf->bIsComplete())
            throw runtime_error("Framebuffer is not complete");
        m_frameBuf->Unbind();

    }
//endregion

//region Math3D

    trquat RotationBetweenVectors(const trvec3 &from, const trvec3 &to) {
        trscalar dt = dot(from, to);
        trscalar rotationAngle = acos(dt);
        trvec3 rotationAxis = normalize(cross(from, to));
        return normalize(angleAxis(rotationAngle, rotationAxis));
    }

    trscalar AngleBetween(const trvec3 &a, const trvec3 &b, const trvec3 &n) {
        return atan2(dot(cross(b, a), n), dot(a, b));
    }

    trquat LookAtRotation(const trvec3 &dir, const trvec3 &up) {
        trvec3 afwd(0, 0, -1);
        trquat yrot = angleAxis(AngleBetween(dir, afwd, up), up);
        trvec3 fwd = normalize(yrot * afwd);
        trvec3 rt = cross(fwd, up);
        trquat xrot = angleAxis(AngleBetween(dir, fwd, rt), rt);
        return xrot * yrot;
    }

    trvec3 ProjectOnPlane(const trvec3 &vec, const trvec3 &up) {
        return vec - proj(vec, up);
    }
//endregion

//region Transform

    trmat4 ones(1);

    void Transform::UpdateMatrix() {
        m_localMatrix = translate(ones, m_localPos);
        m_localMatrix *= toMat4(m_localRot);
        UpdateMatrixRecursive();
        OnTransformUpdate();
    }

    void Transform::UpdateMatrixRecursive() {
        m_matrix = m_parent ? m_parent->m_matrix * m_localMatrix : m_localMatrix;
        m_matrixInv = transpose(m_matrix);
        for (auto &tr : m_childs)
            tr->UpdateMatrix();
    }

    Transform::Transform() {
        m_parent= nullptr;
        m_localPos = {0, 0, 0};
        m_localRot = trquat();
        UpdateMatrix();
    }

    Transform *Transform::CreateChild() {
        Transform *res = new Transform();
        m_childs.emplace_back(res);
        return res;
    }

    void Transform::SetParent(Transform *parent) {
        if (m_parent != nullptr)
            m_parent->m_childs.remove_if([this](const unique_ptr<Transform> &tr) { return tr.get() == this; });
        parent->m_childs.emplace_back(this);
        m_parent = parent;
    }
//endregion

//region Camera

    Camera::Camera(const ivec2 &frameSize) {
        m_frameSize = frameSize;
    }

    void PerspectiveCamera::UpdateProjection() {
        m_projMatrix = perspectiveFov(m_fov, static_cast<trscalar>(GetFrameSize().x),
                                      static_cast<trscalar>(GetFrameSize().y), m_zNear, m_zFar);
        m_viewProjMatrix = m_projMatrix * m_viewMatrix;
    }

    void PerspectiveCamera::UpdateView() {
        m_viewMatrix = inverse(GetMatrix());
        m_viewProjMatrix = m_projMatrix * m_viewMatrix;
    }

    void PerspectiveCamera::OnTransformUpdate() {
        UpdateView();
    }

    void PerspectiveCamera::OnFrameSizeChanged() {
        UpdateProjection();
    }

    PerspectiveCamera::PerspectiveCamera(const ivec2 &frameSize, trscalar fov, trscalar zNear, trscalar zFar) :
            Camera(frameSize) {
        m_fov = fov;
        m_zNear = zNear;
        m_zFar = zFar;
        UpdateProjection();
        UpdateView();
    }

    void Camera::LookAround(const dvec2 &delta, trscalar speed, trscalar xMin, trscalar xMax) {
        m_eulerAngles.y -= delta.x * speed;
        m_eulerAngles.x -= delta.y * speed;
        m_eulerAngles.x = glm::clamp(m_eulerAngles.x, xMin, xMax);
        SetLocalEulerAngles(m_eulerAngles);
    }
//endregion

//region Mesh

    MeshBase::MeshBase(Topology topology) {
        m_vao = make_unique<VertexArray>(topology);
    }

    void MeshBase::SetBuffer(int index, int vectorSize, const BufferBase *buffer, int offset) {
        m_vao->Bind();
        if (buffer == nullptr)
            m_vao->bDisableVertexAttribArray(index);
        else {
            m_vao->bEnableVertexAttribArray(index);
            m_vao->bAttachBuffer(index, vectorSize, buffer, offset);
        }
        m_vao->Unbind();
    }

    void ModelMesh::SetIndexBuffer(UnsignedIntBuffer<unsigned int> *buffer) {
        SetCount(buffer->GetSize());
        m_indexes.reset(buffer);
        GetVAO()->Bind();
        GetVAO()->bAttachElementBuffer(buffer);
        GetVAO()->Unbind();
    }

    ScreenQuadMesh::ScreenQuadMesh() : MeshBase(Topology::TriangleStrip) {
        Vertex data[4]{
                {vec2(0, 0), vec3(-1, -1, -1), vec3(-1, -1, 1)},
                {vec2(0, 1), vec3(-1, 1, -1),  vec3(-1, 1, 1)},
                {vec2(1, 0), vec3(1, -1, -1),  vec3(1, -1, 1)},
                {vec2(1, 1), vec3(1, 1, -1),   vec3(1, 1, 1)}
        };
        SetCount(4);
        m_vertexBuffer = make_unique<FloatBuffer<Vertex>>(BufferFlags::DynamicStorage, 4, data);
        SetBuffer(0, 2, m_vertexBuffer.get(), 0);
        SetBuffer(1, 3, m_vertexBuffer.get(), 2);
        SetBuffer(2, 3, m_vertexBuffer.get(), 5);
    }

    TerrainMesh::TerrainMesh(int size) : MeshBase(Topology::Triangles) {
        m_size = size;
        int count = size * size;
        int elemCount = (size - 1) * (size - 1) * 6;
        SetCount(elemCount);
        unique_ptr<unsigned int[]> indexes(new unsigned int[elemCount]);
        unique_ptr<ivec2[]> coords(new ivec2[count]);
        unique_ptr<vec2[]> uvs(new vec2[count]);
        int i = 0;
        for (int x = 0; x < size - 1; x++)
            for (int y = 0; y < size - 1; y++) {
                indexes[i++] = (y) * size + x;
                indexes[i++] = (y + 1) * size + x;
                indexes[i++] = y * size + x + 1;
                indexes[i++] = (y + 1) * size + x + 1;
                indexes[i++] = y * size + x + 1;
                indexes[i++] = (y + 1) * size + x;
            }
        for (int x = 0; x < size; x++)
            for (int y = 0; y < size; y++) {
                coords[x + y * size] = {x, y};
                uvs[x + y * size] = {((float) x) / (size - 1), ((float) y) / (size - 1)};
            }
        m_indexes.reset(new UnsignedIntBuffer<unsigned int>(BufferFlags::None, elemCount, indexes.get()));
        m_coord.reset(new IntBuffer<ivec2>(BufferFlags::None, count, coords.get()));
        m_uv.reset(new FloatBuffer<vec2>(BufferFlags::None, count, uvs.get()));
        GetVAO()->Bind();
        GetVAO()->bAttachElementBuffer(m_indexes.get());
        GetVAO()->bAttachBuffer(0, 2, m_coord.get());
        GetVAO()->bAttachBuffer(1, 2, m_uv.get());
        GetVAO()->Unbind();
    }
//endregion

//region Terrain

    Terrain::Terrain(const ivec2 &size, int chunkSize) {
        m_size = size;
        m_chunkSize = chunkSize;
        ivec2 totalSize = size * chunkSize;
        m_offset = {-totalSize.x / 2, 0, -totalSize.y / 2};
        m_plane = make_unique<TerrainMesh>(chunkSize);
        m_heightmap = make_unique<Texture2D>(totalSize, false, TextureFormat::R32F);
        m_normalmap = make_unique<Texture2D>(totalSize, false, TextureFormat::RGBA32F);
        m_heightGen = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>("TerrainGen");
        m_normals = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>("TerrainNormals");
        m_terrainDraw = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>("TerrainDraw");
        m_offsets = make_unique<IntBuffer<ivec2>>(BufferFlags::DynamicStorage, m_size.x * m_size.y, nullptr);
        m_drawRange = 3;
    }

    void Terrain::Generate() {
        ivec2 groupNum = m_chunkSize * m_size / 32;
        m_heightGen->Bind();
        m_heightGen->SetTexture("Heights", m_heightmap.get());
        m_heightGen->bDispatchCompute({groupNum.x, 1, groupNum.y});
        m_heightGen->Unbind();
        Barrier(MemoryBarriers::ShaderImageAccess);
        m_normals->Bind();
        m_normals->SetTexture("Heights", m_heightmap.get());
        m_normals->SetTexture("Normals", m_normalmap.get());
        m_normals->bDispatchCompute({groupNum.x, 1, groupNum.y});
        m_normals->Unbind();
    }

    void Terrain::Draw(Camera *cam) const {
        vec3 campos = (cam->GetLocalPos() - m_offset) / (float) m_chunkSize;
        vec2 camindex = {campos.x, campos.z};
        m_terrainDraw->Bind();
        m_terrainDraw->Set("VP", cam->GetViewProjMatrix());
        m_terrainDraw->SetTexture("Normals", m_normalmap.get());
        m_terrainDraw->SetTexture("Heights", m_heightmap.get());
        m_terrainDraw->Set("Offset", m_offset);
        m_terrainDraw->SetBuffer("Offsets", m_offsets.get());
        m_currentChunks.clear();
        for (int x = 0; x < m_size.x; x++)
            for (int y = 0; y < m_size.y; y++) {
                if (glm::distance(vec2(x, y), camindex) < m_drawRange) {
                    m_currentChunks.push_back(ivec2(x, y) * (m_chunkSize - 1));
                }
            }
        m_offsets->Bind();
        m_offsets->bSetData(m_currentChunks.data(), 0, m_currentChunks.size());
        m_offsets->Unbind();
        m_plane->Draw(m_currentChunks.size());
        m_terrainDraw->Unbind();
    }
//endregion

//region DeferredPBRPipeline

    DeferredPBRPipeline::DeferredPBRPipeline(const ivec2 &frameSize) {
        m_GBufFBO = make_unique<RenderTarget>(frameSize, 4, TextureFormat::RGBA32F, false, true);
        m_LightsFBO = make_unique<RenderTarget>(frameSize, 2, TextureFormat::RGBA32F);
        m_FinalFBO = make_unique<RenderTarget>(frameSize, 1, TextureFormat::RGB8);
        m_lightCompute = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>(
                "LightingCompute");
        m_lightCompose = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>(
                "LightingCompose");
        m_screenQuad = Application::GetInstance()->GetResources()->GetResourceByName<ScreenQuadMesh>("ScreenQuad");
        m_brdflut = Application::GetInstance()->GetResources()->GetResourceByName<Texture2D>("BRDFLUT");
    }

    void DeferredPBRPipeline::Resize(const ivec2 &size) {
        m_GBufFBO.reset(new RenderTarget(size, 4, TextureFormat::RGBA32F, false, true));
        m_LightsFBO.reset(new RenderTarget(size, 2, TextureFormat::RGB32F));
        m_FinalFBO.reset(new RenderTarget(size, 1, TextureFormat::RGB8));
    }

    void DeferredPBRPipeline::Unbind() {
        trmat4 VP = m_cam->GetViewProjMatrix();
        trmat4 IVP = inverse(VP);

        Disable(Feature::CullFace);
        Disable(Feature::DepthTest);
        Disable(Feature::Blend);

        m_LightsFBO->Bind();
        m_lightCompute->Bind();

        m_lightCompute->Set("IVP", IVP);

        m_lightCompute->SetTexture("AlbedoRough", (*m_GBufFBO.get())[0]);
        m_lightCompute->SetTexture("PosDepth", (*m_GBufFBO.get())[1]);
        m_lightCompute->SetTexture("NormalMetal", (*m_GBufFBO.get())[2]);
        m_lightCompute->SetTexture("Emission", (*m_GBufFBO.get())[3]);
        m_lightCompute->SetTexture("BRDFLUT", m_brdflut);

        m_screenQuad->Draw();
        m_lightCompute->Unbind();
        m_LightsFBO->Unbind();

        m_FinalFBO->Bind();
        m_lightCompose->Bind();

        m_lightCompose->Set("IVP", IVP);

        m_lightCompose->SetTexture("AlbedoRough", (*m_GBufFBO.get())[0]);
        m_lightCompose->SetTexture("PosDepth", (*m_GBufFBO.get())[1]);
        m_lightCompose->SetTexture("NormalMetal", (*m_GBufFBO.get())[2]);
        m_lightCompose->SetTexture("Emission", (*m_GBufFBO.get())[3]);

        m_lightCompose->SetTexture("Diffuse", (*m_LightsFBO.get())[0]);
        m_lightCompose->SetTexture("Specular", (*m_LightsFBO.get())[1]);

        m_screenQuad->Draw();
        m_FinalFBO->Unbind();
    }
//endregion

}