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
	Vector2 operator* (double other); // ���Ϳ� ��Į�� ��
	Vector2 operator- (Vector2& other);
	double Dot(Vector2& other); // ����
	double Magnitude(); // ���� ������
	Vector2 Rotate(double angle); // ������ŭ ȸ����Ų ���͸� ��ȯ�Ѵ�.
	Vector2 Normal(); // ����ȭ�� ���͸� ��ȯ

public:
	double x;
	double y;
};

