#ifndef _BOOST_UBLAS_SMALL_
#define _BOOST_UBLAS_SMALL_

// types and functions for small vectors and matrices (2x2, 3x3...)

#include "fwdtools.hpp"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>

#include <cfloat>

namespace boost { namespace numeric { namespace ublas {

class vector2: public bounded_vector<double, 2> {
    typedef bounded_vector<double, 2> vector_type;
  public:
    vector2 (): vector_type() {}
        
    vector2 (const vector_type &v): vector_type (v) {}
        
    template<class A2>
    vector2 (const vector<double, A2> &v): vector_type (v) {}
        
    template<class AE>
    vector2 (const vector_expression<AE> &ae): vector_type (ae) {}
        
    vector2(double x, double y) {
        data()[0] = x;
        data()[1] = y;
    }
};

class vector3: public bounded_vector<double, 3> {
    typedef bounded_vector<double, 3> vector_type;
  public:
    vector3 (): vector_type() {}
        
    vector3 (const vector_type &v): vector_type (v) {}
        
    template<class A2>
    vector3 (const vector<double, A2> &v): vector_type (v) {}
        
    template<class AE>
    vector3 (const vector_expression<AE> &ae): bounded_vector<double, 3> (ae) {}
        
    vector3(double x, double y, double z) {
        data()[0] = x;
        data()[1] = y;
        data()[2] = z;
    }
};

class vector4: public bounded_vector<double, 4> {
    typedef bounded_vector<double, 4> vector_type;
  public:
    vector4 (): vector_type() {}
        
    vector4 (const vector_type &v): vector_type (v) {}
        
    template<class A2>
    vector4 (const vector<double, A2> &v): vector_type (v) {}
        
    template<class AE>
    vector4 (const vector_expression<AE> &ae): vector_type (ae) {}
        
    vector4(double x, double y, double z, double t) {
        data()[0] = x;
        data()[1] = y;
        data()[2] = z;
        data()[3] = t;
    }
};
    
class matrix22: public bounded_matrix<double, 2, 2, row_major> {
    typedef bounded_matrix<double, 2, 2, row_major> this_matrix_type;
  public:
    matrix22 (): this_matrix_type() {}
        
    matrix22 (const this_matrix_type &v): this_matrix_type (v) {}
        
    template<class A2>
    matrix22 (const matrix<double, row_major, A2> &v): this_matrix_type (v) {}
        
    template<class AE>
    matrix22 (const matrix_expression<AE> &ae): this_matrix_type (ae) {}
        
    matrix22(double a, double b,
             double c, double d) {
        data()[0] = a; data()[1] = b;
        data()[2] = c; data()[3] = d;
    }
};

class matrix24: public bounded_matrix<double, 2, 4, row_major> {
    typedef bounded_matrix<double, 2, 4, row_major> this_matrix_type;
  public:
    matrix24 (): this_matrix_type() {}
        
    matrix24 (const this_matrix_type &v): this_matrix_type (v) {}
        
    template<class A2>
    matrix24 (const matrix<double, row_major, A2> &v): this_matrix_type (v) {}
        
    template<class AE>
    matrix24 (const matrix_expression<AE> &ae): this_matrix_type (ae) {}
        
    matrix24(double a, double b, double c, double d,
             double e, double f, double g, double h) {
        data()[0] = a; data()[1] = b; data()[2] = c; data()[3] = d;
        data()[4] = e; data()[5] = f; data()[6] = g; data()[7] = h;
    }
};

class matrix33: public bounded_matrix<double, 3, 3, row_major> {
    typedef bounded_matrix<double, 3, 3, row_major> this_matrix_type;
  public:
    matrix33 (): this_matrix_type() {}
        
    matrix33 (const this_matrix_type &v): this_matrix_type (v) {}
        
    template<class A2>
    matrix33 (const matrix<double, row_major, A2> &v): this_matrix_type (v) {}
        
    template<class AE>
    matrix33 (const matrix_expression<AE> &ae): this_matrix_type (ae) {}
        
    matrix33(double a, double b, double c,
             double d, double e, double f,
             double g, double h, double i) {
        data()[0] = a; data()[1] = b; data()[2] = c;
        data()[3] = d; data()[4] = e; data()[5] = f;
        data()[6] = g; data()[7] = h; data()[8] = i;
    }
};

template<class L>
inline double
determinant(const bounded_matrix<double,2,2,L>& A)
{
    return A(0,0)*A(1,1) - A(0,1)*A(1,0);
}

template<class L1, class L2>
inline void
inverse(const bounded_matrix<double,2,2,L1>& A, double det, bounded_matrix<double,2,2,L2>& B)
{
    BOOST_UBLAS_CHECK(det != 0., boost::numeric::ublas::divide_by_zero());
    double fac = 1./det;
    B(0,0) =  A(1,1)*fac;
    B(0,1) = -A(0,1)*fac;
    B(1,0) = -A(1,0)*fac;
    B(1,1) =  A(1,1)*fac;
}

template<class L>
inline double
determinant(const bounded_matrix<double,3,3,L>& A)
{
    return (  A(0,0)*(A(2,2)*A(1,1)-A(2,1)*A(1,2))
              - A(1,0)*(A(2,2)*A(0,1)-A(2,1)*A(0,2))
              + A(2,0)*(A(1,2)*A(0,1)-A(1,1)*A(0,2)));
}

template<class L1, class L2>
inline void
inverse(const bounded_matrix<double,3,3,L1>& A, double det, bounded_matrix<double,3,3,L2>& B)
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
    return matrix33(m00, m01, m02, m10, m11, m12, m20, m21, m22);
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
    return vector3(v0, v1, v2);
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
    return vector4(v0, v1, v2, v3);
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
inline bool
homography_from_four_points(const vector3 &p1, const vector3 &p2, const vector3 &p3, const vector3 &p4,
                            const vector3 &q1, const vector3 &q2, const vector3 &q3, const vector3 &q4,
                            matrix33 *H)
{
    matrix33 invHp;
    matrix33 Hp = matrix33_from_columns(crossprod(crossprod(p1,p2),crossprod(p3,p4)),
                                        crossprod(crossprod(p1,p3),crossprod(p2,p4)),
                                        crossprod(crossprod(p1,p4),crossprod(p2,p3)));
    double detHp = determinant(Hp);
    if (detHp == 0.) {
        return false;
    }
    matrix33 Hq = matrix33_from_columns(crossprod(crossprod(q1,q2),crossprod(q3,q4)),
                                        crossprod(crossprod(q1,q3),crossprod(q2,q4)),
                                        crossprod(crossprod(q1,q4),crossprod(q2,q3)));
    double detHq = determinant(Hq);
    if (detHq == 0.) {
        return false;
    }
    inverse(Hp, detHp, invHp);
    *H = prod(Hq, invHp);
    return true;
}

// RQ decomposition of a 3x3 matrix. very useful to recover [KR,t] from a 3x4 P-matrix
// inspired by cvRQDecomp3x3 from OpenCV
inline void rqdecomp33( const matrix33 &M, matrix33 &R, matrix33 &Q)
{
    double z, c, s;

    /* Find Givens rotation Q_x for x axis (left multiplication). */
    /*
     *      ( 1  0  0 )
     * Qx = ( 0  c  s ), c = m33/sqrt(m32^2 + m33^2), s = m32/sqrt(m32^2 + m33^2)
     *      ( 0 -s  c )
     */
    // We look for the rotation that sets M(2,1) to zero
    // If it's already zero we set Qx to the Id
    
    s = M(2,1);
    
    if (fabs(s) < FLT_EPSILON) {
        c = 1.;
        s = 0.;
    } else {
        c = M(2,2);
        z = 1./sqrt(c * c + s * s /* + DBL_EPSILON */);
        c *= z;
        s *= z;
    }
    matrix33 Qx(1, 0, 0,
                0, c, s,
                0, -s, c);

    noalias(R) = prod(M,Qx);

    BOOST_UBLAS_CHECK(fabs(R(2,1)) < FLT_EPSILON, singular ());
    R(2,1) = 0;

    /* Find Givens rotation for y axis. */
    /*
     *      ( c  0  s )
     * Qy = ( 0  1  0 ), c = m33/sqrt(m31^2 + m33^2), s = m31/sqrt(m31^2 + m33^2)
     *      (-s  0  c )
     */
    // We look for the rotation that sets R(2,0) to zero
    // If it's already zero we set Qy to the Id
    s = R(2,0);
    
    if (fabs(s) < FLT_EPSILON) {
        c = 1.;
        s = 0.;
    } else {
        c = R(2,2);
        z = 1./sqrt(c * c + s * s /* + DBL_EPSILON */);
        c *= z;
        s *= z;
    }
    matrix33 Qy(c, 0, s,
                0, 1, 0,
                -s, 0, c);

    matrix33 N = prod(R,Qy);

    BOOST_UBLAS_CHECK(fabs(N(2,0)) < FLT_EPSILON, singular ());
    N(2,0) = 0;

    /* Find Givens rotation for z axis. */
    /*
     *      ( c  s  0 )
     * Qz = (-s  c  0 ), c = m22/sqrt(m21^2 + m22^2), s = m21/sqrt(m21^2 + m22^2)
     *      ( 0  0  1 )
     */
    // We look for the rotation that sets N(1,0) to zero
    // If it's already zero we set Qz to the Id

    s = N(1,0);
    
    if (fabs(s) < FLT_EPSILON) {
        c = 1.;
        s = 0.;
    } else {
        c = N(1,1);
        z = 1./sqrt(c * c + s * s /* + DBL_EPSILON */);
        c *= z;
        s *= z;
    }

    matrix33 Qz(c, s, 0,
                -s, c, 0,
                0, 0, 1);

    noalias(R) = prod(N,Qz);

    BOOST_UBLAS_CHECK(fabs(R(1,0)) < FLT_EPSILON, singular ());
    R(1,0) = 0;
    
    // Solve the decomposition ambiguity.
    // Diagonal entries of R shall have the same sign. All positive
    // or all negative. For the CameraMatrix decomposition the up to
    // scale normalization allows to make them all positive.
    // Further rotate R by 180 degree if necessary
    if( R(0,0) < 0 ) {
        if( R(1,1) < 0 ) {
            if (R(2,2) < 0) {
                // - , - , -
                // All Negative: OK
            } else {
                // - , - , +
                // rotate around z for 180 degree, i.e. a rotation matrix of
                // [-1,  0,  0],
                // [ 0, -1,  0],
                // [ 0,  0,  1]
                R(0,0) *= -1;
                R(0,1) *= -1;
                R(1,1) *= -1;

                Qz(0,0) *= -1;
                Qz(0,1) *= -1;
                Qz(1,0) *= -1;
                Qz(1,1) *= -1;
            }
        } else {
            if (R(2,2) > 0) {
                // - , + , +
                // Rotate around x for 180 degree, i.e. a rotation matrix of
                // [ 1,  0,  0],
                // [ 0, -1,  0],
                // [ 0,  0, -1]
                // All diagonal elements will be negative but up-to-scale 
                // normalization will cover this
                R(0,1) *= -1;
                R(0,2) *= -1;
                R(1,1) *= -1;
                R(1,2) *= -1;
                R(2,2) *= -1;

                Qz = trans(Qz);
                Qy = trans(Qy);
                
                Qx(1,1) *= -1;
                Qx(1,2) *= -1;
                Qx(2,1) *= -1;
                Qx(2,2) *= -1;
            } else {
                // - , + , -
                // rotate around y for 180 degree, i.e. a rotation matrix of
                // [-1,  0,  0],
                // [ 0,  1,  0],
                // [ 0,  0, -1]
                R(0,0) *= -1;
                R(0,2) *= -1;
                R(1,2) *= -1;
                R(2,2) *= -1;

                Qz = trans(Qz);

                Qy(0,0) *= -1;
                Qy(0,2) *= -1;
                Qy(2,0) *= -1;
                Qy(2,2) *= -1;
            }
        }
    } else {
        if( R(1,1) < 0 )  {
            // ??? for some reason, we never get here ???
            if (R(2,2) > 0) {
                // + , - , +
                // rotate around y for 180 degree, i.e. a rotation matrix of
                // [-1,  0,  0],
                // [ 0,  1,  0],
                // [ 0,  0, -1]
                // All diagonal elements will be negative but up-to-scale 
                // normalization will cover this
                R(0,0) *= -1;
                R(0,2) *= -1;
                R(1,2) *= -1;
                R(2,2) *= -1;

                Qz = trans(Qz);

                Qy(0,0) *= -1;
                Qy(0,2) *= -1;
                Qy(2,0) *= -1;
                Qy(2,2) *= -1;
            } else {
                // + , - , -
                // rotate around x for 180 degree, i.e. a rotation matrix of
                // [ 1,  0,  0],
                // [ 0, -1,  0],
                // [ 0,  0, -1]
                R(0,1) *= -1;
                R(0,2) *= -1;
                R(1,1) *= -1;
                R(1,2) *= -1;
                R(2,2) *= -1;

                Qz = trans(Qz);
                Qy = trans(Qy);

                Qx(1,1) *= -1;
                Qx(1,2) *= -1;
                Qx(2,1) *= -1;
                Qx(2,2) *= -1;
            }
        } else {
            if (R(2,2) > 0) {
                // + , + , +
                // All positive : OK
            } else {
                // + , + , -
                // rotate around z for 180 degree, i.e. a rotation matrix of
                // [-1,  0,  0],
                // [ 0, -1,  0],
                // [ 0,  0,  1]
                // All diagonal elements will be negative but up-to-scale 
                // normalization will cover this
                R(0,0) *= -1;
                R(0,1) *= -1;
                R(1,1) *= -1;

                Qz(0,0) *= -1;
                Qz(0,1) *= -1;
                Qz(1,0) *= -1;
                Qz(1,1) *= -1;
            }
        }
    }
    
    /* Calulate orthogonal matrix. */
    /*
     * Q = QzT * QyT * QxT
     */
    noalias(N) = prod(trans(Qz),trans(Qy));
    noalias(Q) = prod(N,trans(Qx));
}

inline matrix33 rotation3_x(double angle)
{
    double c = cos(angle);
    double s = sin(angle);
    return matrix33(1,  0,  0,
                    0,  c, -s,
                    0,  s,  c);
}

inline matrix33 rotation3_y(double angle)
{
    double c = cos(angle);
    double s = sin(angle);
    return matrix33( c, 0, s,
                    0, 1, 0,
                    -s, 0, c);
}

inline matrix33 rotation3_z(double angle)
{
    double c = cos(angle);
    double s = sin(angle);
    return matrix33(c, -s,  0,
                    s,  c,  0,
                    0,  0,  1);
}

inline matrix33 rotation3_euler_xyz(double x, double y, double z)
{
    matrix33 rotxy = prod(rotation3_y(y), rotation3_x(x));
    return prod(rotation3_z(z), rotxy);
}

inline matrix33 lookat(vector3 center) {
  vector3 zc = center;
  double normzc = norm_2(zc);
  BOOST_UBLAS_CHECK(normzc != 0, divide_by_zero () );
  zc /= normzc;
  vector3 xc = crossprod(vector3(0,1,0),zc);
  double normxc = norm_2(xc);
  BOOST_UBLAS_CHECK(normxc != 0, divide_by_zero () );
  xc /= normxc;
  vector3 yc = crossprod(zc,xc);
  matrix33 R;
  row(R,0) = xc;
  row(R,1) = yc;
  row(R,2) = zc;
  return R;
}

}}}
#endif //_BOOST_UBLAS_SMALL_
