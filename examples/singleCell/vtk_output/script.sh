#!/bin/bash
FILENAME= fluid.pvd
FILELIST=`ls | grep .vti$`
echo "<VTKFile type=\"Collection\" version=\"1.0\" byte_order=\"LittleEndian\" header_type=\"UInt64\">" > $FILENAME
echo "  <Collection>" >> $FILENAME
COUNT=0
for i in $FILELIST
do
  echo "    <DataSet part=\"$COUNT\" file=\"$i\"/>" >> $FILENAME
  ((COUNT+=1))
done
echo "  </Collection>" >> $FILENAME
echo "</VTKFile>" >> $FILENAME
