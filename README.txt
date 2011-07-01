This library is a C implementation of the functionalities found in the Matlab parts of:
- VLPov, a set of utilities to use the POV-Ray raytracer in order to generate and analyze
  Computer Vision datasets. http://www.vlfeat.org/~vedaldi/code/vlpovy.html
- CVPoV is a computational tool that finds point correspondence between synthetic images
  of the same scene under different viewing configurations http://ranger.uta.edu/~guerra/CVPoV.html
  CVPov uses the POVRay/MegaPOV patch from VLPov

Using this library and a vlpov-patched POVRay, you can for example:
- generate ground truth optical flow from camera motion with ray-traced images
- generate ground truth disparity maps
