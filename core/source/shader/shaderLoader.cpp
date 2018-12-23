#include <stdafx.h>
#include <ogele.h>
using namespace std;
namespace ogele {
    map<string, ShaderType> ShaderLoader::m_shaderTypeMap = {
            {"compute",        ShaderType::Compute},
            {"fragment",       ShaderType::Fragment},
            {"geometry",       ShaderType::Geometry},
            {"tessControl",    ShaderType::TessControl},
            {"tessEvaluation", ShaderType::TessEvaluation},
            {"vertex",         ShaderType::Vertex}
    };

    ShaderProgram *CreateShaderProgram(const string &name, const string &vertex, const string &fragment) {
        ShaderProgram *res = new ShaderProgram();
        res->SetName(name);
        res->AddTag("Internal");
        auto vertexStage = make_unique<ShaderStage>(name, ShaderType::Vertex, vertex);
        auto fragmentStage = make_unique<ShaderStage>(name, ShaderType::Fragment, fragment);
        res->AttachStage(vertexStage.get());
        res->AttachStage(fragmentStage.get());
        res->Link();
        res->DetachStage(vertexStage.get());
        res->DetachStage(fragmentStage.get());
        return res;
    }

    ShaderProgram *CreateShaderProgram(const string &name, const string &compute) {
        ShaderProgram *res = new ShaderProgram();
        res->SetName(name);
        res->AddTag("Internal");
        auto computeStage = make_unique<ShaderStage>(name, ShaderType::Compute, compute);
        res->AttachStage(computeStage.get());
        res->Link();
        res->DetachStage(computeStage.get());
        return res;
    }

    string ReadFile(const string &path) {
        ifstream srcFile(path);
        if (!srcFile.is_open())
            cerr << "\n[ShaderLoader] Failed to open "<<path<<'\n';
        stringstream buffer;
        buffer << srcFile.rdbuf();
        return buffer.str();
    }

    string ApplyIncludes(const string &source) {
        stringstream result;
        regex inclReg(R"(#include\s*\".*\")");
        for (sregex_token_iterator i{source.begin(), source.end(), inclReg, {-1, 0}}, end;
             i != end; ++i) {
            string token = *i;
            if (regex_match(token, inclReg)) {
                unsigned long long int pos = token.find('"');
                string path = token.substr(pos + 1, token.length() - pos - 2);
                result << ReadFile(path);
            } else
                result << token;
        }
        return result.str();
    }

    ResourceProxy* ShaderLoader::Load(const Jzon::Node *reader) const {
        ShaderProgram::Proxy *res = new ShaderProgram::Proxy();
        LoadNameTags(reader, res);
        for (const auto stage : reader->get("stages"))
            res->AddStage(m_shaderTypeMap[stage.first], fs::absolute(stage.second.toString()));
        return res;
    }

    Resource * ShaderProgram::Proxy::Build() const
    {
        ShaderProgram* res = new ShaderProgram();
        res->CopyNameTagsFrom(this);
        list<unique_ptr<ShaderStage>> stages;
        for (const auto& stage : m_stages) {
            fs::path path=fs::current_path();
            fs::current_path(stage.second.parent_path());
            string source = ReadFile(stage.second.string());
            source = ApplyIncludes(source);
            stages.emplace_back(make_unique<ShaderStage>(res->GetName(), stage.first, source));
            fs::current_path(path);
        }
        for (const auto &s : stages)
            res->AttachStage(s.get());
        res->Link();
        for (const auto &s : stages)
            res->DetachStage(s.get());
        return res;
    }
}
