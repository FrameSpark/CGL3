/*

	���������� � ��������!!!
	��� �� ������ ���� � ����� ������ !!!

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
	// �������
	/*
		x
		y
		���-��
	*/
	Matrix<> Vertices;
	// ����
	/*
		������� ������  ������� �����

		� ������������� ������� ������� K �� 2 (��� K � ����� ����) � ��� ���� � ������ ������ ������� �������� ������ ���� ������, ���������� ������;
	*/
	Matrix<int> Edges;

	Matrix<> CumulativeAT;
	Matrix<> InitialVertices;

public:
	// �����������
	Model2D() : Vertices(), Edges(), CumulativeAT(Identify()), InitialVertices() {}
	// ����������� ����������� 
	Model2D(const Matrix<> Vertices, const Matrix<int> Edges) : Vertices(Vertices), Edges(Edges), CumulativeAT(Identify()), InitialVertices(Vertices) {}

	//  ����������� �������� ������ �� ������ ������, � ������� ����� ����� ������ � ����� ����
	Model2D(string fileVert, string fileEdges) : CumulativeAT(Identify()) {
		Vertices = readFromFile<double>(fileVert);
		//cout << Vertices << endl << endl;
		InitialVertices = Vertices;
		
		/*
			������ [K][2]
		*/
		Edges = readFromFile<int>(fileEdges);

		// �������� �� ������ Edges
		if (Edges.getColmnSize() != 2)
			cout << "ERROR. Edges colmns != 2 !!!" << endl;

		if (Edges.getRowSize() == 0)
			cout << "ERROR. Edges rows == 0 !!!" << endl;

		//cout << Edges << endl << endl;
		//cout << Edges << endl << endl;
	}

	// ��������� �������
	Matrix<> GetVertices() { return Vertices; }
	// ��������� �����
	Matrix<int> GetEdges() { return Edges; }

	// ��������� ���������� X ������� �� �������
	double GetVertexX(int index) {
		return Vertices(1, index);
	}

	// ��������� ���������� Y ������� �� �������
	double GetVertexY(int index) {
		return Vertices(2, index);
	}

	// ��������� ���������� X ������� �� �������
	double GetEdgeFirst(int index) {
		return Edges(index, 1);
	}

	// ��������� ���������� Y ������� �� �������
	double GetEdgeSecond(int index) {
		return Edges(index, 2);
	}

	// ���������� �� � 
	void Apply(Matrix<> AT) {
		CumulativeAT = AT * CumulativeAT;
		Vertices = CumulativeAT * InitialVertices;
		//cout << Vertices << endl << endl;
	}


	template <typename Cell>
	Matrix<Cell> readFromFile(string fileName) {
		/*

		���������� � ��������!!!
		��� �� ������ ���� � ����� ������ !!!

		�������� �� ������ ������ ����
		*/

		fstream inFile;

		inFile.open(fileName, ios::in);

		if (inFile.is_open())
		{
			//���� �������� ����� ������ �������

			//������� ��������� ������� ����� � �����
			int count = 0;// ����� ����� � �����
			Cell temp;     //��������� ����������

			// ��������� ���� �� �������� ����� ����� eof
			while (!inFile.eof()) {
				inFile >> temp;//� ������� ��������� �� ����� �����
				count++;// ����������� ������� ����� �����
			}

			//����� ����� ���������, ������ ��� ����� ������ ������� ����� � ����� ������
			//��� ����� ��������� ����� �������� �� ����� �������� �� ����� ������ 

			//������� ��������� ������� � ������ � ������ �����
			inFile.seekg(0, ios::beg);
			inFile.clear();

			//����� �������� � ������ ������� ������� ����� 0
			int count_space = 0;
			char symbol;
			//�� ������ ������ ���� ������������ ������ �����
			//while (!inFile.eof()) {
				//������ ��� ����� ��������� �� �����, � ����������� ��������� ������
			//	inFile.get(symbol);//������� ������� ������
			//	if (symbol == ' ') count_space++;//���� ��� ������, �� ����� �������� �����������
			//	if (symbol == '\n') break;//���� ����� �� ����� ������, �� ������� �� �����
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

			//����� ��������� � ������ � ������ �����
			inFile.seekg(0, ios::beg);
			inFile.clear();

			//������ �� ����� ������� ����� � ����� � ������� �������� � ������ ������.
			//������ ����� ������� �������.

			int n = count / (count_space + 1);//����� �����
			int m = count_space + 1;//����� �������� �� ������� ������ ����� ��������
			Cell *x = new Cell[n*m];

			inFile.close();
			inFile.open(fileName, ios::in);

			//������� ������� �� �����
			for (int i = 0; i < n; i++)
				for (int j = 0; j < m; j++)
					inFile >> x[m*i + j];

			inFile.close();
			//������� �������
			// ��� ���������
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
			//���� �������� ����� ������ �� �������
			cout << "!!!CAN'T OPEN A FILE: " << fileName;
			return Matrix<Cell>(1, 1);
		}
	}
};
#endif MODEL_2D_H
