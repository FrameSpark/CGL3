#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H

Matrix<> Translation(double x, double y)
{
	double T[9] = {
		1, 0, x,
		0, 1, y,
		0, 0, 1 };

	return Matrix<>(3, 3, T);
}

// Identity() - тождественное АП;
Matrix<> Identify() {
	return Translation(0, 0);
}

// Rotation(t) - поворот на угол t;
Matrix<> Rotation(double t) {
	double cosT = cos(t);
	double sinT = sin(t);

	double R[9] = {
		cosT, -sinT, 0,
		sinT, cosT, 0,
		0, 0, 1 };

	return Matrix<>(3, 3, R);
}

Matrix<> Rotation(double c, double s) {
	double module = sqrt(c*c + s*s);
	double d = 1.0 / module;
	double R[9] = {
		d*c, -s * d, 0,
		s*d, d*c, 0,
		0, 0, 1 };

	return Matrix<>(3, 3, R);
}

Matrix<> Scalling(double kx, double ky) {
	double S[9] = {
		kx, 0, 0,
		0, ky, 0,
		0, 0, 1 };

	return Matrix<>(3, 3, S);
}

enum {Mx = -1, My, XY};

Matrix<> Mapping(int type) {
	// type = -1 = Mx
	// type = 0 = My
	// type = 1 = XY
	switch (type) {
	case Mx:
		return Scalling(1, -1);
		break;
	case My:
		return Scalling(-1, 1);
		break;
	case XY:
		return Scalling(-1, -1);
		break;
	}
}
// Rotation(c, s) - поворот на угол, косинус и синус которого пропорциональны величинам c и s;
// Scaling(kx, ky) - масштабирование;
// Mapping (различные виды отражений) - по желанию, для создания матриц отражения можно использовать функцию Scaling.

// В приведённом примере используется конструктор матрицы из списка.
// Приветствуются идеи о других способах создания матриц.
// Например, вызов функции, создающей единичную матрицу,
// с последующим переопределением некоторых её коэффициентов.

#endif AFFINE_TRANSFORM_H
