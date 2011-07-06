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

Here is how to generate some individual frames from the sample scenes:

mkdir results; cd results
megapov +w320 +h240 +a0.0 +j0.0 +L../data +Itest.pov +Otest.png

megapov +w320 +h240 +a0.0 +j0.0 +L../data +KFI1 +KFF30 +KI0.0 +KF3.0 +Itest_anim.pov +Otest_anim.png

megapov -UV +w320 +h240 +a0.0 +j0.0 +L../data +Itest_nonortho.pov +Otest_nonortho.png
megapov -UV +w320 +h240 +a0.0 +j0.0 +L../data +KFI1 +KFF10 +KI0.0 +KF1.0 +Itest_stereo.pov +Otest_stereo.png

*** POV-Ray scenes

You can get more POV-Ray scenes from:

** patio

http://www.ignorancia.org/en/index.php?page=Patio

requires:
- http://www.ignorancia.org/en/index.php?page=Lightsys
- http://www.aust-manufaktur.de/tomtree.zip

apply patch patio.patch found in the data dir:

(cd ../data; patch -p0 -d. < patio.patch)

megapov +Q9 -UV +w960 +h540 +a0.0 +j0.0 +L../data +L../data/patio +L../data/patio/maps +L../data/LightsysIV +L../data/tomtree +K0.0 +Ipatio_stereo.pov +Opatio_stereo1.png&
megapov +Q9 -UV +w960 +h540 +a0.0 +j0.0 +L../data +L../data/patio +L../data/patio/maps +L../data/LightsysIV +L../data/tomtree +K1.0 +Ipatio_stereo.pov +Opatio_stereo2.png&
wait
../build/Release/vlpov_motionfield2 patio_stereo1 patio_stereo2

** other scenes

http://www.ignorancia.org/en/index.php?page=Childhood
http://www.ignorancia.org/en/index.php?page=Gardens
http://www.ignorancia.org/en/index.php?page=The_office
http://www.ignorancia.org/en/index.php?page=IRTC_entries
http://hof.povray.org/
