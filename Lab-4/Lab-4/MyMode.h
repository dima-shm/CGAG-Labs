CMatrix SpaceToWindow(CRectD& RS, CRect& RW)
{
	CMatrix M(3, 3);
	CSize sz = RW.Size();          // ������ ������� � ����
	int dwx = sz.cx;               // ������
	int dwy = sz.cy;               // ������
	CSizeD szd = RS.SizeD();       // ������ ������� � ������� �����������

	double dsx = szd.cx;           // ������ � ������� �����������
	double dsy = szd.cy;           // ������ � ������� �����������

	double kx = (double)dwx / dsx; // ������� �� x
	double ky = (double)dwy / dsy; // ������� �� y

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

	dc.SetMapMode(MM_ANISOTROPIC);       // ��������� ������ �����������
	dc.SetWindowExt((int)dsx, (int)dsy); // ����������� ���������� � ��������
	dc.SetViewportExt(dwx, -dwy);
	dc.SetWindowOrg((int)xsL, (int)ysL); // ��������� ��������
	dc.SetViewportOrg(xwL, ywL);
}