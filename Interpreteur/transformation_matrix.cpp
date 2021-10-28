#include "transformation_matrix.h"

#include <vector>
#include <iostream>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace std;
namespace ublas = boost::numeric::ublas;

ublas::matrix<float> transformationMatrix(vector<float> vecX, vector<float> vecZ, vector<float> vecL)
{
	ublas::matrix<float> m(4, 4);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m(i, j) = 0;
		}
	}
	float i = 0;
	float j = 0;
	for (auto element : vecX)
	{
		m(i,j)=element;
		i++;
	}
	j++;
	i = 0;
	std::vector<float> vecY(3);
	vecY[0] = vecZ[1] * vecX[2] - vecZ[2] * vecX[1];
	vecY[1] = vecZ[2] * vecX[0] - vecZ[0] * vecX[2];
	vecY[2] = vecZ[0] * vecX[1] - vecZ[1] * vecX[0];
	for (auto element : vecY)
	{
		m(i, j) = element;
		i++;
	}
	j++;
	i = 0;
	for (auto element : vecZ)
	{
		m(i, j) = element;
		i++;
	}
	j++;
	i = 0;
	for (auto element : vecL)
	{
		m(i, j) = element;
		i++;
	}
	m(3, 3) = 1;
	return m;
}

ublas::matrix<float> inverseMatrix(boost::numeric::ublas::matrix<float> m)
{
	float A2323 = m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2);
	float A1323 = m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1);
	float A1223 = m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1);
	float A0323 = m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0);
	float A0223 = m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0);
	float A0123 = m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0);
	float A2313 = m(1, 2) * m(3, 3) - m(1, 3) * m(3, 2);
	float A1313 = m(1, 1) * m(3, 3) - m(1, 3) * m(3, 1);
	float A1213 = m(1, 1) * m(3, 2) - m(1, 2) * m(3, 1);
	float A2312 = m(1, 2) * m(2, 3) - m(1, 3) * m(2, 2);
	float A1312 = m(1, 1) * m(2, 3) - m(1, 3) * m(2, 1);
	float A1212 = m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1);
	float A0313 = m(1, 0) * m(3, 3) - m(1, 3) * m(3, 0);
	float A0213 = m(1, 0) * m(3, 2) - m(1, 2) * m(3, 0);
	float A0312 = m(1, 0) * m(2, 3) - m(1, 3) * m(2, 0);
	float A0212 = m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0);
	float A0113 = m(1, 0) * m(3, 1) - m(1, 1) * m(3, 0);
	float A0112 = m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0);

	float det = m(0,0) * (m(1,1) * A2323 - m(1,2) * A1323 + m(1,3) * A1223)
		- m(0,1) * (m(1,0) * A2323 - m(1,2) * A0323 + m(1,3) * A0223)
		+ m(0,2) * (m(1,0) * A1323 - m(1,1) * A0323 + m(1,3) * A0123)
		- m(0,3) * (m(1,0) * A1223 - m(1,1) * A0223 + m(1,2) * A0123);
	det = 1 / det;

	ublas::matrix<float> n (4,4);

	n(0, 0) = det * (m(1, 1) * A2323 - m(1, 2) * A1323 + m(1, 3) * A1223);
	n(0, 1) = det * -(m(0, 1) * A2323 - m(0, 2) * A1323 + m(0, 3) * A1223);
	n(0, 2) = det * (m(0, 1) * A2313 - m(0, 2) * A1313 + m(0, 3) * A1213);
	n(0, 3) = det * -(m(0, 1) * A2312 - m(0, 2) * A1312 + m(0, 3) * A1212);
	n(1, 0) = det * -(m(1, 0) * A2323 - m(1, 2) * A0323 + m(1, 3) * A0223);
	n(1, 1) = det * (m(0, 0) * A2323 - m(0, 2) * A0323 + m(0, 3) * A0223);
	n(1, 2) = det * -(m(0, 0) * A2313 - m(0, 2) * A0313 + m(0, 3) * A0213);
	n(1, 3) = det * (m(0, 0) * A2312 - m(0, 2) * A0312 + m(0, 3) * A0212);
	n(2, 0) = det * (m(1, 0) * A1323 - m(1, 1) * A0323 + m(1, 3) * A0123);
	n(2, 1) = det * -(m(0, 0) * A1323 - m(0, 1) * A0323 + m(0, 3) * A0123);
	n(2, 2) = det * (m(0, 0) * A1313 - m(0, 1) * A0313 + m(0, 3) * A0113);
	n(2, 3) = det * -(m(0, 0) * A1312 - m(0, 1) * A0312 + m(0, 3) * A0112);
	n(3, 0) = det * -(m(1, 0) * A1223 - m(1, 1) * A0223 + m(1, 2) * A0123);
	n(3, 1) = det * (m(0, 0) * A1223 - m(0, 1) * A0223 + m(0, 2) * A0123);
	n(3, 2) = det * -(m(0, 0) * A1213 - m(0, 1) * A0213 + m(0, 2) * A0113);
	n(3, 3) = det * (m(0, 0) * A1212 - m(0, 1) * A0212 + m(0, 2) * A0112);

	return n;
}