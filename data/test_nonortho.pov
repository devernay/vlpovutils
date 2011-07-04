#version unofficial MegaPov 1.2 ;

#include "pprocess.inc"

PP_Init_Depth_Output()

global_settings {
  assumed_gamma 1.0
}

#include "ppcontent.inc"


camera{
  location  <0.5, 1.0, 0.0>
  direction 0.8*z+0.1*x
  right     x*image_width/image_height+0.2*y
  up        y
look_at   <0.0, 0.5, 4.0>
angle 77
}
