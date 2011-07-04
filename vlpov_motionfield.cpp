/*
 *  vlpov_motionfield.cpp
 *  vlpovutils
 *
 *  Created by Frédéric Devernay on 01/07/11.
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

#define USAGE "Usage: %s <frame1> <frame2>\n"\
"Help: Compute a motion field from a POV-Ray rendered depth map to another camera\n"\
"      The motion field is written to <frame1>.<frame2>.mx and\n"\
"      and <frame1>.<frame2>.my as raw images containing big-endian\n"\
"     (network-order) doubles - the same format as the depth map.\n"\
"Arguments:\n"\
"<frame1> first frame basename (files with extensions .depth and .txt will be \n"\
"         read)\n"\
"<frame2> second frame basename (file with extension .txt will be read)\n"

/* Return the basename of a pathname.
 This file is in the public domain. */
#ifndef DIR_SEPARATOR
#define DIR_SEPARATOR '/'
#endif

#if defined (_WIN32) || defined (__MSDOS__) || defined (__DJGPP__) || \
defined (__OS2__)
#define HAVE_DOS_BASED_FILE_SYSTEM
#ifndef DIR_SEPARATOR_2 
#define DIR_SEPARATOR_2 '\\'
#endif
#endif

/* Define IS_DIR_SEPARATOR.  */
#ifndef DIR_SEPARATOR_2
# define IS_DIR_SEPARATOR(ch) ((ch) == DIR_SEPARATOR)
#else /* DIR_SEPARATOR_2 */
# define IS_DIR_SEPARATOR(ch) \
(((ch) == DIR_SEPARATOR) || ((ch) == DIR_SEPARATOR_2))
#endif /* DIR_SEPARATOR_2 */

char * mybasename (const char *name)
{
    const char *base;
    
#if defined (HAVE_DOS_BASED_FILE_SYSTEM)
    /* Skip over the disk name in MSDOS pathnames. */
    if (ISALPHA (name[0]) && name[1] == ':') 
        name += 2;
#endif
        
    for (base = name; *name; name++) {
        if (IS_DIR_SEPARATOR (*name)) {
            base = name + 1;
        }
    }
    return (char *) base;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, USAGE, argv[0]);
        exit(2);
    }
    const char *frame1 = argv[1];
    const char *frame2 = argv[2];
    const char *frame2_basename = mybasename(frame2);

    {
        VLPovImage<double> z1;
        VLPovCamera cam1;
        VLPovCamera cam2;
        int retval;
    
        retval = vlpov_povread(frame1, z1, cam1);
        if(retval != 0) {
            fprintf(stderr,"Error %d while reading %s.{png,depth,txt}\n", retval, frame1);
            exit(1);
        }
        matrix33 K1, R1;
        vector3 t1;
        cam1.intrinsicMatrix(K1);
        //cam1.extrinsicParameters(R1, t1);
        cout << "Intrinsic matrix 1: " << endl << K1 << endl;
        cout << "Extrinsic 1: " << endl << cam1.R << endl << cam1.T << endl;
        z1.save((std::string(frame1)+".zold").c_str());
                
        retval = vlpov_povread(frame2, cam2);
        if(retval != 0) {
            fprintf(stderr,"Error %d while reading %s.txt\n", retval, frame2);
            exit(1);
        }
        
        VLPovImage<double> m1x, m1y;
        vlpov_motionfield(cam1, z1, cam2, m1x, m1y);
        std::string m1x_name = std::string(frame1)+"."+std::string(frame2_basename)+".mxold";
        std::string m1y_name = std::string(frame1)+"."+std::string(frame2_basename)+".myold";
        retval = m1x.save(m1x_name.c_str());
        if(retval != 0) {
            fprintf(stderr,"Error %d while saving %s\n", retval, m1x_name.c_str());
            exit(1);
        }
    
        m1y.save(m1y_name.c_str());
        if(retval != 0) {
            fprintf(stderr,"Error %d while saving %s\n", retval, m1y_name.c_str());
            exit(1);
        }
    }
  
    {
        VLPovImage<double> z1;
        VLPovCamera2 cam1;
        VLPovCamera2 cam2;
        int retval;
        
        retval = vlpov_povread2(frame1, z1, cam1);
        if(retval != 0) {
            fprintf(stderr,"Error %d while reading %s.{png,depth,txt}\n", retval, frame1);
            exit(1);
        }
 
        cout << "Intrinsic matrix 1(new): " << endl << cam1.K << endl;
        cout << "Extrinsic 1(new): " << endl << cam1.R << endl << cam1.t << endl;
        z1.save((std::string(frame1)+".znew").c_str());
        
        retval = vlpov_povread2(frame2, cam2);
        if(retval != 0) {
            fprintf(stderr,"Error %d while reading %s.txt\n", retval, frame2);
            exit(1);
        }
        
        VLPovImage<double> m1x, m1y;
        vlpov_motionfield2(cam1, z1, cam2, m1x, m1y);
        std::string m1x_name = std::string(frame1)+"."+std::string(frame2_basename)+".mxnew";
        std::string m1y_name = std::string(frame1)+"."+std::string(frame2_basename)+".mynew";
        retval = m1x.save(m1x_name.c_str());
        if(retval != 0) {
            fprintf(stderr,"Error %d while saving %s\n", retval, m1x_name.c_str());
            exit(1);
        }
        
        m1y.save(m1y_name.c_str());
        if(retval != 0) {
            fprintf(stderr,"Error %d while saving %s\n", retval, m1y_name.c_str());
            exit(1);
        }
    }
    
    return 0;
}
