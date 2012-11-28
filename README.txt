VLPovUtils 1.0
by Frederic Devernay <frederic.devernay@inria.fr>
(c) INRIA 2012

This software allows the correct extraction of the camera parameters from POV-Ray images
generated using the MegaPOV with the "annotation" patch from VLPov:
- VLPov, a set of utilities to use the POV-Ray raytracer in order to generate and analyze
  Computer Vision datasets. http://www.vlfeat.org/~vedaldi/code/vlpovy.html

Using this software and a vlpov-patched POVRay, you can for example:
- generate ground truth optical flow from camera motion with ray-traced images
- generate ground truth disparity maps

This library also handles POV-Ray renders where the up, right and dir vectors of the
POV-Ray camera don't form an orthonormal set. This allows for cameras with:
- the principal point not at the image center (very useful for stereoscopic rendering)
- non-orthogonal x and y axes

In order to render these images, the Vista buffer feature of POV-Ray must be disabled
(option -UV in megapov).

Please check out the following documentation on how to create
stereoscopic pairs with POV-Ray:
- "Creating stereoscopic left-right image pairs with POVRay" http://www.triplespark.net/render/stereo/create.html
- "Paul Bourke on PovRay" http://paulbourke.net/exhibition/vpac/povray.html

*** Generating data with MegaPOV and the annotation patch (aka VLPov)

Here is how to generate some individual frames from the sample scenes:

mkdir results; cd results
megapov +w320 +h240 +a0.0 +j0.0 +L../data +Itest.pov +Otest.png

An animation with 30 frames and a forward motion:

megapov +w320 +h240 +a0.0 +j0.0 +L../data +KFI1 +KFF30 +KI0.0 +KF3.0 +Itest_anim.pov +Otest_anim.png

A camera with non-orthogonal principal axes:

megapov -UV +w320 +h240 +a0.0 +j0.0 +L../data +Itest_nonortho.pov +Otest_nonortho.png

A multi-viewpoint stereo set, with 10 viewpoints (output images are rectified):

megapov -UV +w320 +h240 +a0.0 +j0.0 +L../data +KFI1 +KFF10 +KI0.0 +KF1.0 +Itest_stereo.pov +Otest_stereo.png

*** Computing motion fields (optical flow) and disparity maps

Suppose you generated frame1.png and frame2.png with megapov, together with the files created by the annotation patch (frame1.depth, frame1.txt, frame2.depth, frame2.txt).

There are 3 utilities: vlpov_project, vlpov_motionfield, vlpov_motionfield2. The description of each utility is described below.

* vlpov_poject

Usage: vlpov_project <frame1> [<frame2>]
Help: Compute the projection (x, y, depth) of 3D points in <frame1> and
      optionally pixel motion to <frame2>.
      3D points coordinates are read from standard input.
Arguments:
<frame1> base frame basename (file with extension .txt will be read)
<frame2> second frame basename (file with extension .txt will be read)

*Important note*: The 3D points must be given in a right-handed coordinate
                  system, where the Z is the *opposite* of POV-Ray's Z.

* vlpov_motionfield

Usage: vlpov_motionfield <frame1> <frame2>
Help: Compute a motion field from a POV-Ray rendered depth map to another camera
      The motion field is written to <frame1>.<frame2>.mx and
      and <frame1>.<frame2>.my as raw images containing big-endian
     (network-order) doubles - the same format as the depth map.
Arguments:
<frame1> first frame basename (files with extensions .depth and .txt will be 
         read)
<frame2> second frame basename (file with extension .txt will be read)
Output files:
<frame1>.<frame2>.mx motion field, x-component (raw big-endian doubles)
<frame1>.<frame2>.my motion field, y-component (raw big-endian doublest)

* vlpov_motionfield2

Usage: vlpov_motionfield2 <frame1> <frame2>
Help: Compute a motion field from a POV-Ray rendered depth map to another camera
Arguments:
<frame1> first frame basename (files with extensions .depth and .txt will be 
         read)
<frame2> second frame basename (files with extensions .depth and .txt will be 
         read)
Output files (all are in TIFF format):
<frame1>.<frame2>.mx.tif motion field, x-component (1-channel float)
<frame1>.<frame2>.my.tif motion field, y-component (1-channel float)
<frame1>.<frame2>.occ.tif occlusion map (1-channel uchar, 255=visible, 0=occluded)
<frame2>.<frame1>.mx.tif motion field, x-component (1-channel float)
<frame2>.<frame1>.my.tif motion field, y-component (1-channel float)
<frame2>.<frame1>.occ.tif occlusion map (1-channel uchar, 255=visible, 0=occluded)

*** Library (libvlpov)

All the functionalities are also directly accessible from C or C++ using libvlpov.
See the library header (vlpov.hpp) for the documentation.
The library replicates some of the functionalities of the Matlab files distributed with VLPov
<http://www.vlfeat.org/~vedaldi/code/vlpovy.html>.

*** Other POV-Ray scenes

Realistic POV-Ray scenes with source files can be found at various places.

Good starting points are:
http://www.ignorancia.org
http://hof.povray.org

The following is based on the Patio scene by Jaime Piquerez, with various settings of focal blur:
http://devernay.free.fr/vision/focus/patio/
