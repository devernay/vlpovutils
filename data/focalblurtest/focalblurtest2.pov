// render with +w512 +h512
global_settings { #if (version < 3.7) assumed_gamma 1.0 #end }
camera
{ perspective
  location <0,0,-10>
  direction <0, 0, 1> // direction_length=0.5*right_length/tan(angle/2) => angle = 53.13 ?
  
  right x // 1x1 aspect ratio
  up y
}

light_source
{ <10,20,-20>
  color 1
}

box
{
  < -0.5, -0.5, -5>, <0.5, 0.5, -4.99>
  pigment
  { color <1,1,1>
  }
}
