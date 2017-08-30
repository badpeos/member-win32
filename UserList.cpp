#define _CRT_SECURE_NO_WARNINGS

#include "UserList.h"
#include "resource.h"
#include "main.h"

#include <deque>
#include <Commctrl.h>

static void updateUserList(HWND hWnd, const std::string &id);

static void processCmd(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, std::string &id)
{
	switch (LOWORD(wParam))
	{
	case IDCANCEL :
		EndDialog(hWnd, 0);
		break;
	case DLG_SELECT :
		{
			HWND hList = GetDlgItem(hWnd, DLG_USERLIST);
			LVITEM item;
			char buffer[1024]={0};
			memset(&item, 0, sizeof(item));

			item.pszText = buffer;
			item.cchTextMax = sizeof(buffer)/sizeof(buffer[0]);

			int iPos = ListView_GetNextItem(hList, -1, LVNI_SELECTED);

			if (iPos != -1)
			{
				SendMessage(hList, LVM_GETITEMTEXT, iPos, (LPARAM) &item);
				id=item.pszText;
				EndDialog(hWnd, 1);
			}
			else
			{
				MessageBox(hWnd, "找不到會員", "錯誤", MB_OK);

//				GetWindowText(GetDlgItem(hWnd, DLG_SEARCHKEY), buffer, sizeof(buffer)/sizeof(buffer[0]));
//				id=buffer;
//				EndDialog(hWnd, 0);
			}			
		}
		break;
	case DLG_SEARCHKEY :
		if (HIWORD(wParam) == EN_CHANGE)
		{
			char buffer[1024]={0};

			GetWindowText(GetDlgItem(hWnd, DLG_SEARCHKEY), buffer, sizeof(buffer)/sizeof(buffer[0]));
			updateUserList(hWnd, std::string(buffer));
		}
		break;
	}
}

static void updateUserList(HWND hWnd, const std::string &id)
{
	std::deque<Customer> customers;
	Database &db = getDatabase();
	db.search(id, customers);

	char buffer[1024]={0};

	HWND hList = GetDlgItem(hWnd, DLG_USERLIST);
	SendMessage(hList, LVM_DELETEALLITEMS, 0, 0);

	LVITEM lvItem;
	memset(&lvItem, 0, sizeof(lvItem));
	lvItem.mask = LVIF_TEXT;
	lvItem.cchTextMax = 256;
	lvItem.iItem = 0;
	lvItem.iSubItem = 0;
	lvItem.pszText = buffer;

	std::deque<Customer>::iterator iter;
	iter = customers.begin();
	while (iter != customers.end())
	{
		sprintf(buffer, "%s", iter->id.c_str());
		lvItem.iSubItem = 0;
		SendMessage(hList, LVM_INSERTITEM, 0, (LPARAM) &lvItem);

		sprintf(buffer, "%s", iter->info.name.c_str());
		lvItem.iSubItem = 1;
		SendMessage(hList, LVM_SETITEM, 0, (LPARAM) &lvItem);

		sprintf(buffer, "%s", iter->info.phone1.c_str());
		lvItem.iSubItem = 2;
		SendMessage(hList, LVM_SETITEM, 0, (LPARAM) &lvItem);

		sprintf(buffer, "%s", iter->info.phone2.c_str());
		lvItem.iSubItem = 3;
		SendMessage(hList, LVM_SETITEM, 0, (LPARAM) &lvItem);

		sprintf(buffer, "%s", iter->info.petName.c_str());
		lvItem.iSubItem = 4;
		SendMessage(hList, LVM_SETITEM, 0, (LPARAM) &lvItem);

		sprintf(buffer, "%ld", iter->point);
		lvItem.iSubItem = 5;
		SendMessage(hList, LVM_SETITEM, 0, (LPARAM) &lvItem);

		iter++;
		lvItem.iItem++;
	}

	ListView_SetItemState (
		hList,         // handle to listview
		0,         // index to listview item
		LVIS_FOCUSED | LVIS_SELECTED, // item state
		LVIS_FOCUSED | LVIS_SELECTED);
}

static void initDialog(HWND hWnd, std::string &id)
{
	SetWindowText(GetDlgItem(hWnd, DLG_SEARCHKEY), id.c_str());
	SendMessage(GetDlgItem(hWnd, DLG_SEARCHKEY), EM_SETSEL, id.length(), id.length());

	std::deque<std::string> userHeader;
	userHeader.push_back("編號       ");
	userHeader.push_back("名稱       ");
	userHeader.push_back("電話1     ");
	userHeader.push_back("電話2     ");
	userHeader.push_back("寵物名稱        ");
	userHeader.push_back("積分    ");
	initListView(hWnd, DLG_USERLIST, userHeader);

	SetFocus(GetDlgItem(hWnd, DLG_SEARCHKEY));
}

BOOL CALLBACK UserListProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static std::string *id;

	switch (msg)
	{
	case WM_INITDIALOG:
		id = (std::string *) lParam;
		initDialog(hWnd, *id);
		updateUserList(hWnd, *id);
		break;

	case WM_COMMAND :
		processCmd(hWnd, msg, wParam, lParam, *id);
		break;
	}
	return FALSE;
}

int usrList(HWND hWnd, std::string &userID)
{
	std::string id(userID);
	if (DialogBoxParam(getInstance(), MAKEINTRESOURCE(IDD_USERLIST), hWnd, UserListProc, (LPARAM) &id))
	{
		userID=id;
		return 1;
	}
	return 0;
}
