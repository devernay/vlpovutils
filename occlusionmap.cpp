/*
 *  occlusionmap.cpp
 *  vlpovutils
 *
 *  Created by Frédéric Devernay on 05/07/11.
 *  Copyright 2011 INRIA. All rights reserved.
 *
 */


#include "vlpov.hpp"

#include <cassert>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/// From the motion field (m1x,m1y) in pixel units from camera1 to camera2 and
/// the motion field (m2x, m2y) from camera 2 to camera 1, compute the occlusion
/// map in camera 1. On output, the occlusion mapo contains 255 for visible points,
/// 0 for others.
/// A pixel is considered occluded if one of these conditions hold:
/// - it is not visible in camera2
/// - the motion fields (m1x,m1y) at (x1,y1) and (m2x,m2y) at (x1+m1x,y1+m1y) are
///   not consistent: norm_inf((m1x,m1y) + (m2x,m2y)) > t. t is measured in
///   pixels - a reasonable value is 1.
void vlpov_occlusionmap_opticalflow(const VLPovImage<double> &m1x, const VLPovImage<double> &m1y,
                                    const VLPovImage<double> &m2x, const VLPovImage<double> &m2y,
                                    double t,
                                    VLPovImage<unsigned char> &occ1)
{
    assert(m1x.width() == m1y.width() && m1x.height() == m1y.height());
    assert(m2x.width() == m2y.width() && m2x.height() == m2y.height());
    
    int width1 = m1x.width();
    int height1 = m1x.height();
    int width2 = m2x.width();
    int height2 = m2x.height();

    occ1.resize(width1, height1);
    
    for(int y1 = 0; y1 < height1; ++y1) {
        for(int x1 = 0; x1 < width1; ++x1) {
            double x2 = x1 + m1x(x1,y1);
            double y2 = y1 + m1y(x1,y1);
            
            // check for visibility
            if (x2 < -0.5 || x2 > width2-0.5 ||
                y2 < -0.5 || y2 > height2-0.5) {
                occ1(x1,y1) = 0;
                continue;
            }
            
            double x1b = x2 + m2x.linear_at(x2,y2);
            double y1b = y2 + m2y.linear_at(x2,y2);
            // check for occlusion
            if (std::abs(x1 - x1b) > t ||
                std::abs(y1 - y1b) > t) {
                occ1(x1,y1) = 0;
                continue;
            }
            
            // point is visible
            occ1(x1,y1) = 255;
        }
    }
    
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/// From the motion field (m1x,0) in pixel units from camera1 to camera2 and
/// the motion field (m2x, 0) from camera 2 to camera 1, compute the occlusion
/// map in camera 1. On output, the occlusion mapo contains 255 for visible points,
/// 0 for others.
/// A pixel is considered occluded if one of these conditions hold:
/// - it is not visible in camera2
/// - the motion fields (m1x,0) at (x1,y1) and (m2x,0) at (x+m1x,y+m1y) are
///   not consistent: abs(m1x(x1,y1) + m2x(x1+m1x,y1)) > t. t is measured in
///   pixels - a reasonable value is 1.
void vlpov_occlusionmap_stereo(const VLPovImage<double> &m1x, const VLPovImage<double> &m2x,
                               double t,
                               VLPovImage<unsigned char> &occ1)
{
    int width1 = m1x.width();
    int height1 = m1x.height();
    int width2 = m2x.width();
    int height2 = m2x.height();
    
    occ1.resize(width1, height1);
    
    for(int y1 = 0; y1 < height1; ++y1) {
        for(int x1 = 0; x1 < width1; ++x1) {
            double x2 = x1 + m1x(x1,y1);
            double y2 = y1;
            
            // check for visibility
            if (x2 < -0.5 || x2 > width2-0.5 ||
                y2 < -0.5 || y2 > height2-0.5) {
                occ1(x1,y1) = 0;
                continue;
            }
            
            double x1b = x2 + m2x.linear_at(x2,y2);
            // check for occlusion
            if (std::abs(x1 - x1b) > t) {
                occ1(x1,y1) = 0;
                continue;
            }
            
            // point is visible
            occ1(x1,y1) = 255;
        }
    }
    
}
