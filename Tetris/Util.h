#pragma once

// random generator
extern void DebugWriteMsg(const std::wstring arg);
extern void DebugWriteMsg(const std::string arg);
extern void DebugWriteMsg(std::wostringstream &oss);
extern size_t DiceRoll(size_t max);
extern bool FileExists(std::string &filename);
extern bool FolderExists(std::string &foldername);
extern size_t WriteFile(const char *filename, const char *mode, const char *data, const size_t siz);
extern std::string GetAppdata();
extern void Extract(size_t id, std::string filename, const char *typeinfo);
extern long ExtractNE(size_t id, std::string filename, const char *typeinfo);
extern void DisplayFrame(); // move the method elsewhere
extern int ErrMsg(const char *msg, int status); // move the method elsewhere
extern bool EnumerateResources();

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	// TODO fix for locales
	auto size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1);
}

//random_device dev;
//mt19937 gen(dev());
