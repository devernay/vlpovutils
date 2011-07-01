#include "vlpov.hpp"

#include <cassert>

using namespace std;
using namespace boost::numeric::ublas;

/*
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CVPoV function: [Mx, My] = computeMotionField(I1,x2,y2,z2,K1,R1,T1,R2,T2)
% 
% This function computes the point correspondance using the camera
% calibration data and returns the motion field matrices to the calling
% function.
% 
% INPUTS:
% I1 = input image
% x2,y2,z2 = 3d coordinates of the point in the second camera frame
% K1 = intrincic matrix of the first camera
% R1, T1 and R2, T2 = rotation and translation matrices of
% camera 1 and 2 respectively
% 
% OUTPUTS: returns the motion field matrices Mx and My. Mx = displacement
% in x axis, My is the displacement in y axis.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/
#define DEBUG

// Compute the motion field (m1x,m1y) in pixel units from scene with depth z1 seen from camera cam1 to camera cam2
// the dimensions of z1, m1x and m1y are the same as cam1
void vlpov_motionfield(const VLPovCamera &cam1, const VLPovImage<double> &z1, const VLPovCamera &cam2, VLPovImage<double> &m1x, VLPovImage<double> &m1y)
{
    assert(cam1.width == z1.width() && cam1.height == z1.height());
    m1x.resize(cam1.width, cam1.height);
    m1y.resize(cam1.width, cam1.height);
    matrix33 K2, R1, R2;
    vector3 T1, T2;
    cam1.extrinsicParameters(R1,T1);
    cam2.extrinsicParameters(R2,T2);
    cam2.intrinsicMatrix(K2);
#   ifdef DEBUG
    matrix33 K1;
    cam2.intrinsicMatrix(K1);
#   endif

    for(int y1 = 0; y1 < z1.height(); ++y1) {
        for(int x1 = 0; x1 < z1.width(); ++x1) {
            // compute the camera coordinates of pixel x,y
            double x1i = cam1.topx + x1 * cam1.psx;
            double y1i = cam1.topy + y1 * cam1.psy;
            // compute the 3-D reconstruction P1 in camera 1 reference frame
            double Z1 = z1(x1,y1);
            vector3 P1 = vector3_from_coeffs(x1i * Z1, y1i * Z1, Z1);
#           ifdef DEBUG
            // project
            vector3 p1 = prod(K1 , P1);
            // compute coordinates
            double x1p = p1(0)/p1(2);
            double y1p = p1(1)/p1(2);
            assert(std::abs(x1p - x1) < 1e-3 && std::abs(y1p - y1) < 1e-3);
#           endif
            // reconstruct in the global reference frame
            vector3 P = prod(R1,P1+T1);
            // reconstruct in camera 2 reference frame
            vector3 P2 = prod(trans(R2),P)-T2;
            // project
            vector3 p2 = prod(K2 , P2);
            // compute coordinates
            double x2 = p2(0)/p2(2);
            double y2 = p2(1)/p2(2);
            // compute motion field
            m1x(x1,y1) = x2-x1;
            m1y(x1,y1) = y2-y1;
        }
    }
    
}

// Compute the motion field (m1x,m1y) in pixel units from scene with depth z1 seen from camera cam1 to camera cam2
// the dimensions of z1, m1x and m1y are the same as cam1
void vlpov_motionfield2(const VLPovCamera &cam1, const VLPovImage<double> &z1, const VLPovCamera &cam2, VLPovImage<double> &m1x, VLPovImage<double> &m1y)
{
    assert(cam1.width == z1.width() && cam1.height == z1.height());
    m1x.resize(cam1.width, cam1.height);
    m1y.resize(cam1.width, cam1.height);
    matrix33 K2, R1, R2;
    vector3 T1, T2;
    cam1.extrinsicParameters(R1,T1);
    cam2.extrinsicParameters(R2,T2);
    cam2.intrinsicMatrix(K2);
#   ifdef DEBUG
    matrix33 K1;
    cam2.intrinsicMatrix(K1);
#   endif
    
    for(int y1 = 0; y1 < z1.height(); ++y1) {
        for(int x1 = 0; x1 < z1.width(); ++x1) {
            // compute the camera coordinates of pixel x,y
            double x1i = cam1.topx + x1 * cam1.psx;
            double y1i = cam1.topy + y1 * cam1.psy;
            // compute the 3-D reconstruction P1 in camera 1 reference frame
            double Z1 = z1(x1,y1);
            vector3 P1 = vector3_from_coeffs(x1i * Z1, y1i * Z1, Z1);
#           ifdef DEBUG
            // project
            vector3 p1 = prod(K1 , P1);
            // compute coordinates
            double x1p = p1(0)/p1(2);
            double y1p = p1(1)/p1(2);
            assert(std::abs(x1p - x1) < 1e-3 && std::abs(y1p - y1) < 1e-3);
#           endif
            // reconstruct in the global reference frame
            vector3 P = prod(R1,P1+T1);
            // reconstruct in camera 2 reference frame
            vector3 P2 = prod(trans(R2),P)-T2;
            // project
            vector3 p2 = prod(K2 , P2);
            // compute coordinates
            double x2 = p2(0)/p2(2);
            double y2 = p2(1)/p2(2);
            // compute motion field
            m1x(x1,y1) = x2-x1;
            m1y(x1,y1) = y2-y1;
        }
    }
    
}
