#!/bin/bash
# Runs all .txt files found in the /tests folder through `prodcomm`, and compares their
# output to sed
rm test.log

let "TOTAL=0"
let "OK=0"
let "FAIL=0"

for TEST in tests/*.txt
do
    let "TOTAL+=1" 
    echo -e '\n\n'$TOTAL. $TEST >> test.log
    BUF=$((./prodcomm <$TEST >>/dev/null | cmp <(sed '/^.\{4096\}/d' <$TEST | sed 's/ /\*/g' | sed 's/[a-z]/\U&/g') &>> test.log) 2>&1)
    if [ $? -eq 0 ]
    then
        let "OK+=1" 
        printf "\n\e[1;32m%s\e[0m (%-2i) \e[1;4m%s\e[0m\n" "OK  " $TOTAL $(basename $TEST .txt)
        printf "%s" $BUF | sed 's/^/  /'
    else
        let "FAIL+=1" 
        printf "\n\e[1;31m%s\e[0m (%-2i) \e[1;4m%s\e[0m\n" "FAIL" $TOTAL $(basename $TEST .txt)
        printf "%s" $BUF | sed 's/^/  /'
    fi
    echo $BUF >> test.log
done

printf "\n( \e[1;32m%i\e[0m passing | \e[1;31m%i\e[0m failed | \e[1;1m%i\e[0m total )\n" $OK $FAIL $TOTAL
exit 0