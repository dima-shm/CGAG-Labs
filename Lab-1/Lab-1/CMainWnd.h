#pragma once
#include <ctime>
#include "resource.h"
#include "CMatrix.h"

void PrintMatrix(CDC&, int, int, CMatrix&);
CMatrix VectorMult(CMatrix&, CMatrix&);
double ScalarMult(CMatrix&, CMatrix&);
double ModVec(CMatrix&);
double CosV1V2(CMatrix&, CMatrix&);
CMatrix SphereToCart(CMatrix&);

class CMainWnd : public CFrameWnd
{
private:
	enum { MatrixC1, MatrixC2, MatrixD, MatrixQ, MatrixP, 
		   FuncVectorMult, FuncScalarMult, FuncModVec, 
		   FuncCosV1V2, FuncSphereToCart, Default, clear } condition;
	CMatrix *A, *B, *V1, *V2;
	static int num;
	CMenu menu;
	DECLARE_MESSAGE_MAP()
	int OnCreate(LPCREATESTRUCT);
	void initMatrix(CMatrix&);

public:
	CMainWnd::CMainWnd() {
		Create(NULL, L"Lab-1", WS_OVERLAPPEDWINDOW, CRect(10, 10, 600, 600), NULL, NULL);

		condition = Default;

		A = new CMatrix(3, 3);
		B = new CMatrix(3, 3);
		V1 = new CMatrix(3);
		V2 = new CMatrix(3);
		initMatrix(*A);
		initMatrix(*B);
		initMatrix(*V1);
		initMatrix(*V2);
	}
	
	void OnPaint();

	void MenuTestMatrixC1();
	void MenuTestMatrixC2();
	void MenuTestMatrixD();
	void MenuTestMatrixQ();
	void MenuTestMatrixP();
	
	void MenuTestFunctionVectorMult();
	void MenuTestFunctionScalarMult();
	void MenuTestFunctionModVec();
	void MenuTestFunctionCosV1V2();
	void MenuTestFunctionSphereToCart();

	void Clear();
};

int CMainWnd::num = 2;

BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd)
	ON_WM_CREATE() /* Событие создания окна */
	ON_WM_PAINT()
	ON_COMMAND(ID_MATRIX_C1, MenuTestMatrixC1)
	ON_COMMAND(ID_MATRIX_C2, MenuTestMatrixC2)
	ON_COMMAND(ID_MATRIX_D, MenuTestMatrixD)
	ON_COMMAND(ID_MATRIX_Q, MenuTestMatrixQ)
	ON_COMMAND(ID_MATRIX_P, MenuTestMatrixP)
	ON_COMMAND(ID_FUNCTIONS_40005, MenuTestFunctionVectorMult)
	ON_COMMAND(ID_FUNCTIONS_40006, MenuTestFunctionScalarMult)
	ON_COMMAND(ID_FUNCTIONS_40007, MenuTestFunctionModVec)
	ON_COMMAND(ID_FUNCTIONS_40008, MenuTestFunctionCosV1V2)
	ON_COMMAND(ID_FUNCTIONS_40009, MenuTestFunctionSphereToCart)
	ON_COMMAND(ID_TESTS_CLEAR40020, Clear)
	ON_WM_PAINT()
END_MESSAGE_MAP()
int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	menu.LoadMenu(IDR_MENU1);
	SetMenu(&menu);
	return 0;
}

void CMainWnd::OnPaint()
{
	CPaintDC dc(this);
	switch (condition)
	{
	case MatrixC1: {
		TextOut(dc, 150, 200, L"A", 1); PrintMatrix(dc, 150, 220, *A);
		TextOut(dc, 250, 200, L"B", 1); PrintMatrix(dc, 250, 220, *B);
		TextOut(dc, 350, 200, L"A + B", 5); PrintMatrix(dc, 350, 220, (*A) + (*B));
	} break;
	case MatrixC2: {
		TextOut(dc, 150, 200, L"A", 1); PrintMatrix(dc, 150, 220, *A);
		TextOut(dc, 250, 200, L"B", 1); PrintMatrix(dc, 250, 220, *B);
		TextOut(dc, 350, 200, L"A * B", 5); PrintMatrix(dc, 350, 220, (*A) * (*B));
	} break;
	case MatrixD: {
		TextOut(dc, 150, 200, L"A", 1);  PrintMatrix(dc, 150, 220, *A);
		TextOut(dc, 250, 200, L"V1", 2); PrintMatrix(dc, 250, 220, *V1);
		TextOut(dc, 350, 200, L"A * V1", 6); PrintMatrix(dc, 350, 220, (*A) * (*V1));
	} break;
	case MatrixQ: {
		TextOut(dc, 150, 200, L"V1", 2); PrintMatrix(dc, 150, 220, *V1);
		TextOut(dc, 250, 200, L"V1", 2); PrintMatrix(dc, 250, 220, *V2);
		TextOut(dc, 350, 200, L"V1^T * V2", 9); PrintMatrix(dc, 350, 220, V1->Transp() * (*V2));
	} break;
	case MatrixP: {
		TextOut(dc, 150, 200, L"V1", 2); PrintMatrix(dc, 150, 220, *V1);
		TextOut(dc, 150, 200, L"A", 1);  PrintMatrix(dc, 150, 220, *A);
		TextOut(dc, 250, 200, L"V1", 2); PrintMatrix(dc, 250, 220, *V2);
		TextOut(dc, 350, 200, L"V1^T * A * V2", 13); PrintMatrix(dc, 350, 220, V1->Transp() * (*A) * (*V2));
	} break;
	case FuncVectorMult: {
		TextOut(dc, 150, 200, L"V1", 2); PrintMatrix(dc, 150, 220, *V1);
		TextOut(dc, 250, 200, L"V1", 2); PrintMatrix(dc, 250, 220, *V2);
		TextOut(dc, 350, 200, L"V1 *VectorMult* V2", 18); PrintMatrix(dc, 350, 220, VectorMult(*V1, *V2));
	} break;
	case FuncScalarMult: {
		WCHAR buf[5];
		TextOut(dc, 150, 200, L"V1", 2); PrintMatrix(dc, 150, 220, *V1);
		TextOut(dc, 250, 200, L"V2", 2); PrintMatrix(dc, 250, 220, *V2);
		TextOut(dc, 350, 200, L"V1 *ScalarMult* V2", 18); TextOut(dc, 350, 220, _itow(ScalarMult(*V1, *V2), buf, 10), 2);
	} break;
	case FuncModVec: {
		WCHAR buf[5];
		TextOut(dc, 150, 200, L"V1", 2); PrintMatrix(dc, 150, 220, *V1);
		TextOut(dc, 250, 200, L"|V1|", 4); TextOut(dc, 250, 220, _itow(ModVec(*V1), buf, 10), 2);
	} break;
	case FuncCosV1V2: {
		WCHAR buf[5];
		TextOut(dc, 150, 200, L"V1", 2); PrintMatrix(dc, 150, 220, *V1);
		TextOut(dc, 250, 200, L"V2", 2); PrintMatrix(dc, 250, 220, *V2);
		TextOut(dc, 350, 200, L"Cos V1 V2", 9); TextOut(dc, 350, 220, _itow(CosV1V2(*V1, *V2), buf, 10), 2);
	} break;
	case FuncSphereToCart: {
		TextOut(dc, 150, 200, L"V1", 2); PrintMatrix(dc, 10, 390, *V1);
		TextOut(dc, 250, 200, L"SphereToCart V1", 15); PrintMatrix(dc, 190, 390, SphereToCart(*V1));
	} break;
	case clear: {
		Invalidate();
	} break;
	}
}

void CMainWnd::initMatrix(CMatrix &obj)
{
	srand(time(NULL));
	for (int i = 0; i < obj.rows(); i++)
		for (int j = 0; j < obj.cols(); j++)
			obj(i, j) = 0 + rand() % num;
	num++;
	if (num > 4)
		num = 2;
}

void CMainWnd::MenuTestMatrixC1()
{
	condition = MatrixC1;
	Invalidate();
}
void CMainWnd::MenuTestMatrixC2()
{
	condition = MatrixC2;
	Invalidate();
}
void CMainWnd::MenuTestMatrixD()
{
	condition = MatrixD;
	Invalidate();
}
void CMainWnd::MenuTestMatrixQ()
{
	condition = MatrixQ;
	Invalidate();
}
void CMainWnd::MenuTestMatrixP()
{
	condition = MatrixP;
	Invalidate();
}

void CMainWnd::MenuTestFunctionVectorMult()
{
	condition = FuncVectorMult;
	Invalidate();
}
void CMainWnd::MenuTestFunctionScalarMult()
{
	condition = FuncScalarMult;
	Invalidate();
}
void CMainWnd::MenuTestFunctionModVec()
{
	condition = FuncModVec;
	Invalidate();
}
void CMainWnd::MenuTestFunctionCosV1V2()
{
	condition = FuncCosV1V2;
	Invalidate();
}
void CMainWnd::MenuTestFunctionSphereToCart()
{
	condition = FuncSphereToCart;
	Invalidate();
}

void CMainWnd::Clear()
{
	condition = clear;
	Invalidate();
}