class CSunSystem
{
	CRect Sun;		  // ������������� ������
	CRect Earth;      // ������������� �����
	CRect Moon;		  // ������������� ����
	CRect EarthOrbit; // �������������, ��������� ������ ������ �����
	CRect MoonOrbit;  // �������������, ��������� ������ ������ ����
	CMatrix ECoords;  // ������� ���������� ����� � �� ������ ( x,y,1)
	CMatrix MCoords;  // ������� ���������� ���� � �� ����� ( x,y,1)
	CMatrix MCoords1; // ������� ���������� ���� � �� ������ ( x,y,1)
	CMatrix PM;		  // ������� �������� ��� ����
	CMatrix PE;		  // ������� �������� ��� �����
	CRect RW;		  // ������������� � ����
	CRectD RS;		  // ������������� ������� � ���
	double wEarth;	  // ������� �������� ����� ������������ ������, ����./���.
	double wMoon;	  // ������� �������� ����� ������������ ������, ����./���.
	double fiE;		  // ������� ��������� ����� � ������� �������� ������, ����
	double dfiE;	  // ���� �������� ����� �� ����� dt.
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
	double rS = 30, rE = 20, rM = 10;
	double RoE = 10 * rS, RoM = 5 * rE;
	double d = RoE + RoM + rM;
	RS.SetRectD(-d, d, d, -d);
	RW.SetRect(0, 0, 600, 600);
	Sun.SetRect(-rS, rS, rS, -rS);
	Earth.SetRect(-rE, rE, rE, -rE);
	Moon.SetRect(-rM, rM, rM, -rM);
	EarthOrbit.SetRect(-RoE, RoE, RoE, -RoE);
	MoonOrbit.SetRect(-RoM, RoM, RoM, -RoM);
	fiE = 0;
	fiM = 0;
	wEarth = 5;
	wMoon = 50;
	dt = 0.2;
	dfiE = wEarth * dt;
	dfiM = wMoon * dt;
	MCoords.RedimMatrix(3);
	MCoords1.RedimMatrix(3);

	RoE = (EarthOrbit.right - EarthOrbit.left) / 2;
	RoM = (MoonOrbit.right - MoonOrbit.left) / 2;
	double ff = (fiM / 180.0) * pi;
	double x = RoM * cos(ff);
	double y = RoM * sin(ff);
	MCoords(0) = x; MCoords(1) = y; MCoords(2) = 1;

	ff = (fiE / 180.0) * pi;
	x = RoE * cos(ff);
	y = RoE * sin(ff);
	ECoords(0) = x; ECoords(1) = y; ECoords(2) = 1;

	PE = CreateRotate2D(dfiE);
	PM = CreateRotate2D(dfiM);
}