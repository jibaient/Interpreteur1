#ifndef INTERPOLATE_HPP
#define INTERPOLATE_HPP
#include <string>
#include <vector>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "toolpath.h"

namespace ublas = boost::numeric::ublas;
void linearIPO_Pre(Toolpath_section* section);
void circleIPO_Pre(Toolpath_section* section);
void mapping(Toolpath_section* section);

#endif // INTERPOLATE_HPP