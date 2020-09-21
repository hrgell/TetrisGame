#include "stdafx.h"
#include "BasicResources.h"
#include "Util.h"
#include "Resource.h"

BasicResources::BasicResources()
	:
	appdata(GetAppdata()),
	// colors
	color_default{ 0, 0, 0 },
	color_clear_to{ 0, 0, 0 },
	color_bg{ 0, 0, 0 },
	color_white{ 0.99, 0.99, 0.99 },
	color_gray{ 0.8, 0.8, 0.8 },
	color_1{ 0.99, 0, 0 },
	color_2{ 0.99, 0.99, 0 },
	color_3{ 0, 0.99, 0 },
	color_4{ 0, 0, 0.99 },
	color_5{ 0, 0.99, 0.99 },
	color_6{ 0.99, 0.49, 0 },
	color_7{ 0.99, 0, 0.99 },
	color_8{ 0.09, 0.52, 0.9 },
	color_9{ 0.7, 0.9, 0.84 },
	// fonts
	fontname_arial("C:\\Windows\\Fonts\\arial.ttf"),
	fontname_verdana("C:\\Windows\\Fonts\\verdana.ttf"),
	fontname_fixed("C:\\Windows\\Fonts\\cour.ttf"),
	// music
	wavname_theme1(appdata + "Theme1.wav"),
	wavname_theme2(appdata + "Theme2.wav"),
	wavname_theme3(appdata + "Theme3.wav"),
	wavname_theme4(appdata + "Theme4.wav"),
	wavname_theme5(appdata + "Theme5.wav"),
	wavname_theme6(appdata + "Theme6.wav"),
	wavname_theme7(appdata + "Theme7.wav"),
	wavname_theme8(appdata + "Theme8.wav"),
	wavname_theme9(appdata + "Theme9.wav"),
	wavname_tada("C:\\Windows\\media\\tada.wav"),
	shape_types({
		{ 0, 4, {{0,2}, {1,2}, {2,2}, {3,2} }, color_1 },
		{ 1, 3, {{0,2}, {0,1}, {1,1}, {2,1} }, color_2 },
		{ 2, 3, {{2,2}, {0,1}, {1,1}, {2,1} }, color_3 },
		{ 3, 2, {{0,1}, {1,1}, {0,0}, {1,0} }, color_4 },
		{ 4, 3, {{1,2}, {2,2}, {0,1}, {1,1} }, color_5 },
		{ 5, 3, {{1,2}, {0,1}, {1,1}, {2,1} }, color_6 },
		{ 6, 3, {{0,2}, {1,2}, {1,1}, {2,1} }, color_7 }
		}),
	oddshape_types({ { 7, 3, {{0,2}, {0,1}, {0,0}, {1,0}, {2,0}}, color_8 }, }),
	debris_types({ { 8, 1, {{0,0}}, color_1 }, })
{
} // constructor

BasicResources::~BasicResources()
{
} // destructor

// Returns the path to the folder we extract resources into.
std::string BasicResources::GetAppdata()
{
	const size_t siz = BUFSIZ;
	size_t len = siz;
	char *val;
	errno_t err = _dupenv_s(&val, &len, "APPDATA");
	if (err != 0)
		return "";
	if (val == NULL || len == 0)
		return "";
	std::string folder_name = string_format("%s\\Tetris", val);
	free(val);
	// if _mkdir == -1 and errno == EEXIST then all is ok.
	auto res = _mkdir(folder_name.c_str());
	if(res == -1 && errno != EEXIST)
		return "";
	if (!FolderExists(folder_name))
		return "";
	return folder_name + "\\";
}


