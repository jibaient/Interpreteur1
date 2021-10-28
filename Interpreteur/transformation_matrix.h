#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include <vector>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

boost::numeric::ublas::matrix<float> transformationMatrix(std::vector<float> vecX, std::vector<float> vecZ, std::vector<float> vecL);
boost::numeric::ublas::matrix<float> inverseMatrix(boost::numeric::ublas::matrix<float> m);
#endif // TRANSFORMATION_HPP



