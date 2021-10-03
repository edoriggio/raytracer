#!/bin/bash

cd ./out

frames=1

_start=1
_end=100

function ProgressBar {
  let _progress=(${1}*100/${2}*100)/100
  let _done=(${_progress}*4)/10
  let _left=40-$_done

  _fill=$(printf "%${_done}s")
  _empty=$(printf "%${_left}s")

  printf "\rProgress : [${_fill// /#}${_empty// /-}] ${_progress}%%"
}

for i in *.ppm;
  do name=${i%.ppm}
  ../bin/ffmpeg -i "$i" "${name}.png" -hide_banner -loglevel error
  rm "$name.ppm"

  let percent=($frames*100)/120
  ProgressBar ${percent} ${_end}

  (( frames++ ))
done

echo

../bin/ffmpeg -r 60 -s 1024x768 -i frame_%d.png -vcodec libx264 -crf 15 ../../artifacts/assignment\ 2/result.mp4 -hide_banner -loglevel error
rm ./*.png

cd ../
make clean
