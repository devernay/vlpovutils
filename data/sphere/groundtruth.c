#include <math.h>

#define SIZE 512
#define RADIUS 0.45				/* synthetic sphere radius */
/*
  sqrty := sqrt(1-y*y/(r*r)):
  disp := -2*sina*(sina*x+cosa*sqrt(r*r-y*y-x*x))*SIZE;
  diff(disp,x)/SIZE;
  -diff(disp,y)/SIZE;
 */

float disp_sphere(double x, double y, double r)
{
	double cosa = 2/sqrt(4.25);
	double sina = 0.5/sqrt(4.25);
	double sqrty;

	if ((y<=-r) || (y>=r))
		return 0.0;

	sqrty = sqrt(1-y*y/(r*r));
	
	if ((x<=-sqrty*r) || (x>=sqrty*r))
		return 0.0;
	
	if ( x<=-cosa*sqrty*r )
		return 1.0;             /* undefined (occlusion) */

	return -2*sina*(sina*x+cosa*sqrt(r*r-y*y-x*x))*SIZE;

}

float dispx_sphere(double x, double y, double r)
{
	double cosa = 2/sqrt(4.25);
	double sina = 0.5/sqrt(4.25);
	double sqrty;

	if ((y<=-r) || (y>=r))
		return 0.0;

	sqrty = sqrt(1-y*y/(r*r));
	
	if ((x<=-sqrty*r) || (x>=sqrty*r))
		return 0.0;
	
	if ( x<=-cosa*sqrty*r )
		return 0.0;				/* undefined (occlusion) */

	return -2*sina*(sina-cosa*x/sqrt(r*r-y*y-x*x));

}

float dispy_sphere(double x, double y, double r)
{
	double cosa = 2/sqrt(4.25);
	double sina = 0.5/sqrt(4.25);
	double sqrty;
	double sqrtxy;

	if ((y<=-r) || (y>=r))
		return 0.0;

	sqrty = sqrt(1-y*y/(r*r));
	
	if ((x<=-sqrty*r) || (x>=sqrty*r))
		return 0.0;
	
	if ( x<=-cosa*sqrty*r )
		return 0.0;				/* undefined (occlusion) */

	sqrtxy = sqrt(r*r-x*x-y*y);

	return -2*sina*cosa*y/sqrt(r*r-y*y-x*x);
}

float flowx_sphere(double x, double y, double r, double t)
{
	double sqrty;
        double t0;
        
	if ((y<=-r) || (y>=r))
		return 0.0;

	sqrty = sqrt(1-y*y/(r*r));
	
	if ((x<=-sqrty*r) || (x=>sqrty*r))
		return 0.0;
	t0 = acos(x/(sqrty*r));
        if ( (t0+t) < 0 || (t0+t) > M_PI)
            return 0;				/* undefined (occlusion) */
        
	return sqrty*r*(cos(t0+t)-x/(sqrty*r))*SIZE;
}

float flowy_sphere(double x, double y, double r, double t)
{
    return 0.0;
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
