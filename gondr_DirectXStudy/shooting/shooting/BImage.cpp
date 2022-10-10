#include "BImage.h"

DWORD BImage::m_TransparentColor = RGB(255, 0, 255); // ����ƽ ������� �ʱ�ȭ

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
		DeleteObject(m_BitmapHandle); // ��Ʈ���� �����ϸ� �����ش�.
	}
}

bool BImage::Load(const TCHAR* strLoadFile)
{
	/*
		LoadImage�� �Ķ����:
		HINSTANCE, �����̸�, Ÿ��, x, y, �ε� �ɼ�
		Ÿ��: IMAGE_BITMAP(0), IMAGE_CURSOR(2), IMAGE_ICON(1)
		x, y : �̹����� ũ�⸦ ���ϴµ� ���࿡ �� ���� ���� 0�̸鼭 LR_DEFAULTSIZE �� �ɼ����� ����
			�ý����� SM_CXICON�� �̿��ؼ� ũ�⸦ �����ϰ� �ǰ�
			���� LR_DEFAULTSIZE�� �����Ǿ� ���� �ʴٸ� ���ҽ��� ���� ũ��� �ε�ȴ�.

		DIB = Device-Independent Bitmap : ��ġ ������ ��Ʈ��, ������ ���̺��� ������ �ִ� ��Ʈ��
		LR_LOADFROMFILE : ���ҽ� ��� ���� ��θ� �̿��Ͽ� �ҷ��ðŴ�

		GetModuleHandle => Module�� ���� �ν��Ͻ��ڵ�, dll�̳� exe�� ����� �ϰ� �Ǵµ�
		���⼭�� nullptr�� ������ ���� �ν��Ͻ��� �ڵ�� �Ѿ���� �ȴ�.
	*/
	m_BitmapHandle = (HBITMAP)LoadImage(GetModuleHandle(nullptr), strLoadFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);

	if (this->IsEmpty())
	{
		return false;
	}

	// �ε� �����ߴٸ� ����� ��
	// �ε��� ��Ʈ���ڵ��� �̿��ؼ� �ش� ��Ʈ���� ������ m_BitmapInfo�� �̾ƿ´�
	GetObject(m_BitmapHandle, sizeof(m_BitmapInfo), &m_BitmapInfo);
	return true;
}

bool BImage::Load(HBITMAP bitmapHandle)
{
	// ������ �׸��� �ε�Ǿ��ִ� �ν��Ͻ���� �����׸��� ����� �ٽ� �ε�
	if (this->IsEmpty() == false)
	{
		DeleteObject(m_BitmapHandle); // ���� ������ �ִ� ��Ʈ���� ����
	}
	m_BitmapHandle = bitmapHandle; // ���ε��� �ڵ��� �߰�
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
	return m_BitmapHandle == NULL;	//���⼱ nullptr�� ���� ������ �߻��� �� �ִ�.
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
		FPOINT center{ x + width * 0.5, y + height * 0.5 }; // �׷��� �̹����� �߽���
		POINT apt[3] = { 0, }; // �»�, ���, ����
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

		// 0��° : �»�, 1��° ���, 2��°�� ����

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
