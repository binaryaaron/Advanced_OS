echo "running unit tests"

for i in ./*_test*
do
  if test -f $i
  then 
    ./$i
    if ./$i 2>> tests.log
    then
      echo $i PASS
    else
      echo "ERROR in test $i: Here is the log"
      echo "------"
      tail test.log
      exit 1
    fi
  fi
done
echo ""

