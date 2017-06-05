// Member.cpp : Defines the entry point for the application.
//
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable : 4786)

#include <windows.h>
#include <time.h>
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
static int searchCustomer(HWND hWnd);
static int initCouponType(HWND hWnd);
static int initCouponUsageType(HWND hWnd);
static int initSingleType(HWND hWnd);
static int initSingleType(HWND hWnd);
static int initDaycareType(HWND hWnd);

static int newSales(HWND hWnd);

static int initListView(HWND hWnd, UINT ctrlID, std::deque<std::string> &header);
static int updateCouponList(HWND hWnd, struct Coupons &coupon);
static int useCoupon(HWND hWnd);
static int updatePoint(HWND hWnd);
static Database _db;

static const char* getBasePath(void)
{
	static std::string _basePath;

	if (_basePath.length()==0)
	{
		char basePath[_MAX_PATH];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char filename[_MAX_FNAME];
		char ext[_MAX_EXT];

		GetModuleFileName(NULL, basePath, sizeof(basePath)/sizeof(basePath[0]));
		_splitpath(basePath, drive, dir, filename, ext);
		_makepath(basePath, drive, dir, NULL, NULL);

		strcat(basePath, basePath[strlen(basePath)-1]=='\\'?"":"\\");

		_basePath=basePath;
	}

	return _basePath.c_str();
}

static int getDate(std::string &date)
{
	time_t ltime;
	struct tm stime;
	char buffer[1024];

	time(&ltime);
	stime = *localtime(&ltime);

	sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d", 
		stime.tm_year+1900, stime.tm_mon+1, stime.tm_mday, 
		stime.tm_hour, stime.tm_min, stime.tm_sec);

	date = buffer;
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	INITCOMMONCONTROLSEX ctrls;
	std::string date;

	gInstance = hInstance;

	_db.init("test.db");
	getBasePath();
	getDate(date);

	Customer customer;
	customer.id="111";
	customer.info.name="abc";
	customer.info.petName="pet";
	customer.info.phone1="ph1";
	customer.info.phone2="ph2";
	customer.info.remark="remark";

	_db.createCustomer(customer, date);

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

static void enableWindows(HWND hWnd, bool enable)
{
	BOOL status = enable ? TRUE : FALSE;
	EnableWindow(GetDlgItem(hWnd, DLG_NAME), status);
	EnableWindow(GetDlgItem(hWnd, DLG_ID), status);
	EnableWindow(GetDlgItem(hWnd, DLG_PHONE1), status);
	EnableWindow(GetDlgItem(hWnd, DLG_PHONE2), status);
	EnableWindow(GetDlgItem(hWnd, DLG_PETNAME), status);
	EnableWindow(GetDlgItem(hWnd, DLG_POINTBAL), status);
	EnableWindow(GetDlgItem(hWnd, DLG_REMARK), status);
	EnableWindow(GetDlgItem(hWnd, DLG_NEWSAVE), status);
	EnableWindow(GetDlgItem(hWnd, DLG_COUPONDATE), status);
	EnableWindow(GetDlgItem(hWnd, DLG_COUPONTYPE), status);
	EnableWindow(GetDlgItem(hWnd, DLG_COUPONQTY), status);
	EnableWindow(GetDlgItem(hWnd, DLG_COUPONUSE), status);
	EnableWindow(GetDlgItem(hWnd, DLG_COUPONBAL), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESCOUPONS), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESCOUPONSTYPE), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESCOUPONSQTY), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESCOUPONSPRICE), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESCOUPONSREMARK), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESSINGLE), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESSINGLETYPE), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESSINGLEPRICE), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESSINGLEREMARK), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESSALES), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESSALESPRICE), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESSALESREMARK), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESDAY), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESDAYQTY), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESDAYNAME), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESDAYPRICE), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESDAYREMARK), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESNEW), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESSAVE), status);
	EnableWindow(GetDlgItem(hWnd, DLG_HISTORY), status);
	EnableWindow(GetDlgItem(hWnd, DLG_HISTORYALL), status);
	EnableWindow(GetDlgItem(hWnd, DLG_HISTORYCOUPONS), status);
	EnableWindow(GetDlgItem(hWnd, DLG_HISTORYSINGLE), status);
	EnableWindow(GetDlgItem(hWnd, DLG_HISTORYSALES), status);
	EnableWindow(GetDlgItem(hWnd, DLG_HISTORYDAY), status);
	EnableWindow(GetDlgItem(hWnd, DLG_SALESDATE), status);
}

static void newCustomerClick(HWND hWnd)
{
	BOOL status = TRUE;
	EnableWindow(GetDlgItem(hWnd, DLG_NAME), status);
	EnableWindow(GetDlgItem(hWnd, DLG_ID), status);
	EnableWindow(GetDlgItem(hWnd, DLG_PHONE1), status);
	EnableWindow(GetDlgItem(hWnd, DLG_PHONE2), status);
	EnableWindow(GetDlgItem(hWnd, DLG_PETNAME), status);
	EnableWindow(GetDlgItem(hWnd, DLG_POINTBAL), status);
	EnableWindow(GetDlgItem(hWnd, DLG_REMARK), status);
	EnableWindow(GetDlgItem(hWnd, DLG_NEWSAVE), status);

	SetWindowText(GetDlgItem(hWnd, DLG_NAME), "");
	SetWindowText(GetDlgItem(hWnd, DLG_ID), "");
	SetWindowText(GetDlgItem(hWnd, DLG_PHONE1), "");
	SetWindowText(GetDlgItem(hWnd, DLG_PHONE2), "");
	SetWindowText(GetDlgItem(hWnd, DLG_PETNAME), "");
	SetWindowText(GetDlgItem(hWnd, DLG_POINTBAL), "");
	SetWindowText(GetDlgItem(hWnd, DLG_REMARK), "");
}

static BOOL initMainDlg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	enableWindows(hWnd, false);
	initCouponType(hWnd);
	initSingleType(hWnd);
	initDaycareType(hWnd);

	std::deque<std::string> historyHeader;
	std::deque<std::string> couponHeader;

	historyHeader.push_back("Date       ");
	historyHeader.push_back("Type    ");
	historyHeader.push_back("Sub-type  ");
	historyHeader.push_back("Quantity");
	historyHeader.push_back("Price     ");
	historyHeader.push_back("Remark                         ");
	initListView(hWnd, DLG_HISTORY, historyHeader);

	couponHeader.push_back ("Date       ");
	couponHeader.push_back ("Type    ");
	couponHeader.push_back ("Quantity");
	initListView(hWnd, DLG_COUPONBAL, couponHeader);

	return FALSE;
}

static BOOL cmdMainDlg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char buffer[1024]={0};
	std::string string;

	switch (LOWORD(wParam))
	{
	case IDOK:
//		EndDialog(hWnd, 0);
		break;
	case DLG_QUIT:
		EndDialog(hWnd, 0);
		break;
	case DLG_SEARCH :
		GetWindowText(GetDlgItem(hWnd, DLG_SEARCHKEY), 
			buffer, sizeof(buffer)/sizeof(buffer[0]));
		string=buffer;
		searchCustomer(hWnd);
		initCouponUsageType(hWnd);
		//searchRecordFromDB(string);
		break;
	case DLG_NEWCUSTOMER :
		newCustomerClick(hWnd);
		break;
	case DLG_NEWSAVE :
		saveCustomer(hWnd);
		break;
	case DLG_COUPONUSE:
		useCoupon(hWnd);
		break;
	case DLG_SALESNEW :
		break;
	case DLG_SALESSAVE :
		newSales(hWnd);
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

static int displayCustomer(HWND hWnd, Customer customer)
{
	char buffer[512];

	sprintf (buffer, "%s", customer.id.c_str());
	SetWindowText(GetDlgItem(hWnd, DLG_ID), buffer);

	sprintf(buffer, "%s", customer.info.name.c_str());
	SetWindowText(GetDlgItem(hWnd, DLG_NAME), buffer);

	sprintf(buffer, "%s", customer.info.petName.c_str());
	SetWindowText(GetDlgItem(hWnd, DLG_PETNAME), buffer);

	sprintf(buffer, "%s", customer.info.phone1.c_str());
	SetWindowText(GetDlgItem(hWnd, DLG_PHONE1), buffer);

	sprintf(buffer, "%s", customer.info.phone2.c_str());
	SetWindowText(GetDlgItem(hWnd, DLG_PHONE2), buffer);

	sprintf(buffer, "%s", customer.info.remark.c_str());
	SetWindowText(GetDlgItem(hWnd, DLG_REMARK), buffer);

	sprintf(buffer, "%ld", customer.point);
	SetWindowText(GetDlgItem(hWnd, DLG_POINTBAL), buffer);

	return 0;
}

static int searchCustomer(HWND hWnd)
{
	std::string id;
	char buffer[512];

	std::deque<Customer> customers;

	GetWindowText(GetDlgItem(hWnd, DLG_SEARCHKEY), buffer, sizeof(buffer)/sizeof(buffer[0]));
	id=buffer;

	_db.search(id, customers);
	if (customers.size()==1)
	{
		displayCustomer(hWnd, customers[0]);
		enableWindows(hWnd, true);
		EnableWindow(GetDlgItem(hWnd, DLG_ID), FALSE);
	}

	return 0;
}

static int saveCustomer(HWND hWnd)
{
	char buffer[2048]={0};
	Customer customer;
	struct Info info;
	std::string id;

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

	GetWindowText(GetDlgItem(hWnd, DLG_ID), buffer, sizeof(buffer)/sizeof(buffer[0]));
	id=buffer;

	if (id.length() && info.name.length() && (info.phone1.length() || info.phone2.length()))
	{
		customer.init(id, info);
		std::string date;
		getDate(date);
		_db.createCustomer(customer, date);

		EnableWindow(GetDlgItem(hWnd, DLG_ID), FALSE);
	}

	return 0;
}

static int readFromConfig(const std::string &filename, const std::string &section, std::deque<std::string> &list)
{
	char key[512];
	char buffer[512];
	int i;
	int length;
	char file[_MAX_PATH];
	sprintf(file, "%s%s", getBasePath(), filename.c_str());

	i = 1;
	do {
		sprintf(key, "name%d", i);
		GetPrivateProfileString(section.c_str(), key, "", buffer, sizeof(buffer)/sizeof(buffer[0]), 
			file);
		length=strlen(buffer);
		if (length>0)
			list.push_back(buffer);

		i++;
	} while (length>0);

	return list.size();
}

static int initCouponType(HWND hWnd)
{
	std::deque<std::string> list;
	readFromConfig(std::string("config.ini"), std::string("bath"), list);

	std::deque<std::string>::iterator iter;
	iter = list.begin();
	while (iter != list.end())
	{
		std::string type = *iter;
		SendMessage(GetDlgItem(hWnd, DLG_SALESCOUPONSTYPE), CB_INSERTSTRING, (WPARAM) -1, (LPARAM) type.c_str());

		iter++;
	}

	return 0;
}

static int initCouponUsageType(HWND hWnd)
{
	std::deque<struct Coupons> coupons;
	std::string id;
	char buffer[1024];
	int size=sizeof(buffer)/sizeof(buffer[0]);

	GetWindowText(GetDlgItem(hWnd, DLG_ID), buffer, size);
	id=buffer;
	
	_db.readCoupons(id, coupons);

	SendMessage(GetDlgItem(hWnd, DLG_COUPONTYPE), CB_RESETCONTENT, (WPARAM) 0, (LPARAM) 0);

	std::deque<struct Coupons>::iterator iter;
	iter = coupons.begin();
	while (iter != coupons.end())
	{
		if (iter->quantity > 0)
		{
			SendMessage(GetDlgItem(hWnd, DLG_COUPONTYPE), CB_INSERTSTRING, (WPARAM) -1, (LPARAM) iter->type.c_str());
		}

		iter++;
	}

	return 0;
}

static int initSingleType(HWND hWnd)
{
	std::deque<std::string> list;
	readFromConfig(std::string("config.ini"), std::string("bath"), list);

	std::deque<std::string>::iterator iter;
	iter = list.begin();
	while (iter != list.end())
	{
		std::string type = *iter;
		SendMessage(GetDlgItem(hWnd, DLG_SALESSINGLETYPE), CB_INSERTSTRING, (WPARAM) -1, (LPARAM) type.c_str());

		iter++;
	}

	return 0;
}

static int initDaycareType(HWND hWnd)
{
	return 0;
}

static int addSalesCoupons(HWND hWnd)
{
	char buffer[512];
	std::string id;
	std::string type;
	std::string remark;
	std::string date;
	long qty;
	double total;
	long size = sizeof(buffer)/sizeof(buffer[0]);

	GetWindowText(GetDlgItem(hWnd, DLG_ID), buffer, size);
	id=buffer;
	GetWindowText(GetDlgItem(hWnd, DLG_SALESCOUPONSTYPE), buffer, size);
	type=buffer;
	GetWindowText(GetDlgItem(hWnd, DLG_SALESCOUPONSQTY), buffer, size);
	qty=atoi(buffer);
	GetWindowText(GetDlgItem(hWnd, DLG_SALESCOUPONSPRICE), buffer, size);
	total=atof(buffer);
	GetWindowText(GetDlgItem(hWnd, DLG_SALESCOUPONSREMARK), buffer, size);
	remark=buffer;
	GetWindowText(GetDlgItem(hWnd, DLG_SALESDATE), buffer, size);
	date=buffer;

	_db.updateCoupons(id, type, qty, total, remark, date);

	SendMessage(GetDlgItem(hWnd, DLG_SALESCOUPONSTYPE), CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0); 
	SetWindowText(GetDlgItem(hWnd, DLG_SALESCOUPONSQTY), "");
	SetWindowText(GetDlgItem(hWnd, DLG_SALESCOUPONSPRICE), "");
	SetWindowText(GetDlgItem(hWnd, DLG_SALESCOUPONSREMARK), "");
	SetWindowText(GetDlgItem(hWnd, DLG_SALESDATE), "");
	CheckDlgButton(hWnd, DLG_SALESCOUPONS, BST_UNCHECKED);
	
	return 0;
}

static int addSalesSingle(HWND hWnd)
{
	char buffer[512];
	std::string id;
	std::string type;
	std::string remark;
	std::string date;
	double total;
	long size = sizeof(buffer)/sizeof(buffer[0]);

	GetWindowText(GetDlgItem(hWnd, DLG_ID), buffer, size);
	id=buffer;
	GetWindowText(GetDlgItem(hWnd, DLG_SALESSINGLETYPE), buffer, size);
	type=buffer;
	GetWindowText(GetDlgItem(hWnd, DLG_SALESSINGLEPRICE), buffer, size);
	total=atoi(buffer);
	GetWindowText(GetDlgItem(hWnd, DLG_SALESSINGLEREMARK), buffer, size);
	remark=buffer;
	GetWindowText(GetDlgItem(hWnd, DLG_SALESDATE), buffer, size);
	date=buffer;

	_db.updateBath(id, type, 1, total, remark, date);

	SendMessage(GetDlgItem(hWnd, DLG_SALESSINGLETYPE), CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0); 
	SetWindowText(GetDlgItem(hWnd, DLG_SALESSINGLEPRICE), "");
	SetWindowText(GetDlgItem(hWnd, DLG_SALESSINGLEREMARK), "");
	SetWindowText(GetDlgItem(hWnd, DLG_SALESDATE), "");
	CheckDlgButton(hWnd, DLG_SALESSINGLE, BST_UNCHECKED);

	return 0;
}

static int addSalesShopping(HWND hWnd)
{
	char buffer[512];
	std::string id;
	std::string type;
	std::string remark;
	std::string date;
	double total;
	long size = sizeof(buffer)/sizeof(buffer[0]);

	GetWindowText(GetDlgItem(hWnd, DLG_ID), buffer, size);
	id=buffer;
	GetWindowText(GetDlgItem(hWnd, DLG_SALESSALESPRICE), buffer, size);
	total=atof(buffer);
	GetWindowText(GetDlgItem(hWnd, DLG_SALESSALESREMARK), buffer, size);
	remark=buffer;
	GetWindowText(GetDlgItem(hWnd, DLG_SALESDATE), buffer, size);
	date=buffer;

	_db.updateShopping(id, total, remark, date);

	SetWindowText(GetDlgItem(hWnd, DLG_SALESSALESPRICE), "");
	SetWindowText(GetDlgItem(hWnd, DLG_SALESSALESREMARK), "");
	SetWindowText(GetDlgItem(hWnd, DLG_SALESDATE), "");
	CheckDlgButton(hWnd, DLG_SALESSALES, BST_UNCHECKED);

	return 0;
}

static int addSalesDayCare(HWND hWnd)
{
	char buffer[512];
	std::string id;
	std::string type;
	std::string remark;
	std::string date;
	double total;
	long qty;
	long size = sizeof(buffer)/sizeof(buffer[0]);

	GetWindowText(GetDlgItem(hWnd, DLG_ID), buffer, size);
	id=buffer;
	GetWindowText(GetDlgItem(hWnd, DLG_SALESDAYQTY), buffer, size);
	qty=atoi(buffer);
	GetWindowText(GetDlgItem(hWnd, DLG_SALESDAYPRICE), buffer, size);
	total=atof(buffer);
	GetWindowText(GetDlgItem(hWnd, DLG_SALESDAYREMARK), buffer, size);
	remark=buffer;
	GetWindowText(GetDlgItem(hWnd, DLG_SALESDATE), buffer, size);
	date=buffer;

	_db.updateDaycare(id, type, qty, total, remark, date);

	SetWindowText(GetDlgItem(hWnd, DLG_SALESDAYQTY), "");
	SetWindowText(GetDlgItem(hWnd, DLG_SALESDAYPRICE), "");
	SetWindowText(GetDlgItem(hWnd, DLG_SALESDAYREMARK), "");
	SetWindowText(GetDlgItem(hWnd, DLG_SALESDATE), "");
	CheckDlgButton(hWnd, DLG_SALESDAY, BST_UNCHECKED);

	return 0;
}

static int newSales(HWND hWnd)
{
	if (IsDlgButtonChecked(hWnd, DLG_SALESCOUPONS))
	{
		addSalesCoupons(hWnd);
		initCouponUsageType(hWnd);
	}
	if (IsDlgButtonChecked(hWnd, DLG_SALESSINGLE))
	{
		addSalesSingle(hWnd);
	}
	if (IsDlgButtonChecked(hWnd, DLG_SALESSALES))
	{
		addSalesShopping(hWnd);
	}
	if (IsDlgButtonChecked(hWnd, DLG_SALESDAY))
	{
		addSalesDayCare(hWnd);
	}
	updatePoint(hWnd);

	return 0;
}
static int initListView(HWND hWnd, UINT ctrlID, std::deque<std::string> &header)
{
	HWND hList = GetDlgItem(hWnd, ctrlID);

	LVCOLUMN LvCol;
	int col=0;
	char buffer[512];

	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	memset(&LvCol, 0, sizeof(LvCol));
	LvCol.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
	LvCol.pszText="";
	LvCol.cx=100;

	for (col = 0; col < header.size(); col++)
	{
		strcpy(buffer, header[col].c_str());
		LvCol.pszText=buffer;
		LvCol.cx=strlen(buffer)*10;
		SendMessage(hList, LVM_INSERTCOLUMN, col, (LPARAM) &LvCol);
	}

	return 0;
}

static int useCoupon(HWND hWnd)
{
	char buffer[2048];
	int size=sizeof(buffer)/sizeof(buffer[0]);
	std::string id, type, date;
	long qty;

	GetWindowText(GetDlgItem(hWnd, DLG_ID), buffer, size);
	id = buffer;
	GetWindowText(GetDlgItem(hWnd, DLG_COUPONDATE), buffer, size);
	date = buffer;
	GetWindowText(GetDlgItem(hWnd, DLG_COUPONTYPE), buffer, size);
	type = buffer;
	GetWindowText(GetDlgItem(hWnd, DLG_COUPONQTY), buffer, size);
	qty= atol(buffer);
	_db.updateCoupons(id, type, -qty, 0, "use", date);

	initCouponUsageType(hWnd);
	
	struct Coupons coupon;
	coupon.date = date;
	coupon.type = type;
	coupon.quantity = qty;
	updateCouponList(hWnd, coupon);

	SetWindowText(GetDlgItem(hWnd, DLG_COUPONDATE), "");
	SendMessage(GetDlgItem(hWnd, DLG_COUPONTYPE), CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
	SetWindowText(GetDlgItem(hWnd, DLG_COUPONQTY), "");

	return 0;
	// update coupon list view ui
}

// update history list view
// update coupon ui
static int updateCouponList(HWND hWnd, struct Coupons &coupon)
{
	HWND hList = GetDlgItem(hWnd, DLG_COUPONBAL);
	char buffer[256];

	LVITEM lvItem;
	memset(&lvItem, 0, sizeof(lvItem));
	lvItem.mask = LVIF_TEXT;
	lvItem.cchTextMax = 256;
	lvItem.iItem = 0;
	lvItem.iSubItem = 0;
	lvItem.pszText = buffer;

	strcpy(buffer, coupon.date.c_str());
	lvItem.iSubItem = 0;
	SendMessage(hList, LVM_INSERTITEM, 0, (LPARAM) &lvItem);

	strcpy(buffer, coupon.type.c_str());
	lvItem.iSubItem = 1;
	SendMessage(hList, LVM_SETITEM, 0, (LPARAM) &lvItem);

	sprintf(buffer, "%ld", coupon.quantity);
	lvItem.iSubItem = 2;
	SendMessage(hList, LVM_SETITEM, 0, (LPARAM) &lvItem);

	return 0;
}

static int updateHistoryList(HWND hWnd)
{
	return 0;
}

static int updatePoint(HWND hWnd)
{
	char buffer[512];
	int size=sizeof(buffer)/sizeof(buffer[0]);
	long point;
	std::string id;

	GetWindowText(GetDlgItem(hWnd, DLG_ID), buffer, size);
	id = buffer;

	_db.readPoint(id, point);
	sprintf(buffer, "%ld", point);
	SetWindowText(GetDlgItem(hWnd, DLG_POINTBAL), buffer);

	return 0;
}
