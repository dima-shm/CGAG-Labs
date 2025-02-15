#include "stdafx.h"
#include "Pyramid.h"
#include "ChildView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

CChildView::CChildView()
{
	Index=0;
	PView.RedimMatrix(3);
	PView(0) = 10; PView(1) = 300; PView(2) = 135;
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_PYRAMID_PYRAMID1, &CChildView::OnPyramidPyramid1)
	ON_COMMAND(ID_PYRAMID_PYRAMID2, &CChildView::OnPyramidPyramid2)
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this);
	if(Index==1)
		PIR.Draw(dc,PView,WinRect);
	if(Index==2)
		PIR.ColorDraw(dc, PView, WinRect, RGB(0, 0, 255));
}


void CChildView::OnPyramidPyramid1()	
{
	Index=1;
	Invalidate();
}
void CChildView::OnPyramidPyramid2()
{
	Index = 2;
	Invalidate();
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if((Index==1)||(Index==2))
  {
	switch(nChar)
	{
	  case VK_UP:
		{
		  double d=PView(2)-5;
		  if(d>=0)
			  PView(2)=d;
		  break;
		}
	  case VK_DOWN:
		{
		  double d=PView(2)+5;
		  if(d<=180)
			  PView(2)=d;
		  break;
		}
	  case VK_LEFT:
		{
		  double d=PView(1)-10;
		  if(d>=-180)
			  PView(1)=d;
		  else 
			  PView(1)=d+360;
		  break;
		}
	  case VK_RIGHT:
		{
		  double d=PView(1)+10;
		  if(d<=180)
			  PView(1)=d;
		  else 
			  PView(1)=d-360;
		  break;
		}
	}
	Invalidate();
  }
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	WinRect.SetRect(100,100,cx - 100, cy - 100);
}