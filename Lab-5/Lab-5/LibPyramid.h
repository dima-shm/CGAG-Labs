#pragma once
class CPyramid
{
private:
	CMatrix Vertices; // ���������� ������
	void GetRect(CMatrix& Vert, CRectD&  RectView);

public:
	CPyramid();
	void Draw(CDC& dc, CMatrix& P, CRect& RW);
	void Draw1(CDC& dc, CMatrix& P, CRect& RW);
};

CPyramid::CPyramid()
{
	Vertices.RedimMatrix(4, 8);	// ABCDE: ABCD - ���������, 
	// A'B'C'D' ������� ���������
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
// ��������� ���������� ��������������, ������������� �������� 
// �������� �� ��������� XY � ������� ������� ���������
// Vert - ���������� ������ (� ��������)
// RectView - �������� - ������������ �������������
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
// ������ �������� � ��������� ��������� �����
// ��������������� �������� ��������� �� ������� � ������� (MM_TEXT)
// dc - ������ �� ����� CDC MFC
// P - ���������� ����� ���������� � ������� ����������� ������� //���������
// (r, fi(����.), q(����.))
// RW - ������� � ���� ��� �����������
{
	CMatrix ViewCart = SphereToCart(PView); // ��������� ���������� ����� ����������
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2)); // ������� (4x4)
	CMatrix ViewVert = MV*Vertices; // ���������� ������ �������� � ������� ��
	CRectD RectView;
	GetRect(ViewVert, RectView); // �������� ������������ �������������
	CMatrix MW = SpaceToWindow(RectView, RW); // ������� (3x3) ��� ���������

	CPoint MasVert[8]; // ����� ������� ��������� ������,
	CMatrix V(3);
	V(2) = 1;
	// ���� �� ���������� ������ � ��������� ������� ���������
	for (int i = 0; i < 8; i++)
	{
		V(0) = ViewVert(0, i); // x
		V(1) = ViewVert(1, i); // y
		V = MW * V;            // ������� ���������� �����
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}

	// ������
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
	// ���������� ������ O 	����������� ���������� ���������
	int x0 = (MasVert[0].x + MasVert[2].x) / 2;
	int y0 = (MasVert[0].y + MasVert[2].y) / 2;
	int x0s = (MasVert[4].x + MasVert[6].x) / 2;
	int y0s = (MasVert[4].y + MasVert[6].y) / 2;

	CPen Pen1(PS_DASH, 1, RGB(250, 0, 0));
	dc.SelectObject(&Pen1);
	dc.MoveTo(x0, y0);
	dc.LineTo(x0s, y0s);
	dc.MoveTo(MasVert[0]);	// ���� �� ������� A
	dc.LineTo(MasVert[2]);	// ���������
	dc.MoveTo(MasVert[1]);	// ���� �� ������� B
	dc.LineTo(MasVert[3]);	// ���������

	dc.MoveTo(MasVert[4]);	// ���� �� ������� A'
	dc.LineTo(MasVert[6]);	// ���������
	dc.MoveTo(MasVert[5]);	// ���� �� ������� B'
	dc.LineTo(MasVert[7]);	// ���������
	dc.SelectObject(pOldPen);
}
void CPyramid::Draw1(CDC& dc, CMatrix& PView, CRect& RW)
// ������ �������� ��� �������� ��������� �����
// ��������������� �������� ��������� �� ������� � ������� (MM_TEXT)
// dc - ������ �� ����� CDC MFC
// P - ���������� ����� ���������� � ������� ����������� �������
// ���������
// (r,fi(����.), q(����.))
// RW - ������� � ���� ��� �����������	
{
	CMatrix ViewCart = SphereToCart(PView); // ��������� ���������� ����� ����������
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2)); // ������� (4x4)
	CMatrix ViewVert = MV*Vertices; // ���������� ������ �������� � ������� ��
	CRectD RectView;
	GetRect(ViewVert, RectView); // �������� ������������ �������������
	CMatrix MW = SpaceToWindow(RectView, RW); // ������� (3x3) ��� ���������

	CPoint MasVert[8]; // ����� ������� ��������� ������,
	CMatrix V(3);
	V(2) = 1;
	// ���� �� ���������� ������ � ��������� ������� ���������
	for (int i = 0; i < 8; i++)
	{
		V(0) = ViewVert(0, i); // x
		V(1) = ViewVert(1, i); // y
		V = MW * V;            // ������� ���������� �����
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}

	// ������
	CPen Pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen* pOldPen = dc.SelectObject(&Pen);
	CBrush Brush(RGB(255, 0, 0));
	CBrush* pOldBrush = dc.SelectObject(&Brush);
	CMatrix R1(3), R2(3), VN(3);
	double sm;
	for (int i = 0; i < 4; i++)
	{
		CMatrix VE = Vertices.GetCol(i + 4, 0, 2); // ������� E
		int k;
		if (i == 3)
			k = 0;
		else
			k = i + 1;
		R1 = Vertices.GetCol(i, 0, 2); // ������� ����� ��������
		R2 = Vertices.GetCol(k, 0, 2); // ��������� ����� ���������
		CMatrix V1 = R2 - R1;          // ������ � ����� � ���������
		CMatrix V2 = VE - R1;          // ������ � ����� � �������
		VN = VectorMult(V2, V1);       // ������ ������� �������
		sm = ScalarMult(VN, ViewCart); // ��������� ������������ 

		if (sm >= 0) // ����� ������ � ������ ������� �����
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
	if (sm >= 0) // ���������
		dc.Polygon(MasVert, 4);
	else
	{
		CBrush *topBrush = new CBrush(RGB(0, 0, 255));
		dc.SelectObject(topBrush);
		dc.Polygon(MasVert + 4, 4);	// ������� ���������
	}

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}