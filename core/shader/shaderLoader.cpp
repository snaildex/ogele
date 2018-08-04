//
// Created by ??????? on 21.07.2018.
//

#include "../ogele.h"
#include <regex>

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {
    map<string, ShaderType> ShaderLoader::m_shaderTypeMap = {
            {"compute",        ShaderType::Compute},
            {"fragment",       ShaderType::Fragment},
            {"geometry",       ShaderType::Geometry},
            {"tessControl",    ShaderType::TessControl},
            {"tessEvaluation", ShaderType::TessEvaluation},
            {"vertex",         ShaderType::Vertex}
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

    string ReadFile(const string &path) {
        ifstream srcFile(path);
        if (!srcFile.is_open())
            cout << "\n[ShaderLoader] Failed to include "<<path;
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

    ogele::Resource *ShaderLoader::Load(const Jzon::Node *reader) const {
        list<unique_ptr<ShaderStage>> stages;
        ShaderProgram *res = new ShaderProgram();
        LoadNameTags(reader, res);
        for (const auto stage : reader->get("stages")) {
            string source = ReadFile(stage.second.toString());
            source = ApplyIncludes(source);
            stages.emplace_back(make_unique<ShaderStage>(m_shaderTypeMap[stage.first], source));
        }
        for (const auto &s : stages)
            res->AttachStage(s.get());
        res->Link();
        for (const auto &s : stages)
            res->DetachStage(s.get());
        return res;
    }
}
