/*
 *  vec2camera.cpp
 *  vlpovutils
 *
 *  Created by Frédéric Devernay on 01/07/11.
 *  Copyright 2011 INRIA. All rights reserved.
 *
 */


#include "vlpov.hpp"
#define DEBUG
#ifdef DEBUG
#include <iostream>
#include <boost/numeric/ublas/io.hpp>
using namespace std;
#endif

using namespace boost::numeric::ublas;

// VEC2MOTION  Convert "look-at" camera specification to matrices
//   [R,T]=VEC2MOTION(POS, DIR, UP) take position POS, "direction"
//   vector DIR and "up" vector UP and returns camera rotation R
//   and translation T. 
//
//   [R,T] specifies the transformation from the inertial ref. frame to
//   the camera ref. frame.
//
//   See also LOOK2MOTION().
void vlpov_vec2camera(int width,
                      int height,
                      const boost::numeric::ublas::vector3 &pos,
                      const boost::numeric::ublas::vector3 &dir,
                      const boost::numeric::ublas::vector3 &up,
                      const boost::numeric::ublas::vector3 &right,
                      boost::numeric::ublas::matrix33 &K,
                      boost::numeric::ublas::matrix33 &R,
                      boost::numeric::ublas::vector3 &T)
{
    // Summary:
    // any 3-D point M=(X,Y,Z) can be decomposed in the (right, up, dir) frame, as
    // M = pos + x*right + y*up + z*dir,
    // or M = pos + [right,up,dir]*m, with m=(x,y,z)
    // camera coordinates are (x/z,y/z), in [-0.5,0.5]x[-0.5,0.5], and are then mapped
    // to pixel coordinates in the range [-0.5,width-0.5]x[height-0.5,-0.5] (the y-axis is inverted)
    // the projection matrix can then be written as:
    // P = B[A -A*pos] = BA[I3 -pos], with A = inverse([right,up,dir]) and B = [[width;0;0.5*(width-1)][0;-height;0.5*(height-1)][0;0;1]] 
    // BA can then be decomposed into KR (using RQ decomposition), so that
    // P = K[R t], with t = -R*pos

    matrix33 A;
    matrix33 RUD = matrix33_from_columns(right,up,dir);
    double det = determinant(RUD);
    inverse(RUD, det, A);
    matrix33 B = matrix33_from_coeffs(width, 0, 0.5*(width-1),
                                      0, -height, 0.5*(height-1),
                                      0, 0, 1);
    matrix33 BA = prod(B,A);
    rqdecomp33(BA,K,R);
#   ifdef DEBUG
    //cout << "BA=" << endl << BA << endl << "KR=" << endl <<  prod(K,R) << endl;
#   endif
    K /= K(2,2); // normalize K
    noalias(T) = -prod(R,pos);
}

