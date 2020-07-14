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

	bool CheckTail(const std::string & str, const std::string & tail)
	{
		if (str.length() < tail.length()) return false;
		return strncmp(&str[str.length() - tail.length()], tail.data(), tail.length()) == 0;
	}

	std::string StripTail(const std::string& str, size_t tailLength)
	{
		return str.substr(0, str.length() - tailLength);
	}

	int StripTailNum(const std::string& str)
	{
		size_t i = str.length() - 1;
		if (!isdigit(str[i])) return -1;
		while (isdigit(str[i--]));
		return atoi(&str.data()[i + 1]);
	}

	int StripTailNum(const std::string& str, std::string& leftover)
	{
		size_t i = str.length() - 1;
		if (!isdigit(str[i])) return -1;
		while (isdigit(str[i--]));
		leftover = str.substr(0, i + 2);
		return atoi(&str.data()[i + 1]);
	}

	mutex LogLock;

	int LogIndentLevel;
	void ___LogIndent() { lock_guard<mutex> g(LogLock); LogIndentLevel++; }
	void ___LogUnindent() { lock_guard<mutex> g(LogLock); LogIndentLevel--; }

	ofstream LogFile("Log.txt");
	thread_local char LogBuf[4096];
	void ___Log(const char * file, int line, const char * fmt, ...)
	{
		lock_guard<mutex> g(LogLock);
		char buf[64];
		sprintf(buf, "%s:%03d,", file, line);
		if (fmt[0] == '[')
			sprintf(LogBuf, "[%-32s th %05d]", buf, std::this_thread::get_id());
		else
			sprintf(LogBuf, "[%-32s th %05d] ", buf, std::this_thread::get_id());
		LogFile << LogBuf;
		for (int i = 0; i < LogIndentLevel; ++i) LogFile << "    ";
		va_list vargs;
		va_start(vargs, fmt);
		vsprintf(LogBuf, fmt, vargs);
		va_end(vargs);
		LogFile << LogBuf << std::endl;
		std::cout << LogBuf << std::endl;
	}

	void LogSpace()
	{
		lock_guard<mutex> g(LogLock);
		LogFile << std::endl;
	}
}