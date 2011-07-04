#version unofficial megapov 1.1;

camera{
  location  <0.0, 0.5, 0.0>
  direction 1.5*z
  right     x*image_width/image_height
  look_at   <0.0, 0.5, 4.0>
  angle 77
}

sky_sphere {
  pigment {
    gradient y
    color_map {
      [0.0 rgb <0.6,0.7,1.0>]
      [0.7 rgb <0.0,0.1,0.8>]
    }
  }
}

light_source { <-30, 30, -30> rgb <1.8, 1.5, 1.2> }

// ----------------------------------------

plane {               // checkered floor
  y, -1
  texture
  {
    pigment {
      checker
      color rgb <0.9, 0.9, 1.0>
      color rgb <0.1, 0.3, 0.7>
      scale 0.5
      rotate 30*y
    }
    finish{
      diffuse 0.7
      ambient 0.1
      brilliance 0
    }
  }
}
  
union {
  sphere { 0.0, 1 }           // reflective sphere
  cylinder { <2, -0.1, -3>, <3, -0.1, 10>, 0.25 }
  plane { z, 5 }
  texture {
    pigment {
      color rgb <0.8,0.8,1.0>
    }
    finish{
      diffuse 0.3
      ambient 0.0
      specular 0.6
      reflection {
        0.8
        metallic
      }
      conserve_energy
    }
  }
  translate <-1.5,0,5>
}
