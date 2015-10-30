#ifndef __MAIN_H__
#define __MAIN_H__

#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

extern	HINSTANCE	g_hInst;
extern	HWND		hWndMain;
extern	HBITMAP		hbackbit; //¹é¹öÆÛ
#endif