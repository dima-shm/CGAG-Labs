#pragma once
#include "resource.h"
#include "CMatrix.h"
#include "Lib.h"
#include "CPlot2D.h"


CMatrix SpaceToWindow(CRectD& RS, CRect& RW)
{
	CMatrix M(3, 3);
	CSize sz = RW.Size();          // Размер области в ОКНЕ
	int dwx = sz.cx;               // Ширина
	int dwy = sz.cy;               // Высота
	CSizeD szd = RS.SizeD();       // Размер области в МИРОВЫХ координатах

	double dsx = szd.cx;           // Ширина в мировых координатах
	double dsy = szd.cy;           // Высота в мировых координатах

	double kx = (double)dwx / dsx; // Масштаб по x
	double ky = (double)dwy / dsy; // масштаб по y

	M(0, 0) = kx; M(0, 1) = 0;   M(0, 2) = (double)RW.left - kx*RS.left;
	M(1, 0) = 0;  M(1, 1) = -ky; M(1, 2) = (double)RW.bottom + ky*RS.bottom;
	M(2, 0) = 0;  M(2, 1) = 0;   M(2, 2) = 1;

	return M;
}
void SetMyMode(CDC& dc, CRectD& RS, CRect& RW)
{
	double dsx = RS.right - RS.left;
	double dsy = RS.top - RS.bottom;
	double xsL = RS.left;
	double ysL = RS.bottom;

	int dwx = RW.right - RW.left;
	int dwy = RW.bottom - RW.top;
	int xwL = RW.left;
	int ywL = RW.bottom;

	dc.SetMapMode(MM_ANISOTROPIC);       // Установка режима отображения
	dc.SetWindowExt((int)dsx, (int)dsy); // Измененение ориентации и масштаба
	dc.SetViewportExt(dwx, -dwy);
	dc.SetWindowOrg((int)xsL, (int)ysL); // Установка смещения
	dc.SetViewportOrg(xwL, ywL);
}


class CMainWnd : public CFrameWnd
{
private:
	CMatrix X, Y;
	CRect RW;
	CRectD RS;
	CPlot2D Graph, Graphs[4];
	CMyPen PenLine, PenAxis;
	enum { menuTestsF1, menuTestsF2, menuTestsF3, menuTestsF4, menuTestsF1234, clear } condition;
	CMenu menu;
	DECLARE_MESSAGE_MAP()
	int OnCreate(LPCREATESTRUCT);

public:
	CMainWnd::CMainWnd() {
		Create(NULL, L"Lab-3", WS_OVERLAPPEDWINDOW, CRect(10, 10, 800, 600), NULL, NULL);
		condition = clear;
	}

	void OnPaint();

	void MenuTestsF1();
	void MenuTestsF2();
	void MenuTestsF3();
	void MenuTestsF4();
	void MenuTestsF1234();
	void Func1();
	void Func2();
	void Func3();
	void Func4();

	double MyF1(double);
	double MyF2(double);
	double MyF3(double);
	double MyF4(double);

	void Clear();
	void Exit();
};

BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_COMMAND(ID_TESTS_F1, MenuTestsF1)
	ON_COMMAND(ID_TESTS_F2, MenuTestsF2)
	ON_COMMAND(ID_TESTS_F3, MenuTestsF3)
	ON_COMMAND(ID_TESTS_F4, MenuTestsF4)
	ON_COMMAND(ID_TESTS_F1234, MenuTestsF1234)
	ON_COMMAND(ID_CLEAR, Clear)
	ON_COMMAND(ID_EXIT, Exit)
	ON_WM_PAINT()
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
	switch (condition)
	{
	case menuTestsF1: {
		Graph.Draw(dc, 1, 1);
	} break;
	case menuTestsF2: {
		Graph.GetRS(RS);
		SetMyMode(dc, RS, RW); // Устанавливаем режим отображения MM_ANISOTROPIC
		Graph.Draw1(dc, 1, 1);
		dc.SetMapMode(MM_TEXT); // Устанавливаем режим отображения MM_TEXT
	} break;
	case menuTestsF3: {
		Graph.Draw(dc, 1, 1);
	} break;
	case menuTestsF4: {
		Graph.GetRS(RS);
		SetMyMode(dc, RS, RW);
		Graph.Draw1(dc, 1, 1);
		dc.SetMapMode(MM_TEXT);
	} break;
	case menuTestsF1234: {
		Func1();
		Graphs[0].Draw(dc, 1, 1);

		Func3();
		Graphs[2].Draw(dc, 1, 1);

		Func2();
		Graphs[1].GetRS(RS);
		SetMyMode(dc, RS, RW);
		Graphs[1].Draw1(dc, 1, 1);
		dc.SetMapMode(MM_TEXT);

		Func4();
		Graphs[3].GetRS(RS);
		SetMyMode(dc, RS, RW);
		Graphs[3].Draw1(dc, 1, 1);
		dc.SetMapMode(MM_TEXT);
	} break;
	case clear: {
		Invalidate();
	} break;
	}
}

void CMainWnd::MenuTestsF1()
{
	double dx = pi / 36;
	double xL = -3 * pi;
	double xH = -xL;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);

	for (int i = 0; i <= N; i++) {
		X(i) = xL + i*dx;
		Y(i) = MyF1(X(i));
	}
	PenLine.Set(PS_SOLID, 1, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 255));
	RW.SetRect(150, 50, 600, 450);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);

	condition = menuTestsF1;
	this->Invalidate();
}
void CMainWnd::MenuTestsF2()
{
	double dx = 0.25;
	double xL = -5;
	double xH = -xL;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);

	for (int i = 0; i <= N; i++) {
		X(i) = xL + i*dx;
		Y(i) = MyF2(X(i));
	}
	PenLine.Set(PS_SOLID, 1, RGB(0, 255, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 255));
	RW.SetRect(150, 50, 600, 450);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);

	condition = menuTestsF2;
	this->Invalidate();
}
void CMainWnd::MenuTestsF3()
{
	double dx = pi / 36;
	double xL = 0;
	double xH = 6 * pi;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);

	for (int i = 0; i <= N; i++) {
		X(i) = xL + i*dx;
		Y(i) = MyF3(X(i));
	}
	PenLine.Set(PS_DASHDOT, 1, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 0));
	RW.SetRect(150, 50, 600, 450);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);

	condition = menuTestsF3;
	this->Invalidate();
}
void CMainWnd::MenuTestsF4()
{
	double dx = 0.25;
	double xL = -10;
	double xH = -xL;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);

	for (int i = 0; i <= N; i++) {
		X(i) = xL + i*dx;
		Y(i) = MyF4(X(i));
	}
	PenLine.Set(PS_SOLID, 2, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 255));
	RW.SetRect(150, 50, 600, 450);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);

	condition = menuTestsF4;
	this->Invalidate();
}
void CMainWnd::MenuTestsF1234()
{
	condition = menuTestsF1234;
	this->Invalidate();
}
void CMainWnd::Func1()
{
	double dx = pi / 36;
	double xL = -3 * pi;
	double xH = -xL;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);
	for (int i = 0; i <= N; i++) {
		X(i) = xL + i*dx;
		Y(i) = MyF1(X(i));
	}
	PenLine.Set(PS_SOLID, 1, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 255));
	RW.SetRect(50, 50, 300, 200);
	Graphs[0].SetParams(X, Y, RW);
	Graphs[0].SetPenLine(PenLine);
	Graphs[0].SetPenAxis(PenAxis);
}
void CMainWnd::Func2()
{
	double dx = 0.25;
	double xL = -5;
	double xH = -xL;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);
	for (int i = 0; i <= N; i++) {
		X(i) = xL + i*dx;
		Y(i) = MyF2(X(i));
	}
	PenLine.Set(PS_SOLID, 1, RGB(0, 255, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 255));
	RW.SetRect(450, 50, 700, 200);
	Graphs[1].SetParams(X, Y, RW);
	Graphs[1].SetPenLine(PenLine);
	Graphs[1].SetPenAxis(PenAxis);
}
void CMainWnd::Func3()
{
	double dx = pi / 36;
	double xL = 0;
	double xH = 6 * pi;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);
	for (int i = 0; i <= N; i++) {
		X(i) = xL + i*dx;
		Y(i) = MyF3(X(i));
	}
	PenLine.Set(PS_DASHDOT, 1, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 0));
	RW.SetRect(50, 300, 300, 450);
	Graphs[2].SetParams(X, Y, RW);
	Graphs[2].SetPenLine(PenLine);
	Graphs[2].SetPenAxis(PenAxis);
}
void CMainWnd::Func4()
{
	double dx = 0.25;
	double xL = -10;
	double xH = -xL;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);
	for (int i = 0; i <= N; i++) {
		X(i) = xL + i*dx;
		Y(i) = MyF4(X(i));
	}
	PenLine.Set(PS_SOLID, 2, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 255));
	RW.SetRect(450, 300, 700, 450);
	Graphs[3].SetParams(X, Y, RW);
	Graphs[3].SetPenLine(PenLine);
	Graphs[3].SetPenAxis(PenAxis);
}

double CMainWnd::MyF1(double x)
{
	return sin(x) / x;
}
double CMainWnd::MyF2(double x)
{
	return x*x*x;
}
double CMainWnd::MyF3(double x)
{
	return sqrt(x)*sin(x);
}
double CMainWnd::MyF4(double x)
{
	return x*x;
}

void CMainWnd::Clear()
{
	condition = clear;
	RedrawWindow();
}
void CMainWnd::Exit()
{
	DestroyWindow();
}