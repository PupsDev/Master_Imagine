for i in out/*.pgm;   do name=`echo "$i" | cut -d'.' -f1`;   echo "$name";   ffmpeg -y -hide_banner -loglevel error -i "$i" "${name}.png"; done;rm out/*.pgm;
for i in out/*.ppm;   do name=`echo "$i" | cut -d'.' -f1`;   echo "$name";   ffmpeg -y -hide_banner -loglevel error -i "$i" "${name}.png"; done;rm out/*.ppm

