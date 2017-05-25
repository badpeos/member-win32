// Member.cpp : Defines the entry point for the application.
//
#pragma warning (disable : 4786)

#include <windows.h>
#include <Commctrl.h>
#include <string>
#include <deque>

#include "main.h"
#include "connector.h"
#include "resource.h"
#include "customer.h"
#include "database.h"

static HINSTANCE gInstance;
static BOOL CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static int saveCustomer(HWND hWnd);

static Database _db;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	INITCOMMONCONTROLSEX ctrls;

	gInstance = hInstance;

	_db.init("test.db");
	Customer customer;
	customer.id="111";
	customer.info.name="abc";
	customer.info.petName="pet";
	customer.info.phone1="ph1";
	customer.info.phone2="ph2";
	customer.info.remark="remark";

	_db.createCustomer(customer);

	ctrls.dwSize=sizeof(INITCOMMONCONTROLSEX);
	ctrls.dwICC=
		ICC_BAR_CLASSES|ICC_COOL_CLASSES|
		ICC_DATE_CLASSES|ICC_HOTKEY_CLASS|
		ICC_LISTVIEW_CLASSES|ICC_PROGRESS_CLASS|
		ICC_TAB_CLASSES|
		ICC_TREEVIEW_CLASSES|ICC_UPDOWN_CLASS|
		ICC_USEREX_CLASSES;
	InitCommonControlsEx(&ctrls);

	DialogBox(getInstance(), MAKEINTRESOURCE(IDD_MAINNEW), NULL, (DLGPROC) MainProc);

	return 0;
}

HINSTANCE getInstance(void)
{
	return gInstance;
}

static BOOL initMainDlg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

static BOOL cmdMainDlg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char buffer[1024]={0};
	std::string string;

	switch (LOWORD(wParam))
	{
	case IDOK:
		EndDialog(hWnd, 0);
		break;
	case DLG_SEARCH :
		GetWindowText(GetDlgItem(hWnd, DLG_SEARCHKEY), 
			buffer, sizeof(buffer)/sizeof(buffer[0]));
		string=buffer;
		//searchRecordFromDB(string);
		break;
	case DLG_NEWCUSTOMER :
		break;
	case DLG_NEWSAVE :
		saveCustomer(hWnd);
		break;
	case DLG_SALESNEW :
		break;
	case DLG_SALESSAVE :
		break;
	case DLG_SALESCOUPONS :
		break;
	case DLG_SALESSINGLE :
		break;
	case DLG_SALESSALES :
		break;
	case DLG_SALESDAY :
		break;
	case DLG_HISTORYALL :
		break;
	case DLG_HISTORYCOUPONS :
		break;
	case DLG_HISTORYSINGLE :
		break;
	case DLG_HISTORYSALES :
		break;
	case DLG_HISTORYDAY :
		break;
	}
	return FALSE;
}

static BOOL CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		initMainDlg(hWnd, msg, wParam, lParam);
		break;

	case WM_COMMAND:
		cmdMainDlg(hWnd, msg, wParam, lParam);
		break;

	case WM_DESTROY:
		break;
	}
	return FALSE;
}

static int saveCustomer(HWND hWnd)
{
	char buffer[2048]={0};
	Customer customer;
	struct Info info;

	GetWindowText(GetDlgItem(hWnd, DLG_NAME), buffer, sizeof(buffer)/sizeof(buffer[0]));
	info.name=buffer;

	GetWindowText(GetDlgItem(hWnd, DLG_PHONE1), buffer, sizeof(buffer)/sizeof(buffer[0]));
	info.phone1=buffer;
	
	GetWindowText(GetDlgItem(hWnd, DLG_PHONE2), buffer, sizeof(buffer)/sizeof(buffer[0]));
	info.phone2=buffer;

	GetWindowText(GetDlgItem(hWnd, DLG_PETNAME), buffer, sizeof(buffer)/sizeof(buffer[0]));
	info.petName=buffer;

	GetWindowText(GetDlgItem(hWnd, DLG_REMARK), buffer, sizeof(buffer)/sizeof(buffer[0]));
	info.remark=buffer;

	//customer.init(info);
	customer.writeInfo(_db);

	return 0;
}
