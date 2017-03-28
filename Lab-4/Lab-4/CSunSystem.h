class CSunSystem
{
	CRect Sun;		  // Прямоугольник Солнца
	CRect Earth;      // Прямоугольник Земли
	CRect Moon;		  // Прямоугольник Луны
	CRect EarthOrbit; // Прямоугольник, описанный вокруг орбиты Земли
	CRect MoonOrbit;  // Прямоугольник, описанный вокруг орбиты Луны
	CMatrix ECoords;  // Текущие координаты Земли в СК Солнца ( x,y,1)
	CMatrix MCoords;  // Текущие координаты Луны в СК Земли ( x,y,1)
	CMatrix MCoords1; // Текущие координаты Луны в СК Солнца ( x,y,1)
	CMatrix PM;		  // Матрица поворота для луны
	CMatrix PE;		  // Матрица поворота для Земли
	CRect RW;		  // Прямоугольник в окне
	CRectD RS;		  // Прямоугольник области в МСК
	double wEarth;	  // Угловая скорость Земли относительно Солнца, град./сек.
	double wMoon;	  // Угловая скорость Земли относительно Солнца, град./сек.
	double fiE;		  // Угловое положение Земли в системе кординат Солнца, град
	double dfiE;	  // Угол поворота Земли за время dt.
	double fiM;		  // Угловое положение Луны в системе кординат Земли, град
	double dfiM;	  // Угол поворота Земли за время dt

	double dt;		  // Интервал дискретизации, сек.
public:
	CSunSystem();
	void SetDT(double dtx){ dt = dtx; }; // Установка интервала дискретизации
	void SetNewCoords();                 // Вычисляет новые координаты планет
	void GetRS(CRectD& RSX);	         // Возвращает область графика в мировой СК
	CRect GetRW(){ return RW; };	     // Возвращает область графика в окне	
	void Draw(CDC& dc);		             // Рисование без самостоятельного пересчета координат
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

void CSunSystem::SetNewCoords()
{
	MCoords = PM * MCoords;

	double x = ECoords(0); double y = ECoords(1);
	CMatrix P = CreateTranslate2D(x, y);
	MCoords1 = P * MCoords;

	MCoords1 = PE * MCoords1;
	ECoords = PE * ECoords;
}

void CSunSystem::Draw(CDC &dc)
{
	CBrush SBrush, EBrush, MBrush, *pOldBrush;
	CRect R;

	SBrush.CreateSolidBrush(RGB(255, 0, 0));
	EBrush.CreateSolidBrush(RGB(0, 0, 255));
	MBrush.CreateSolidBrush(RGB(0, 255, 0));

	// Рисуем орбиты
	dc.SelectStockObject(NULL_BRUSH);
	dc.Ellipse(EarthOrbit);

	int d = MoonOrbit.right;
	R.SetRect(ECoords(0) - d, ECoords(1) + d, ECoords(0) + d, ECoords(1) - d);
	dc.Ellipse(R);

	// Рисуем Солнце
	pOldBrush = dc.SelectObject(&SBrush);
	dc.Ellipse(Sun);

	// Рисуем Землю
	d = Earth.right;
	R.SetRect(ECoords(0) - d, ECoords(1) + d, ECoords(0) + d, ECoords(1) - d);
	dc.SelectObject(&EBrush);
	dc.Ellipse(R);

	// Рисуем Луну
	d = Moon.right;
	R.SetRect(MCoords1(0) - d, MCoords1(1) + d, MCoords1(0) + d, MCoords1(1) - d);
	dc.SelectObject(&MBrush);
	dc.Ellipse(R);

	dc.SelectObject(pOldBrush);
}