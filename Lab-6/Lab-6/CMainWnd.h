#pragma once
#include "resource.h"
#include "CMatrix.h"
#include <float.h> // DBL_MAX, DBL_MIN
#include <math.h>
#include <vector>

#include "LibGraph.h"
#include "LibSurface.h"


class CMainWnd : public CFrameWnd
{
private:
	CRect WinRect; // Область в окне
	int Index;
	double r, fi, q;
	CPlot3D Graph1, Graph2, Graph3;
	CMenu menu;
	DECLARE_MESSAGE_MAP()
	int OnCreate(LPCREATESTRUCT);

public:
	CMainWnd::CMainWnd()
	{
		Create(NULL, L"Lab-6", WS_OVERLAPPEDWINDOW, CRect(10, 10, 700, 700), NULL, NULL);
		Index = 0;
	}

	void OnPaint();
	void OnFunction1();
	void OnFunction2();
	void OnFunction3();
	void Exit();
};

BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_COMMAND(ID_FUNCTIONS_F1, OnFunction1)
	ON_COMMAND(ID_FUNCTIONS_F2, OnFunction2)
	ON_COMMAND(ID_FUNCTIONS_F3, OnFunction3)
	ON_COMMAND(ID_EXIT, Exit)
END_MESSAGE_MAP()
int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	menu.LoadMenu(IDR_MENU1); // Загрузить меню из файла ресурса
	SetMenu(&menu); // Установить меню
	return 0;
}


void CMainWnd::OnPaint()
{
	CPaintDC dc(this);
	CString ss;
	if (Index == 1)
	{
		double dx = 0.25, dy = 0.25;
		r = 50, fi = -45, q = 15;
		CRectD SpaceRect(-5, 5, 5, -5);
		this->GetClientRect(WinRect);
		WinRect.SetRect(WinRect.left + 350, WinRect.top + 560, WinRect.right - 50, WinRect.bottom - 50);
		Graph1.SetFunction(Function1, SpaceRect, dx, dy);
		Graph1.SetViewPoint(r, fi, q);
		Graph1.SetWinRect(WinRect);
		Graph1.Draw(dc);
	}
	if (Index == 2)
	{
		double dx = 0.25, dy = 0.25;
		r = 50, fi = 30, q = 415;
		CRectD SpaceRect(-5, 5, 5, -5);
		this->GetClientRect(WinRect);
		WinRect.SetRect(WinRect.left + 200, WinRect.top + 350, WinRect.right - 50, WinRect.bottom - 50);
		Graph2.SetFunction(Function2, SpaceRect, dx, dy);
		Graph2.SetViewPoint(r, fi, q);
		Graph2.SetWinRect(WinRect);
		Graph2.Draw(dc);
	}
	if (Index == 3)
	{
		double dx = 0.25, dy = 0.25;
		r = 50, fi = 30, q = 490;
		CRectD SpaceRect(-5, 5, 5, -5);
		this->GetClientRect(WinRect);
		WinRect.SetRect(WinRect.left + 200, WinRect.top + 400, WinRect.right - 50, WinRect.bottom - 50);
		Graph3.SetFunction(Function3, SpaceRect, dx, dy);
		Graph3.SetViewPoint(r, fi, q);
		Graph3.SetWinRect(WinRect);
		Graph3.Draw(dc);
	}
}


void CMainWnd::OnFunction1()
{
	Index = 1;
	Invalidate();
}
void CMainWnd::OnFunction2()
{
	Index = 2;
	Invalidate();
}
void CMainWnd::OnFunction3()
{
	Index = 3;
	Invalidate();
}
void CMainWnd::Exit()
{
	DestroyWindow();
}