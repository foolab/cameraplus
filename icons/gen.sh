 cat icons.txt| while read l; do echo $l | sed -e 's/^\([^ ]*\) \([0-9]*\) \([0-9]*\) \(.*\)/inkscape -C -e cameraplus-\1.png -w \2 -h \3 \4/'; done | bash
