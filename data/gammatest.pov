/*
You know your gamma settings are *wrong* if...

... you see a chrome sphere on a /checkered/ plane (even if you squint 
your eyes or look from a distance) in this image/scene.

Ideally, the plane should appear in a featureless uniform grey (except 
for some moiree effects near the horizon and maybe in the reflection). 
If instead it appears to show squares of alternating shades of grey, 
something's bogus:

(A) If you see checkering in the attached image file, then your image 
viewing software / operating system / graphics drivers / display 
hardware is poorly calibrated, and you should fix it before proceeding.

(B) If (after fixing (A)) you see checkering in the preview window 
and/or the output file when rendering the scene file below, then your 
Display_Gamma (POV-Ray 3.6 and 3.7) and/or File_Gamma setting (POV-Ray 
3.7 only), respectively, is wrong.
*/
// +w640 +h480 +a0.3

#include "colors.inc"

global_settings { #if (version < 3.7) assumed_gamma 1.0 #end }

#local CameraPos   = <0.0,0.0,-4.0>;
#local CameraDepth = 1.0;
#local CameraTilt  = 5;

camera {
   location  <0,0,0>
   direction z*CameraDepth
   right     x*image_width/image_height
   up        y
   translate CameraPos
   rotate    x*CameraTilt
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

light_source {
   <-30,30,-30>
   color rgb 1
   parallel
}

#declare Ratio = image_height*CameraDepth;
#declare FnScanlines = function { abs(mod(abs(Ratio*y/z+0.5),2.0)-1.0) }

plane {
   y, -1
   pigment {
     checker
     pigment { color rgb 0.5 }
     pigment {
       function { FnScanlines(x,y,z) }
       color_map {
         [0.0 color rgb 0.0 ]
         [0.5 color rgb 0.0 ]
         [0.5 color rgb 1.0 ]
         [1.0 color rgb 1.0 ]
       }
       translate CameraPos
       rotate x*CameraTilt
       rotate y*-30
       scale 1/3
     }
     scale 3
     rotate y*30
   }
}

sphere {
   0, 1
   pigment { color rgb 1 }
   finish { ambient 0 diffuse 0 reflection { 1 } }
}

