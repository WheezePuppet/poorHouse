
if [ $# != 1 ]; then
    echo "Usage splitFile.sh [filename]."
    exit 1
fi

base=`basename $1 .csv`
egrep -e ".*,.*,.*,.*" $1 > ${base}_agents.csv
egrep -v ".*,.*,.*,.*" $1 > ${base}_stats.csv
