/*
 *  vlpov_project.cpp
 *  vlpovutils
 *
 *  Created by Frédéric Devernay on 06/07/11.
 *  Copyright 2011 INRIA. All rights reserved.
 *
 */


#include "vlpov.hpp"

#include <string>
#include <limits>
#include <cstdio>
#include <boost/numeric/ublas/io.hpp>

using namespace std;
using namespace boost::numeric::ublas;

#define USAGE "Usage: %s <frame1> [<frame2>]\n"\
"Help: Compute the projection (x, y, depth) of 3D points in <frame1> and\n"\
"      optionally pixel motion to <frame2>.\n"\
"      3D points coordinates are read from standard input.\n"\
"Arguments:\n"\
"<frame1> base frame basename (file with extension .txt will be read)\n"\
"<frame2> second frame basename (file with extension .txt will be read)\n"\
"\n"\
"*Important note*: The 3D points must be given in a right-handed coordinate\n"\
"                  system, where the Z is the *opposite* of POV-Ray's Z.\n"


int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3) {
        fprintf(stderr, USAGE, argv[0]);
        exit(2);
    }
    bool do_motion = (argc == 3);
    const char *frame1 = argv[1];
    const char *frame2 = do_motion ? argv[2] : NULL;
    
    {
        VLPovCamera cam1;
        VLPovCamera cam2;
        int retval;
        
        retval = vlpov_povread(frame1, cam1);
        if(retval != 0) {
            fprintf(stderr,"Error %d while reading %s.txt\n", retval, frame1);
            exit(1);
        }
        
        //cout << "Intrinsic matrix 1(new): " << endl << cam1.K << endl;
        //cout << "Extrinsic 1(new): " << endl << cam1.R << endl << cam1.t << endl;
      
        if (do_motion) {
            retval = vlpov_povread(frame2, cam2);
            if(retval != 0) {
                fprintf(stderr,"Error %d while reading %s.txt\n", retval, frame2);
                exit(1);
            }
        }
        
        vector3 P; // position in the global reference frame
        while(scanf("%lg %lg %lg", &P(0), &P(1), &P(2)) == 3) {
              
            // reconstruct in camera 1 reference frame
            vector3 P1 = prod(cam1.R,P)+cam1.t;
            // project
            vector3 p1 = prod(cam1.K , P1);
            // compute coordinates
            double x1 = p1(0)/p1(2);
            double y1 = p1(1)/p1(2);
            if (!do_motion) {
                printf("%g %g %g\n", x1, y1, P1(2));
            } else {
                // reconstruct in camera 2 reference frame
                vector3 P2 = prod(cam2.R,P)+cam2.t;
                // project
                vector3 p2 = prod(cam2.K , P2);
                // compute coordinates
                double x2 = p2(0)/p2(2);
                double y2 = p2(1)/p2(2);
                printf("%g %g %g %g %g\n", x1, y1, P1(2), x2-x1, y2-y1);
            }
        }
    }
    
    return 0;
}
