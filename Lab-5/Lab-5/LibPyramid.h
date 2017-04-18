#pragma once
class CPyramid
{
private:
	CMatrix Vertices; // Координаты вершин
	void GetRect(CMatrix& Vert, CRectD&  RectView);

public:
	CPyramid();
	void Draw(CDC& dc, CMatrix& P, CRect& RW);
	void Draw1(CDC& dc, CMatrix& P, CRect& RW);
};

CPyramid::CPyramid()
{
	Vertices.RedimMatrix(4, 8);	// ABCDE: ABCD - основание, 
	// A'B'C'D' верхнее основание
	Vertices(0, 0) = 6;		// A: x=10,y=0,z=0
	Vertices(0, 4) = 4;		// A': x=6,y=0,z=0
	Vertices(2, 4) = 6;		// A': x=6,y=0,z=10

	Vertices(1, 1) = -6;	// B: x=0,y=-10,z=0
	Vertices(1, 5) = -4;	// B': x=0,y=-6,z=0
	Vertices(2, 5) = 6;	    // B': x=0,y=-6,z=10

	Vertices(0, 2) = -6;	// C: x=-10,y=0,z=0
	Vertices(0, 6) = -4;	// C': x=-6,y=0,z=0
	Vertices(2, 6) = 6;	    // C': x=-6,y=0,z=10

	Vertices(1, 3) = 6;		// D: x=0,y=10,z=0
	Vertices(1, 7) = 4;		// D': x=0,y=6,z=0
	Vertices(2, 7) = 6;		// D': x=0,y=6,z=10

	for (int i = 0; i<8; i++)
		Vertices(3, i) = 1;
}
void CPyramid::GetRect(CMatrix& Vert, CRectD& RectView)
// Вычисляет координаты прямоугольника, охватывающего проекцию 
// пирамиды на плоскость XY в ВИДОВОЙ системе координат
// Vert - координаты вершин (в столбцах)
// RectView - проекция - охватывающий прямоугольник
{
	CMatrix V = Vert.GetRow(0);
	double xMin = V.MinElement();
	double xMax = V.MaxElement();
	V = Vert.GetRow(1);
	double yMin = V.MinElement();
	double yMax = V.MaxElement();
	RectView.SetRectD(xMin, yMax, xMax, yMin);
}

void CPyramid::Draw(CDC& dc, CMatrix& PView, CRect& RW)
// Рисует пирамиду С УДАЛЕНИЕМ невидимых ребер
// Самостоятельный пересчет координат из мировых в оконные (MM_TEXT)
// dc - ссылка на класс CDC MFC
// P - координаты точки наблюдения в мировой сферической системе //координат
// (r, fi(град.), q(град.))
// RW - область в окне для отображения
{
	CMatrix ViewCart = SphereToCart(PView); // Декартовы координаты точки наблюдения
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2)); // Матрица (4x4)
	CMatrix ViewVert = MV*Vertices; // Кооржинаты вершин пирамиды в видовой СК
	CRectD RectView;
	GetRect(ViewVert, RectView); // Получаем охватывающий прямоугольник
	CMatrix MW = SpaceToWindow(RectView, RW); // Матрица (3x3) для пересчета

	CPoint MasVert[8]; // Масив оконных координат вершин,
	CMatrix V(3);
	V(2) = 1;
	// Цикл по количеству вершин – вычисляем оконные коодинаты
	for (int i = 0; i < 8; i++)
	{
		V(0) = ViewVert(0, i); // x
		V(1) = ViewVert(1, i); // y
		V = MW * V;            // Оконные координаты точки
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}

	// Рисуем
	CPen Pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen* pOldPen = dc.SelectObject(&Pen);
	dc.MoveTo(MasVert[3]);
	for (int i = 0; i < 4; i++)
	{
		dc.LineTo(MasVert[i]);
	}
	dc.MoveTo(MasVert[7]);
	for (int i = 4; i < 8; i++)
	{
		dc.LineTo(MasVert[i]);
	}
	for (int i = 0; i<4; i++)
	{
		dc.MoveTo(MasVert[i]);
		dc.LineTo(MasVert[i + 4]);
	}
	// Координаты центра O 	пересечения диагоналей основания
	int x0 = (MasVert[0].x + MasVert[2].x) / 2;
	int y0 = (MasVert[0].y + MasVert[2].y) / 2;
	int x0s = (MasVert[4].x + MasVert[6].x) / 2;
	int y0s = (MasVert[4].y + MasVert[6].y) / 2;

	CPen Pen1(PS_DASH, 1, RGB(250, 0, 0));
	dc.SelectObject(&Pen1);
	dc.MoveTo(x0, y0);
	dc.LineTo(x0s, y0s);
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
void CPyramid::Draw1(CDC& dc, CMatrix& PView, CRect& RW)
// Рисует пирамиду БЕЗ удаления невидимых ребер
// Самостоятельный пересчет координат из мировых в оконные (MM_TEXT)
// dc - ссылка на класс CDC MFC
// P - координаты точки наблюдения в мировой сферической системе
// координат
// (r,fi(град.), q(град.))
// RW - область в окне для отображения	
{
	CMatrix ViewCart = SphereToCart(PView); // Декартовы координаты точки наблюдения
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2)); // Матрица (4x4)
	CMatrix ViewVert = MV*Vertices; // Кооржинаты вершин пирамиды в видовой СК
	CRectD RectView;
	GetRect(ViewVert, RectView); // Получаем охватывающий прямоугольник
	CMatrix MW = SpaceToWindow(RectView, RW); // Матрица (3x3) для пересчета

	CPoint MasVert[8]; // Масив оконных координат вершин,
	CMatrix V(3);
	V(2) = 1;
	// Цикл по количеству вершин – вычисляем оконные коодинаты
	for (int i = 0; i < 8; i++)
	{
		V(0) = ViewVert(0, i); // x
		V(1) = ViewVert(1, i); // y
		V = MW * V;            // Оконные координаты точки
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}

	// Рисуем
	CPen Pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen* pOldPen = dc.SelectObject(&Pen);
	CBrush Brush(RGB(255, 0, 0));
	CBrush* pOldBrush = dc.SelectObject(&Brush);
	CMatrix R1(3), R2(3), VN(3);
	double sm;
	for (int i = 0; i < 4; i++)
	{
		CMatrix VE = Vertices.GetCol(i + 4, 0, 2); // Вершина E
		int k;
		if (i == 3)
			k = 0;
		else
			k = i + 1;
		R1 = Vertices.GetCol(i, 0, 2); // Текущая точка осноания
		R2 = Vertices.GetCol(k, 0, 2); // Следующая точка основания
		CMatrix V1 = R2 - R1;          // Вектор – ребро в основании
		CMatrix V2 = VE - R1;          // Вектор – ребро к вершине
		VN = VectorMult(V2, V1);       // Вектор ВНЕШНЕЙ нормали
		sm = ScalarMult(VN, ViewCart); // Скалярное произведение 

		if (sm >= 0) // Грань видима – рисуем боковую грань
		{
			dc.MoveTo(MasVert[i]);
			dc.LineTo(MasVert[k]);
			dc.LineTo(MasVert[k + 4]);
			dc.LineTo(MasVert[i + 4]);
			dc.LineTo(MasVert[i]);
		}
	}

	VN = VectorMult(R1, R2);
	sm = ScalarMult(VN, ViewCart);
	if (sm >= 0) // Основание
		dc.Polygon(MasVert, 4);
	else
	{
		CBrush *topBrush = new CBrush(RGB(0, 0, 255));
		dc.SelectObject(topBrush);
		dc.Polygon(MasVert + 4, 4);	// верхнее основание
	}

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}