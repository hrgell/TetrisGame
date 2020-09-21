#include "stdafx.h"
#include "Util.h"
#include "BasicResources.h"

using namespace std;

// todo fix the random device to make it more random
random_device my_random_device;
mt19937 my_random(my_random_device());

size_t DiceRoll(size_t max) {
	uniform_int_distribution<mt19937::result_type> dist(1, static_cast<unsigned int>(max));
	return dist(my_random);
}

// information
void DebugWriteMsg(const wstring arg) {
	wostringstream oss;
	oss << "---- ";
	oss << arg;
	//oss << " ----";
	oss << std::endl;
	OutputDebugStringW(oss.str().c_str());
}

void DebugWriteMsg(const string arg) {
	wostringstream oss;
	oss << "---- ";
	oss << arg.c_str();
	//oss << " ----";
	oss << std::endl;
	OutputDebugStringW(oss.str().c_str());
}

void DebugWriteMsg(wostringstream& oss) {
	DebugWriteMsg(oss.str());
}

template <class T> void DebugWriteMsg(const T arg) {
	DebugWriteMsg(to_string(arg));
}

bool FileExists(std::string& filename) {
	struct stat fs;
	return (stat(filename.c_str(), &fs) == 0);
}

bool FolderExists(std::string& foldername)
{
	struct stat fs;
	stat(foldername.c_str(), &fs);
	return (fs.st_mode & S_IFDIR);
}

size_t WriteFile(const char* filename, const char* mode, const char* data, const size_t siz)
{
	size_t written = 0;
	FILE* file;
	errno_t err = fopen_s(&file, filename, "wb");
	if (err == 0 && file != nullptr) {
		size_t num = fwrite(data, 1, sizeof(char) * siz, file);
		if (errno == 0) {
			written += num;
		}
		// todo handle when fwrite() does not write all the data in one go, but subsequent writes will.
		fclose(file);
	}
	return written;
}

// Returns the path to the folder we extract resources into.
inline string GetAppdata()
{
	static string path = "";
	if (!path.empty())
		return path;
	const size_t siz = BUFSIZ;
	size_t len = siz;
	char* val;
	errno_t err = _dupenv_s(&val, &len, "APPDATA");
	if (err != 0)
		return "";
	if (val == nullptr || len == 0)
		return "";
	string folder_name = string_format("%s\\Tetris", val);
	free(val);
	static_cast<void>(_mkdir(folder_name.c_str()));
	if (!FolderExists(folder_name))
		return "";
	path = folder_name + "\\";
	return path;
}

// Extract the embedded data identified by [id] and [typeinfo] to the specified file [filename].
long Extract(size_t id, std::string filename, const char* typeinfo)
{
	//DebugWriteMsg(string_format("id: %u,  filename == %s", (USHORT)id, filename.c_str()));
	if (id == 0)
		return 0;
	/* According to MSDN the acquisition of the handles below does not leak any
	   memory. The handles below refer to static data, so no "frees" are needed. */
	if (filename.empty()) {
		DebugWriteMsg(string_format("Failed to extract file, filename not specified: %u", (USHORT)id));
		return 1;
	}
	if (FileExists(filename))
		return 0;
	HMODULE module = GetModuleHandleA(nullptr);
	if (module == 0) {
		DebugWriteMsg(string_format("%s", "Failed to load module: %u: %s", (USHORT)id, filename.c_str()));
		return 2;
	}
	HRSRC res = FindResourceA(module, MAKEINTRESOURCEA(id), typeinfo);
	if (res == nullptr) {
		DebugWriteMsg(string_format("Failed to find resource: %u: %s", (USHORT)id, filename.c_str()));
		return 3;
	}
	HGLOBAL ptr = LoadResource(module, res);
	if (ptr == nullptr) {
		DebugWriteMsg(string_format("%s", "Failed to load resource: %u: %s", (USHORT)id, filename.c_str()));
		return 4;
	}
	DWORD siz = SizeofResource(module, res);
	if (siz == 0) {
		DebugWriteMsg(string_format("%s", "Failed to load size of resource: %u: %s", (USHORT)id, filename.c_str()));
		return 5;
	}
	LPVOID src = LockResource(ptr);
	if (src == nullptr) {
		DebugWriteMsg(string_format("%s", "Failed to access resource: %u: %s", (USHORT)id, filename.c_str()));
		return 6;
	}
	WriteFile(filename.c_str(), "wb", static_cast<char*>(src), siz * sizeof(char));
	if (!FileExists(filename)) {
		DebugWriteMsg(string_format("%s", "Failed to write file: %u: %s", (USHORT)id, filename.c_str()));
		return 7;
	}
	return 0;
} // method

// Extract the embedded data identified by [id] and [typeinfo] to the specified file [filename].
long ExtractNE(size_t id, std::string filename, const char* typeinfo)
{
	if (id == 0)
		return 0; // ok
	if (filename.empty())
		return -1;
	HMODULE module = GetModuleHandleA(nullptr); // the module of this process
	if (module == 0)
		return -2;
	HRSRC res = FindResourceA(module, MAKEINTRESOURCEA(id), typeinfo);
	if (res == nullptr)
		return -3;
	HGLOBAL ptr = LoadResource(module, res);
	if (ptr == NULL)
		return -4;
	DWORD siz = SizeofResource(module, res);
	if (siz == 0)
		return -5;
	LPVOID src = LockResource(ptr);
	if (src == NULL)
		return -6;
	WriteFile(filename.c_str(), "wb", static_cast<char*>(src), siz * sizeof(char));
	return 0;
} // method

void DisplayFrame() {
	al_flip_display();
	al_clear_to_color(BasicResources::Instance().color_clear_to);
}

int ErrMsg(const char* msg, int status) {
	al_show_native_message_box(nullptr, "Error", "Error", msg, nullptr, ALLEGRO_MESSAGEBOX_ERROR);
	return status;
}

BOOL EnumNamesFunc(HMODULE hModule, LPCTSTR lpType, LPTSTR lpName, LONG lParam);

// Resource language callback
BOOL EnumLangsFunc(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, WORD wLang, LONG lParam)
{
	HRSRC info = FindResourceEx(hModule, lpType, lpName, wLang);
	if (info == 0)
		return FALSE;
	size_t siz = SizeofResource(hModule, info);
	if (siz == 0)
		return FALSE;
#pragma warning( suppress: 4302 )
	DebugWriteMsg(string_format("\t\tLanguage: %u,  hResInfo == %lx,  Size == %lu\r\n", (USHORT)wLang, info, siz));
	return TRUE;
}

// Resource name callback
BOOL EnumNamesFunc(HMODULE hModule, LPCTSTR lpType, LPTSTR lpName, LONG lParam)
{
	if (!IS_INTRESOURCE(lpName))
	{
		//lpName[bufsiz - 1] = '\0';
		DebugWriteMsg(string_format("\tName: %s", lpName));
	}
	else
	{
#pragma warning( suppress: 4302 )
		DebugWriteMsg(string_format("\tName: %u", (USHORT)lpName));
	}
	EnumResourceLanguages(hModule, lpType, lpName, (ENUMRESLANGPROC)EnumLangsFunc, 0);
	return TRUE;
}

// Resource type callback
BOOL EnumTypesFunc(HMODULE hModule, LPTSTR lpType, LONG lParam)
{
	if (!IS_INTRESOURCE(lpType))
	{
		DebugWriteMsg(string_format("Type: %s", lpType));
	}
	else
	{
#pragma warning( suppress: 4302 )
		DebugWriteMsg(string_format("Type: %u", USHORT(lpType)));
	}
	EnumResourceNames(hModule, lpType, (ENUMRESNAMEPROC)EnumNamesFunc, 0);
	return TRUE;
}

bool EnumerateResources() {
	HMODULE module = GetModuleHandleA(nullptr);
	if (module == 0)
		return false;
	BOOL rst = EnumResourceTypes(module, (ENUMRESTYPEPROC)EnumTypesFunc, 0);
	// FreeLibrary(module);
	return rst;
} // method
