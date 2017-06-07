#pragma once
#include "resource.h"
#include "CMatrix.h"
#include "Lib.h"
#include "CPlot2D.h"

class CMainWnd : public CFrameWnd
{
private:
	CMatrix X, Y;
	CRect RW;
	CRectD RS;
	CPlot2D Graph;
	CMyPen PenLine, PenAxis;
	int Index = 0;
	CMenu menu;
	DECLARE_MESSAGE_MAP()
	int OnCreate(LPCREATESTRUCT);

public:
	CMainWnd::CMainWnd()
	{
		Create(NULL, L"Lab-9", WS_OVERLAPPEDWINDOW, CRect(10, 10, 650, 600), NULL, NULL);
	}

	void OnPaint();

	void TestsF1();
	void TestsF2();
	void MenuTestsF1();
	void MenuTestsF2();

	void Exit();
};

BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_COMMAND(ID_LINES_LAGR, MenuTestsF1)
	ON_COMMAND(ID_LINES_BEZIER, MenuTestsF2)
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
	RW.SetRect(150, 50, 600, 450);
	if (Index == 1)
		Graph.Draw(dc, 0, 0);
	if (Index == 2)
		Graph.DrawBezier(dc, 10);
}

double Lagr(CMatrix& X, CMatrix& Y, double x, int size)
{
	double lagrange_pol = 0;
	double basics_pol;

	for (int i = 0; i < size; i++)
	{
		basics_pol = 1;
		for (int j = 0; j < size; j++)
		{
			if (j == i) 
				continue;
			basics_pol *= (x - X(j)) / (X(i) - X(j));
		}
		lagrange_pol += basics_pol*Y(i);
	}
	return lagrange_pol;
}

void CMainWnd::TestsF1()
{
	double dx = pi / 4;
	double xL = 0;
	double xH = pi;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);
	for (int i = 0; i <= N; i++)
	{
		X(i) = xL + i*dx;
		Y(i) = pow(2 + cos(X(i)), sin(2 * X(i)));
	}

	dx = 0.2;
	int NL = (xH - xL) / dx;
	CMatrix XL(NL + 1);
	CMatrix YL(NL + 1);

	for (int i = 0; i <= NL; i++)
	{
		XL(i) = xL + i*dx;
		YL(i) = Lagr(X, Y, XL(i), N + 1);
	}
	Graph.SetParams(XL, YL, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
}
void CMainWnd::TestsF2()
{
	CMatrix X(3);
	CMatrix Y(3);

	X(0) = 0;  Y(0) = 1;
	X(1) = 7;  Y(1) = 6;
	X(2) = 10; Y(2) = 0;

	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
}
void CMainWnd::MenuTestsF1()
{
	RW.SetRect(100, 50, 500, 450);
	PenLine.Set(PS_SOLID, 3, RGB(209, 0, 228));
	PenAxis.Set(PS_SOLID, 3, RGB(209, 0, 228));
	TestsF1();
	Index = 1;
	Invalidate();
}
void CMainWnd::MenuTestsF2()
{
	RW.SetRect(100, 50, 500, 450);
	PenLine.Set(PS_SOLID, 3, RGB(209, 0, 228));
	PenAxis.Set(PS_SOLID, 3, RGB(209, 0, 228));
	TestsF2();
	Index = 2;
	Invalidate();
}
void CMainWnd::Exit()
{
	DestroyWindow();
}