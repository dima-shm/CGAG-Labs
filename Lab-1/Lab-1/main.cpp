#define _CRT_SECURE_NO_WARNINGS
#include <afxwin.h>
#include <math.h>
#include "CMainWnd.h"
#include "CMyApp.h"

void PrintMatrix(CDC& dc, int x, int y, CMatrix& M)
{
	WCHAR buf[5];
	for (int i = 0; i < M.rows(); i++)
		for (int j = 0; j < M.cols(); j++)
		{
			_itow(M(i, j), buf, 10);		
			TextOut(dc, x + i * 15, y + j * 15, buf, 2);
		}
}
CMatrix VectorMult(CMatrix& V1, CMatrix& V2)
{
	CMatrix temp(V1.rows());
		temp(0) = (V1(1)*V2(2)) - (V1(2)*V2(1));
		temp(1) = (V1(2)*V2(0)) - (V1(0)*V2(2));
		temp(2) = (V1(0)*V2(1)) - (V1(1)*V2(0));
	return temp;
}
double ScalarMult(CMatrix& V1, CMatrix& V2)
{
	return V1(0)*V2(0) + V1(1)*V2(1) + V1(2)*V2(2);
}
double ModVec(CMatrix& V)
{
	return sqrt((V(0)*V(0)) + (V(1)*V(1)) + (V(2)*V(2)));
}
double CosV1V2(CMatrix& V1, CMatrix& V2)
{
	return(V1(0)*V2(0) + V1(1)*V1(1) + V1(2)*V1(2)) / (ModVec(V1)*ModVec(V2));
}
CMatrix SphereToCart(CMatrix& PView)
{
	CMatrix temp(PView.rows());
	temp(0) = PView(2) * cos(PView(1)) * sin(PView(0));
	temp(1) = PView(2) * sin(PView(1)) * sin(PView(0));
	temp(2) = PView(2) * cos(PView(0));
	return temp;
}

CMyApp theApp;