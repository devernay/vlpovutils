//#version 3.5;
#version unofficial MegaPov 1.2 ;

#include "pprocess.inc"

PP_Init_Depth_Output()

#include "patio.pov"

// the following values were gathered from the txt file with the vlpov patch
// Fountain is at depth 550
// The bush on the right is at depth 320
//#declare cam_pos=<190,165,-550>;
#declare cam_pos=<200,165,-550>;
#declare cam_dir=<-0.8162898,0.01288879,2.362944>;
#declare cam_up=<0.004040112,2.399968,-0.01169506>;
#declare cam_right=<3.024609,0,1.044865>;

#declare dir_right=vnormalize(cam_right);
//#declare convergence_dist=550;
#declare convergence_dist=320;
#declare half_interocular=4;

#declare openbook_pos=<20,45.5,245>;
// fountain is at (926,874) Z=584 d=16.96
#declare fountain_pos=<0,-6,0>;
// tree_backleft is at (661,769) Z=927 d=24.56
#declare tree_backleft_pos=<-270,-20, 270>;
#declare tree_backright_pos=< 270,-10, 270>;
#declare tree_frontleft_pos=<-270,-10,-270>;
#declare tree_frontright_pos=< 270,-10,-270>;

camera{
  location cam_pos+half_interocular*dir_right*(clock*2-1)
  direction cam_dir-vlength(cam_dir)*dir_right*half_interocular/convergence_dist*(clock*2-1)
  up        cam_up
  right     cam_right
  // look_at   <0.0, 0.5, 4.0>
  //angle 77
  // focal_point fountain_pos
  // focal_point tree_backleft_pos
  ////aperture 0.4     // a nice compromise
  ////  aperture 0.05    // almost everything is in focus
  //aperture 10     // much blurring
  ////  blur_samples 4       // fewer samples, faster to render
  ////  blur_samples 20      // more samples, higher quality image
  ////  blur_samples 200      // many samples, high quality image
  //blur_samples 100
  //variance 0.0001 // see pov-ray soc on focal blur
}
