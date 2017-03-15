#pragma once
#include "resource.h"

class CMainWnd : public CFrameWnd
{
private:
	enum { leftButtonDown, showBitMap, rightButtonDown, clear } condition;
	HBITMAP Bit;
	RECT lpRect, Rect;
	POINT ptLeftButtonDown, ptRightButton0, ptRightButton1;
	int count = 0;

	CMenu menu;
	DECLARE_MESSAGE_MAP()
	int OnCreate(LPCREATESTRUCT);

public:
	CMainWnd::CMainWnd() {
		Create(NULL, L"Lab-2", WS_OVERLAPPEDWINDOW, CRect(10, 10, 800, 600), NULL, NULL);
		condition = clear;
	}

	void OnPaint();

	void OnLButtonDown(UINT, CPoint);
	void DrawPoint(CPaintDC&, long, long, COLORREF);

	void MenuTestImage();
	int ShowBitMap(HWND, HBITMAP, int, int);

	void OnRButtonDown(UINT, CPoint);
	int ClientToBmp(HWND, RECT, LPCWSTR);

	void Clear();
	void Exit();
};

BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_TEST_IMAGE, MenuTestImage)
	ON_COMMAND(ID_CLEAR, Clear)
	ON_COMMAND(ID_EXIT, Exit)
	ON_WM_PAINT()
END_MESSAGE_MAP()
int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	menu.LoadMenu(IDR_MENU1); // Загрузить меню из файла ресурса
	SetMenu(&menu); // Установить меню
	return 0;
}


void CMainWnd::OnPaint()
{
	CPaintDC dc(this);
	switch (condition)
	{
	case leftButtonDown:
		DrawPoint(dc, ptLeftButtonDown.x, ptLeftButtonDown.y, RGB(255, 0, 0));
		break;
	case showBitMap:
		ShowBitMap(m_hWnd, Bit, ptLeftButtonDown.x, ptLeftButtonDown.y);
		break;
	case rightButtonDown: {
		if (count == 0) {
			DrawPoint(dc, ptRightButton0.x - 3, ptRightButton0.y - 3, RGB(0, 255, 0));
			count++;
		}
		else if (count == 1) {
			DrawPoint(dc, ptRightButton1.x, ptRightButton1.y, RGB(0, 0, 255));
			ClientToBmp(m_hWnd, Rect, L"Images\\screenShot.bmp");
			count = 0;
		}
	} break;
	case clear:
		Invalidate();
		break;
	}
}

void CMainWnd::OnLButtonDown(UINT flags, CPoint Loc)
{
	GetWindowRect(&lpRect);
	GetCursorPos(&ptLeftButtonDown);
	ptLeftButtonDown.x = ptLeftButtonDown.x - lpRect.left - 10;
	ptLeftButtonDown.y = ptLeftButtonDown.y - lpRect.top - 10 - 40 - 3;
	condition = leftButtonDown;
	Invalidate(FALSE);
}
void CMainWnd::DrawPoint(CPaintDC &dc, long x, long y, COLORREF color)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			dc.SetPixel(x + i, y + j, color);
}

void CMainWnd::MenuTestImage()
{
	CFileDialog dlg(TRUE); // Панель выбора файлов
	dlg.m_ofn.lpstrTitle = L"Открыть файл";
	int result = dlg.DoModal(); // Отображаем диалоговое окно
	LPCWSTR fileName = dlg.m_ofn.lpstrFile;
	Bit = (HBITMAP)LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	condition = showBitMap;
	Invalidate(FALSE);
}
int CMainWnd::ShowBitMap(HWND hWnd, HBITMAP hBit, int x, int y)
{
	BITMAP BitMap;
	GetObject(hBit, sizeof(BITMAP), &BitMap);
	HDC hdc = ::GetDC(hWnd);
	HDC hdcMem = CreateCompatibleDC(hdc); // Создаём контекст устройства
	HBITMAP OldBitMap = (HBITMAP)SelectObject(hdcMem, hBit);
	BitBlt(hdc, x, y, BitMap.bmWidth, BitMap.bmHeight, hdcMem, 0, 0, SRCCOPY); // Выполняем передачу битовых блоков данных о цвете
	SelectObject(hdcMem, OldBitMap);
	::ReleaseDC(hWnd, hdc); // Освобождаем конекст устройства
	DeleteDC(hdcMem);

	return 0;
}

void CMainWnd::OnRButtonDown(UINT, CPoint)
{
	GetWindowRect(&lpRect);
	if (count == 0) {
		GetCursorPos(&ptRightButton0);
		ptRightButton0.x = ptRightButton0.x - lpRect.left - 10;
		ptRightButton0.y = ptRightButton0.y - lpRect.top - 10 - 40 - 3;
	}
	else if (count == 1)	{
		GetCursorPos(&ptRightButton1);
		ptRightButton1.x = ptRightButton1.x - lpRect.left - 10;
		ptRightButton1.y = ptRightButton1.y - lpRect.top - 10 - 40 - 3;

		Rect.left = ptRightButton0.x;
		Rect.top = ptRightButton0.y;
		Rect.right = ptRightButton1.x;
		Rect.bottom = ptRightButton1.y;
	}
	condition = rightButtonDown;
	Invalidate(FALSE);
}
int CMainWnd::ClientToBmp(HWND hWnd, RECT rect, LPCWSTR Name)
{
	BITMAPFILEHEADER bmfHdr; // Тип, размер и макет файла в формате DIB
	BITMAPINFOHEADER bi;
	int BitToPixel = 16; //Устанавливаем цветовую глубину 16 бит
	HDC hdc = ::GetDC(hWnd);
	HDC hdcMem = CreateCompatibleDC(hdc);

	HBITMAP BitMap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
	HBITMAP OldBitmap = (HBITMAP)SelectObject(hdcMem, BitMap);
	BitBlt(hdcMem, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdc, rect.left, rect.top, SRCCOPY);
	BitMap = (HBITMAP)SelectObject(hdcMem, OldBitmap);

	ZeroMemory(&bi, sizeof(BITMAPINFOHEADER));
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = rect.right - rect.left;
	bi.biHeight = rect.bottom - rect.top;
	bi.biPlanes = 1;
	bi.biBitCount = BitToPixel;
	bi.biSizeImage = (rect.right * BitToPixel + 31) / 32 * 4 * rect.bottom;
	DWORD dwWritten; //Количество записанных файлов

	//Открываем файл
	HANDLE fh = CreateFile((LPCWSTR)Name, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL |
		FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE)
		return 2;
	bmfHdr.bfType = ('M' << 8) | 'B'; //Заполняем дисковый заголовок
	bmfHdr.bfSize = bi.biSizeImage + sizeof(bmfHdr) + bi.biSize; //Размер файла
	bmfHdr.bfReserved1 = bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = sizeof(bmfHdr) + bi.biSize; // Смещение до начала данных

	//Запись заголовка в файл
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(bmfHdr), &dwWritten, NULL);

	// Запись в файл загружаемого заголовка
	WriteFile(fh, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);

	//Выделяем место в памяти для того,чтобы функция GetDIBits()перенесла
	//туда коды цвета в DIB-формате.
	char *lp = (char *)GlobalAlloc(GMEM_FIXED, bi.biSizeImage);

	// Из карты BitMap строки с нулевой по bi.biHeight функция пересылает
	// в массив lp по формату bi ( беспалитровый формат)
	int err = GetDIBits(hdc, BitMap, 0, (UINT)rect.bottom, lp, (LPBITMAPINFO)&bi, DIB_RGB_COLORS);

	//Запись изображения на диск
	WriteFile(fh, lp, bi.biSizeImage, &dwWritten, NULL);

	//Освобождение памяти и закрытие файла
	GlobalFree(GlobalHandle(lp));
	CloseHandle(fh);
	::ReleaseDC(hWnd, hdc);
	DeleteDC(hdcMem);

	if (dwWritten == 0)
		return 2;

	return 0;
}

void CMainWnd::Clear()
{
	condition = clear;
	RedrawWindow();
}
void CMainWnd::Exit()
{
	DestroyWindow();
}