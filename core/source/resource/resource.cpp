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

	void ResourceLoader::LoadNameTags(const Jzon::Node *reader, ResourceProxy *res) const {
		res->SetName(reader->get("name").toString());
		if (reader->has("tags")) {
			Jzon::Node t = reader->get("tags");
			for (int i = 0; i < t.getCount(); i++)
				res->AddTag(t.get(i).toString());
		}
	}

	string ResourceLoader::ReadStringProperty(const Jzon::Node *reader, const string &propName, const string &defaultValue) const {
		if (reader->has(propName))
			return reader->get(propName).toString();
		else return defaultValue;
	}
	std::vector<std::string> ResourceLoader::ReadStringArrayProperty(const Jzon::Node *reader, const std::string &propName) const {
		std::vector<std::string> res;
		if (reader->has(propName)) {
			Jzon::Node node = reader->get(propName);
			if (node.isArray())
				for (size_t i = 0; i < node.getCount(); ++i)
					res.push_back(node.get(i).toString());
			else
				res.push_back(node.toString());
		}
		return res;
	}

	int ResourceLoader::ReadIntProperty(const Jzon::Node *reader, const string &propName, int defaultValue) const {
		if (reader->has(propName))
			return reader->get(propName).toInt();
		else return defaultValue;
	}

	float ResourceLoader::ReadFloatProperty(const Jzon::Node *reader, const string &propName, float defaultValue) const {
		if (reader->has(propName))
			return reader->get(propName).toFloat();
		else return defaultValue;
	}

	double ResourceLoader::ReadDoubleProperty(const Jzon::Node *reader, const string &propName, double defaultValue) const {
		if (reader->has(propName))
			return reader->get(propName).toDouble();
		else return defaultValue;
	}

	bool ResourceLoader::ReadBoolProperty(const Jzon::Node *reader, const string &propName, bool defaultValue) const {
		if (reader->has(propName))
			return reader->get(propName).toBool();
		else return defaultValue;
	}

}