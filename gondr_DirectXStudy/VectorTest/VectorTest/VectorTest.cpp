#include <Windows.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <iostream>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

// FXMVECTOR, GXMVECTOR, HXMVECTOR, CXMVECTOR

ostream& XM_CALLCONV operator << (ostream& os, FXMVECTOR v)
{
    XMFLOAT3 dest;
    XMStoreFloat3(&dest, v);

    os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ")";
    return os;
}


const float Epsilon = 0.0001f;

bool Equals(float lhs, float rhs)
{
    return fabs(lhs - rhs) < Epsilon ? true : false;
}

int main()
{
    if (XMVerifyCPUSupport() == false)
    {
        cout << "다이렉트 Math를 지원하지 않는 CPU 입니다." << endl;
        return 0;
    }
    cout.setf(ios_base::boolalpha); // 이거하면 true, false를 문자열로 찍어줌.

    XMVECTOR u = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
    XMVECTOR n = XMVector3Normalize(u);

    float lenU = XMVectorGetX(XMVector3Length(n));  //1이 될것이라고 생각
    cout.precision(8);

    cout << "normal Length : " << lenU << endl;

    cout << powf(lenU, 1.0e6f) << endl;



    /*
    XMVECTOR n = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 4.0f);
    XMVECTOR v = XMVectorSet(-2.0f, 1.0f, -3.0f, 0.0f);
    XMVECTOR w = XMVectorSet(0.707f, 0.707f, 0.0f, 0.0f);

    XMVECTOR a = u + v;
    cout << "u + v : " << a << endl;
    XMVECTOR b = u - v;
    cout << "u - v : " << b << endl;
    XMVECTOR c = 10.0f * u;
    cout << "10.0f * u : " << c << endl;
    XMVECTOR len = XMVector3Length(u);
    cout << "len(u) : " << len << endl;
    XMVECTOR nor = XMVector3Normalize(u);
    cout << "normal(u) : " << nor << endl;
    XMVECTOR dot = XMVector3Dot(u, v); // u, v 내적
    cout << "dot u,v : " << dot << endl;

    XMVECTOR cross = XMVector3Cross(u, v); // u, v 외적
    cout << "cross u x v : " << cross << endl;

    XMVECTOR projW;
    XMVECTOR perpW;
    XMVector3ComponentsFromNormal(&projW, &perpW, w, n);
    // w벡터를 n벡터에 의해 분해해서 n벡터에 직교 투영인 벡터와
    // n벡터에 직교인 벡터로 분해해준다.
    cout << "projW : " << projW << endl;
    cout << "perpW : " << perpW << endl;

    cout << "projW + perpW : " << (projW + perpW) << endl;
    cout << "W : " << w << endl;

    if (XMVector3Equal(w, projW + perpW) == true)
    {
        cout << "W랑 합이랑 같아요" << endl;
    }

    XMVECTOR angleVector = XMVector2AngleBetweenVectors(projW, perpW);

    cout << "angle Vector : " << angleVector << endl;
    */

    /*
    XMVECTOR p = XMVectorZero();
    XMVECTOR q = XMVectorSplatOne();                    // 벡터의 모든 컴포넌트를 1로 변경
    XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);
    XMVECTOR v = XMVectorReplicate(-2.0f);              // 부동 소수점 값을 벡터의 네 가지 구 성 요소 모두에 복제
    XMVECTOR w = XMVectorSplatZ(u);                     // 벡터의 모든 컴포넌트를 z로 변경

    cout << "p = " << p << endl;
    cout << "q = " << q << endl;
    cout << "u = " << u << endl;
    cout << "v = " << v << endl;
    cout << "w = " << w << endl;
    */
}