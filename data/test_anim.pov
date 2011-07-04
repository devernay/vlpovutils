#version unofficial MegaPov 1.2 ;

#include "pprocess.inc"

PP_Init_Depth_Output()

global_settings {
  assumed_gamma 1.0
}

#include "ppcontent.inc"

#declare cam_start=<0.0, 0.5, 0.0> ;
#declare cam_end=<0.0,1.0,1.0> ;
#declare l=clock / 10 ;

camera{
  location  (1-l)*cam_start + l*cam_end
  direction 1.5*z
  right     x*image_width/image_height
  look_at   <0.0, 0.5, 4.0>
  angle 77
}
