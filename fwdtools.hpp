#ifndef _TOOLS_BOOST_UBLAS_FWD_
#define _TOOLS_BOOST_UBLAS_FWD_

#include <boost/numeric/ublas/fwd.hpp>
namespace boost { namespace numeric { namespace ublas {

// from small.hpp

class vector2;
class vector3;
class vector4;
typedef bounded_vector<double, 5> vector5;
typedef bounded_vector<double, 9> vector9;
typedef bounded_vector<double, 12> vector12;
typedef bounded_vector<double, 16> vector16;
class matrix22;
typedef bounded_matrix<double, 2, 3> matrix23;
class matrix24;
typedef bounded_matrix<double, 3, 2> matrix32;
class matrix33;
typedef bounded_matrix<double, 3, 4> matrix34;
typedef bounded_matrix<double, 4, 4> matrix44;

template<class L>
inline double determinant(const bounded_matrix<double,2,2,L>& A);
template<class L1, class L2>
inline void inverse(const bounded_matrix<double,2,2,L1>& A, double det, bounded_matrix<double,2,2,L2>& B);
template<class L>
inline double determinant(const bounded_matrix<double,3,3,L>& A);
template<class L1, class L2>
inline void inverse(const bounded_matrix<double,3,3,L1>& A, double det, bounded_matrix<double,3,3,L2>& B);
inline matrix33 skew(double s0, double s1, double s2);
inline matrix33 skew(const vector3& s);
inline matrix33 rodrigues(double rx, double ry, double rz);
inline matrix33 rodrigues(const vector3& r);
inline matrix33 diagonal(double d0, double d1, double d2);
inline matrix33 matrix33_from_coeffs(double m00, double m01, double m02, double m10, double m11, double m12, double m20, double m21, double m22);
inline matrix34 matrix34_from_coeffs(double m00, double m01, double m02, double m03, double m10, double m11, double m12, double m13, double m20, double m21, double m22, double m23);
inline vector3 vector3_from_coeffs(double v0, double v1, double v2);
inline vector4 vector4_from_coefs(double v0, double v1, double v2, double v3);

}}}

#endif // _TOOLS_BOOST_UBLAS_FWD_
