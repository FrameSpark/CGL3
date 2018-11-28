#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windows.h>

class Camera2D
{
protected:
	double L, R, B, T;					// Мировые координаты границ рабочей области окна
	double prevX, prevY;
	int W, H;							// Разрешение рабочей области окна
	int WorldToScreenX(double X)		// Переход от мировых координат к экранным (для абсциссы)
	{
		return ((X - L) / (R - L))*W;
	}
	int WorldToScreenY(double Y)		// Переход от мировых координат к экранным (для ординаты)
	{
		return ((T - Y) / (T - B))*H;
	}
	double ScreenToWorldX(int X)		// Переход от экранных координат к мировым (для абсциссы)
	{
		return L + ((R - L)*((double)X + 0.5)) / W;
	}
	double ScreenToWorldY(int Y)		// Переход от экранных координат к мировым (для ординаты)
	{
		return T - ((T - B)*((double)Y + 0.5)) / H;
	}
private:
	double posX, posY;					// Позиция графического курсора в мировых координатах (для функций MoveTo и LineTo)
public:
	bool isDragging;

	Camera2D(double L, double R, double B, double T) : L(L), R(R), B(B), T(T)
	{
	}
	void Clear(HDC dc)
	{
		Rectangle(dc, 0, 0, W, H);
	}
	void SetResolution(HDC dc)
	{
		// Данная процедура вызывается при изменении размеров окна
		// В ней задаются значения величин W, H, а также настраиваются значения параметров L, R, B, T таким образом, чтобы обеспечить одинаковые масштабы по координатным осям
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);
		// установка одинакового масштаба

		W = r.right + 1;
		H = r.bottom + 1;

		double tmp = double (W) / H;
		double t1 = (L + R) / 2.0;
		double t2 = (T - B) / 2.0;

		L = t1 - t2*tmp;
		R = t1 + t2*tmp;
	}
	void MoveTo(double X, double Y)
	{
		// Перемещение графического курсора (posX, posY)
		// Обратите внимание, что мы действуем в мировых координатах
		posX= X;
		posY= Y;
	}
	void LineTo(HDC dc, double X, double Y)
	{
		// Отрисовка линии из текущей позиции графического курсора в точку (X, Y) и его перемещение в эту точку
		// Обратите внимание, что мы действуем в мировых координатах
		// При отрисовке линии могут быть использованы WinApi функции
		// ::MoveToEx(dc, Xs, Ys, nullptr) и ::LineTo(dc, Xs, Ys)
		WIN32::MoveToEx(dc, WorldToScreenX(posX), WorldToScreenY(posY), nullptr);
		WIN32::LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));
		MoveTo(X, Y);
	}
	void Axes(HDC dc)
	{
		// Абцисса
		MoveTo(L, 0);
		LineTo(dc, W, 0);

		// Ордината
		MoveTo(0, T);
		LineTo(dc, 0, B);
	}

	void polarAxes(HDC dc)
	{
		//MoveTo(0, 0);
		//LineTo(dc, W, WorldToScreenY(0));
		WIN32::MoveToEx(dc, WorldToScreenX(0), WorldToScreenY(0), nullptr);
		WIN32::LineTo(dc, W, WorldToScreenY(0));
	}

	void scale(int X, int Y, double k)
	{
		double
			Xw = ScreenToWorldX(X),
			Yw = ScreenToWorldY(Y);

		L = Xw - (Xw - L) / k;
		R = Xw + (R - Xw) / k;
		B = Yw - (Yw - B) / k;
		T = Yw + (T - Yw) / k;
	}

	void StartDragging(int X, int Y)
	{
		prevX = X;
		prevY = Y;
		isDragging = true;
	}

	void drag(int X, int Y)
	{
		double deltaX = X - prevX;
		double deltaY = Y - prevY;
		double length = R - L;
		double height = T - B;
		
		L = L - length * deltaX / W;
		R = R - length * deltaX / W;
		B = B + height * deltaY / H;
		T = T + height * deltaY / H;

		prevX = X;
		prevY = Y;
	}

	void StopDragging()
	{
		isDragging = false;
	}

};

#endif CAMERA_2D_H
