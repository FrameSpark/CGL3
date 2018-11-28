/*

	Аккуратней с пробелом!!!
	Его не должно юыть в конце строки !!!

	*/
#ifndef MODEL_2D_H

#define MODEL_2D_H

#include <string>
#include <fstream>
#include "Matrix.h"
#include "AffineTransform.h"

class Model2D
{
private:
	// вершины
	/*
		x
		y
		что-то
	*/
	Matrix<> Vertices;
	// рёбра
	/*
		вершина Начало  вершина Конец

		в целочисленной матрице размера K на 2 (где K – число рёбер) – при этом в каждой строке матрицы хранятся номера двух вершин, соединённых рёбрами;
	*/
	Matrix<int> Edges;

	Matrix<> CumulativeAT;
	Matrix<> InitialVertices;

public:
	// конструктор
	Model2D() : Vertices(), Edges(), CumulativeAT(Identify()), InitialVertices() {}
	// конструктор копирования 
	Model2D(const Matrix<> Vertices, const Matrix<int> Edges) : Vertices(Vertices), Edges(Edges), CumulativeAT(Identify()), InitialVertices(Vertices) {}

	//  конструктор создания модели по именам файлов, в которых лежат карта вершин и карта рёбер
	Model2D(string fileVert, string fileEdges) : CumulativeAT(Identify()) {
		Vertices = readFromFile<double>(fileVert);
		//cout << Vertices << endl << endl;
		InitialVertices = Vertices;
		
		/*
			размер [K][2]
		*/
		Edges = readFromFile<int>(fileEdges);

		// проверка на размер Edges
		if (Edges.getColmnSize() != 2)
			cout << "ERROR. Edges colmns != 2 !!!" << endl;

		if (Edges.getRowSize() == 0)
			cout << "ERROR. Edges rows == 0 !!!" << endl;

		//cout << Edges << endl << endl;
		//cout << Edges << endl << endl;
	}

	// получение вершины
	Matrix<> GetVertices() { return Vertices; }
	// получение ребра
	Matrix<int> GetEdges() { return Edges; }

	// получение координаты X вершины по индексу
	double GetVertexX(int index) {
		return Vertices(1, index);
	}

	// получение координаты Y вершины по индексу
	double GetVertexY(int index) {
		return Vertices(2, index);
	}

	// получение координаты X вершины по индексу
	double GetEdgeFirst(int index) {
		return Edges(index, 1);
	}

	// получение координаты Y вершины по индексу
	double GetEdgeSecond(int index) {
		return Edges(index, 2);
	}

	// применение АП к 
	void Apply(Matrix<> AT) {
		CumulativeAT = AT * CumulativeAT;
		Vertices = CumulativeAT * InitialVertices;
		//cout << Vertices << endl << endl;
	}


	template <typename Cell>
	Matrix<Cell> readFromFile(string fileName) {
		/*

		Аккуратней с пробелом!!!
		Его не должно юыть в конце строки !!!

		Перехода на другую строку тоже
		*/

		fstream inFile;

		inFile.open(fileName, ios::in);

		if (inFile.is_open())
		{
			//Если открытие файла прошло успешно

			//Вначале посчитаем сколько чисел в файле
			int count = 0;// число чисел в файле
			Cell temp;     //Временная переменная

			// пробегаем пока не встретим конец файла eof
			while (!inFile.eof()) {
				inFile >> temp;//в пустоту считываем из файла числа
				count++;// увеличиваем счетчик числа чисел
			}

			//Число чисел посчитано, теперь нам нужно понять сколько чисел в одной строке
			//Для этого посчитаем число пробелов до знака перевода на новую строку 

			//Вначале переведем каретку в потоке в начало файла
			inFile.seekg(0, ios::beg);
			inFile.clear();

			//Число пробелов в первой строчке вначале равно 0
			int count_space = 0;
			char symbol;
			//на всякий случай цикл ограничиваем концом файла
			//while (!inFile.eof()) {
				//теперь нам нужно считывать не числа, а посимвольно считывать данные
			//	inFile.get(symbol);//считали текущий символ
			//	if (symbol == ' ') count_space++;//Если это пробел, то число пробелов увеличиваем
			//	if (symbol == '\n') break;//Если дошли до конца строки, то выходим из цикла
			//}
			//cout << count_space << endl;

			string str;
			getline(inFile, str);

			int i = 0;
			while (i < str.length()) {
				if (str[i] == ' ')
					count_space++;
				i++;
			}

			//Опять переходим в потоке в начало файла
			inFile.seekg(0, ios::beg);
			inFile.clear();

			//Теперь мы знаем сколько чисел в файле и сколько пробелов в первой строке.
			//Теперь можем считать матрицу.

			int n = count / (count_space + 1);//число строк
			int m = count_space + 1;//число столбцов на единицу больше числа пробелов
			Cell *x = new Cell[n*m];

			inFile.close();
			inFile.open(fileName, ios::in);

			//Считаем матрицу из файла
			for (int i = 0; i < n; i++)
				for (int j = 0; j < m; j++)
					inFile >> x[m*i + j];

			inFile.close();
			//Выведем матрицу
			// для дебагинга
			/*for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < m; j++)
					cout << x[i][j] << "\t";
				cout << "\n";
			}*/

			Matrix<Cell> x_m(n, m, x);
			return x_m;
		}
		else
		{
			//Если открытие файла прошло не успешно
			cout << "!!!CAN'T OPEN A FILE: " << fileName;
			return Matrix<Cell>(1, 1);
		}
	}
};
#endif MODEL_2D_H
