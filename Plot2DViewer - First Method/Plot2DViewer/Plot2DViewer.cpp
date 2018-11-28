#include <windows.h>
#include <windowsx.h>

#include "Data.h"
#include "Scene2D.h"
#include "Matrix.h"
#include "AffineTransform.h"
#include "Model2D.h"
#include "cmath"

#define TRNCOEFF 0.2 	// скорость переноса
#define SCLCOEFF 0.2  	// скорость масштабирования
#define RTCOEFF 0.3  	// скорость вращения

Scene2D scene(L, R, B, T);

int WIN_HIGHT = 400;
int WIN_WIDTH = 400;

double x, y,xT1,xT2,yT1,yT2;

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// прототип оконной процедуры
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// основна¤ процедура
{
	// ѕерва¤ составл¤юща¤ часть основной процедуры - создание окна: сначала описываетс¤ оконный класс wc, затем создаЄтс¤ окно hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// им¤ оконной процедуры, закрепленной за данным классом
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// идентификатор приложени¤, содержащий адрес начала выделенной ему области пам¤ти
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;							// меню в оконном классе отсутствует
	wc.lpszClassName = (LPCSTR)"MainWindowClass";	// им¤ оконного класса, используемое при создании экземпл¤ров окна
	RegisterClass(&wc);								// регистраци¤ класса wc

	HWND hWnd = CreateWindow(						// hWnd - дескриптор, идентифицирующий окно; функци¤ создани¤ окна заполн¤ет дескриптор hWnd ненулевым значением
		(LPCSTR)"MainWindowClass",					// им¤ оконного класса
		(LPCSTR)"Plot2D Viewer",					// заголовок окна
		WS_OVERLAPPEDWINDOW,						// стиль окна
		200, 200, WIN_HIGHT, WIN_WIDTH,							// координаты на экране левого верхнего угла окна, его ширина и высота
		nullptr, nullptr, hInstance, nullptr);

	scene.initModel("Vertices.txt", "Edges.txt");

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ¬тора¤ составл¤юща¤ часть основной процедуры - основной цикл обработки системных сообщений, который ожидает сообщени¤ и рассылает их соответствующим окнам
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))				// функци¤ GetMessage выбирает из очереди сообщение и заносит его в структуру msg
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						// функци¤ DispatchMessage оповещает систему о необходимости вызова оконной процедуры
	}

	return 0;
}

// тип графика
// false - декартова система координат
// true - полярная система координат
bool plotType = true;

LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// оконна¤ процедура принимает и обрабатывает все сообщени¤, отправленные окну
{
	switch (msg)
	{
	case WM_PAINT:
	{
		HDC dc = GetDC(hWnd);
		scene.Clear(dc);

		scene.Render(dc);

		ReleaseDC(hWnd, dc);
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	case WM_SIZE: {
		HDC dc = GetDC(hWnd);
		scene.SetResolution(dc);
		ReleaseDC(hWnd, dc);
		InvalidateRect(hWnd, nullptr, false);
		return 0;
	}

	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		scene.StartDragging(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}
	case WM_LBUTTONUP:
	{
		scene.StopDragging();
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(hWnd, &pt);

		double k = (GET_WHEEL_DELTA_WPARAM(wParam) > 0) ? 1.1 : 0.9;
		scene.scale(pt.x, pt.y, k);

		InvalidateRect(hWnd, nullptr, false);
		return 0;
	}

	case WM_MOUSEMOVE:
	{
		if (scene.isDragging)
		{
			scene.drag(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			InvalidateRect(hWnd, nullptr, false);
		}
		break;
	}

	case WM_KEYDOWN:
	{
		switch (wParam) {
			// Translation section
		case VK_LEFT:
			scene.model->Apply(Translation(-TRNCOEFF, 0));
			break;
		case VK_RIGHT:
			scene.model->Apply(Translation(TRNCOEFF, 0));
			break;
		case VK_UP:
			scene.model->Apply(Translation(0, TRNCOEFF));
			break;
		case VK_DOWN:
			scene.model->Apply(Translation(0, -TRNCOEFF));
			break;

			// Rotation section
			// поворот направо (A)
		case 0x41:
			scene.model->Apply(Rotation(RTCOEFF));
			break;
			// поворот налево (D)
		case 0x44:
			scene.model->Apply(Rotation(-RTCOEFF));
			break;

			// Scalling section
			// увеличиваем модель (+)
		case VK_OEM_PLUS:
			x = scene.model->GetVertexX(scene.model->GetEdgeFirst(27));
			y = scene.model->GetVertexY(scene.model->GetEdgeSecond(27));
			
			scene.model->Apply(Translation(x, y)*scene.angleForVector(true, 7, 8)*Scalling(1+SCLCOEFF, 1)*scene.angleForVector(false, 7, 8)*Translation(-x, -y));
			
			break;



			// уменьшаем модель (-)
		case VK_OEM_MINUS:
			x = scene.model->GetVertexX(scene.model->GetEdgeFirst(27));
			y = scene.model->GetVertexY(scene.model->GetEdgeSecond(27));
			
			scene.model->Apply(Translation(x, y)*scene.angleForVector(true, 7, 8)*Scalling(1 - SCLCOEFF, 1)*scene.angleForVector(false, 7, 8)*Translation(-x, -y));
			break;

			// Отражения
			//	Mx (X)
		case 0x58:
			scene.model->Apply(Mapping(Mx));
			break;
			//	My (C)
		case 0x43:
			scene.model->Apply(Mapping(My));
			break;
			//	XY (V)
		case 0x56:
			scene.model->Apply(Mapping(XY));
			break;



		}
			InvalidateRect(hWnd, nullptr, false);
			return 0;
		}

	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}


	return 0;
	}
	
	