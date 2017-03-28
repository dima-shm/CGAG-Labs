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