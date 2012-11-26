/* 
 * generate a disp map for the synthetic sphere
 * 
 * $Log: genSphere.c,v $
 * Revision 1.4  2003/05/13 08:40:24  devernay
 * deal with oversampling rectified images over X
 *
 * Revision 1.3  2001/01/17 10:15:28  devernay
 * handle new API
 *
 * Revision 1.2  2000/05/05 10:35:35  devernay
 * a bunch of cygwin fixes
 *
 * Revision 1.1  2000/02/21 09:28:09  devernay
 * added genSphere: comparison with theoretical disparity and orientation
 *
 */
/*
  gcc -I/home/movi/devernay/robotvis/include -O3 genSphere.c -L/home/movi/devernay/robotvis/lib -lRimage -lRsys -lm -o genSphere
*/
#ifndef lint
static char vcid[] = "$Id: genSphere.c,v 1.4 2003/05/13 08:40:24 devernay Exp $";
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <math.h>
#include <robotvis.h>
#include <Rimage1.h>
#include <Rstereo.h>

#define SIZE 512
#define RADIUS 0.45				/* synthetic sphere radius */

/* angle_X, angle_Y and angle_Z */
void flow_sphere(int x_pixels,          /* IN PIXELS */
                 int y_pixels,
                 int image_width,
                 double sphere_radius, /* from 0 to 0.5 */
                 double angle_X,        /* IN DEGREES */
                 double angle_Y,
                 double angle_Z,
                 double *flow_x,        /* IN PIXELS */
                 double *flow_y,        /* IN PIXELS */
                 double *disp)          /* IN PIXELS */
{
    double x, y;
    double Xl0,Yl0,Zl0;
    double Xw,Yw,Zw;
    double Xl,Yl,Zl;
    double Xr,Yr,Zr;
    double X,Y,Z;
    const double cosa = 2/sqrt(4.25);
    const double sina = 0.5/sqrt(4.25);
    
    /* normalize image coordinates */
    x = (x_pixels-image_width/2)/(double)(image_width); /* x positive to the right */
    y = -(y_pixels-image_width/2+1)/(double)(image_width); /* y positive to the top */

    /* convert angles to radians */
    angle_X *= M_PI/180.;
    angle_Y *= M_PI/180.;
    angle_Z *= M_PI/180.;
    
    Xl0 = x/sphere_radius;
    Yl0 = y/sphere_radius;
    Zl0 = Xl0*Xl0+Yl0*Yl0 - 1;                   /* Z is negative towards the viewer */
    if (Zl0 > 0) {                 /* out of the sphere */
        *flow_x = 0;
        *flow_y = 0;
        *disp = 0;
        return;
    }
    Zl0 = -sqrt(-Zl0);
    /* convert from left image view to world coordinates: rotate around y axis */
    Xw = Zl0*sina+Xl0*cosa;
    Yw = Yl0;
    Zw = Zl0*cosa-Xl0*sina;

    if (Xw > 0) {                 /* in right hemisphere */
        angle_X = -angle_X;
        /* uncomment the following to disable right hemisphere: */
        /*
        *flow_x = 0;
        *flow_y = 0;
        *disp = 0;
        return;
        */
    }

    /* rotate around X axis */
    X = Xw;
    Y = Yw*cos(angle_X)-Zw*sin(angle_X);
    Z = Yw*sin(angle_X)+Zw*cos(angle_X);

    Xw = X;
    Yw = Y;
    Zw = Z;

    /* rotate around Y axis */
    Y = Yw;
    Z = Zw*cos(angle_Y)-Xw*sin(angle_Y);
    X = Zw*sin(angle_Y)+Xw*cos(angle_Y);

    Xw = X;
    Yw = Y;
    Zw = Z;
    
    /* rotate around Z axis */
    Z = Zw;
    X = Xw*cos(angle_Z)-Yw*sin(angle_Z);
    Y = Xw*sin(angle_Z)+Yw*cos(angle_Z);

    Xw = X;
    Yw = Y;
    Zw = Z;
    
    /* convert from world coordinates to left image view: rotate around y axis */
    Xl = -Zw*sina+Xw*cosa;
    Yl = Yw;
    Zl = Zw*cosa+Xw*sina;

    /* convert from world coordinates to right image view: rotate around y axis */
    Xr = Zw*sina+Xw*cosa;
    Yr = Yw;
    Zr = Zw*cosa-Xw*sina;

    if (Zl > 0) {				/* undefined (flow occlusion) */
        *flow_x = 0;
        *flow_y = 0;
        *disp = 0;
        return;
    }
    
    *flow_x = (Xl-Xl0) * sphere_radius * image_width;
    *flow_y = -(Yl-Yl0) * sphere_radius * image_width;
    /* compute disparity */
    if (Zr > 0) {                       /* disparity occlusion */
        *disp = 0;
    }
    else
        *disp = (Xr - Xl) * sphere_radius * image_width;

}

/* reconstruction a partir de deux vues orthographiques */
void recons(double x, double y, double d,
			double *X, double *Y, double *Z)
{
	double cosa = 2/sqrt(4.25);
	double sina = 0.5/sqrt(4.25);

	*Y = -(y-SIZE/2+1)/(double)(SIZE);
	*X = (x-SIZE/2+d/2)/(double)(SIZE*cosa);
	*Z = d/(2*SIZE*sina);
}

/* projection */
void proj(double X, double Y, double Z,
		  double *x, double *y, double *d)			
{
	double cosa = 2/sqrt(4.25);
	double sina = 0.5/sqrt(4.25);

	*d = Z*(2*SIZE*sina);
	*x = X*SIZE*cosa + SIZE/2 - *d/2;
	*y = -Y*SIZE +SIZE/2 -1;
}

/* compute optical flow and disparity */
int
main(int argc, char **argv)
{
	float *flowx;
	float *flowy;
	float *disp;
	int x,y;
	double fx, fy, d;
	double X, Y, Z;
        double aX, aY, aZ, tXY;
        
        if (argc != 4) {
            fprintf(stderr,"Usage: %s <angle_X_degrees> <angle_Y_degrees> <angle_Z_degrees>\n", argv[0]);
            exit(1);
        }
        aX = atof(argv[1]);
        aY = atof(argv[2]);
        aZ = atof(argv[3]);
	flowx = (float*)malloc(SIZE*SIZE*sizeof(float));
	flowy = (float*)malloc(SIZE*SIZE*sizeof(float));
	disp = (float*)malloc(SIZE*SIZE*sizeof(float));
	for (y=0; y<SIZE; y++) {
		for (x=0; x<SIZE; x++) {
                    /* coordonnees dans le repere image de povray du CENTRE du
                       pixel */
                    /* n'a ete teste que pour SIZE multiple de 2 */
                    flow_sphere(x, y, SIZE, RADIUS, aX, aY, aZ, &fx, &fy, &d);
                    flowx[y*SIZE+x] = fx;
                    flowy[y*SIZE+x] = fy;
                    disp[y*SIZE+x] = d;
		}
	}
	image1Save("flowx.inr", (IMAGE1)flowx, SIZE, SIZE,
                   IMAGE1_FLOAT|IMAGE1_INRIMAGE_FORMAT);
	image1Save("flowy.inr", (IMAGE1)flowy, SIZE, SIZE,
                   IMAGE1_FLOAT|IMAGE1_INRIMAGE_FORMAT);
	image1Save("disp.inr", (IMAGE1)disp, SIZE, SIZE,
                   IMAGE1_FLOAT|IMAGE1_INRIMAGE_FORMAT);
	free(flowx);
	free(flowy);
	exit(0);
}
