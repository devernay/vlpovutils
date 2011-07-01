#include "vlpov.hpp"

// LOOK2MOTION  Convert "look-at" camera specification to matrices
//   [R,T]=LOOK2MOTION(POS, DIR, UP) take position POS, "look-at"
//   vector DIR and "up" vector UP and returns camera rotation R
//   and translation T. 
//
//   [R,T] specifies the transformation from the inertial ref. frame to
//   the camera ref. frame.
//
//   See also VEC2MOTION().
void vlpov_look2motion(const boost::numeric::ublas::vector3 &pos,
                       const boost::numeric::ublas::vector3 &look,
                       const boost::numeric::ublas::vector3 &up,
                       boost::numeric::ublas::matrix33 &R,
                       boost::numeric::ublas::vector3 &T)
{
    using namespace boost::numeric::ublas;
    //% Direction
    //z = (look(:,k) - pos(:,k)) ;
    vector3 z = look - pos;
    //z = z / norm(z) ;
    z /= norm_2(z);
  
    //% Right vector (our Y points down)
    //x = cross(-up, z) ;
    vector3 x = crossprod(-up, z);
    //x = x / norm(x) ;
    x /= norm_2(x);

    //% Up vector
    //y = cross(z,x) ;
    vector3 y = crossprod(z,x);

    //% Translation camera -> inertial
    //T = pos(:,k) ;
    T = pos;

    //% Rotation camera -> inertial
    //R = [x y z] ;
    R = matrix33_from_columns(x,y,z);
  
    //% Invert: inertial -> camera
    //R = R' ;
    R = trans(R);
    //T = -R*T ;
    T = -prod(R,T);
}
