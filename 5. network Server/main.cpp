#include "socket.h"
#include "function.h"
#include "resource.h"
#include "avltree.h"
#include "prettyprintTree.h"
#include <process.h>
#include <iostream>
using namespace std;

#define WIDTH 530
#define HEIGHT 500

//������â ���� ���� ����
char		*title = "��Ʈ��ũ����";
char		*lpszname = "softwareclass";

HINSTANCE hInst;
HWND hWnd;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
void destoryThread();


//���Ͼ�����
HANDLE	ServerThread;
int port;

BTreeNode* sample;
BTreeNode* car;

int main()
{	
	hInst = (HINSTANCE)::GetModuleHandle(NULL);

	//������ Ŭ���� ���� �� ���
	WNDCLASS wc =
	{
		CS_CLASSDC
		, WndProc
		, 0L
		, 0L
		, hInst
		, NULL
		, LoadCursor(NULL, IDC_ARROW)
		/*, (HBRUSH)GetStockObject(LTGRAY_BRUSH)*/
		, CreateSolidBrush(RGB(219, 233, 236))				
		, NULL
		, lpszname
	};

	::RegisterClass(&wc);


	hWnd = CreateWindow(lpszname
		, title
		, WS_OVERLAPPEDWINDOW | WS_VISIBLE
		, 0
		, 0
		, WIDTH
		, HEIGHT
		, NULL
		, NULL
		, hInst
		, NULL);


	::ShowWindow(hWnd, SW_SHOW);
	::UpdateWindow(hWnd);
	::ShowCursor(TRUE);

	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	::UnregisterClass("����ȭ��", hInst);

	return 0;
}


LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	if (msg == WM_CREATE)
	{	
		////////////////////////
		//avl Ʈ�� �ʱ�ȭ
		BSTMakeAndInit(&sample);
		BSTMakeAndInit(&car);
		////////////////////////////

		for (int i = 0; i < 10; i++)
		{
			carArg a;
			memset(&a, 0, sizeof(a));
			a.id = i;
			a.lastVisit = i;
			a.totalPrice = i * 10;
			BSTInsert(&car, i, a);
		}


		//////////////////////
		//��ư�����
		int x = 50, y = 50;
		BITMAP bmp;
		HBITMAP hBitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(IDB_BIT_START), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
		//HBITMAP hBitmap = (HBITMAP)LoadImage(hInst, StartImagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);				
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		HWND hButton = CreateWindow("button", "Click Me", BS_BITMAP | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			x, y, x + bmp.bmWidth, y + bmp.bmHeight, hWnd, (HMENU)1, hInst, NULL);
		MoveWindow(hButton, x, y, bmp.bmWidth, bmp.bmHeight, TRUE);		
		SendMessage(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);

		x += bmp.bmWidth + 50;
		hBitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(IDB_BIT_STOP), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);		
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		HWND hButton2 = CreateWindow("button", "Click Me", BS_BITMAP | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			x, y, x + bmp.bmWidth, y + bmp.bmHeight, hWnd, (HMENU)2, hInst, NULL);
		MoveWindow(hButton2, x, y, bmp.bmWidth, bmp.bmHeight, TRUE);
		SendMessage(hButton2, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
		
		x = 50; y += bmp.bmHeight + 50;
		hBitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(IDB_BIT_INPUT), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);		
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		HWND hButton3 = CreateWindow("button", "Click Me", BS_BITMAP | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			x, y, x + bmp.bmWidth, y + bmp.bmHeight, hWnd, (HMENU)3, hInst, NULL);
		MoveWindow(hButton3, x, y, bmp.bmWidth, bmp.bmHeight, TRUE);
		SendMessage(hButton3, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);

		x += bmp.bmWidth + 50;
		hBitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(IDC_BIT_REMOVE), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		HWND hButton4 = CreateWindow("button", "Click Me", BS_BITMAP | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			x, y, x + bmp.bmWidth, y + bmp.bmHeight, hWnd, (HMENU)4, hInst, NULL);
		MoveWindow(hButton4, x, y, bmp.bmWidth, bmp.bmHeight, TRUE);
		SendMessage(hButton4, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);

		x = 50; y += bmp.bmHeight + 50;
		hBitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(IDB_BIT_FILE), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		HWND hButton5 = CreateWindow("button", "Click Me", BS_BITMAP | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			x, y, x + bmp.bmWidth, y + bmp.bmHeight, hWnd, (HMENU)5, hInst, NULL);
		MoveWindow(hButton5, x, y, bmp.bmWidth, bmp.bmHeight, TRUE);
		SendMessage(hButton5, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
		
		return 0;
	}

	

	else if (msg == WM_DESTROY)
	{
		destoryThread();
		destoryWinsock();
		::PostQuitMessage(0);
		return 0;
	}

	else if (msg == WM_PAINT)
	{		

		return 0;
	}

	else if (msg == WM_COMMAND)
	{
		switch (LOWORD(wParam))
		{
		case 1: //�������� --> ���� ���� ���� �� accept Ȱ��ȭ 
			//���̾�α� Ȱ��ȭ
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);

			if (!statusSevrer() && port > 0)
			{
				initializeWinsock();
				//������� ���� ����
				ServerThread = (HANDLE)_beginthreadex(0, 0, startServer, (void*)&port, 0, 0);
			}
			else
			{
				if (port == 0)
					cout << "��Ʈ��ȣ �Է¾���" << endl;
				else
					cout << "���� ������" << endl;
			}
			
			
			/*map<string, class serverFunc *> FindIter = */
			
			//MessageBox(hWnd, "aa", "ok", MB_OK);
			break;

		case 2:
			destoryThread();
			break;

		case 3:
			break;

		case 4: //Ʈ�� ����
			break;

		case 5: //Ʈ�� ���� ���
			/*for (int i = 0; i < 100; i++)
			{
				carArg a;
				memset(&a, 0, sizeof(a));
				BSTInsert(&sample, i, a);
			}
			ofstream fout("Ʈ���������.txt");
			printPretty(sample, 3, 0, fout);*/

			
			
			break;

		}
		
		

		return 0;
	}

	else if (msg == WM_KEYDOWN)
	{
		switch (wParam)
		{
			//esc������ ����
		case VK_ESCAPE:
			destoryThread();
			destoryWinsock();
			PostQuitMessage(0);
			break;
		}

		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
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
		case IDOK:
			port = GetDlgItemInt(hDlg, IDC_EDIT_PORT, NULL, FALSE);
			EndDialog(hDlg, IDOK);			
			return TRUE;			
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//�������ı�
void destoryThread()
{
	if (ServerThread)
	{
		TerminateThread(ServerThread, 0);
		CloseHandle(ServerThread);
	}

	StopServer();

	//destoryWinsock();

}