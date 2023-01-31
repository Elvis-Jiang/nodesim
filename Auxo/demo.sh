if [ $# -eq 2 ]
then
   ./main  -fpl 16 -width 1732 -dataset ../data/uk2007/newdata.txt -result uk2007.txt -type $1 -cols $2
else
   ./main -fpl 16 -width $4 -dataset ../data/$3/newdata.txt -result $3.txt -type $1 -cols $2
fi

