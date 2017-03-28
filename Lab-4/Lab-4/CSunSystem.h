CMatrix CreateTranslate2D(double, double);
CMatrix CreateRotate2D(double);

class CSunSystem
{
	CRect Sun;		  // �������������� ������
	CRect Earth;
	CRect Moon;
	CRect Venus;
	CRect Mercury;
	CRect Mars;
	CRect EarthOrbit; // ��������������, ��������� ������ ������ ������
	CRect MoonOrbit;
	CRect VenusOrbit;
	CRect MercuryOrbit;
	CRect MarsOrbit;
	CMatrix ECoords;  // ������� ���������� ����� � �� ������ ( x,y,1)
	CMatrix VCoords;
	CMatrix MeCoords;
	CMatrix MaCoords;
	CMatrix MCoords;  // ������� ���������� ���� � �� ����� ( x,y,1)
	CMatrix MCoords1; // ������� ���������� ���� � �� ������ ( x,y,1)
	CMatrix PM;		  // ������� ��������
	CMatrix PE;
	CMatrix PV;
	CMatrix PMe;
	CMatrix PMa;
	CRect RW;		  // ������������� � ����
	CRectD RS;		  // ������������� ������� � ���
	double wEarth;	  // ������� �������� ������
	double wMoon;
	double wVenus;
	double wMercury;
	double wMars;
	double fiE;		  // ������� ��������� ����� � ������� �������� ������, ����
	double dfiE;	  // ���� �������� ����� �� ����� dt.
	double fiV;
	double dfiV;
	double fiMe;
	double dfiMe;
	double fiMa;
	double dfiMa;
	double fiM;		  // ������� ��������� ���� � ������� �������� �����, ����
	double dfiM;	  // ���� �������� ����� �� ����� dt

	double dt;		  // �������� �������������, ���.
public:
	CSunSystem();
	void SetDT(double dtx){ dt = dtx; }; // ��������� ��������� �������������
	void SetNewCoords();                 // ��������� ����� ���������� ������
	void GetRS(CRectD& RSX);	         // ���������� ������� ������� � ������� ��
	CRect GetRW(){ return RW; };	     // ���������� ������� ������� � ����	
	void Draw(CDC& dc);		             // ��������� ��� ���������������� ��������� ���������
};

CSunSystem::CSunSystem()
{
	double rS = 35, rE = 20, rM = 5,                  // ������� ������
		rV = 23, rMe = 15, rMa = 17;              
	double RoE = 7.5 * rS, RoM = 2 * rE,              // ������� �����
		RoV = 6 * rV, RoMe = 5.5 * rMe,
		RoMa = 21 * rMa;
	double d = RoE + RoM + rM + RoMa;
	RS.SetRectD(-d, d, d, -d);                        // ������� ������� � ���
	RW.SetRect(0, 0, 650, 650);                       // ������� ������� � ����
	Sun.SetRect(-rS, rS, rS, -rS);                    // ������������� ������
	Earth.SetRect(-rE, rE, rE, -rE);
	Moon.SetRect(-rM, rM, rM, -rM);
	Venus.SetRect(-rV, rV, rV, -rV);
	Mercury.SetRect(-rMe, rMe, rMe, -rMe);
	Mars.SetRect(-rMa, rMa, rMa, -rMa);
	EarthOrbit.SetRect(-RoE, RoE, RoE, -RoE);         // �������������� �����
	MoonOrbit.SetRect(-RoM, RoM, RoM, -RoM);
	VenusOrbit.SetRect(-RoV, RoV, RoV, -RoV);
	MercuryOrbit.SetRect(-RoMe, RoMe, RoMe, -RoMe);
	MarsOrbit.SetRect(-RoMa, RoMa, RoMa, -RoMa);
	fiE = 0;                                          // ������ ���������
	fiM = 0;
	fiV = 0;
	fiMe = 0;
	fiMa = 0;
	wEarth = 15;                                       // ������� ��������
	wMoon = 170;
	wVenus = 20;
	wMercury = 23;
	wMars = 11;
	dt = 0.2;
	dfiM = wMoon * dt;
	dfiE = wEarth * dt;
	dfiV = wVenus * dt;
	dfiMe = wMercury * dt;
	dfiMa = wMars * dt;

	MCoords.RedimMatrix(3);
	MCoords1.RedimMatrix(3);
	ECoords.RedimMatrix(3);
	VCoords.RedimMatrix(3);
	MeCoords.RedimMatrix(3);
	MaCoords.RedimMatrix(3);
	PE.RedimMatrix(3, 3);
	PM.RedimMatrix(3, 3);
	PV.RedimMatrix(3, 3);
	PMe.RedimMatrix(3, 3);
	PMa.RedimMatrix(3, 3);

	RoM = (MoonOrbit.right - MoonOrbit.left)   / 2; // ������ �����
	RoE = (EarthOrbit.right - EarthOrbit.left) / 2;
	RoV = (VenusOrbit.right - VenusOrbit.left) / 2;
	RoMe = (MercuryOrbit.right - MercuryOrbit.left) / 2;
	RoMa = (MarsOrbit.right - MarsOrbit.left) / 2;
	double ff = (fiM / 180.0) * pi;                 // �������
	double x = RoM * cos(ff);
	double y = RoM * sin(ff);
	MCoords(0) = x; MCoords(1) = y; MCoords(2) = 1;

	ff = (fiE / 180.0) * pi;
	x = RoE * cos(ff);
	y = RoE * sin(ff);
	ECoords(0) = x; ECoords(1) = y; ECoords(2) = 1;

	ff = (fiE / 180.0) * pi;
	x = RoV * cos(ff);
	y = RoV * sin(ff);
	VCoords(0) = x; VCoords(1) = y; VCoords(2) = 1;

	ff = (fiE / 180.0) * pi;
	x = RoMe * cos(ff);
	y = RoMe * sin(ff);
	MeCoords(0) = x; MeCoords(1) = y; MeCoords(2) = 1;

	ff = (fiE / 180.0) * pi;
	x = RoMa * cos(ff);
	y = RoMa * sin(ff);
	MaCoords(0) = x; MaCoords(1) = y; MaCoords(2) = 1;

	PE = CreateRotate2D(dfiE);
	PM = CreateRotate2D(dfiM);
	PV = CreateRotate2D(dfiV);
	PMe = CreateRotate2D(dfiMe);
	PMa = CreateRotate2D(dfiMa);
}

void CSunSystem::SetNewCoords()
{
	MCoords = PM * MCoords;

	double x = ECoords(0); double y = ECoords(1);
	CMatrix P = CreateTranslate2D(x, y);
	MCoords1 = P * MCoords;

	MCoords1 = PE * MCoords1;
	ECoords = PE * ECoords;
	VCoords = PV * VCoords;
	MeCoords = PMe * MeCoords;
	MaCoords = PMa * MaCoords;
}

void CSunSystem::Draw(CDC &dc)
{
	CBrush SBrush, EBrush, MBrush, VBrush, MeBrush, MaBrush, *pOldBrush;
	CRect R;

	SBrush.CreateSolidBrush(RGB(255, 69, 0));
	EBrush.CreateSolidBrush(RGB(0, 191, 255));
	MBrush.CreateSolidBrush(RGB(128, 128, 128));
	VBrush.CreateSolidBrush(RGB(205, 133, 63));
	MeBrush.CreateSolidBrush(RGB(160, 82, 45));
	MaBrush.CreateSolidBrush(RGB(255, 0, 0));

	// ������ ������
	dc.SelectStockObject(NULL_BRUSH);
	dc.Ellipse(EarthOrbit);
	dc.Ellipse(VenusOrbit);
	dc.Ellipse(MercuryOrbit);
	dc.Ellipse(MarsOrbit);

	int d = MoonOrbit.right;
	R.SetRect(ECoords(0) - d, ECoords(1) + d, ECoords(0) + d, ECoords(1) - d);
	dc.Ellipse(R);

	// ������ ������
	pOldBrush = dc.SelectObject(&SBrush);
	dc.Ellipse(Sun);

	// ������ �����
	d = Earth.right;
	R.SetRect(ECoords(0) - d, ECoords(1) + d, ECoords(0) + d, ECoords(1) - d);
	dc.SelectObject(&EBrush);
	dc.Ellipse(R);

	// ������ ����
	d = Moon.right;
	R.SetRect(MCoords1(0) - d, MCoords1(1) + d, MCoords1(0) + d, MCoords1(1) - d);
	dc.SelectObject(&MBrush);
	dc.Ellipse(R);

	// ������ ������
	d = Venus.right;
	R.SetRect(VCoords(0) - d, VCoords(1) + d, VCoords(0) + d, VCoords(1) - d);
	dc.SelectObject(&VBrush);
	dc.Ellipse(R);

	// ������ ��������
	d = Mercury.right;
	R.SetRect(MeCoords(0) - d, MeCoords(1) + d, MeCoords(0) + d, MeCoords(1) - d);
	dc.SelectObject(&MeBrush);
	dc.Ellipse(R);

	// ������ ����
	d = Mars.right;
	R.SetRect(MaCoords(0) - d, MaCoords(1) + d, MaCoords(0) + d, MaCoords(1) - d);
	dc.SelectObject(&MaBrush);
	dc.Ellipse(R);

	dc.SelectObject(pOldBrush);
}

void CSunSystem::GetRS(CRectD& RSX)
{
	RSX.left = RS.left;
	RSX.top = RS.top;
	RSX.right = RS.right;
	RSX.bottom = RS.bottom;
}