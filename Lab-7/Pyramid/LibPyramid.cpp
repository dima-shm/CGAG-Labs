#include "stdafx.h"

CPyramid::CPyramid()
{
	Vertices.RedimMatrix(4,8);	// ABCDE:
								// ABCD - основание, 
								// A'B'C'D' верхнее основание
	Vertices(0,0)=6;		// A: x=6,y=0,z=0
	Vertices(0,4)=1;		// A': x=1,y=0,z=0
	Vertices(2,4)=6;		// A': x=6,y=0,z=10

	Vertices(1,1)=-6;	// B: x=0,y=-6,z=0
	Vertices(1,5)=-1;	// B': x=0,y=-1,z=0
	Vertices(2,5)=6;	// B': x=0,y=-6,z=6

	Vertices(0,2)=-6;	// C: x=-6,y=0,z=0
	Vertices(0,6)=-1;	// C': x=-1,y=0,z=0
	Vertices(2,6)=6;	// C': x=-1,y=0,z=6

	Vertices(1,3)=6;		// D: x=0,y=6,z=0
	Vertices(1,7)=1;		// D': x=0,y=1,z=0
	Vertices(2,7)=6;		// D': x=0,y=1,z=6

	for(int i=0;i<8;i++)
		Vertices(3,i)=1; 
}

void CPyramid::Draw1(CDC& dc,CMatrix& PView,CRect& RW)
{
	CMatrix ViewCart=SphereToCart(PView);
	CMatrix MV=CreateViewCoord(PView(0),PView(1),PView(2));	
	CMatrix ViewVert=MV*Vertices; 
	CRectD RectView;
	GetRect(ViewVert,RectView);		
	CMatrix MW=SpaceToWindow(RectView,RW);
                                          
	CPoint MasVert[8];
	CMatrix V(3);
	V(2)=1;	
	for(int i=0;i<8;i++)		
	{
		V(0)=ViewVert(0,i); // x
		V(1)=ViewVert(1,i); // y

		V=MW*V;	
		MasVert[i].x=(int)V(0);
		MasVert[i].y=(int)V(1);			
	}


	CPen Pen(PS_SOLID, 10, RGB(0, 255, 120));
	CPen* pOldPen =dc.SelectObject(&Pen);
	CMatrix R1(3),R2(3),VN(3);
	double sm;
	for(int i=0;i<4;i++)	
	{
		CMatrix VE=Vertices.GetCol(i + 4,0,2);
		int k;
		if(i==3) k=0;
		else k=i+1;
		R1=Vertices.GetCol(i,0,2);
		R2=Vertices.GetCol(k,0,2);
		CMatrix V1=R2-R1;			 
		CMatrix V2=VE-R1;			 
		VN=VectorMult(V2,V1);		 
		sm=ScalarMult(VN,ViewCart);  
		if(sm>=0) 
			{	
				dc.MoveTo(MasVert[i]);
				dc.LineTo(MasVert[k]);
				dc.LineTo(MasVert[k + 4]);
				dc.LineTo(MasVert[i + 4]);			
				dc.LineTo(MasVert[i]);
			}
	}
	CBrush brush1(RGB(255, 0, 0));
	CBrush brush2(RGB(0, 0, 255));
	COLORREF color = RGB(255, 0, 0);
	dc.SetDCBrushColor(color);
	dc.SelectObject(&brush1);
	VN=VectorMult(R1,R2);	
	sm=ScalarMult(VN,ViewCart);

	if (sm >= 0)
	{
		dc.Polygon(MasVert, 4);
	}
	else
	{
		dc.SelectObject(&brush2);
		dc.Polygon(MasVert + 4, 4);
	}
	

	dc.SelectObject(pOldPen);
}

void CPyramid::Draw(CDC& dc,CMatrix& PView,CRect& RW)
{
	CMatrix ViewCart=SphereToCart(PView);	
	CMatrix MV=CreateViewCoord(PView(0),PView(1),PView(2));									
	CMatrix ViewVert=MV*Vertices; 
	CRectD RectView;
	GetRect(ViewVert,RectView);		
	CMatrix MW=SpaceToWindow(RectView,RW);
                                          
	CPoint MasVert[8];
	CMatrix V(3);
	V(2)=1;
	
	for(int i=0;i<8;i++)		
	{
		V(0)=ViewVert(0,i); // x
		V(1)=ViewVert(1,i); // y		
		V=MW*V;			
		MasVert[i].x=(int)V(0);
		MasVert[i].y=(int)V(1);			
	}
// Рисуем

	CPen Pen(PS_SOLID, 10, RGB(0, 255, 120));
  CPen* pOldPen =dc.SelectObject(&Pen);  
  dc.MoveTo(MasVert[3]);
  for(int i = 0; i < 4; i++)
  {
	  dc.LineTo(MasVert[i]);
  }
  dc.MoveTo(MasVert[7]);
  for(int i = 4; i < 8; i++)
  {
	  dc.LineTo(MasVert[i]);
  }	
	for(int i=0;i<4;i++)	
	{
		dc.MoveTo(MasVert[i]);	
		dc.LineTo(MasVert[i+4]);
	}
// Координаты центра O 	пересечения диагоналей основания
	int x0=(MasVert[0].x+MasVert[2].x)/2;
	int y0=(MasVert[0].y+MasVert[2].y)/2;
	int x0s=(MasVert[4].x+MasVert[6].x)/2;
	int y0s=(MasVert[4].y+MasVert[6].y)/2;

	CPen Pen1(PS_DASH, 1, RGB(40, 255, 120));
	dc.SelectObject(&Pen1);
	dc.MoveTo(x0,y0);
	dc.LineTo(x0s,y0s);
	dc.MoveTo(MasVert[0]);	// Перо на вершину A
	dc.LineTo(MasVert[2]);	// Диагональ
	dc.MoveTo(MasVert[1]);	// Перо на вершину B
	dc.LineTo(MasVert[3]);	// Диагональ

	dc.MoveTo(MasVert[4]);	// Перо на вершину A'
	dc.LineTo(MasVert[6]);	// Диагональ
	dc.MoveTo(MasVert[5]);	// Перо на вершину B'
	dc.LineTo(MasVert[7]);	// Диагональ
	dc.SelectObject(pOldPen);
}


void CPyramid::ColorDraw(CDC& dc, CMatrix& PView, CRect& RW, COLORREF Color)
{
	BYTE red = GetRValue(Color);
	BYTE green = GetGValue(Color);
	BYTE blue = GetBValue(Color);

	CMatrix ViewCart = SphereToCart(PView);		//Преобразует сферические координаты PView  точки в декартовы
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2));

	CMatrix ViewVert = MV*Vertices;
	CRectD RectView;
	GetRect(ViewVert, RectView);
	CMatrix MW = SpaceToWindow(RectView, RW);

	CPoint MasVert[8];
	CMatrix V(3);
	V(2) = 1;
	for (int i = 0; i<8; i++)
	{
		V(0) = ViewVert(0, i); // x
		V(1) = ViewVert(1, i); // y
							   //V=V-V0;			// x-xL, y-yL, 1
		V = MW*V;
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}
	CMatrix VE = Vertices.GetCol(4, 0, 2);	//// Возвращает подстолбец из столбца матрицы с номером k
	CMatrix R1(3), R2(3), VN(3);
	double sm;
	for (int i = 0; i<4; i++)
	{
		int k;
		if (i == 3) k = 0;
		else k = i + 1;
		R1 = Vertices.GetCol(i, 0, 2);
		R2 = Vertices.GetCol(k, 0, 2);
		CMatrix V1 = R2 - R1;
		CMatrix V2 = VE - R1;
		VN = VectorMult(V2, V1);	//векторное произведение векторов V1 и V2

		sm = CosV1V2(VN, ViewCart);

		if (sm >= 0)
		{
			CPen Pen(PS_SOLID, 2, RGB(sm*sm*red, sm*sm*green, sm*sm*blue));
			CPen* pOldPen = dc.SelectObject(&Pen);
			CBrush Brus(RGB(sm*sm*red, sm*sm*green, sm*sm*blue));
			CBrush* pOldBrush = dc.SelectObject(&Brus);
			CPoint MasVertR[4] = { MasVert[i+4],MasVert[k+4],MasVert[k],MasVert[i] };
			dc.Polygon(MasVertR, 4);	// Ребро
			dc.SelectObject(pOldBrush);
			dc.SelectObject(pOldPen);
		}
	}
	VN = VectorMult(R1, R2);
	sm = CosV1V2(VN, ViewCart);
	if (sm >= 0)
	{
		CPen Pen(PS_SOLID, 2, RGB(sm*sm*red, sm*sm*green, sm*sm*blue));
		CPen* pOldPen = dc.SelectObject(&Pen);
		CBrush Brus(RGB(sm*sm*red, sm*sm*green, sm*sm*blue));
		CBrush* pOldBrush = dc.SelectObject(&Brus);
		dc.Polygon(MasVert, 4);	// Основание
		dc.SelectObject(pOldBrush);
		dc.SelectObject(pOldPen);
	}
	else
	{
		CPen Pen(PS_SOLID, 2, RGB(sm*sm*red, sm*sm*green, sm*sm*blue));
		CPen* pOldPen = dc.SelectObject(&Pen);
		CBrush Brus(RGB(sm*sm*red, sm*sm*green, sm*sm*blue));
		CBrush* pOldBrush = dc.SelectObject(&Brus);
		dc.Polygon(MasVert+4, 4);	// Основание
		dc.SelectObject(pOldBrush);
		dc.SelectObject(pOldPen);
	}
}


void CPyramid::GetRect(CMatrix& Vert,CRectD& RectView)
{
	CMatrix V=Vert.GetRow(0);		
	double xMin=V.MinElement();
	double xMax=V.MaxElement();
	V=Vert.GetRow(1);				
	double yMin=V.MinElement();
	double yMax=V.MaxElement();
	RectView.SetRectD(xMin,yMax,xMax,yMin);
}

//---------------------------------------------------------------------