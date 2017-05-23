// Member.cpp : Defines the entry point for the application.
//

#include <windows.h>
#include "main.h"
#include "resource.h"

static HINSTANCE gInstance;
static BOOL CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	gInstance = hInstance;

	DialogBox(getInstance(), MAKEINTRESOURCE(IDD_MAINNEW), NULL, (DLGPROC) MainProc);

	return 0;
}

HINSTANCE getInstance(void)
{
	return gInstance;
}

static BOOL CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hWnd, 0);
			break;
		}
		break;

	case WM_DESTROY:
		break;
	}
	return FALSE;
}
