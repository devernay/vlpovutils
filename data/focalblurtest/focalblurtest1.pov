// render with +w512 +h512
global_settings { #if (version < 3.7) assumed_gamma 1.0 #end }
camera
{ perspective
  location <0,0,-10>
  direction <0, 0, 1> // direction_length=0.5*right_length/tan(angle/2) => angle = 53.13 ?
  
  right x // 1x1 aspect ratio
  up y
  aperture 0.5
  blur_samples 200
  variance 0.0
}

light_source
{ <10,20,-20>
  color 1
}

box
{
  < -0.01, -0.01, -5>, <0.01, 0.01, -4.99>
  pigment
  { color <100,100,100>
  }
}
