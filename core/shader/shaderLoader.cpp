//
// Created by ??????? on 21.07.2018.
//

#include "../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {
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
}
