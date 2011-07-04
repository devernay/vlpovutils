/*
 *  vlpov.hpp
 *  vlpovutils
 *
 *  Created by Frédéric Devernay on 30/06/11.
 *  Copyright 2011 INRIA. All rights reserved.
 *
 */

#include <tools/boost/numeric/ublas/small.hpp>

class VLPovCamera {
public:
    int width; //< image width in pixels
    int height; //< image height in pixels
    //double focal; //< focal length in camera units (normalized to 1 by vlpov_cam_read() )
    double angle; //< ???
    double aspect; //< pixel aspect ratio (x-size/y-size)
    double psx; //< pixel x-size
    double psy; //< pixel y-size
    double topx; //< x-coordinate of the top-left pixel
    double topy; //< y-coordinate of the top-left pixel
    // the camera coordinates of pixel (x,y) are (cam.topx + x * cam.psx, cam.topy + y * cam.psy)
    boost::numeric::ublas::matrix33 R; //< [R,T] specifies the transformation from the inertial ref. frame to the camera ref. frame.
    boost::numeric::ublas::vector3 T;
    void intrinsicMatrix(boost::numeric::ublas::matrix33 &K) const {
        K(0,0) = 1./psx; K(0,1) = 0.; K(0,2) = (width-1)/2.;
        K(1,0) = 0.; K(1,1) = 1./psy; K(1,2) = (height-1)/2.;
        K(2,0) = 0.; K(2,1) = 0.; K(2,2) = 1.;
    };
       
    void extrinsicParameters(boost::numeric::ublas::matrix33 &Rext, boost::numeric::ublas::vector3 &Text) const {
        Text = -T;
        Rext = trans(R);
    }
};

class VLPovCamera2 {
public:
    int width; //< image width in pixels
    int height; //< image height in pixels
    boost::numeric::ublas::vector3 pos;
    boost::numeric::ublas::vector3 dir;
    boost::numeric::ublas::vector3 up;
    boost::numeric::ublas::vector3 right;
    boost::numeric::ublas::vector3 sky;
    boost::numeric::ublas::vector3 lookat;
    boost::numeric::ublas::vector3 fpoint;
    double focal; //< focal length in camera units (normalized to 1 by vlpov_cam_read() )
    double angle;
    
    boost::numeric::ublas::matrix33 K;
    boost::numeric::ublas::matrix33 R;
    boost::numeric::ublas::vector3 t;
};

// a minimalist image class for depth and motion images
template<typename T>
class VLPovImage {
public:
    VLPovImage() : _width(0), _height(0), _data(0) {};
    VLPovImage(int w, int h) : _width(w), _height(w) { _data = new T[w*h];};

    ~VLPovImage() { delete [] _data; };

    // accessors
    const T& operator() (int x, int y) const { return _data[x+y*_width]; }
    int width() const { return _width; }
    int height() const { return _height; }
    const T *data() const { return _data; }

    // modifiers
    T& operator() (int x, int y) { return _data[x+y*_width]; }
    void resize(int w, int h) {
        delete [] _data;
        if (w > 0 && h > 0) {
            _data = new double[w*h];
            _width = w; _height = h;
        } else {
            _width = _height = 0;
            _data = 0;
        }
    }
    void clear() { resize(0,0); }
    int load(const char *filename) {
        if (_width <= 0 || _height <= 0) {
            return -1;
        }
        unsigned int siz = _width*_height;
        std::FILE *const nfile = std::fopen(filename,"rb");
        if (!nfile) {
            return -1;
        }
        std::fseek(nfile,0,SEEK_END);
        const long fsiz = (unsigned int)std::ftell(nfile)/sizeof(T);
        if (fsiz < siz) {
            std::fclose(nfile);
            return -1;
        }
        std::fseek(nfile,0,SEEK_SET);
        size_t nr = std::fread(_data,sizeof(T),siz,nfile);
        std::fclose(nfile);
        if (nr != siz) {
            return -1;
        }
        if (!endianness()) {
            invert_endianness();
        }
        return 0;
    }
    int save(const char *filename) { // should be const, but no because of endianness stuff
        if (_width <= 0 || _height <= 0) {
            return -1;
        }
        unsigned int siz = _width*_height;
        std::FILE *const nfile = std::fopen(filename,"wb");
        if (!nfile) {
            return -1;
        }
        if (!endianness()) {
            invert_endianness();
        }
        size_t nw = std::fwrite(_data,sizeof(T),siz,nfile);
        std::fclose(nfile);
        if (nw != siz) {
            return -1;
        }
        if (!endianness()) {
            invert_endianness();
        }
        return 0;
    }
private:
    VLPovImage( const VLPovImage& ); // noncopyable
        const VLPovImage& operator=( const VLPovImage& ); // noncopyable
    
    // host CPU endianness: return false for "Little Endian", true for "Big Endian".
    bool endianness() { const int x = 1; return ((unsigned char*)&x)[0]?false:true;};
    void invert_endianness() {
        unsigned int size = _width * _height;
        switch (sizeof(T)) {
            case 1:
                break;
            case 2:
                for (unsigned short *ptr = (unsigned short*)_data+size; ptr>(unsigned short*)_data; ) {
                    const unsigned short val = *(--ptr);
                    *ptr = (unsigned short)((val>>8)|((val<<8)));
                }
                break;
            case 4:
                for (unsigned int *ptr = (unsigned int*)_data+size; ptr>(unsigned int*)_data; ) {
                    const unsigned int val = *(--ptr);
                    *ptr = (val>>24)|((val>>8)&0xff00)|((val<<8)&0xff0000)|(val<<24);
                }
                break;
            default:
                for (T* ptr = _data+size; ptr>_data; ) {
                    unsigned char *pb = (unsigned char*)(--ptr), *pe = pb + sizeof(T);
                    for (int i = 0; i<(int)sizeof(T)/2; ++i) std::swap(*(pb++),*(--pe));
                }
                break;
        }
        
    }
    int _width; //< image width in pixels
    int _height; //< image height in pixels
    T *_data;
};

void vlpov_vec2camera(int width,
                      int height,
                      const boost::numeric::ublas::vector3 &pos,
                      const boost::numeric::ublas::vector3 &dir,
                      const boost::numeric::ublas::vector3 &up,
                      const boost::numeric::ublas::vector3 &right,
                      boost::numeric::ublas::matrix33 &K,
                      boost::numeric::ublas::matrix33 &R,
                      boost::numeric::ublas::vector3 &T);

// VEC2MOTION  Convert "look-at" camera specification to matrices
//   [R,T]=VEC2MOTION(POS, DIR, UP) take postion POS, "direction"
//   vector DIR and "up" vector UP and returns camera rotation R
//   and translation T. 
//
//   [R,T] specifies the transformation from the inertial ref. frame to
//   the camera ref. frame.
//
//   See also LOOK2MOTION().
void vlpov_vec2motion(const boost::numeric::ublas::vector3 &pos,
                      const boost::numeric::ublas::vector3 &dir,
                      const boost::numeric::ublas::vector3 &up,
                      boost::numeric::ublas::matrix33 &R,
                      boost::numeric::ublas::vector3 &T);

// LOOK2MOTION  Convert "look-at" camera specification to matrices
//   [R,T]=VEC2MOTION(POS, DIR, UP) take postion POS, "look-at"
//   vector LOOK and "up" vector UP and returns camera rotation R
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
                       boost::numeric::ublas::vector3 &T);


// --------------------------------------------------------------------
// Reads some camera parameters from info file (right_length,
// direction_length, up_length, near and far values)
int vlpov_get_cam(const char* info_file, VLPovCamera &cam);

int vlpov_povread(const char* file, VLPovCamera &cam);
int vlpov_povread(const char* file, VLPovImage<double> &z, VLPovCamera &cam);

// --------------------------------------------------------------------
// Reads some camera parameters from info file (right_length,
// direction_length, up_length, near and far values)
int vlpov_get_cam2(const char* info_file, VLPovCamera2 &cam);

int vlpov_povread2(const char* file, VLPovCamera2 &cam);
int vlpov_povread2(const char* file, VLPovImage<double> &z, VLPovCamera2 &cam);

// Compute the motion field (m1x,m1y) in pixel units from scene with depth z1 seen from camera cam1 to camera cam2
// the dimensions of z1, m1x and m1y are the same as cam1
void vlpov_motionfield(const VLPovCamera &cam1, const VLPovImage<double> &z1, const VLPovCamera &cam2, VLPovImage<double> &m1x, VLPovImage<double> &m1y);
void vlpov_motionfield2(const VLPovCamera2 &cam1, const VLPovImage<double> &z1, const VLPovCamera2 &cam2, VLPovImage<double> &m1x, VLPovImage<double> &m1y);

