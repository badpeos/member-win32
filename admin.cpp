#include "admin.h"
#include "resource.h"
#include "main.h"
#include "database.h"

#include <fstream>
#include <string>

static int getOpenFileName(HWND hWnd, std::string &path)
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260] = { 0 };       // buffer for file name
	//HANDLE hf;              // file handle

							// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "";// "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 0;// 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_DONTADDTORECENT | OFN_OVERWRITEPROMPT;
	//OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == TRUE)
	{
		path = ofn.lpstrFile;
	}
	return path.length();
}

static bool resetPoint()
{
	Database &db = getDatabase();
	return db.resetPoint();
}

static bool exportPoint(const std::string &path)
{
	Database &db = getDatabase();
	std::map<std::string, struct Point> memberPoint;

	db.readPoint(memberPoint);

	std::ofstream of(path.c_str());
	of << "id" << ", " << "name" << ", " << "points" << std::endl;

	for (auto member : memberPoint)
	{
		std::string id = member.first;
		struct Point point = member.second;
		of << id << ", " << point.name << ", " << point.point << std::endl;
	}
	of.close();

	return true;
}

static BOOL CALLBACK AdminProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		break;

	case WM_COMMAND :
		switch (LOWORD(wParam))
		{
		case DLG_RESETPOINT:
			resetPoint();
			break;
		case DLG_EXPORTPOINT:
			{
				std::string path;
				if (getOpenFileName(hWnd, path))
				{
					exportPoint(path);
				}
			}
			break;
		case IDOK:
			EndDialog(hWnd, 0);
			break;
		}
		break;
	}

	return FALSE;
}

int adminPage(HWND hWnd)
{
	DialogBox(getInstance(), MAKEINTRESOURCE(IDD_ADMIN), hWnd, AdminProc);

	return 0;
}
