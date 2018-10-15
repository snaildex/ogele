#include <object/resource.h>
#include <shader/shaderLoader.h>
#include <texture/texture2D/texture2DLoader.h>
#include <texture/textureCube/textureCubeLoader.h>
#include <imgui/imgui.h>
#include <sstream>
#include <string>

using namespace std;

namespace ogele {

    ResourceContainer::ResourceContainer() {
        m_loaders["Shader"] = make_unique<ShaderLoader>();
        m_loaders["Texture2D"] = make_unique<Texture2DLoader>();
        m_loaders["TextureCube"] = make_unique<TextureCubeLoader>();
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

    void ResourceLoader::LoadNameTags(const Jzon::Node *reader, Resource *res) const {
        res->SetName(reader->get("name").toString());
        if (reader->has("tags")) {
            Jzon::Node t = reader->get("tags");
            for (int i = 0; i < t.getCount(); i++)
                res->AddTag(t.get(i).toString());
        }
    }

    void ResourceContainer::LoadFromFilesystem() {
        fs::path cpath = fs::current_path();
        Jzon::Parser parser;
        list<fs::path> configs = ScanFiles(".json");
        for (const auto &p : configs) {
            Jzon::Node node = parser.parseFile(p.string());
            if (node.isValid()) {
                fs::current_path(p.parent_path());
                for (int i = 0; i < node.getCount(); i++) {
                    Jzon::Node cnode = node.get(i);
                    AddResource(m_loaders[cnode.get("type").toString()]->Load(&cnode));
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