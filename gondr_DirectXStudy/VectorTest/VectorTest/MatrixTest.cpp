#include <Windows.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <iostream>
#include <memory>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

ostream& XM_CALLCONV operator << (ostream& os, FXMVECTOR v)
{
	XMFLOAT4 dest;
	XMStoreFloat4(&dest, v);

	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ", " << dest.w << ")" << endl;
	return os;
}
// 4x4	1x4
ostream& XM_CALLCONV operator << (ostream& os, FXMMATRIX m)
{
	for (int i = 0; i < 4; ++i)
	{
		os << XMVectorGetX(m.r[i]) << "\t";
		os << XMVectorGetY(m.r[i]) << "\t";
		os << XMVectorGetZ(m.r[i]) << "\t";
		os << XMVectorGetW(m.r[i]) << "\t";
		os << endl;
	}
	return os;
}

void ScaleVector();
void Rotation();

int main()
{
	if (!XMVerifyCPUSupport())
	{
		cout << "다이렉트 Math를 지원하지 않아요" << endl;
		return 0;
	}

	//16개의 float를 받는다.
	//XMMATRIX A(
	//	1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 2.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 4.0f, 0.0f,
	//	1.0f, 2.0f, 3.0f, 1.0f);
	//XMMATRIX B = XMMatrixIdentity(); // 4x4 단위행렬이 나온다.

	//XMMATRIX C = A * B;
	//XMMATRIX D = XMMatrixTranspose(A); // A행렬의 전치행렬을 구한다.
	//
	//XMVECTOR E = XMMatrixDeterminant(A);
	//XMMATRIX Inv = XMMatrixInverse(&E, A);

	//ScaleVector();

	Rotation();

}

void ScaleVector()
{
	float x, y, z;

	cout << "Scale Vector x, y, z 를 입력하세요" << endl;
	cout << "X : ";
	cin >> x;
	cout << "Y : ";
	cin >> y;
	cout << "Z : ";
	cin >> z;

	XMVECTOR min = XMVectorSet(-4.0f, -4.0f, 0.0f, 0.0f);
	XMVECTOR max = XMVectorSet(4.0f, 4.0f, 0.0f, 0.0f);

	shared_ptr<XMMATRIX> A = make_shared<XMMATRIX>(
		x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
		);

	XMVECTOR scaleMin = XMVector3Transform(min, *A);
	cout << min << " => Scale : " << scaleMin << endl;
	XMVECTOR scaleMax = XMVector3Transform(max, *A);
	cout << max << " => Scale : " << scaleMax << endl;
}

void Rotation()
{
	double rot;

	cout << "회전 각도를 입력하세요" << endl;
	cin >> rot;

	XMVECTOR min = XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f);
	XMVECTOR max = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f);

	double rad = rot * M_PI / 100;

	shared_ptr<XMMATRIX> A = make_shared<XMMATRIX>(
		cos(rad), sin(rad), 0.0f, 0.0f,
		-sin(rad), cos(rad), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	XMVECTOR scaleMin = XMVector3Transform(min, *A);
	cout << min << " => Rot : " << scaleMin << endl;
	XMVECTOR scaleMax = XMVector3Transform(max, *A);
	cout << max << " => Rot : " << scaleMax << endl;
}