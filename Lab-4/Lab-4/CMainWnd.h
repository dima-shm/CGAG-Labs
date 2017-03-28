#pragma once
#include "resource1.h"
#include "CMatrix.h"
#include "Lib.h"
#include "CPlot2D.h"
#include "MyMode.h"
#include "CSunSystem.h"
#include "AffineTransformation.h"


class CMainWnd : public CFrameWnd
{
private:
	bool IsData;
	int dT_Timer;
	CRect RW;
	CRectD RS;
	CSunSystem SunSystem;
	CMenu menu;
	DECLARE_MESSAGE_MAP()
	int OnCreate(LPCREATESTRUCT);

public:
	CMainWnd::CMainWnd() {
		Create(NULL, L"Lab-4", WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX, CRect(10, 10, 700, 730), NULL, NULL);
		IsData = false;
	}

	void OnPaint();

	void OnLabPlanets();
	void OnTimer(UINT_PTR);
	void OnLButtonDblClk(UINT, CPoint);
	void OnRButtonDblClk(UINT, CPoint);

	void Exit();
};

BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_COMMAND(ID_LABPLANETS_PLANETS, OnLabPlanets)
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
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
	
	if (IsData) {
		SunSystem.GetRS(RS);
		RW = SunSystem.GetRW();
		SetMyMode(dc, RS, RW); // Устанавливаем режим MM_ANISOTROPIC
		SunSystem.Draw(dc);
		dc.SetMapMode(MM_TEXT);
	}
}

void CMainWnd::OnLabPlanets()
{
	SunSystem.SetDT(0);
	SunSystem.SetNewCoords();
	SunSystem.SetDT(0.2);
	dT_Timer = 100;
	IsData = true;
	Invalidate();
}
void CMainWnd::OnTimer(UINT_PTR nIDEvent)
{
	SunSystem.SetNewCoords();
	Invalidate();
	CWnd::OnTimer(nIDEvent);
}
void CMainWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (IsData) {
		SetTimer(1, dT_Timer, NULL);
		CWnd::OnLButtonDblClk(nFlags, point);
	}
}
void CMainWnd::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	KillTimer(1);
	CWnd::OnRButtonDblClk(nFlags, point);
}

void CMainWnd::Exit()
{
	DestroyWindow();
}