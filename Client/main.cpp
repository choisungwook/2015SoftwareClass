#include "socket.h"
#include "main.h"
#include "enum.h"
#include "resource.h"
#include "carThread.h"
#include "handle.h"
#include "fileIO.h"
#include "utility.h"
#include <list>
#include <commctrl.h>
#include <vector>
#include <regex>
using namespace std;

#pragma comment (lib, "msimg32.lib")

//local 함수
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK MovieProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CarOptionProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

//local 변수
LPCTSTR			lpszClass = TEXT("3조 소프트웨어설계");
HINSTANCE		g_hInst;
HWND			hWndMain;
HBITMAP			hbackbit; //백버퍼

//carTread 전역변수 참조
extern	int				numOfcar;
extern	int				numOfturnel;

//종료플래그
bool	exitFlag;
//시작플래그
bool	movieFlag;
bool	portFlag;
vector<movieTag> Movietag;
Cloud cloud[numOfCloud];

//캐릭터 좌표 테스트
int posX, posY;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	hWndMain = hWnd;

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}


void updateMenu()
{
	HMENU hMenu = GetMenu(hWndMain);
	HMENU hMenu1 = GetSubMenu(hMenu, 0);

	if (movieFlag && portFlag)
		EnableMenuItem((HMENU)hMenu1, ID_FILE_START, MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem((HMENU)hMenu1, ID_FILE_START, MF_BYCOMMAND | MF_GRAYED);
	
}
BOOL CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	RECT wrt, crt;

	switch (iMessage) {
	case WM_INITDIALOG:
		GetWindowRect(GetParent(hDlg), &wrt);
		GetWindowRect(hDlg, &crt);
		SetWindowPos(hDlg, HWND_NOTOPMOST, wrt.left + (wrt.right - wrt.left) / 2 - (crt.right - crt.left) / 2,
			wrt.top + (wrt.bottom - wrt.top) / 2 - (crt.bottom - crt.top) / 2, 0, 0, SWP_NOSIZE);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		case IDC_OK:	
			destoryThreads(); //쓰레드파괴
			destoryHandles(); //핸들파괴
			//destorythread();	
			numOfcar = GetDlgItemInt(hDlg, IDC_EDIT_CAR, NULL, FALSE);
			numOfturnel = GetDlgItemInt(hDlg, IDC_EDIT_TURNEL, NULL, FALSE);
			EndDialog(hDlg, IDC_OK);
			
			initializeHandles(); //핸들초기화			
			createThreads(); //쓰레드생성
			return TRUE;

		case IDC_CANCEL:			
			EndDialog(hDlg, IDC_CANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void updateListview(HWND hDlg)
{
	LVITEM LI;
	
	HWND hwndList = GetDlgItem(hDlg, IDC_LIST1);

	for (int i = 0; i < Movietag.size(); i++)
	{
		LI.mask = LVIF_TEXT;
		LI.iItem = i;
		LI.iSubItem = 0;
		LI.iImage = 0;
		LI.pszText = Movietag[i].name;
		ListView_InsertItem(hwndList, &LI);

		char buf[10];
		LI.iSubItem = 1;
		sprintf(buf, "%d", Movietag[i].price);
		LI.pszText = buf;
		SendMessage(hwndList, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 2;
		sprintf(buf, "%d", Movietag[i].time);
		LI.pszText = buf;
		SendMessage(hwndList, LVM_SETITEM, 0, (LPARAM)&LI);
	}
}

VOID OnInitializeListView(HWND hDlg)
{
	LVCOLUMN lvc = { 0 };
	INT nSubItem = 0;
	HWND hWndHdr = NULL;
	INT nCount = 0;

	LPTSTR lpszTitle[] = { TEXT("영화이름"), TEXT("영화가격"), TEXT("영화시청시간")};
	nCount = sizeof(lpszTitle) / sizeof(LPCTSTR);

	HWND hwndList = GetDlgItem(hDlg, IDC_LIST1);

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.iImage = 0;
	lvc.cx = 100;

	for (nSubItem = 0; nSubItem < nCount; ++nSubItem) {
		lvc.iSubItem = nSubItem;
		lvc.pszText = lpszTitle[nSubItem];
		lvc.fmt = LVCFMT_LEFT;
		ListView_InsertColumn(hwndList, lvc.iSubItem, &lvc);
	}

	updateListview(hDlg);
	return;
}

bool checkWhiteSpace(string in)
{
	if (in.length() == 0)
		return false;

	regex re("\\w+");

	if (regex_match(in, re))	
		return true;
	
	return false;
}

void inputListview(HWND hDlg)
{
	bool flag = false;
	LVITEM LI;
	
	HWND hwndList = GetDlgItem(hDlg, IDC_LIST1);

	char moviename[20];
	LI.mask = LVIF_TEXT;
	LI.iSubItem = 0;
	LI.iImage = 0;
	LI.iItem = Movietag.size();
	GetDlgItemText(hDlg, IDC_EDIT_MOVIE, moviename, sizeof(moviename));

	//check white space
	if(checkWhiteSpace(moviename))
	{
		//check duplication
		vector<movieTag>::iterator End = Movietag.end();
		for (vector<movieTag>::iterator iterPos = Movietag.begin(); iterPos != End; iterPos++)
		{
			if (!strcmp(moviename, iterPos->name))
			{
				flag = true;
				break;
			}
		}

		if (!flag)
		{
			LI.pszText = moviename;
			ListView_InsertItem(hwndList, &LI);

			char buf[10];
			int _price = myrand(10000);
			LI.iSubItem = 1;
			sprintf(buf, "%d", _price);
			LI.pszText = buf;
			SendMessage(hwndList, LVM_SETITEM, 0, (LPARAM)&LI);

			LI.iSubItem = 2;
			int _time = myrand(movie::randTime) + movie::minTime;
			sprintf(buf, "%d", _time);
			LI.pszText = buf;
			SendMessage(hwndList, LVM_SETITEM, 0, (LPARAM)&LI);

			movieTag t;
			strcpy(t.name, moviename);
			t.price = _price;
			t.time = _time;

			Movietag.push_back(t);
		}
		else
			MessageBox(hDlg, "이미 존재하는 영화입니다", "error", MB_OK);
	}
	else
		MessageBox(hDlg, "특수문자, 공백은 허용되지 않습니다", "error", MB_OK);
	
}

BOOL CALLBACK MovieProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	RECT wrt, crt;

	switch (iMessage) {
	case WM_INITDIALOG:
		{
			GetWindowRect(GetParent(hDlg), &wrt);
			GetWindowRect(hDlg, &crt);
			SetWindowPos(hDlg, HWND_NOTOPMOST, wrt.left + (wrt.right - wrt.left) / 2 - (crt.right - crt.left) / 2,
				wrt.top + (wrt.bottom - wrt.top) / 2 - (crt.bottom - crt.top) / 2, 0, 0, SWP_NOSIZE);
			OnInitializeListView(hDlg);
			return TRUE;
		}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		case ID_MOVIE_CANCEL:
			if (Movietag.size() > 0)
			{					
				movieFlag = true;
			}
			else
				movieFlag = false;
			EndDialog(hDlg, 0);
			updateMenu();
			return TRUE;
		case IDC_BUTTON_ADD:
			{
				inputListview(hDlg); 
				return TRUE;
			}
		case IDC_BUTTON_DEL:
			{
				LVITEM LI;

				HWND hwndList = GetDlgItem(hDlg, IDC_LIST1);
				int idx = ListView_GetNextItem(hwndList, -1, LVNI_ALL | LVNI_SELECTED);
				if (idx == -1)
					MessageBox(hDlg, "삭제할 항목을 먼저 선택하십시오", "error", MB_OK);
				else
				{
					Movietag.erase(Movietag.begin() + idx);
					ListView_DeleteItem(hwndList, idx);
				}
				break;
			}
			
		}
		break;
	}
	return FALSE;
}

extern int port;
int checkDigit(string in)
{
	if (in.length() == 0)
		return -1;

	regex re("[\\d]+");

	if (regex_match(in, re))
	{
		int r; sscanf(in.c_str(), "%d", &r);
		return r;
	}

	return -1;
}
BOOL CALLBACK OptionProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	RECT wrt, crt;

	switch (iMessage) {
	case WM_INITDIALOG:
		GetWindowRect(GetParent(hDlg), &wrt);
		GetWindowRect(hDlg, &crt);
		SetWindowPos(hDlg, HWND_NOTOPMOST, wrt.left + (wrt.right - wrt.left) / 2 - (crt.right - crt.left) / 2,
			wrt.top + (wrt.bottom - wrt.top) / 2 - (crt.bottom - crt.top) / 2, 0, 0, SWP_NOSIZE);
		SetDlgItemInt(hDlg, IDC_EDIT_PORT, DEFAULTPORT, FALSE);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;

		case ID_OPTION_OK:
			char value[30];
			GetDlgItemText(hDlg, IDC_EDIT_PORT, value, sizeof(value));
			if ((port = checkDigit(value)) != -1)
			{
				char reulstmsg[30];
				sprintf(reulstmsg, "포트 : %d\n설정완료", port);
				MessageBox(hDlg, reulstmsg, "ok", MB_OK);
				portFlag = true;
				updateMenu();
			}
			else
			{
				char reulstmsg[30];
				sprintf(reulstmsg, "숫자만 입력가능합니다.\n또한 공백 허용하지 않습니다", port);
				MessageBox(hDlg, reulstmsg, "ok", MB_OK);
			}			
			return TRUE;

		case ID_OPTION_CANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

extern int mincarnumber;
extern int maxcarnumber;
BOOL CALLBACK CarOptionProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	RECT wrt, crt;

	switch (iMessage) {
	case WM_INITDIALOG:
		GetWindowRect(GetParent(hDlg), &wrt);
		GetWindowRect(hDlg, &crt);
		SetWindowPos(hDlg, HWND_NOTOPMOST, wrt.left + (wrt.right - wrt.left) / 2 - (crt.right - crt.left) / 2,
			wrt.top + (wrt.bottom - wrt.top) / 2 - (crt.bottom - crt.top) / 2, 0, 0, SWP_NOSIZE);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;

		case ID_CAR_OK:
			{
				char value[10];
				int min, max;
				GetDlgItemText(hDlg, IDC_EDIT_MIN, value, sizeof(value));

				if ((min = checkDigit(value)) != -1)
				{
					GetDlgItemText(hDlg, IDC_EDIT_MAX, value, sizeof(value));
					if ((max = checkDigit(value)) != -1)
					{
						if (min > max)
							MessageBox(hWndMain,"최소값이 최대값보다 큽니다", "error", MB_OK);
						else
						{
							mincarnumber = min;
							maxcarnumber = max;
							MessageBox(hWndMain, "설정 성공", "success", MB_OK);
						}
					}
					else
						MessageBox(hWndMain, "숫자만 입력가능합니다.", "error", MB_OK);
				}
				else
					MessageBox(hWndMain, "숫자만 입력가능합니다.", "error", MB_OK);
				return TRUE;
			}
		}
		return FALSE;
	}
	return FALSE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage) {
	case WM_INITMENU:
	{
		updateMenu();
		//EnableMenuItem((HMENU)wParam, ID_FILE_START, MF_BYCOMMAND|  MF_GRAYED | MF_ENABLED);
		break;
	}
	case WM_CREATE:
		{		
			initializeWinsock();
			exitFlag = false;
			movieFlag = false;
			portFlag = false;
			cloud[0].setcloud(cloudbackgroundPath);
			cloud[1].setcloud(cloud0);
			cloud[2].setcloud(cloudbackgroundPath);
			cloud[3].setcloud(cloudbackgroundPath);
			cloud[3].x = collectionXYcloud::cloud3X;
			cloud[3].y = collectionXYcloud::cloud3Y;
			cloud[4].setcloud(cloud1);
			cloud[4].x = collectionXYcloud::cloud2X;
			cloud[4].y = collectionXYcloud::cloud2Y;
			return 0;
		}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_START:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
			break;
		case ID_FILE_EXIT:
			destoryThreads(); //핸들파괴
			destoryHandles(); //쓰레드파괴
			destoryWinsock(); //윈속종료
			PostQuitMessage(0);
			break;
		case ID_FILE_MOVIE: //영화입력
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_MOVIE), hWnd, MovieProc);
			break;
		case ID_TOSERVER_CHECK:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_OPTION), hWnd, OptionProc);
			break;
		case ID_TOSERVER_CARSETTINGS:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_carsettings), hWnd, CarOptionProc);
			break;
		}
		return 0;
	case WM_PAINT:
		HDC hdc, MemDC;
		RECT crt;
		HBITMAP OldBit;
		PAINTSTRUCT ps;

		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &crt);
		MemDC = CreateCompatibleDC(hdc);
		OldBit = (HBITMAP)SelectObject(MemDC, hbackbit);
		BitBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, SRCCOPY);
		//TransparentBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, crt.right, crt.bottom, RGB(255, 255, 255));		
		SelectObject(MemDC, OldBit);
		DeleteDC(MemDC);
		EndPaint(hWnd, &ps);

		return 0;
	case WM_DESTROY:			
		destoryThreads(); //핸들파괴
		destoryHandles(); //쓰레드파괴
		destoryWinsock();
		PostQuitMessage(0);
	
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

/////////////////////////////////////////////////////////////////
//화면 출력에 관련된 함수들
/////////////////////////////////////////////////////////////////
//차인자
extern list<carArgument>		L_carArg;
extern list<Person*>			L_personArg;

//뮤택스
extern	HANDLE					M_accessArg;
//상단 직원 차량인식기
extern	bool topUpReader;
//하단 차량인식기
extern	bool upReader;

void drawOnMemory(char* Imagepath, HDC BackDC, HDC MemDC, int memoryX, int memoryY, int mode);
void drawcar0(list<carArgument>::iterator arg, HDC bitDC, HDC memDC);
void drawcar1(list<carArgument>::iterator arg, HDC bitDC, HDC memDC);
void drawcar2(list<carArgument>::iterator arg, HDC bitDC, HDC memDC);
void drawcar3(list<carArgument>::iterator arg, HDC bitDC, HDC memDC);

void(*carfunc[sortOfcar])(list<carArgument>::iterator, HDC, HDC) =
{
	drawcar0,
	drawcar1,
	drawcar2,
	drawcar3,
};


void Update()
{
	HDC hdc, MemDC;
	HDC BitDC;
	HBITMAP OldBit;
	RECT crt;

	//초기화
	GetClientRect(hWndMain, &crt);
	hdc = GetDC(hWndMain);
	if (hbackbit == NULL)
		hbackbit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
	MemDC = CreateCompatibleDC(hdc);
	BitDC = CreateCompatibleDC(hdc);
	OldBit = (HBITMAP)SelectObject(MemDC, hbackbit);

	//배경화면 그리기
	drawOnMemory(backgroundImagePath, BitDC, MemDC, 0, 0, 1);

	//말풍선
	SetBkMode(MemDC, TRANSPARENT);
	for (int i = 0; i < numOfCloud; i++)
	{
		if (cloud[i].getstatus())
		{
			//배경출력
			drawOnMemory(cloud[i].getcloud(), BitDC, MemDC, cloud[i].x, cloud[i].y - 10, 2);
			//글씨출력
			if (i==1)
				TextOut(MemDC, cloud[i].x, cloud[i].y + 10, cloud[i].gettext(), strlen(cloud[i].gettext()));
			else
				TextOut(MemDC, cloud[i].x, cloud[i].y, cloud[i].gettext(), strlen(cloud[i].gettext()));						
		}
	}

	////차량인식기 그리기
	if (upReader)
		drawOnMemory(carReaderUpImagePath, BitDC, MemDC, 10, 580, 2);
	else
		drawOnMemory(carReaderDownImagePath, BitDC, MemDC, 11, 650, 2);

	//상단 계산직원
	if (topUpReader)
		drawOnMemory(TopcarReaderDownImagePath, BitDC, MemDC, 825, 25, 2);
	else
		drawOnMemory(TopcarReaderUpImagePath, BitDC, MemDC, 825, 25, 2);
	
	//차량그리기
	watiAndcheckExited(M_accessArg);
	list<carArgument>::iterator End = L_carArg.end();
	for (list<carArgument>::iterator iterPos = L_carArg.begin(); iterPos != End; iterPos++)
	{
		//차량그리기
		carfunc[iterPos->sort](iterPos, BitDC, MemDC);
		//차량진행상태
		//@[0] --> 영화 보는중
		//@[1] --> 화장실 가는중
		//@[2] --> 매점 가는중
		if (iterPos->status == 1)
		{
			if (iterPos->seat == 0)
				drawOnMemory(watchingmoviePath, BitDC, MemDC, collectionXYstatus::seat0X, collectionXYstatus::seat0Y, 2);
			else if (iterPos->seat == 1)
				drawOnMemory(watchingmoviePath, BitDC, MemDC, collectionXYstatus::seat1X, collectionXYstatus::seat0Y, 2);
			else if (iterPos->seat == 2)
				drawOnMemory(watchingmoviePath, BitDC, MemDC, collectionXYstatus::seat2X, collectionXYstatus::seat0Y, 2);
			else if (iterPos->seat == 3)
				drawOnMemory(watchingmoviePath, BitDC, MemDC, collectionXYstatus::seat3X, collectionXYstatus::seat0Y, 2);
			else if (iterPos->seat == 4)
				drawOnMemory(watchingmoviePath, BitDC, MemDC, collectionXYstatus::seat4X, collectionXYstatus::seat0Y, 2);
			else if (iterPos->seat == 5)
				drawOnMemory(watchingmoviePath, BitDC, MemDC, collectionXYstatus::seat5X, collectionXYstatus::seat0Y, 2);
		}
		else if (iterPos->status == 2)//장실
		{
			if (iterPos->seat == 0)
				drawOnMemory(gotiolet, BitDC, MemDC, collectionXYstatus::seat0X, collectionXYstatus::seat0Y, 2);
			else if (iterPos->seat == 1)
				drawOnMemory(gotiolet, BitDC, MemDC, collectionXYstatus::seat1X, collectionXYstatus::seat0Y, 2);
			else if (iterPos->seat == 2)
				drawOnMemory(gotiolet, BitDC, MemDC, collectionXYstatus::seat2X, collectionXYstatus::seat0Y, 2);
			else if (iterPos->seat == 3)
				drawOnMemory(gotiolet, BitDC, MemDC, collectionXYstatus::seat3X, collectionXYstatus::seat0Y, 2);
			else if (iterPos->seat == 4)
				drawOnMemory(gotiolet, BitDC, MemDC, collectionXYstatus::seat4X, collectionXYstatus::seat0Y, 2);
			else if (iterPos->seat == 5)
				drawOnMemory(gotiolet, BitDC, MemDC, collectionXYstatus::seat5X, collectionXYstatus::seat0Y, 2);
		}
		else if (iterPos->status == 3)//매점
		{
			if (iterPos->seat == 0)
				drawOnMemory(gostore, BitDC, MemDC, collectionXYstatus::seat0X, collectionXYstatus::seat0Y, 2);
			else if (iterPos->seat == 1)
				drawOnMemory(gostore, BitDC, MemDC, collectionXYstatus::seat1X, collectionXYstatus::seat0Y, 2);
			else if (iterPos->seat == 2)
				drawOnMemory(gostore, BitDC, MemDC, collectionXYstatus::seat2X, collectionXYstatus::seat0Y, 2);
			else if (iterPos->seat == 3)
				drawOnMemory(gostore, BitDC, MemDC, collectionXYstatus::seat3X, collectionXYstatus::seat0Y, 2);
			else if (iterPos->seat == 4)
				drawOnMemory(gostore, BitDC, MemDC, collectionXYstatus::seat4X, collectionXYstatus::seat0Y, 2);
			else if (iterPos->seat == 5)
				drawOnMemory(gostore, BitDC, MemDC, collectionXYstatus::seat5X, collectionXYstatus::seat0Y, 2);
		}		
	}
	ReleaseMutex(M_accessArg);
		
	//터널그리기
	drawOnMemory(turnelImagePath, BitDC, MemDC, 125, 670, 2);

	//다리 그리기
	drawOnMemory(BridgeImagePath, BitDC, MemDC, 145, 245, 2);

	//사람그리기
	list<Person*>::iterator personEnd = L_personArg.end();
	for (list<Person*>::iterator iterPos = L_personArg.begin(); iterPos != personEnd; iterPos++)
	{
		if ((*iterPos)->character == 1) //쵸파
		{
			if ((*iterPos)->direction == 1)
				drawOnMemory(character1backimagePath, BitDC, MemDC, (*iterPos)->posX, (*iterPos)->posY, 2);
			else if ((*iterPos)->direction == 2)			
				drawOnMemory(character1rightimagePath, BitDC, MemDC, (*iterPos)->posX, (*iterPos)->posY, 2);
			else if ((*iterPos)->direction == 3)
				drawOnMemory(character1leftimagePath, BitDC, MemDC, (*iterPos)->posX, (*iterPos)->posY, 2);
			else
				drawOnMemory(character1frontimagePath, BitDC, MemDC, (*iterPos)->posX, (*iterPos)->posY, 2);
			
		}
		else if ((*iterPos)->character == 2) //쵸파
		{
			if ((*iterPos)->direction == 1)
				drawOnMemory(character2backimagePath, BitDC, MemDC, (*iterPos)->posX, (*iterPos)->posY, 2);
			else if ((*iterPos)->direction == 2)
				drawOnMemory(character2rightimagePath, BitDC, MemDC, (*iterPos)->posX, (*iterPos)->posY, 2);
			else if ((*iterPos)->direction == 3)
				drawOnMemory(character2leftimagePath, BitDC, MemDC, (*iterPos)->posX, (*iterPos)->posY, 2);
			else
				drawOnMemory(character2frontimagePath, BitDC, MemDC, (*iterPos)->posX, (*iterPos)->posY, 2);
		}
		else if ((*iterPos)->character == 3) //쵸파
		{
			if ((*iterPos)->direction == 1)			
				drawOnMemory(character3backimagePath, BitDC, MemDC, (*iterPos)->posX, (*iterPos)->posY, 2);
			else if ((*iterPos)->direction == 2)
				drawOnMemory(character3rightimagePath, BitDC, MemDC, (*iterPos)->posX, (*iterPos)->posY, 2);
			else if ((*iterPos)->direction == 3)
				drawOnMemory(character3leftimagePath, BitDC, MemDC, (*iterPos)->posX, (*iterPos)->posY, 2);
			else
				drawOnMemory(character3frontimagePath, BitDC, MemDC, (*iterPos)->posX, (*iterPos)->posY, 2);			
		}
		else
		{
			if ((*iterPos)->direction == 1)
				drawOnMemory(character4backimagePath, BitDC, MemDC, (*iterPos)->posX, (*iterPos)->posY, 2);
			else if ((*iterPos)->direction == 2)
				drawOnMemory(character4rightimagePath, BitDC, MemDC, (*iterPos)->posX, (*iterPos)->posY, 2);
			else if ((*iterPos)->direction == 3)
				drawOnMemory(character4leftimagePath, BitDC, MemDC, (*iterPos)->posX, (*iterPos)->posY, 2);
			else
				drawOnMemory(character4frontimagePath, BitDC, MemDC, (*iterPos)->posX, (*iterPos)->posY, 2);
		}
	}

	//종료
	DeleteDC(MemDC);
	DeleteDC(BitDC);
	ReleaseDC(hWndMain, hdc);
	InvalidateRect(hWndMain, NULL, FALSE);
}


void drawOnMemory(char* Imagepath, HDC BackDC, HDC MemDC, int memoryX, int memoryY, int mode)
{
	HBITMAP ImageBit = (HBITMAP)LoadImage(g_hInst, Imagepath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	HBITMAP OldBit = (HBITMAP)SelectObject(BackDC, ImageBit);
	BITMAP	bitmap;
	int bw, bh;

	GetObject(ImageBit, sizeof(BITMAP), &bitmap);
	bw = bitmap.bmWidth;
	bh = bitmap.bmHeight;

	if (mode == 1)
		BitBlt(MemDC, memoryX, memoryY, bw, bh, BackDC, 0, 0, SRCCOPY);
	else if (mode == 2)
		TransparentBlt(MemDC, memoryX, memoryY, bw, bh, BackDC, 0, 0, bw, bh, RGB(255, 255, 255));

	DeleteObject(SelectObject(BackDC, OldBit));
}

////차종류
void drawcar0(list<carArgument>::iterator arg, HDC bitDC, HDC memDC)
{
		//왼쪽
		if (arg->direction == 0)
			drawOnMemory(car0leftImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
		//오른쪽
		else if (arg->direction == 1)
			drawOnMemory(car0rightImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
		//위
		if (arg->direction == 2)
			drawOnMemory(car0upImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
		//아래
		if (arg->direction == 3)
			drawOnMemory(car0downImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	
}

void drawcar1(list<carArgument>::iterator arg, HDC bitDC, HDC memDC)
{
	//왼쪽
	if (arg->direction == 0)
		drawOnMemory(car1leftImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//오른쪽
	else if (arg->direction == 1)
		drawOnMemory(car1rightImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//위
	if (arg->direction == 2)
		drawOnMemory(car1upImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//아래
	if (arg->direction == 3)
		drawOnMemory(car1downImagePath, bitDC, memDC, arg->posX, arg->posY, 2);

}

void drawcar2(list<carArgument>::iterator arg, HDC bitDC, HDC memDC)
{
	//왼쪽
	if (arg->direction == 0)
		drawOnMemory(car2leftImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//오른쪽
	else if (arg->direction == 1)
		drawOnMemory(car2rightImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//위
	if (arg->direction == 2)
		drawOnMemory(car2upImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//아래
	if (arg->direction == 3)
		drawOnMemory(car2downImagePath, bitDC, memDC, arg->posX, arg->posY, 2);

}

void drawcar3(list<carArgument>::iterator arg, HDC bitDC, HDC memDC)
{
	//왼쪽
	if (arg->direction == 0)
		drawOnMemory(car3leftImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//오른쪽
	else if (arg->direction == 1)
		drawOnMemory(car3rightImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//위
	if (arg->direction == 2)
		drawOnMemory(car3upImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//아래
	if (arg->direction == 3)
		drawOnMemory(car3downImagePath, bitDC, memDC, arg->posX, arg->posY, 2);

}




