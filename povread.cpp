#include "vlpov.hpp"

#include <cstdio>
#include <cmath>
#include <cstring>

using namespace boost::numeric::ublas;
using namespace std;

// inspired by povread.m from vlpov
// function [I,xi,yi,z,cam] = povread(file,n)
// % POVREAD  Reads POV-Ray output
// %   [I,XI,YI,Z] = POVREAD(FILE) reads the files FILE.TXT (camera
// %   parameters), FILE.PNG (rendered scene), FILE.DEPTH (Z buffer)
// %   returns the radience image I, the pixel coordinates XI,YI and the
// %   z-buffer Z. The convention is as such the 3D backprojection of the
// %   pixels is given by (XI.*Z, YI.*Z, Z).
// %
// %   [I,XI,YI,Z,CAM] = POVREAD(FILE) returns the camera intrinsics
// %   and estrinsics as well. These are
// %
// %   - R       camera rotation
// %   - T       camera translation
// %   - angle   full horizontal view angle [rad]
// %   - focal   focal length (always =1)
// %   - aspect  aspect ratio
// %   - psx     x size of pixels
// %   - psy     y size of pixels
// %   - prx     x pixel range
// %   - pry     y pixel range
// %
// %   CAM = POVREAD(FILE,1) reads only the camera parameters.

int vlpov_povread(const char* file, VLPovCamera &cam)
{
    size_t file_len = strlen(file);
    char *info_file = new char[file_len+5];
    const char *info_suffix = ".txt";
    std::copy(file, file+file_len, info_file);
    std::copy(info_suffix, info_suffix+5, info_file+file_len);
    int retval = vlpov_get_cam(info_file, cam);
    delete [] info_file;
    return retval;
}

int vlpov_povread(const char* file, VLPovImage<double> &z, VLPovCamera &cam)
{
    int retval;
    
    // [path name] = fileparts(file) ;

    // depth_file_endianess = 'b' ;
    // depth_file           = fullfile(path,[name '.depth']) ;
    // info_file            = fullfile(path,[name '.txt']  ) ;
    // rad_file             = fullfile(path,[name '.png']  ) ;

    size_t file_len = strlen(file);

    // % --------------------------------------------------------------------
    // %                                                               Camera
    // % --------------------------------------------------------------------
    
    // % working with focal is rather inessential as the focal length can be
    // % set to 1 ... but conceptually more obvious
    
    // cam = get_cam(info_file, name) ;
    
    // focal  = cam.focal ;
    // aspect = cam.aspect ;
    // angle  = cam.angle ;
    // M      = cam.height ;
    // N      = cam.width ;
    
    // % pixel size 
    // psx = 2*focal*tan(0.5*angle)/N ;
    // psy = 2*focal*tan(0.5*angle)/aspect/M ;
    
    // % pixel range
    // prx = linspace(-(N/2-0.5)*psx,+(N/2-0.5)*psx,N) ;
    // pry = linspace(-(M/2-0.5)*psy,+(M/2-0.5)*psy,M) ; 
    
    // % check for stripped down output
    // if nargin > 1
    // I = cam ;
    // if nargout > 1
    // error('POVREAD(FILE,1) returns only the camera parameters CAM');
    // end
    // return ;
    // end
    
    // % pixel grid
    // [xi,yi]=meshgrid(prx, pry) ;
    
    char *info_file = new char[file_len+5];
    const char *info_suffix = ".txt";
    std::copy(file, file+file_len, info_file);
    std::copy(info_suffix, info_suffix+5, info_file+file_len);
    retval = vlpov_get_cam(info_file, cam);
    delete [] info_file;
    if(retval != 0) {
        return retval;
    }
    
    // % --------------------------------------------------------------------
    // %                                                             Z-buffer
    // % --------------------------------------------------------------------
    
    // f = fopen( depth_file ) ;
    // z = fread( f, 'double', depth_file_endianess ) ;
    // fclose(f) ;
    
    // z = reshape(z,N,M)' ;
    
    char *depth_file = new char[file_len+7];
    const char *depth_suffix = ".depth";
    std::copy(file, file+file_len, depth_file);
    std::copy(depth_suffix, depth_suffix+7, depth_file+file_len);
    z.resize(cam.width, cam.height);
    retval = z.load(depth_file);
    delete [] depth_file;
    if(retval != 0) {
        return retval;
    }
    
    // % The z stored by POV is the euclidean distance from the camera
    // % center. Recover the right one.
    // z = focal * sqrt(z.^2 ./ (xi.^2+yi.^2+focal.^2)) ;
    for(int y = 0; y < z.height(); ++y) {
        for(int x = 0; x < z.width(); ++x) {
            double xi = cam.topx + x * cam.psx;
            double yi = cam.topy + y * cam.psy;
            z(x, y) = sqrt(pow(z(x, y),2)/(pow(xi,2)+pow(yi,2)+1.)); // focal = 1.
        }
    }
    
    
    // % --------------------------------------------------------------------
    // %                                                             Radiance
    // % --------------------------------------------------------------------
    
    // I = imread(rad_file) ;
    // [M,N,K] = size(I) ;
    // %I = im2double(I) ;
    // %I = I .^ (1/2.2) ;
    /*
    char *rad_file = new char[file_len+5];
    const char *rad_suffix = ".png";
    std::copy(file, file+file_len, rad_file);
    std::copy(rad_suffix, rad_suffix+5, rad_file+file_len);
    try {
        I.load(rad_file);
        I.pow(1./2.2);
    } catch (CImgIOException &e) {
        retval = -1;
    }
    delete [] rad_file;
    */

    return retval;
}



// % --------------------------------------------------------------------
// function cam = get_cam(info_file, name)
// % --------------------------------------------------------------------
// % Reads some camera parameters from info file (right_length,
// % direction_length, up_length, near and far values)
int vlpov_get_cam(const char* info_file, VLPovCamera &cam)
{
    cam.width  = 1 ;
    cam.height = 1 ;
    //cam.focal  = 1. ;
    cam.angle  = M_PI/2 ;
    cam.aspect = 320./240. ;
    cam.psx = 0.;
    cam.psy = 0.;
    cam.topx = 0.;
    cam.topy = 0.;
    cam.R      = identity_matrix<double>(3) ;
    cam.T      = vector3_from_coeffs(0.,0.,0.) ;
    
    // f = fopen(info_file, 'r') ;
    FILE *f = fopen(info_file, "r");
    if (f == NULL) {
        return -1;
    }
    
    char output_name[256];
    int screen_height;
    int screen_width;
    char cam_type[256];
    vector3 cam_pos;
    vector3 cam_dir;
    vector3 cam_up;
    vector3 cam_right;
    vector3 cam_sky;
    vector3 cam_lookat;
    vector3 cam_fpoint;
    double cam_focal;
    double cam_angle;
    
    // the scanning format comes from the vlpov povray patch
    int count = fscanf(f,
                       "output_name   = '%256s ;\n"
                       "screen_height = %d ;\n"
                       "screen_width  = %d ;\n"
                       "cam_type      = '%256s ;\n"
                       "cam_pos       = [%lf %lf %lf]' ;\n"
                       "cam_dir       = [%lf %lf %lf]' ;\n"
                       "cam_up        = [%lf %lf %lf]' ;\n"
                       "cam_right     = [%lf %lf %lf]' ;\n"
                       "cam_sky       = [%lf %lf %lf]' ;\n"
                       "cam_lookat    = [%lf %lf %lf]' ;\n"
                       "cam_fpoint    = [%lf %lf %lf]' ;\n"
                       "cam_focal     = %lf ;\n"
                       "cam_angle     = %lf ;\n",
                       output_name,
                       &screen_height,
                       &screen_width,
                       cam_type,
                       &cam_pos[0], 
                       &cam_pos[1], 
                       &cam_pos[2],
                       &cam_dir[0], 
                       &cam_dir[1], 
                       &cam_dir[2],
                       &cam_up[0], 
                       &cam_up[1], 
                       &cam_up[2],
                       &cam_right[0], 
                       &cam_right[1], 
                       &cam_right[2],
                       &cam_sky[0], 
                       &cam_sky[1], 
                       &cam_sky[2],
                       &cam_lookat[0],
                       &cam_lookat[1],
                       &cam_lookat[2],
                       &cam_fpoint[0],
                       &cam_fpoint[1],
                       &cam_fpoint[2],
                       &cam_focal,
                       &cam_angle) ;
    if (count != 27) {
        return -1;
    }
    // % After sourcing the script we are left with (at least)
    // %
    // % cam_pos
    // % cam_dir
    // % cam_up
    // % cam_right
    // %
    // % We compute: R, T, focal, angle (rad), aspect

    // % pov uses a left-handed system: convert.
    // e=diag([1 1 -1]) ;
    matrix33 e = diagonal(1.,1.,-1.);
    // cam_pos  = e*cam_pos ;
    cam_pos = prod(e,cam_pos);
    // cam_up   = e*cam_up ;
    cam_up = prod(e, cam_up);
    // cam_dir  = e*cam_dir ;
    cam_dir = prod(e, cam_dir);
    // camright = e*cam_right ; <= bug in vlpov, but only its norm is used anyway
    cam_right = prod(e, cam_right);

    double norm_cam_dir = norm_2(cam_dir);
    double norm_cam_right = norm_2(cam_right);
    double norm_cam_up = norm_2(cam_up);
    
    // focal  =         norm(cam_dir) ;
    double focal = norm_cam_dir;
    // aspect =         norm(cam_right)   / norm(cam_up) ;  
    double aspect = norm_cam_right   / norm_cam_up;
    // angle  = 2*atan( norm(cam_right)/2 / norm(cam_dir)  ) ;
    double angle = 2.*atan( norm_cam_right/2. / norm_cam_dir  ) ;

    // [R,T] = vec2motion( cam_pos, cam_dir, cam_up ) ; 
    vlpov_vec2motion(cam_pos, cam_dir, cam_up, cam.R, cam.T);
    cam.width  = screen_width ;
    cam.height = screen_height ;
    cam.angle  = angle ;
    cam.aspect = aspect ;
    
    // % --------------------------------------------------------------------
    // %                                                  Remap to unit focal
    // % --------------------------------------------------------------------

    // % Force unitary focal. It does not matter (we have angle) and
    // % equations simplify a lot (from 2D to 3D camera coordinates you just have to multiply by Z).
    // the following come from vlpov_povread()
    // % pixel size 
    // psx = 2*focal*tan(0.5*angle)/N ;
    // psy = 2*focal*tan(0.5*angle)/aspect/M ;
    cam.psx = norm_cam_right / focal / screen_width ;
    cam.psy = norm_cam_up    / focal / screen_height ;
    
    // % topleft pixel coordinates
    cam.topx  = -(screen_width/2.-0.5)*cam.psx; 
    cam.topy  = -(screen_height/2.-0.5)*cam.psy;

    return 0;
}