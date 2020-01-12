#include <stdafx.h>
#include <ogele.h>
using namespace std;
namespace ogele {

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

	Resource* ResourcePtrBase::Get() {
		Wait();
		return m_resource.get();
	}
	void ResourcePtrBase::BuildAsync() {
		m_future = move(Application::LoadAsync(m_proxy.get()));
	}
	void ResourcePtrBase::Wait() {
		if (!m_resource) {
			if (m_future.valid()) {
				m_resource.reset(m_future.get());
			}
			else
				Build();
		}
	}

	void ResourceLoader::LoadNameTags(ResourceProxy* res, const fs::path& path) const {
		res->SetName(path.filename().string());
		fs::path p = Application::RelativePath(path.parent_path());
		while (!p.empty()) {
			res->AddTag(p.filename().string().c_str());
			p = p.parent_path();
		}
	}

}