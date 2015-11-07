#include "socket.h"
#include "function.h"
#include "avltree.h"
#include "resource.h"
#include <stack>
#include <windows.h>
#include <process.h>
#include <chrono>         // std::chrono::seconds
#include <thread>
#include <fstream>
#include <random>
using namespace std;

#define WIDTH 300
#define HEIGHT 100
#define CARIDSIZE 5

BOOL CALLBACK TreeOutputProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK TreeInputProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ServerStartProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ServerRandProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool destoryServerThread();

HINSTANCE g_hInst;
LPSTR lpszClass = "Server";

extern void Filesave(char* path);
extern void Fileopen(char *path);
extern void printPretty(AVLNODE *root, int level, int indentSpace, std::ostream& out);


extern void DebugFunc(TCHAR *pszErr, ...);
extern AVLTree car;
int movieNumber; 
list<string> movielist;
HWND	tmphWnd;
HANDLE	serverThread;
HANDLE  M_accessTree;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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

	tmphWnd = hWnd;
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	char str[100], lpstrFile[100] = "";
	char filter[] = "Binary\0*.bin\0All\0*.*\0";
	OPENFILENAME OFN;        // OFN ����ü ����(���� ����).
	OPENFILENAME SFN;        // SFN ����ü ����(����)               // �ϳ��� ����ü�� �ص� ���������, ���еǰ� �����ֱ� ���� ���.

	if (iMessage == WM_CREATE)
	{
		initializeWinsock(); //���� �ʱ�ȭ
		initializeCommand(); //Ŀ�ǵ� �ʱ�ȭ
	}
	else if (iMessage == WM_DESTROY)
	{
		destoryServerThread();
		destoryWinsock();
		PostQuitMessage(0);		
		return 0;
	}
	else if (iMessage == WM_COMMAND)
	{
		switch (LOWORD(wParam))
		{
		case ID_FILE_OPEN: //���Ͽ���
			memset(&OFN, 0, sizeof(OPENFILENAME));    // memory �ʱ�ȭ  OFN����ü�� 0���� OPENFILENAME��ŭ�� ũ�⸦ �ʱ�ȭ ���ش�.
			OFN.lStructSize = sizeof(OPENFILENAME); // OFN �� ����üũ�⸦ OPENFILENAME�� ũ�⸦ �־��ش�.
			OFN.hwndOwner = hWnd;              // OFN�� �ڵ� ��ġ�� Wnd��ġ�� ���� �ִ� �ڵ鷯�� ���´�.
			OFN.lpstrFilter = filter;                // ������ ������ filter ������ �޾� TEXT����, ��������, ��������� Ȯ���ڸ� ã���ش�.
			OFN.lpstrFile = lpstrFile;            //�� ������ �̸�(��ġ(?))�� ��Ÿ���� ���ϴ�(?)
			OFN.nMaxFile = 100;                // ���ڸ� ���� �ִ� �����̸��� ���̸� ��Ÿ����.
			OFN.lpstrInitialDir = ".";            // ������ ���Ⱑ ����� ��ġ
			if (GetOpenFileName(&OFN) != 0)  // OFN(������ �ϴ� ���õ� ����)�� ����.
			{				
				wsprintf(str, "%s ������ ���ڽ��ϱ�?", OFN.lpstrFile);
				MessageBox(hWnd, str, "���� ����", MB_OK);             // ������ �� ���� ����� �޽����ڽ� â ����(OK��ư �ϳ��� �ִ�.)
				Fileopen(OFN.lpstrFile);
			}
			break;
		case ID_FILE_SAVE: //��������
			memset(&SFN, 0, sizeof(OPENFILENAME));         // �ʱ�ȭ�� ���ֱ� ������, ���� ������ ����ص� ���� ���� ��(?) �ϴ�.
			SFN.lStructSize = sizeof(OPENFILENAME);
			SFN.hwndOwner = hWnd;
			SFN.lpstrFilter = filter;
			SFN.lpstrFile = lpstrFile;                      // ������ ���� �̸�(?)
			SFN.nMaxFile = 256;                           // �����Ϸ��� ���� ũ�⸦ 256
			SFN.lpstrInitialDir = ".";
			if (GetSaveFileName(&SFN) != 0)          // SFN�� ������ ������ �ִ��� ���� �κ�.
			{	
				wsprintf(str, "%s ���Ϸ� �����ϰڽ��ϱ�?", SFN.lpstrFile);
				MessageBox(hWnd, str, "�����ϱ� ����", MB_OK);
				Filesave(SFN.lpstrFile);				
			}
			break;	
		case ID_DATA_INPUTDATA:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_INPUT), hWnd, TreeInputProc);
			break;
		case ID_DATA_PRINTDATA:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_OUTPUT), hWnd, TreeOutputProc);
			break;
		case ID_SERVER_START:
			M_accessTree = CreateMutex(NULL, FALSE, NULL);
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_START), hWnd, ServerStartProc);
			break;
		case ID_SERVER_STOP://��������
			if(destoryServerThread())
				MessageBox(hWnd, "�������ᰡ ���������� �������ϴ�", "exit", MB_OK);
			else
				MessageBox(hWnd, "�ش缭���� �����ϴ�.", "exit", MB_OK);
			break;
		case ID_DATA_PRETTY:
			{
				ofstream fout("tree_pretty.txt");
				printPretty(car.getRoot(), 3, 0, fout);
				fout.close();

				WinExec("notepad.exe tree_pretty.txt", SW_NORMAL);
				break;
			}
		case ID_DATA_RAND:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_RAND), hWnd, ServerRandProc);
			break;
		}	
		
		return 0;
	}	
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

BOOL CALLBACK TreeOutputProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	//RECT wrt, crt;
	 

	if (iMessage == WM_INITDIALOG)
	{
		/*GetWindowRect(GetParent(hDlg), &wrt);
		GetWindowRect(hDlg, &crt);
		SetWindowPos(hDlg, HWND_NOTOPMOST, wrt.left + (wrt.right - wrt.left) / 2 - (crt.right - crt.left) / 2,
			wrt.top + (wrt.bottom - wrt.top) / 2 - (crt.bottom - crt.top) / 2, 0, 0, SWP_NOSIZE);
*/
		HWND hwndList = GetDlgItem(hDlg, IDC_LIST_CARID);

		if (car.getRoot() != NULL)
		{
			int Number = 0;
			stack<AVLNODE*> nodestack;
			nodestack.push(car.getRoot());

			while (nodestack.empty() == false)
			{
				AVLNODE* node = nodestack.top();

				char buf[100] = { 0, };
				sprintf(buf, "%d", node->Data.carnumber);
				int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0,(LPARAM)buf);
				SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)Number++); //������ �߰�

				nodestack.pop();

				if (node->right)
					nodestack.push(node->right);
				if (node->left)
					nodestack.push(node->left);
			}
		}
		SetFocus(hwndList);		
		return TRUE;
	}
	else if (iMessage == WM_COMMAND)
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{			
			EndDialog(hDlg, 0);
			return TRUE;
		}
		case IDC_LIST_CARID: //����Ʈ�̺�Ʈ
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				{					
					char selectcar[CARIDSIZE] = { 0, };
					int index;
					HWND hwndList = GetDlgItem(hDlg, IDC_LIST_CARID);

					//�ε�����ȣ ����
					int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
					//������ ����					
					SendMessage(hwndList, LB_GETTEXT, (WPARAM)lbItem, (LPARAM)selectcar);					
					sscanf(selectcar, "%d", &index);

					AVLNODE* finder =  car.search(index);
					
					if (finder)
					{
						SetDlgItemInt(hDlg, IDC_EDIT_sort, finder->Data.carnumber, TRUE); //������
						SetDlgItemInt(hDlg, IDC_EDIT_PRICE, finder->Data.totalPrice, TRUE); //�ѱݾ�
						SetDlgItemText(hDlg, IDC_EDIT_LASTVISIT, finder->Data.lastVisit.c_str()); //�������湮��¥
						SetDlgItemInt(hDlg, IDC_EDIT_TIME, finder->Data.totalmovieTime, TRUE); //��ȭ�ð�

						////��ȭ���
						int ListNumber = 0;
						HWND MovieListhWnd = GetDlgItem(hDlg, IDC_LIST_MOVIE);
						SendMessage(MovieListhWnd, LB_RESETCONTENT, 0, 0);
						list<string>::iterator End = finder->Data.movieName.end();
						for (list<string>::iterator iterPos = finder->Data.movieName.begin();
							iterPos != End; iterPos++)
						{
							int pos = (int)SendMessage(MovieListhWnd, LB_ADDSTRING, 0, (LPARAM)iterPos->c_str());
							SendMessage(MovieListhWnd, LB_SETITEMDATA, pos, (LPARAM)ListNumber++); //������ �߰�
						}		
					}				
					
					break;
				}
			}
			break;
		}		
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK TreeInputProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	//RECT wrt, crt;
	
	if (iMessage == WM_INITDIALOG)
	{
		/*GetWindowRect(GetParent(hDlg), &wrt);
		GetWindowRect(hDlg, &crt);
		SetWindowPos(hDlg, HWND_NOTOPMOST, wrt.left + (wrt.right - wrt.left) / 2 - (crt.right - crt.left) / 2,
			wrt.top + (wrt.bottom - wrt.top) / 2 - (crt.bottom - crt.top) / 2, 0, 0, SWP_NOSIZE);
*/		
		return TRUE;
	}

	else if (iMessage == WM_COMMAND)
	{
		
		switch (LOWORD(wParam))
		{
		case ID_INPUT_OK:
			{
				int numOfcar = 0, numOfsort, totalPrice = 0, totalTime = 0;

				char lastVisit[20] = { 0, };
				numOfcar = GetDlgItemInt(hDlg, IDC_EDIT_INPUT_NUMBER, NULL, FALSE);
				numOfsort = GetDlgItemInt(hDlg, IDC_EDIT_INPUT_SORT, NULL, FALSE);
				totalPrice = GetDlgItemInt(hDlg, IDC_EDIT_INPUT_PRICE, NULL, FALSE);
				totalTime = GetDlgItemInt(hDlg, IDC_EDIT_INPUT_TIME, NULL, FALSE);
				GetDlgItemText(hDlg, IDC_EDIT_LASTVISIT, lastVisit, sizeof(lastVisit));
				if (strlen(lastVisit) == 0)	strcpy(lastVisit, "X");
				carArgument t(numOfcar, numOfsort, totalPrice, totalTime, lastVisit, movielist);
				car.insert(numOfcar, t);

				MessageBox(hDlg, "�Է��� �����߽��ϴ�", "ok", MB_OK);
				break;
			}		
		case ID_INPUT_CANCEL:
			{				
				movieNumber = 0;
				movielist.clear();
				EndDialog(hDlg, 0);
				break;
			}
		case IDC_BUTTON_MOVIE_ADD: //��ȭ�Է�
			{
				char buf[100]; 
				HWND hwndList = GetDlgItem(hDlg, IDC_LIST_MOVIELIST);
				GetDlgItemText(hDlg, IDC_EDIT_INPUT_MOVIE, buf, sizeof(buf));
				int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)buf);
				SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)movieNumber++); //������ �߰�
				//����Ʈ �߰�				
				movielist.push_back(buf);
				break;
			}
		case IDC_BUTTON_MOVIE_DEL:
			{
				char deletemovie[100];
				HWND hwndList = GetDlgItem(hDlg, IDC_LIST_MOVIELIST);
				//�ε�����ȣ ����
				int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
				SendMessage(hwndList, LB_GETTEXT, (WPARAM)lbItem, (LPARAM)deletemovie);
				//�����ͻ���
				SendMessage(hwndList, LB_DELETESTRING, lbItem, 0);
				//����Ʈ����
				movielist.remove(deletemovie);
				break;
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK ServerStartProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	//RECT wrt, crt;

	if (iMessage == WM_INITDIALOG)
	{
		/*GetWindowRect(GetParent(hDlg), &wrt);
		GetWindowRect(hDlg, &crt);
		SetWindowPos(hDlg, HWND_NOTOPMOST, wrt.left + (wrt.right - wrt.left) / 2 - (crt.right - crt.left) / 2,
		wrt.top + (wrt.bottom - wrt.top) / 2 - (crt.bottom - crt.top) / 2, 0, 0, SWP_NOSIZE);
		*/

		return TRUE;
	}

	else if (iMessage == WM_COMMAND)
	{

		switch (LOWORD(wParam))
		{
		case ID_BUTTON_START:
		{
			int port = GetDlgItemInt(hDlg, IDC_EDIT_PORT, NULL, FALSE);
			serverThread = (HANDLE)_beginthreadex(0, 0, startServer, (void*)&port, 0, 0);
			EndDialog(hDlg, 0);
			break;
		}
		case ID_BUTTON_STOP:
			EndDialog(hDlg, 0);
			break;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK ServerRandProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	//RECT wrt, crt;

	if (iMessage == WM_INITDIALOG)
	{
		/*GetWindowRect(GetParent(hDlg), &wrt);
		GetWindowRect(hDlg, &crt);
		SetWindowPos(hDlg, HWND_NOTOPMOST, wrt.left + (wrt.right - wrt.left) / 2 - (crt.right - crt.left) / 2,
		wrt.top + (wrt.bottom - wrt.top) / 2 - (crt.bottom - crt.top) / 2, 0, 0, SWP_NOSIZE);
		*/

		return TRUE;
	}

	else if (iMessage == WM_COMMAND)
	{
		switch (LOWORD(wParam))
		{
		case ID_RAND_OK:
		{
			int n = GetDlgItemInt(hDlg, IDC_EDIT_RAND_NUMBER, NULL, FALSE);
			int min = GetDlgItemInt(hDlg, IDC_EDIT_RAND_MIN, NULL, FALSE);
			int max = GetDlgItemInt(hDlg, IDC_EDIT_RAND_MAX, NULL, FALSE);

			mt19937 engine((unsigned int)time(NULL));                    // MT19937 ���� ����
			uniform_int_distribution<int> distribution(min, max);       // ���� ����
			auto generator = bind(distribution, engine);

			for (int i = 0; i < n; i++)
			{
				int carnumber = generator();
				carArgument t(carnumber);
				car.insert(carnumber, t);
			}
			MessageBox(hDlg, "�Է� ����", "OK", MB_OK);
			EndDialog(hDlg, 0);
			break;
		}
		case ID_RAND_CANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		return TRUE;
	}
	return FALSE;
}


bool destoryServerThread()
{
	if (serverThread)
	{
		StopServer();
		std::this_thread::sleep_for(std::chrono::seconds(1));; //���ξ����� sleep
		TerminateThread(serverThread, 0);
		serverThread = NULL;
		CloseHandle(M_accessTree);
		return true;
	}	

	return false;
}