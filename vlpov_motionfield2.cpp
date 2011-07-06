/*
 *  vlpov_motionfield2.cpp
 *  vlpovutils
 *
 *  Created by Frédéric Devernay on 05/07/11.
 *  Copyright 2011 INRIA. All rights reserved.
 *
 */

#include "vlpov.hpp"

#include <ctime>
#include <string>
#include <limits>
#include <cstdio>
#include <boost/numeric/ublas/io.hpp>

using namespace std;
using namespace boost::numeric::ublas;

/* Basic TIFF handling, from dcraw.c */
struct tiff_tag {
    uint16_t tag, type;
    int32_t count;
    union { int16_t s0; int32_t i0; } val;
};

struct tiff_hdr {
    uint16_t order, magic;
    int32_t ifd;
    uint16_t pad, ntag;
    struct tiff_tag tag[22];
    //int32_t nextifd;
    //uint16_t pad2;
    //uint16_t nexif;
    //struct tiff_tag exif[4];
    int16_t bps[4];
    //int32_t rat[10];
    char desc[512];
    char make[64];
    char model[64];
    char soft[32];
    char date[20];
    //char artist[64];
    //int16_t cfarepeatpatterndim[2];
    //char cfapattern[4];
    //char dngversion[4];
    //char dngbackwardversion[4];
};

static void tiff_set (uint16_t *ntag,
                      uint16_t tag, uint16_t type, int count, int val)
{
    struct tiff_tag *tt;
    
    tt = (struct tiff_tag *)(ntag+1) + (*ntag)++;
    tt->tag = tag;
    tt->type = type;
    tt->count = count;
    if (type == 3 && count == 1)          /* SHORT */
        tt->val.s0 = val;
    else
        tt->val.i0 = val;                   /* other */
}
#define TOFF(ptr) ((int)((char *)(&(ptr)) - (char *)th))

static void tiff_head (struct tiff_hdr *th, int width, int height, int colors, int bytes_per_sample, bool isfloat, const char *desc)
{
    int c, psize=0;
    struct tm *t;
    int output_bps = bytes_per_sample*8;
    time_t timestamp = time(NULL);
    
    memset (th, 0, sizeof *th);
    th->order = (uint16_t)(htonl(0x4d4d4949) >> 16);
    th->magic = 42;
    th->ifd = 10;
    tiff_set (&th->ntag, 254, 4, 1, 0); /* SUBFILETYPE=0 */
    tiff_set (&th->ntag, 256, 4, 1, width); /* IMAGEWIDTH */
    tiff_set (&th->ntag, 257, 4, 1, height);  /* IMAGELENGTH */
    tiff_set (&th->ntag, 258, 3, colors, output_bps);  /* BITSPERSAMPLE */
    if (colors > 2) {
        th->tag[th->ntag-1].val.i0 = TOFF(th->bps);
    }
    for (c=0; c < 4; c++) {
        th->bps[c] = output_bps;
    }
    tiff_set (&th->ntag, 259, 3, 1, 1); /* COMPRESSION=NONE */
    tiff_set (&th->ntag, 262, 3, 1, 1 + (colors > 1)); /* PHOTOMETRIC= MINISBLACK or RGB (CFA is 32803, LINEAR RAW is 34892) */ 
    if (desc) {
        tiff_set (&th->ntag, 270, 2, 512, TOFF(th->desc));  /* IMAGEDESCRIPTION */
    }
    tiff_set (&th->ntag, 271, 2, 64, TOFF(th->make));  /* MAKE */
    tiff_set (&th->ntag, 272, 2, 64, TOFF(th->model));  /* MODEL */
    //if (full) {
    //if (oprof) psize = ntohl(oprof[0]);
    tiff_set (&th->ntag, 273, 4, 1, sizeof *th + psize);  /* STRIPOFFSETS */
    tiff_set (&th->ntag, 277, 3, 1, colors);  /* SAMPLESPERPIXEL */
    tiff_set (&th->ntag, 278, 4, 1, height);  /* ROWSPERSTRIP */
    tiff_set (&th->ntag, 279, 4, 1, height*width*colors*output_bps/8);  /* STRIPBYTECOUNTS */
    //} else
    //  tiff_set (&th->ntag, 274, 3, 1, 1);  /* ORIENTATION=TOPLEFT */
    //tiff_set (&th->ntag, 282, 5, 1, TOFF(th->rat[6]));  /* XRESOLUTION */
    //tiff_set (&th->ntag, 283, 5, 1, TOFF(th->rat[8]));  /* YRESOLUTION */
    tiff_set (&th->ntag, 284, 3, 1, 1);   /* PLANARCONFIG=CONTIG */
    //tiff_set (&th->ntag, 296, 3, 1, 2);   /* RESOLUTIONUNIT=INCH */
    tiff_set (&th->ntag, 305, 2, 32, TOFF(th->soft));  /* SOFTWARE */
    tiff_set (&th->ntag, 306, 2, 20, TOFF(th->date));  /* DATETIME */
    //tiff_set (&th->ntag, 315, 2, 64, TOFF(th->artist));  /* ARTIST */
    tiff_set (&th->ntag, 339, 3, 1, isfloat ? 3 : 1);  /* SAMPLEFORMAT (3: float 1: uint) */
    //tiff_set (&th->ntag, 34665, 4, 1, TOFF(th->nexif));  /* EXIFIFD */
    //if (psize) tiff_set (&th->ntag, 34675, 7, psize, sizeof *th); /* ICCPROFILE */
    //tiff_set (&th->nexif, 33434, 5, 1, TOFF(th->rat[0]));
    //tiff_set (&th->nexif, 33437, 5, 1, TOFF(th->rat[2]));
    //tiff_set (&th->nexif, 34855, 3, 1, iso_speed);
    //tiff_set (&th->nexif, 37386, 5, 1, TOFF(th->rat[4]));
    //for (c=0; c < 6; c++) th->rat[c] = 1000000;
    //th->rat[0] *= shutter;
    //th->rat[2] *= aperture;
    //th->rat[4] *= focal_len;
    //th->rat[6] = th->rat[8] = 300;
    //th->rat[7] = th->rat[9] = 1;
    if (desc) {
        strncpy (th->desc, desc, 512);
    }
    strncpy (th->make, "INRIA", 64); /* TODO: use Blinky vendor */
    strncpy (th->model, "vlpovutils", 64); /* TODO: use Blinky model */
    
    /* Adobe DNG support:
     - for RAW data with PHOTOMETRIC=CFA
     - for linear data with PHOTOMETRIC=LINEARRAW)
     References:
     http://www.adobe.com/products/dng/pdfs/dng_spec.pdf
     http://www.cybercom.net/~dcoffin/dcraw/elphel_dng.c
     http://www.cybercom.net/~dcoffin/dcraw/libtiff.patch
     */
    //tiff_set (&th->ntag, 33421, 3, 2, TOFF(th->cfarepeatpatterndim) );  /* CFAREPEATPATTERNDIM=2,2 */
    //th->cfarepeatpatterndim[0] = 2;
    //th->cfarepeatpatterndim[1] = 2;
    //tiff_set (&th->ntag, 33422, 1, 4, TOFF(th->cfapattern)); /* CFAPATTERN */
    //th->cfapattern[0] = 1;                /* G */
    //th->cfapattern[1] = 0;                /* R */
    //th->cfapattern[2] = 2;                /* B */
    //th->cfapattern[3] = 1;                /* G */  
    //tiff_set (&th->ntag, 50706, 1, 4,  TOFF(th->dngversion));  /* DNGVERSION=1,1,0,0 */
    //th->dngversion[0] = 1;
    //th->dngversion[1] = 1;
    //th->dngversion[2] = 0;
    //th->dngversion[3] = 0;
    //tiff_set (&th->ntag, 50708, 2, 128, TOFF(th->cameramodel));  /* TODO: use Blinky vendor+model */
    //tiff_set (&th->ntag, 50717, 3, 1, (int16_t)(0xffff<<(16-buffer->header.bpp)));  /* WHITELEVEL=max image level */
    
    
    strcpy (th->soft, "vlpov_motionfield2");
    t = gmtime (&timestamp);
    sprintf (th->date, "%04d:%02d:%02d %02d:%02d:%02d",
             t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
    //strncpy (th->artist, artist, 64);
}

static void tiff_save(const VLPovImage<double> &img, const char *fname, const char *desc)
{
    // convert to float
    float *data = new float[img.width()*img.height()];
    std::copy(img.data(), img.data()+img.width()*img.height(), data);
    //for (int i=0; i<img.width()*img.height(); ++i) {
    //    data[i] = img.data()[i];
    //}
    struct tiff_hdr th;
    tiff_head(&th, img.width(), img.height(), /*colors=*/1, /*bytes_per_sample=*/sizeof(*data), /*isfloat=*/true, desc);
    FILE *f = fopen(fname, "wb");
    if(f == NULL) {
        fprintf(stderr, "Error: cannot open file '%s' for writing\n", fname);
        return;
    }
    fwrite (&th, sizeof th, 1, f);
    fwrite(data, img.width()*img.height()*sizeof(*data), 1, f);
    fclose(f);
    delete [] data;
}
    
static void tiff_save(const VLPovImage<unsigned char> &img, const char *fname, const char *desc)
{
    struct tiff_hdr th;
    tiff_head(&th, img.width(), img.height(), /*colors=*/1, /*bytes_per_sample=*/sizeof(img(0,0)), /*isfloat=*/false, desc);
    FILE *f = fopen(fname, "wb");
    if(f == NULL) {
        fprintf(stderr, "Error: cannot open file '%s' for writing\n", fname);
        return;
    }
    fwrite (&th, sizeof th, 1, f);
    fwrite(img.data(), img.width()*img.height()*sizeof(img(0,0)), 1, f);
    fclose(f);                          
}

#define USAGE "Usage: %s <frame1> <frame2>\n"\
"Help: Compute a motion field from a POV-Ray rendered depth map to another camera\n"\
"Arguments:\n"\
"<frame1> first frame basename (files with extensions .depth and .txt will be \n"\
"         read)\n"\
"<frame2> second frame basename (files with extensions .depth and .txt will be \n"\
"         read)\n"\
"Output files (all are in TIFF format):\n"\
"<frame1>.<frame2>.mx.tif motion field, x-component (1-channel float)\n"\
"<frame1>.<frame2>.my.tif motion field, y-component (1-channel float)\n"\
"<frame1>.<frame2>.occ.tif occlusion map (1-channel uchar, 255=visible, 0=occluded)\n"\
"<frame2>.<frame1>.mx.tif motion field, x-component (1-channel float)\n"\
"<frame2>.<frame1>.my.tif motion field, y-component (1-channel float)\n"\
"<frame2>.<frame1>.occ.tif occlusion map (1-channel uchar, 255=visible, 0=occluded)\n"


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
    const char *frame1_basename = mybasename(frame1);
    const char *frame2_basename = mybasename(frame2);
    
    {
        VLPovImage<double> z1;
        VLPovCamera cam1;
        VLPovImage<double> z2;
        VLPovCamera cam2;
        int retval;
        
        retval = vlpov_povread(frame1, z1, cam1);
        if(retval != 0) {
            fprintf(stderr,"Error %d while reading %s.{depth,txt}\n", retval, frame1);
            exit(1);
        }
        
        //cout << "Intrinsic matrix 1(new): " << endl << cam1.K << endl;
        //cout << "Extrinsic 1(new): " << endl << cam1.R << endl << cam1.t << endl;
        //z1.save((std::string(frame1)+".znew").c_str());
        
        retval = vlpov_povread(frame2, z2, cam2);
        if(retval != 0) {
            fprintf(stderr,"Error %d while reading %s.{depth,txt}\n", retval, frame2);
            exit(1);
        }
        
        VLPovImage<double> m1x, m1y;
        vlpov_motionfield(cam1, z1, cam2, m1x, m1y);
        
        {
            std::string m1x_name = std::string(frame1)+"."+std::string(frame2_basename)+".mx.tif";
            std::string m1x_desc = "X-component of motion field from "+std::string(frame1_basename)+" to "+std::string(frame2_basename);
            tiff_save(m1x, m1x_name.c_str(), m1x_desc.c_str());
        }
        {
            std::string m1y_name = std::string(frame1)+"."+std::string(frame2_basename)+".my.tif";
            std::string m1y_desc = "Y-component of motion field from "+std::string(frame1_basename)+" to "+std::string(frame2_basename);
            tiff_save(m1y, m1y_name.c_str(), m1y_desc.c_str());
        }
        
        VLPovImage<double> m2x, m2y;
        vlpov_motionfield(cam2, z2, cam1, m2x, m2y);
        
        {
            std::string m2x_name = std::string(frame2)+"."+std::string(frame1_basename)+".mx.tif";
            std::string m2x_desc = "X-component of motion field from "+std::string(frame2_basename)+" to "+std::string(frame1_basename);
            tiff_save(m2x, m2x_name.c_str(), m2x_desc.c_str());
        }
        {
            std::string m2y_name = std::string(frame2)+"."+std::string(frame1_basename)+".my.tif";
            std::string m2y_desc = "Y-component of motion field from "+std::string(frame2_basename)+" to "+std::string(frame1_basename);
            tiff_save(m2y, m2y_name.c_str(), m2y_desc.c_str());
        }
        
        VLPovImage<unsigned char> occ1, occ2;
        vlpov_occlusionmap_opticalflow(m1x, m1y, m2x, m2y, 1., occ1);
        {
            std::string occ1_name = std::string(frame1)+"."+std::string(frame2_basename)+".occ.tif";
            std::string occ1_desc = "Occlusion map from "+std::string(frame1_basename)+" to "+std::string(frame2_basename);
            tiff_save(occ1, occ1_name.c_str(), occ1_desc.c_str());
        }
        vlpov_occlusionmap_opticalflow(m2x, m2y, m1x, m1y, 1., occ2);
        {
            std::string occ2_name = std::string(frame2)+"."+std::string(frame1_basename)+".occ.tif";
            std::string occ2_desc = "Occlusion map from "+std::string(frame2_basename)+" to "+std::string(frame1_basename);
            tiff_save(occ2, occ2_name.c_str(), occ2_desc.c_str());
        }
        
    }
    
    return 0;
}
