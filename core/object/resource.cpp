//
// Created by ??????? on 19.07.2018.
//
#include "../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele{

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

}