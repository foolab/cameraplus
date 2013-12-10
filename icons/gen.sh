 cat icons.txt| while read l; do echo $l | sed -e 's/^\([^ ]*\) \([0-9]*\) \([0-9]*\) \(.*\)/inkscape -C -e cameraplus-\1.png -w \2 -h \3 \4/'; done | bash

cat <<EOF  > icons.qrc
<!DOCTYPE RCC>
<RCC version="1.0">
    <qresource prefix="/images">
EOF

for i in *.png; do echo "      <file>"$i"</file>"; done  >> icons.qrc

cat <<EOF  >> icons.qrc
    </qresource>
</RCC>
EOF
