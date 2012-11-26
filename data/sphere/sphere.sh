#!/bin/sh

QUALITY="+AM2 +A0.01 +D +W512 +H512 +q9"
#QUALITY="+W512 +H512"

# seq FIRST INCREMENT LAST
for t in `seq -f %02g 0 1 50`; do
  (echo "#declare CameraX = 0.5;";echo "#declare AngleX = $t;";echo "#declare AngleY = $t;";echo "#declare AngleZ = $t;";echo "#declare Translation = 0;"; cat sphere.pov) > spheretmp.pov
  povray +Ispheretmp.pov +FP +O- $QUALITY > sphere${t}r.pgm
  convert sphere${t}r.pgm sphere${t}r.png && rm sphere${t}r.pgm
  (echo "#declare CameraX = -0.5;";echo "#declare AngleX = $t;";echo "#declare AngleY = $t;";echo "#declare AngleZ = $t;";echo "#declare Translation = 0;"; cat sphere.pov) > spheretmp.pov
  povray +Ispheretmp.pov +FP +O- $QUALITY > sphere${t}l.pgm
  convert sphere${t}l.pgm sphere${t}l.png && rm sphere${t}l.pgm
done
rm spheretmp.pov
