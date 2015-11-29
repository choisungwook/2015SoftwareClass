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
#include <regex>
#include <vector>
using namespace std;

#define WIDTH 300
#define HEIGHT 100
#define CARIDSIZE 5
#define DEFAULTPORT 3333

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
extern void token(regex &re, string msg, vector<string> &a);

extern AVLTree car;
int movieNumber; 
list<string> movielist;
HWND	tmphWnd;
HANDLE	serverThread;

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
	OPENFILENAME OFN;        // OFN 구조체 선언(파일 열때).
	OPENFILENAME SFN;        // SFN 구조체 선언(저장)               // 하나의 구조체로 해도 상관없지만, 구분되게 보여주기 위해 사용.

	if (iMessage == WM_CREATE)
	{
		initializeWinsock(); //소켓 초기화
		initializeCommand(); //커맨드 초기화
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
		case ID_FILE_OPEN: //파일열기
			memset(&OFN, 0, sizeof(OPENFILENAME));    // memory 초기화  OFN구조체에 0으로 OPENFILENAME만큼의 크기를 초기화 해준다.
			OFN.lStructSize = sizeof(OPENFILENAME); // OFN 의 구조체크기를 OPENFILENAME의 크기를 넣어준다.
			OFN.hwndOwner = hWnd;              // OFN의 핸들 위치는 Wnd위치를 갖고 있는 핸들러를 갖는다.
			OFN.lpstrFilter = filter;                // 위에서 선언한 filter 파일을 받아 TEXT파일, 워드파일, 모든파일의 확장자를 찾아준다.
			OFN.lpstrFile = lpstrFile;            //열 파일의 이름(위치(?))을 나타내는 듯하다(?)
			OFN.nMaxFile = 100;                // 글자를 쓸수 있는 파일이름의 길이를 나타낸다.
			OFN.lpstrInitialDir = ".";            // 파일의 열기가 실행된 위치
			if (GetOpenFileName(&OFN) != 0)  // OFN(열려고 하는 선택된 파일)을 연다.
			{				
				wsprintf(str, "%s 파일을 열겠습니까?", OFN.lpstrFile);
				MessageBox(hWnd, str, "열기 선택", MB_OK);             // 파일을 열 건지 물어보는 메시지박스 창 생성(OK버튼 하나만 있다.)
				Fileopen(OFN.lpstrFile);
			}
			break;
		case ID_FILE_SAVE: //파일저장
			memset(&SFN, 0, sizeof(OPENFILENAME));         // 초기화를 해주기 때문에, 같은 변수를 사용해도 문제 없을 듯(?) 하다.
			SFN.lStructSize = sizeof(OPENFILENAME);
			SFN.hwndOwner = hWnd;
			SFN.lpstrFilter = filter;
			SFN.lpstrFile = lpstrFile;                      // 저장할 파일 이름(?)
			SFN.nMaxFile = 256;                           // 저장하려는 파일 크기를 256
			SFN.lpstrInitialDir = ".";
			if (GetSaveFileName(&SFN) != 0)          // SFN에 저장할 파일이 있는지 묻는 부분.
			{	
				wsprintf(str, "%s 파일로 저장하겠습니까?", SFN.lpstrFile);
				MessageBox(hWnd, str, "저장하기 선택", MB_OK);
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
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_START), hWnd, ServerStartProc);
			break;
		case ID_SERVER_STOP://서버중지
			if(destoryServerThread())
				MessageBox(hWnd, "server successfully finish", "exit", MB_OK);
			else
				MessageBox(hWnd, "server is stopped", "stopped", MB_OK);
			break;
		case ID_DATA_PRETTY:
			{
				ofstream fout("tree_pretty.txt");
				printPretty(car.getRoot(), 3, 0, fout);
				fout.close();

				WinExec("notepad.exe tree_pretty.txt", SW_NORMAL);
				break;
			}
		case ID_SERVER_STATUS:
			{
				if (statusSevrer())									
					MessageBox(tmphWnd, "server is running...", "ruuning", MB_OK);
				
				else
					MessageBox(tmphWnd, "server is stopped...", "stopped", MB_OK);
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
				SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)Number++); //데이터 추가

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
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		case IDOK:
		{			
			EndDialog(hDlg, 0);
			return TRUE;
		}
		case IDC_BUTTON_DEL:
		{
			char selectcar[CARIDSIZE] = { 0, };
			int index;
			HWND hwndList = GetDlgItem(hDlg, IDC_LIST_CARID);

			//get data
			int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);			
			SendMessage(hwndList, LB_GETTEXT, (WPARAM)lbItem, (LPARAM)selectcar);
			sscanf(selectcar, "%d", &index);
			
			//remove key
			SendMessage(hwndList, LB_DELETESTRING, lbItem, 0);
			
			car.remove(index);
			break;
		}
		//seasrch
		case IDC_BUTTON_SEARCH:
		{
			HWND hwndList = GetDlgItem(hDlg, IDC_LIST_CARID);

			char searchData[20];
			GetDlgItemText(hDlg, IDC_EDIT_SEARCH, searchData, sizeof(searchData));

			if (!strlen(searchData))			
				MessageBox(tmphWnd, "Please input at least one", "error", MB_OK);
			else
			{
				int r = SendMessage(hwndList, LB_FINDSTRINGEXACT, -1, (LPARAM)searchData);

				if (r == -1)
					MessageBox(tmphWnd, "Not found", "error", MB_OK);
				else
					SendMessage(hwndList, LB_SETCURSEL, (WPARAM)r, 0);
			}			

			break;
		}			
		case IDC_LIST_CARID: 
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				{					
					char selectcar[CARIDSIZE] = { 0, };
					int index;
					HWND hwndList = GetDlgItem(hDlg, IDC_LIST_CARID);

					//get data
					int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);								
					SendMessage(hwndList, LB_GETTEXT, (WPARAM)lbItem, (LPARAM)selectcar);					
					sscanf(selectcar, "%d", &index);

					//search Data
					AVLNODE* finder =  car.search(index);
					
					if (finder)
					{
						SetDlgItemInt(hDlg, IDC_EDIT_sort, finder->Data.carnumber, TRUE); //sort of car
						SetDlgItemInt(hDlg, IDC_EDIT_PRICE, finder->Data.totalPrice, TRUE); //total price
						SetDlgItemText(hDlg, IDC_EDIT_LASTVISIT, finder->Data.lastVisit.c_str()); //lastvisit
						SetDlgItemInt(hDlg, IDC_EDIT_TIME, finder->Data.totalmovieTime, TRUE); //total movie time

						//show movie list
						int ListNumber = 0;
						HWND MovieListhWnd = GetDlgItem(hDlg, IDC_LIST_MOVIE);
						SendMessage(MovieListhWnd, LB_RESETCONTENT, 0, 0);
						list<string>::iterator End = finder->Data.movieName.end();
						for (list<string>::iterator iterPos = finder->Data.movieName.begin();
							iterPos != End; iterPos++)
						{
							int pos = (int)SendMessage(MovieListhWnd, LB_ADDSTRING, 0, (LPARAM)iterPos->c_str());
							SendMessage(MovieListhWnd, LB_SETITEMDATA, pos, (LPARAM)ListNumber++); 
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
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
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

				MessageBox(hDlg, "입력을 성공했습니다", "ok", MB_OK);
				break;
			}		
		case ID_INPUT_CANCEL:
			{				
				movieNumber = 0;
				movielist.clear();
				EndDialog(hDlg, 0);
				break;
			}
		case IDC_BUTTON_MOVIE_ADD: //add movie
			{
				char buf[100]; 
				HWND hwndList = GetDlgItem(hDlg, IDC_LIST_MOVIELIST);
				GetDlgItemText(hDlg, IDC_EDIT_INPUT_MOVIE, buf, sizeof(buf));
				int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)buf);
				SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)movieNumber++); 
				//add list
				movielist.push_back(buf);
				break;
			}
		case IDC_BUTTON_MOVIE_DEL:
			{
				char deletemovie[100];
				HWND hwndList = GetDlgItem(hDlg, IDC_LIST_MOVIELIST);
				//get data
				int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
				SendMessage(hwndList, LB_GETTEXT, (WPARAM)lbItem, (LPARAM)deletemovie);
				//remove data
				SendMessage(hwndList, LB_DELETESTRING, lbItem, 0);				
				movielist.remove(deletemovie); //remove list
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
		SetDlgItemInt(hDlg, IDC_EDIT_PORT, DEFAULTPORT, FALSE);
		return TRUE;
	}

	else if (iMessage == WM_COMMAND)
	{		
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
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


int validateinput(string in)
{
	if (in.length() == 0)
		return -1;

	regex re("\\d+");

	if (regex_match(in, re))
	{
		int r;
		sscanf(in.c_str(), "%d", &r);
		return r;
	}
	return -1;
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
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		case ID_RAND_OK:
		{
			char value[10] = { 0, };
			int min, max, n;

			GetDlgItemText(hDlg, IDC_EDIT_RAND_NUMBER, value, sizeof(value)); 

			if ( (n = validateinput(value)) == -1)
				MessageBox(tmphWnd, "can't input character or blank", "error", MB_OK);
			else
			{
				
				GetDlgItemText(hDlg, IDC_EDIT_RAND_MIN, value, sizeof(value));

				if ((min = validateinput(value)) == -1)
					MessageBox(tmphWnd, "can't input character or blank", "error", MB_OK);
				else
				{					
					GetDlgItemText(hDlg, IDC_EDIT_RAND_MAX, value, sizeof(value));

					if ((max = validateinput(value)) == -1)
						MessageBox(tmphWnd, "can't input character or blank", "error", MB_OK);
					else
					{
						char buf[100];
						sprintf(buf, "%d %d %d", n, min, max);
						OutputDebugString(buf);
						if (min > max)
							MessageBox(tmphWnd, "MIN must be less than MAX", "error", MB_OK);
						else
						{
							int successCount = 0;

							mt19937 engine((unsigned int)time(NULL));
							uniform_int_distribution<int> distribution(min, max);
							auto generator = bind(distribution, engine);

							for (int i = 0; i < n; i++)
							{
								int carnumber = generator();
								carArgument t(carnumber);								
								if (car.insert(carnumber, t))
									successCount++;
							}	

							char resultmsg[100]; sprintf(resultmsg, "%d 생성했습니다", successCount);
							MessageBox(tmphWnd, resultmsg, "success", MB_OK);
						}
					}
				}
			}
					
			
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
		std::this_thread::sleep_for(std::chrono::seconds(1));; //메인쓰레드 sleep
		TerminateThread(serverThread, 0);
		serverThread = NULL;
		return true;
	}	

	return false;
}