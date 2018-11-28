#ifndef SCENE_2D_H
#define SCENE_2D_H

#include "Camera2D.h"
#include "Model2D.h"
#include "Matrix.h"

class Scene2D : public Camera2D
{
private:
	typedef double (*Func)(double);
public:
	Model2D *model;
	Scene2D(double L, double R, double B, double T) : Camera2D(L, R, B, T) {
		model = new Model2D();
	}

	// инициализация model по какой-то новой new_model
	void initModel(Model2D new_model) {
		model = new Model2D(new_model.GetVertices(), new_model.GetEdges());
	}

	// инициализация model чтением из файлов verts, edges
	void initModel(string verts, string edges) {
		model = new Model2D(verts, edges);
	}

	void Render(HDC dc) {
	
		// координаты начальной вершины
		double start_vert_x = 0.0;
		double start_vert_y = 0.0;
		// координаты конечной вершины
		double end_vert_x = 0.0;
		double end_vert_y = 0.0;

		for (int pair = 1; pair <= model->GetEdges().getRowSize(); pair++){
			// получение коорд. первой точки
			start_vert_x = model->GetVertexX(model->GetEdgeFirst(pair));
			start_vert_y = model->GetVertexY(model->GetEdgeFirst(pair));

			// получение коорд. второй точки
			end_vert_x = model->GetVertexX(model->GetEdgeSecond(pair));
			end_vert_y = model->GetVertexY(model->GetEdgeSecond(pair));
			
			// проводим линию
			MoveTo(start_vert_x, start_vert_y);
			LineTo(dc, end_vert_x, end_vert_y);
		}
	}

	
	Matrix<> angleForVector(bool direction, int a, int b) {
		double a_x = model->GetVertexX(model->GetEdgeFirst(a));
		double a_y = model->GetVertexY(model->GetEdgeFirst(a));

		
		double b_x = model->GetVertexX(model->GetEdgeSecond(b));
		double b_y = model->GetVertexY(model->GetEdgeSecond(b));

		double new_x = b_x - a_x;
		double new_y = b_y - a_y;

		double module = 1.0 / sqrt(new_x*new_x + new_y * new_y);

		new_x *= module;
		new_y *= module;

		double
			c = new_x,
			s = new_y;
		if (!direction)
			s = -s;

		return Rotation(c, s);
	}

	Model2D getModel() { return *model; }

	void Plot(HDC dc, Func f, bool axes=true)
	{
		if (axes)
			Axes(dc);

		

		MoveTo( L, f(L));
		for (double p = 0; p < W; p++) 
			LineTo(dc, ScreenToWorldX(p + 1),f(ScreenToWorldX(p + 1)));
	}

	void polarPlot(HDC dc, Func f, bool axes = true)
	{
		if (axes)
			polarAxes(dc);

		const double step = (R-L)/W;
		// x=x*cos(alpha) y=y*sin(alpha)
		MoveTo(f(0)*cos(0), f(0)*sin(0)); 
		double alpha = step;
		for (; alpha <= 2 * M_PI; alpha += step)
			// x=x*cos(alpha) y=y*sin(alpha)
			LineTo(dc, f(alpha)*cos(alpha), f(alpha)*sin(alpha));
		// для дорисовывания до оси X (то есть полный круг)
		LineTo(dc, f(2*M_PI)*cos(2 * M_PI), f(2 * M_PI)*sin(2 * M_PI));
	}
};

#endif SCENE_2D_H
