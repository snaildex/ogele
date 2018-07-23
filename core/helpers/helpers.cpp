//
// Created by ??????? on 18.07.2018.
//

#include "../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {
    list<fs::path> ScanFiles(const string &extension) {
        list<fs::path> files;
        for (const auto &p : experimental::filesystem::v1::__cxx11::recursive_directory_iterator(
                experimental::filesystem::v1::current_path())) {
            const string &ext = p.path().extension().string();
            if (ext == extension)
                files.push_back(p.path());
        }
        return files;
    }
}