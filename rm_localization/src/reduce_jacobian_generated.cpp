// File generated by symbolic_jacobian.py

#include <reduce_jacobian.h>

void reduce_jacobian::compute_jacobian(const Eigen::Vector4f & p, Eigen::Matrix<float, 2, 6> & J) {
	J(0,0) = 1/p(2);
	J(0,1) = 0;
	J(0,2) = -p(0)/( p(2) * p(2) );
	J(0,3) = -p(0)*p(1)/( p(2) * p(2) );
	J(0,4) = (( p(0) * p(0) ) + ( p(2) * p(2) ))/( p(2) * p(2) );
	J(0,5) = -p(1)/p(2);
	J(1,0) = 0;
	J(1,1) = 1/p(2);
	J(1,2) = -p(1)/( p(2) * p(2) );
	J(1,3) = -(( p(1) * p(1) ) + ( p(2) * p(2) ))/( p(2) * p(2) );
	J(1,4) = p(0)*p(1)/( p(2) * p(2) );
	J(1,5) = p(0)/p(2);
}
