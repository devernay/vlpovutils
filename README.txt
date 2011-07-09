This library allows the correct extraction of the camera parameters from POV-Ray images
generated using the VLPov patch:
- VLPov, a set of utilities to use the POV-Ray raytracer in order to generate and analyze
  Computer Vision datasets. http://www.vlfeat.org/~vedaldi/code/vlpovy.html

Using this library and a vlpov-patched POVRay, you can for example:
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

Here is how to generate some individual frames from the sample scenes:

mkdir results; cd results
megapov +w320 +h240 +a0.0 +j0.0 +L../data +Itest.pov +Otest.png

megapov +w320 +h240 +a0.0 +j0.0 +L../data +KFI1 +KFF30 +KI0.0 +KF3.0 +Itest_anim.pov +Otest_anim.png

megapov -UV +w320 +h240 +a0.0 +j0.0 +L../data +Itest_nonortho.pov +Otest_nonortho.png
megapov -UV +w320 +h240 +a0.0 +j0.0 +L../data +KFI1 +KFF10 +KI0.0 +KF1.0 +Itest_stereo.pov +Otest_stereo.png

*** POV-Ray scenes

You can get more POV-Ray scenes from:

** patio

* getting it

http://www.ignorancia.org/en/index.php?page=Patio

requires:
- http://www.ignorancia.org/en/index.php?page=Lightsys
- http://www.aust-manufaktur.de/tomtree.zip

apply patch patio.patch found in the data dir:

(cd ../data; patch -p0 -d. < patio.patch)

* radiosity

in order to apply radiosity:
change in patio/patio.pov the line
#declare usar_rad=0;
to
#declare usar_rad=2;

then, run one rendering. This will save a radiosity file "patio.rad" in the current directory.
for subsequent renderings *from the same point of view*, change the line to
#declare usar_rad=1;
This will load the radiosity file and thus save computations.

* rendering: keep the 4/3 aspect ratio!

Note: antialiasing should be used if there is no focal blur (remove the options +A0.0 +J0.0, which mean perform 3x3 antialiasing on every pixel)

megapov +Q9 -UV +w1152 +h864 +L../data +L../data/patio +L../data/patio/maps +L../data/LightsysIV +L../data/tomtree +K0.0 +Ipatio_stereo.pov +Opatio_stereo1.png
megapov +Q9 -UV +w1152 +h864 +L../data +L../data/patio +L../data/patio/maps +L../data/LightsysIV +L../data/tomtree +K0.0 +Ipatio_stereo_far.pov +Opatio_stereo1_far.png
megapov +Q9 -UV +w1152 +h864 +L../data +L../data/patio +L../data/patio/maps +L../data/LightsysIV +L../data/tomtree +K0.0 +Ipatio_stereo_near.pov +Opatio_stereo1_near.png
megapov +Q9 -UV +w1152 +h864 +L../data +L../data/patio +L../data/patio/maps +L../data/LightsysIV +L../data/tomtree +K0.0 +Ipatio_stereo_near2.pov +Opatio_stereo1_near2.png
megapov +Q9 -UV +w1152 +h864 +A0.0 +J0.0 +L../data +L../data/patio +L../data/patio/maps +L../data/LightsysIV +L../data/tomtree +K0.0 +Ipatio_stereo.pov +Opatio_stereo1_all.png
megapov +Q9 -UV +w1152 +h864 +L../data +L../data/patio +L../data/patio/maps +L../data/LightsysIV +L../data/tomtree +K1.0 +Ipatio_stereo.pov +Opatio_stereo2.png
megapov +Q9 -UV +w1152 +h864 +L../data +L../data/patio +L../data/patio/maps +L../data/LightsysIV +L../data/tomtree +K1.0 +Ipatio_stereo_far.pov +Opatio_stereo2_far.png
megapov +Q9 -UV +w1152 +h864 +L../data +L../data/patio +L../data/patio/maps +L../data/LightsysIV +L../data/tomtree +K1.0 +Ipatio_stereo_near.pov +Opatio_stereo2_near.png
megapov +Q9 -UV +w1152 +h864 +L../data +L../data/patio +L../data/patio/maps +L../data/LightsysIV +L../data/tomtree +K1.0 +Ipatio_stereo_near2.pov +Opatio_stereo2_near2.png
megapov +Q9 -UV +w1152 +h864 +A0.0 +J0.0 +L../data +L../data/patio +L../data/patio/maps +L../data/LightsysIV +L../data/tomtree +K1.0 +Ipatio_stereo.pov +Opatio_stereo2_all.png
wait
../build/Release/vlpov_motionfield2 patio_stereo1_all patio_stereo2_all

 (echo -270 -20 -270 && echo 0 -6 0) | ../../../build/Release/vlpov_project patio_stereo1_all patio_stereo2_all

** other scenes

http://www.ignorancia.org/en/index.php?page=Childhood
http://www.ignorancia.org/en/index.php?page=Gardens
http://www.ignorancia.org/en/index.php?page=The_office
http://www.ignorancia.org/en/index.php?page=IRTC_entries
http://hof.povray.org/
