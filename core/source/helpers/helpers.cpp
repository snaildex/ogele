#include <stdafx.h>
#include <ogele.h>
using namespace std;
namespace fs = std::filesystem;
namespace ogele {
    list<fs::path> ScanFiles(const string &extension) {
        list<fs::path> files;
        for (const auto &p : fs::recursive_directory_iterator(
                fs::current_path())) {
            const string &ext = p.path().extension().string();
            if (ext == extension)
                files.push_back(p.path());
        }
        return files;
    }
}