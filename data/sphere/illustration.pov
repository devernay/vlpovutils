/*
  povray +Iillustration.pov +FP +O- +AM2 +A0.01 +D +W512 +H512 +q9  > illustration.ppm
convert -transparent white illustration.ppm illustration.png

 */

#include "colors.inc"


#declare Sphere =
sphere { <0, 0, 0>, 0.45
}

#declare CutPlane =
plane { <1, 0, 0>, 0
}

#declare LeftSphere =
intersection {
  object{Sphere
	pigment {color Red}
         finish {ambient 0.1
                 diffuse 0.9
                 phong 1}
	}
  object{CutPlane}
}

#declare RightSphere =
intersection {
  object{Sphere
	pigment {color Green}
         finish {ambient 0.1
                 diffuse 0.9
                 phong 1}
}

  object{CutPlane inverse}
}

union {

object {
	RightSphere
}

object {
	LeftSphere
}


object {
	union {
	 difference {
	  torus { 0.3 0.01 }
	  box{<0,-0.01,0>,<0.4,0.01,0.4>}
	 }
	 cone{<0,0,0.3>,0.03,<0.05,0,0.3>,0}
	}
	pigment {color Yellow}
         finish {ambient 0.1
                 diffuse 0.9
                 phong 1}
	translate <0,0.5,0>
	rotate <0,120,90>
}

object {
	union {
	 difference {
	  torus { 0.3 0.01 }
	  box{<0,-0.01,0>,<0.4,0.01,0.4>}
	 }
	 cone{<0.3,0,0>,0.03,<0.3,0,0.05>,0}
	}
	pigment {color Yellow}
         finish {ambient 0.1
                 diffuse 0.9
                 phong 1}
	translate <0,-0.5,0>
	rotate <0,60,90>
}

  	rotate <0,0,45>
}

object {
	union {
	 intersection {
	  torus { 0.6 0.01 }
	  box{<0,-0.01,0>,<1,0.01,1>}
	 }
	 cone{<0.6,0,0>,0.03,<0.6,0,-0.05>,0}
	}
	pigment {color Yellow}
         finish {ambient 0.1
                 diffuse 0.9
                 phong 1}
	rotate <-30,-20,100>
}

camera {
    location <-9, 3, -10>
    up <0,0.09,0>
    right <0.09,0,0>
    look_at  <0, 0, 0>
}
light_source { <-2, 4, -3> color White}
background { color White }
