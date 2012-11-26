/* CameraX= 0.5 pour droite, -0.5 pour gauche */
#ifndef(CameraX)
#declare CameraX = 0;
#end

/* TextureZ pour modifier la composante en Z de la texture (donc la modifier) */
#ifndef(TextureZ)
#declare TextureZ = 0;
#end

/* Angle pour faire tourner la boule autour d'un axe vertical, de droite a gauche */
#ifndef(AngleX)
#declare AngleX = 0;
#end
#ifndef(AngleY)
#declare AngleY = 0;
#end
#ifndef(AngleZ)
#declare AngleZ = 0;
#end

#ifndef(Translation)
#declare Translation = 0;
#end

#include "colors.inc"

/* process with
  (echo "#declare CameraX = 0.5;";echo "#declare TextureZ = 1;"; cat sphere.pov) | povray +I- +FP +O- +AM2 +A0.01 +D +W512 +H512 +q9
  ppmtopgm left.ppm > left.pgm
  ppmtopgm right.ppm > right.pgm

povray +I sphereleft.pov +FP +Ototo.ppm

*/

#declare White_Marble_Map =
color_map {
    [0.0, 1.0 color rgb <0.1, 0.1, 0.1>
              color rgb <0.9, 0.9, 0.9>]
}

#declare My_Marble_Map =
color_map {
  [0.0 color Black]
  [1.0 color White]
}

#declare My_Marble = 
pigment {
    marble
    turbulence 2
    color_map { My_Marble_Map }
}

#declare My_Texture =
    texture { 
        pigment { My_Marble }
        scale 0.05
        translate <0, 0, TextureZ>
	finish { ambient <1,1,1>}
    }


#declare Plane =
plane { <0, 0, 1>, 0
    texture { My_Texture }
}

#declare Sphere =
sphere { <0, 0, 0>, 0.45
    texture { My_Texture }
}

#declare CutPlane =
plane { <1, 0, 0>, 0
    texture { My_Texture }
}

#declare LeftSphere =
intersection {
  object{Sphere}
  object{CutPlane}
}

#declare RightSphere =
intersection {
  object{Sphere}
  object{CutPlane inverse}
}

#declare Torus =
torus { 1 0.2
    texture { My_Texture }
}

object {
	RightSphere
  	rotate <-AngleX,AngleY,AngleZ>
}
	/*translate <-Translation/200.0, Translation/200.0,Translation/200.0>*/

object {
	LeftSphere
  	rotate <AngleX,AngleY,AngleZ>
}

object {
	Plane
}


camera {
    orthographic
  /* x=0.5: right, x=-0.5: left */
    location <CameraX, 0, -2>
    up <0,1,0>
    right <1,0,0>
    look_at  <0, 0, 0>
}
