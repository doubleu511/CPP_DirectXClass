#pragma once
#include "BBasicStd.h"

class Vector2
{
public:
	Vector2();
	Vector2(double x, double y);
	~Vector2();

	Vector2(const Vector2& v);

	Vector2 operator+ (Vector2& other);
	Vector2 operator* (double other); // 벡터와 스칼라 곱
	Vector2 operator- (Vector2& other);
	double Dot(Vector2& other); // 내적
	double Magnitude(); // 길이 연산자
	Vector2 Rotate(double angle); // 각도만큼 회전시킨 벡터를 반환한다.
	Vector2 Normal(); // 정규화된 벡터를 반환

public:
	double x;
	double y;
};

