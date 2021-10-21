#!/bin/bash

frames=1
angle=0

_start=1
_end=100

mkdir -p ./out
g++ main.cpp -w

function ProgressBar {
  let _progress=(${1}*100/${2}*100)/100
  let _done=(${_progress}*4)/10
  let _left=40-$_done

  _fill=$(printf "%${_done}s")
  _empty=$(printf "%${_left}s")

  printf "\rProgress : [${_fill// /#}${_empty// /-}] ${_progress}%%"
}

while [ $angle -le 360 ]
do
  x=`python3 -c "from math import *; print(0 + (cos(radians($angle)) * 5))"`
  y=`python3 -c "from math import *; print(7 + (sin(radians($angle)) * 5))"`
  ./a.out "./out/frame_$frames.ppm" $x $y

  let percent=($frames*100)/120
  ProgressBar ${percent} ${_end}
  
  (( angle = angle + 3 ))
  (( frames++ ))
done

echo
