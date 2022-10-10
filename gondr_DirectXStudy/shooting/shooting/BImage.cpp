#include "BImage.h"

DWORD BImage::m_TransparentColor = RGB(255, 0, 255); // 스태틱 멤버변수 초기화

BImage::BImage() : m_BitmapHandle(nullptr), m_rotateAngle(0)
{
	memset(&m_BitmapInfo, 0, sizeof(m_BitmapInfo));
}

BImage::BImage(const TCHAR* strLoadFile) : BImage()
{
	this->Load(strLoadFile);
}

BImage::~BImage()
{
	if (this->IsEmpty() == false)
	{
		DeleteObject(m_BitmapHandle); // 비트맵이 존재하면 없애준다.
	}
}

bool BImage::Load(const TCHAR* strLoadFile)
{
	/*
		LoadImage의 파라미터:
		HINSTANCE, 파일이름, 타입, x, y, 로드 옵션
		타입: IMAGE_BITMAP(0), IMAGE_CURSOR(2), IMAGE_ICON(1)
		x, y : 이미지의 크기를 말하는데 만약에 이 값이 전부 0이면서 LR_DEFAULTSIZE 가 옵션으로 오면
			시스템의 SM_CXICON을 이용해서 크기를 측정하게 되고
			만약 LR_DEFAULTSIZE가 설정되어 있지 않다면 리소스가 원래 크기로 로드된다.

		DIB = Device-Independent Bitmap : 장치 독립적 비트맵, 색정보 테이블을 가지고 있는 비트맵
		LR_LOADFROMFILE : 리소스 대신 파일 경로를 이용하여 불러올거다

		GetModuleHandle => Module에 대한 인스턴스핸들, dll이나 exe에 사용을 하게 되는데
		여기서는 nullptr을 넣으면 현재 인스턴스의 핸드랑 넘어오게 된다.
	*/
	m_BitmapHandle = (HBITMAP)LoadImage(GetModuleHandle(nullptr), strLoadFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);

	if (this->IsEmpty())
	{
		return false;
	}

	// 로딩 성공했다면 여기로 옴
	// 로딩된 비트맵핸들을 이용해서 해당 비트맵의 정보를 m_BitmapInfo로 뽑아온다
	GetObject(m_BitmapHandle, sizeof(m_BitmapInfo), &m_BitmapInfo);
	return true;
}

bool BImage::Load(HBITMAP bitmapHandle)
{
	// 이전에 그림이 로드되어있던 인스턴스라면 이전그림을 지우고 다시 로드
	if (this->IsEmpty() == false)
	{
		DeleteObject(m_BitmapHandle); // 현재 가지고 있는 비트맵을 삭제
	}
	m_BitmapHandle = bitmapHandle; // 새로들어온 핸들을 추가
	GetObject(m_BitmapHandle, sizeof(m_BitmapInfo), &m_BitmapInfo);
	return false;
}

bool BImage::Release()
{
	if (this->IsEmpty() == false)
	{
		DeleteObject(m_BitmapHandle);
	}

	return true;
}

void BImage::SetRotate(double value)
{
	m_rotateAngle = value;
}

bool BImage::IsEmpty() const
{
	return m_BitmapHandle == NULL;	//여기선 nullptr을 쓰면 문제가 발생할 수 있다.
}

HBITMAP BImage::GetBitmapHandle() const
{
	return m_BitmapHandle;
}

const BITMAP BImage::GetBitmapInfo() const
{
	return m_BitmapInfo;
}

void BImage::DrawBitmap(HDC hdc, int x, int y, int width, int height) const
{
	if (width == 0)
		width = m_BitmapInfo.bmWidth;
	if (height == 0)
		height = m_BitmapInfo.bmHeight;

	this->DrawBitmapByCrop(hdc, x, y, width, height, 0, 0, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight);
}

void BImage::DrawBitmapByCrop(HDC hdc, int x, int y, int width, int height, int sx, int sy, int sw, int sh) const
{
	HDC hMemDC;
	hMemDC = CreateCompatibleDC(hdc);
	SelectObject(hMemDC, m_BitmapHandle);

	if (m_rotateAngle != 0)
	{
		FPOINT center{ x + width * 0.5, y + height * 0.5 }; // 그려질 이미지의 중심점
		POINT apt[3] = { 0, }; // 좌상, 우상, 좌하
		double radian = m_rotateAngle * PI / 180.0f;
		double cosT = cos(radian);
		double sinT = sin(radian);
		/*
			cos, -sin
			sin, cos
		*/

		double dblx, dbly, dblDestX, dblDestY;

		for (int i = 0; i < 3; i++)
		{
			if (i == 0) { dblx = -width * 0.5, dbly = -height * 0.5; }
			if (i == 1) { dblx = width * 0.5, dbly = -height * 0.5; }
			if (i == 2) { dblx = -width * 0.5, dbly = height * 0.5; }

			dblDestX = dblx * cosT - dbly * sinT;
			dblDestY = dblx * sinT + dbly * cosT;
			apt[i].x = center.x + static_cast<long>(dblDestX);
			apt[i].y = center.y + static_cast<long>(dblDestY);
		}

		// 0번째 : 좌상, 1번째 우상, 2번째가 좌하

		HBRUSH hTransparentBrush = CreateSolidBrush(RGB(255, 0, 255));
		RECT rt{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

		HDC rotateDC = CreateCompatibleDC(hdc);
		HBITMAP rotateBitmap = CreateCompatibleBitmap(hdc, SCREEN_WIDTH, SCREEN_HEIGHT);
		SelectObject(rotateDC, rotateBitmap);
		FillRect(rotateDC, &rt, hTransparentBrush);

		PlgBlt(rotateDC, apt, hMemDC, 0, 0, sw, sh, nullptr, 0, 0);

		GdiTransparentBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, rotateDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetTransparentColor());
		// rgb(255, 0, 255)

		DeleteDC(rotateDC);
		DeleteObject(rotateBitmap);
		DeleteObject(hTransparentBrush);
	}
	else
	{
		GdiTransparentBlt(hdc, x, y, width, height, hMemDC, sx, sy, sw, sh, GetTransparentColor());
	}

	DeleteDC(hMemDC);
}

void BImage::SetTransparentColor(DWORD color)
{
	m_TransparentColor = color;
}

DWORD BImage::GetTransparentColor()
{
	return m_TransparentColor;
}
