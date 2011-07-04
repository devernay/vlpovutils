#version unofficial MegaPov 1.2 ;

#include "pprocess.inc"

PP_Init_Depth_Output()

global_settings {
  assumed_gamma 1.0
}

#include "ppcontent.inc"

#declare l=clock / 10 ;

camera{
  location <0.0, 0.5, 0.0>
  direction 0.8*z+l*0.5*x  
  right     x*image_width/image_height
  up        y
  // look_at   <0.0, 0.5, 4.0>
  //angle 77
  ////focal_point < -1.5, 0., 5.>    // sphere in focus
  //focal_point (1-l)*cam_start + l*cam_end +5*z // focus at five units distance
  //aperture 0.4     // a nice compromise
  ////  aperture 0.05    // almost everything is in focus
  ////  aperture 1.5     // much blurring
  ////  blur_samples 4       // fewer samples, faster to render
  ////  blur_samples 20      // more samples, higher quality image
  ////  blur_samples 200      // many samples, high quality image
  //blur_samples 100
  //variance 0.0001 // see pov-ray soc on focal blur
}
