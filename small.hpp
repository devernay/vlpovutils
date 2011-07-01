#ifndef _BOOST_UBLAS_SMALL_
#define _BOOST_UBLAS_SMALL_

// types and functions for small vectors and matrices (2x2, 3x3...)

#include "fwd.hpp"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

namespace boost { namespace numeric { namespace ublas {

typedef bounded_vector<double, 2> vector2;
typedef bounded_vector<double, 3> vector3;
typedef bounded_vector<double, 4> vector4;
typedef bounded_matrix<double, 2, 2> matrix22;
typedef bounded_matrix<double, 3, 3> matrix33;
typedef bounded_matrix<double, 3, 4> matrix34;

inline double
determinant(const matrix22& A)
{
    return A(0,0)*A(1,1) - A(0,1)*A(1,0);
}

inline void
inverse(const matrix22& A, double det, matrix22& B)
{
    BOOST_UBLAS_CHECK(det != 0., boost::numeric::ublas::divide_by_zero());
    double fac = 1./det;
    B(0,0) =  A(1,1)*fac;
    B(0,1) = -A(0,1)*fac;
    B(1,0) = -A(1,0)*fac;
    B(1,1) =  A(1,1)*fac;
}

inline double
determinant(const matrix33& A)
{
    return (  A(0,0)*(A(2,2)*A(1,1)-A(2,1)*A(1,2))
              - A(1,0)*(A(2,2)*A(0,1)-A(2,1)*A(0,2))
              + A(2,0)*(A(1,2)*A(0,1)-A(1,1)*A(0,2)));
}

inline void
inverse(const matrix33& A, double det, matrix33& B)
{
    BOOST_UBLAS_CHECK(det != 0., boost::numeric::ublas::divide_by_zero());
    double fac = 1./det;
    for(int i=0; i<3; ++i) {
        for(int j=0; j<3; ++j) {
            B(j,i) = (  A((i+1)%3,(j+1)%3) * A((i+2)%3,(j+2)%3)
                        - A((i+1)%3,(j+2)%3) * A((i+2)%3,(j+1)%3))*fac;
        }
    }
}

/**
 * \brief Constructs a 3x3 skew-symmetric matrix \f$ S\in so(3)\f$
 * \param s [in] the \f$ s_x \f$, \f$ s_y \f$ and \f$ s_z \f$ of the matrix
 * \return The 3x3 skew-symmetric matrix \f$S\f$
 *
 * \f$
 * S =
 * \left[
 * \begin{array}{ccc}
 *    0 & -s_z &  s_y\\
 *  s_z &    0 & -s_x\\
 * -s_y &  s_x &    0
 * \end{array}
 * \right]
 * \f$
 */
inline matrix33
skew(double s0, double s1, double s2)
{
    matrix33 S;
    S(0,0) = 0.0; S(0,1) = -s2; S(0,2) =  s1;
    S(1,0) =  s2; S(1,1) = 0.0; S(1,2) = -s0;
    S(2,0) = -s1; S(2,1) =  s0; S(2,2) = 0.0;
        
    return S;
}

/**
 * \brief Constructs a 3x3 skew-symmetric matrix \f$ S\in so(3)\f$
 * \param s [in] the \f$ s_x \f$, \f$ s_y \f$ and \f$ s_z \f$ of the matrix
 * \return The 3x3 skew-symmetric matrix \f$S\f$
 *
 * \f$
 * S =
 * \left[
 * \begin{array}{ccc}
 *    0 & -s_z &  s_y\\
 *  s_z &    0 & -s_x\\
 * -s_y &  s_x &    0
 * \end{array}
 * \right]
 * \f$
 */
inline matrix33
skew(const vector3& s)
{
    return skew(s[0], s[1], s[2]);
}

/**
 * \brief Compute the cross-product of two vectors
 *
 */
inline vector3
crossprod(const vector3& a, const vector3& b)
{
    vector3 c;
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
    return c;
}

/**
 * \brief Constructs a 3x3 rotation matrix \f$R\f$ from a rotation vector using Rodrigues' formula
 * \param r [in] the \f$ r_x \f$, \f$ r_y \f$ and \f$ r_z \f$ of the matrix
 * \return The 3x3 rotation matrix \f$R\f$
 */
inline matrix33
rodrigues(double rx, double ry, double rz)
{
    double theta = std::sqrt(rx*rx+ry*ry+rz*rz);
    double c = std::cos(theta);
    double s = std::sin(theta);
    double c1 = 1. - c;
    double itheta = theta ? 1./theta : 0.;

    rx *= itheta;
    ry *= itheta;
    rz *= itheta;

    matrix33 rrt;
    rrt(0,0) = rx*rx;    rrt(0,1) = rx*ry;    rrt(0,2) = rx*rz;
    rrt(1,0) = rrt(0,1); rrt(1,1) = ry*ry;    rrt(1,2) = ry*rz;
    rrt(2,0) = rrt(0,2); rrt(2,1) = rrt(1,2); rrt(2,2) = rz*rz;

    // R = cos(theta)*I + (1 - cos(theta))*r*rT + sin(theta)*[r_x]
    // where [r_x] is [0 -rz ry; rz 0 -rx; -ry rx 0]
    return c*identity_matrix<double>(3) + c1*rrt + s*skew(rx,ry,rz);
}
 
inline matrix33
rodrigues(const vector3& r)
{
    return rodrigues(r[0], r[1], r[2]);
}

/**
 * \brief Constructs a 3x3 diagonal matrix 
 *
 * \f$
 * S =
 * \left[
 * \begin{array}{ccc}
 * d_0 &  0  &  0\\
 *  0  & d_1 &  0\\
 *  0  &  0  & d_2
 * \end{array}
 * \right]
 * \f$
 */
inline matrix33
diagonal(double d0, double d1, double d2) {
    matrix33 D;
    D(0,0) =  d0; D(0,1) = 0.0; D(0,2) = 0.0;
    D(1,0) = 0.0; D(1,1) =  d1; D(1,2) = 0.0;
    D(2,0) = 0.0; D(2,1) = 0.0; D(2,2) =  d2;
    
    return D;
}

/**
 * \brief Constructs a 3x3  matrix 
 */
inline matrix33
matrix33_from_coeffs(double m00, double m01, double m02, double m10, double m11, double m12, double m20, double m21, double m22) {
    matrix33 M;
    M(0,0) = m00; M(0,1) = m01; M(0,2) = m02;
    M(1,0) = m10; M(1,1) = m11; M(1,2) = m12;
    M(2,0) = m20; M(2,1) = m21; M(2,2) = m22;
        
    return M;
}

/**
 * \brief Constructs a 3x3  matrix 
 */
inline matrix33
matrix33_from_columns(const vector3 &m0, const vector3 &m1, const vector3 &m2) {
    matrix33 M;
    M(0,0) = m0[0]; M(0,1) = m1[0]; M(0,2) = m2[0];
    M(1,0) = m0[1]; M(1,1) = m1[1]; M(1,2) = m2[1];
    M(2,0) = m0[2]; M(2,1) = m1[2]; M(2,2) = m2[2];
        
    return M;
}

/**
 * \brief Constructs a 3x3  matrix 
 */
inline matrix33
matrix33_from_rows(const vector3 &m0, const vector3 &m1, const vector3 &m2) {
    matrix33 M;
    M(0,0) = m0[0]; M(0,1) = m0[1]; M(0,2) = m0[2];
    M(1,0) = m1[0]; M(1,1) = m1[1]; M(1,2) = m1[2];
    M(2,0) = m2[0]; M(2,1) = m2[1]; M(2,2) = m2[2];
        
    return M;
}
    
/**
 * \brief Constructs a 3 vector 
 */
inline vector3
vector3_from_coeffs(double v0, double v1, double v2) {
    vector3 v;
    v(0) = v0; v(1) = v1; v(2) = v2;
        
    return v;
}

/**
 * \brief Constructs a 3x4  matrix 
 */
inline matrix34
matrix34_from_coeffs(double m00, double m01, double m02, double m03, double m10, double m11, double m12, double m13, double m20, double m21, double m22, double m23) {
    matrix34 M;
    M(0,0) = m00; M(0,1) = m01; M(0,2) = m02; M(0,3) = m03;
    M(1,0) = m10; M(1,1) = m11; M(1,2) = m12; M(1,3) = m13;
    M(2,0) = m20; M(2,1) = m21; M(2,2) = m22; M(2,3) = m23;
    
    return M;
}

/**
 * \brief Constructs a 4 vector 
 */
inline vector4
vector4_from_coeffs(double v0, double v1, double v2, double v3) {
    vector4 v;
    v(0) = v0; v(1) = v1; v(2) = v2; v(3) = v3;
        
    return v;
}

/**
 * \brief Compute a homography from 4 points correspondences
 * \param p1 source point
 * \param p2 source point
 * \param p3 source point
 * \param p4 source point
 * \param q1 target point
 * \param q2 target point
 * \param q3 target point
 * \param q4 target point
 * \return the homography matrix that maps pi's to qi's
 *
 Using four point-correspondences pi ↔ pi^, we can set up an equation system to solve for the homography matrix H.
 An algorithm to obtain these parameters requiring only the inversion of a 3 × 3 equation system is as follows.
 From the four point-correspondences pi ↔ pi^ with (i ∈ {1, 2, 3, 4}),
 compute h1 = (p1 × p2 ) × (p3 × p4 ), h2 = (p1 × p3 ) × (p2 × p4 ), h3 = (p1 × p4 ) × (p2 × p3 ).
 Also compute h1^ , h2^ , h3^ using the same principle from the points pi^.
 Now, the homography matrix H can be obtained easily from
 H · [h1 h2 h3] = [h1^ h2^ h3^],
 which only requires the inversion of the matrix [h1 h2 h3].
 
 Algo from:
 http://www.dirk-farin.net/phd/text/AB_EfficientComputationOfHomographiesFromFourCorrespondences.pdf
*/
inline matrix33
homography_from_four_points(const vector3 &p1, const vector3 &p2, const vector3 &p3, const vector3 &p4,
                            const vector3 &q1, const vector3 &q2, const vector3 &q3, const vector3 &q4)
{
    matrix33 invHp;
    {
        matrix33 Hp = matrix33_from_columns(crossprod(crossprod(p1,p2),crossprod(p3,p4)),
                                            crossprod(crossprod(p1,p3),crossprod(p2,p4)),
                                            crossprod(crossprod(p1,p4),crossprod(p2,p3)));
        double detHp = determinant(Hp);
        inverse(Hp, detHp, invHp);
    }
    matrix33 Hq = matrix33_from_columns(crossprod(crossprod(q1,q2),crossprod(q3,q4)),
                                        crossprod(crossprod(q1,q3),crossprod(q2,q4)),
                                        crossprod(crossprod(q1,q4),crossprod(q2,q3)));
    return prod(Hq, invHp);
}
           
}}}
#endif //_BOOST_UBLAS_SMALL_
