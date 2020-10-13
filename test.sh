#!/bin/bash
# Global test runner written for CS 537 Programming Assignment 2 (Fall 2020)
# Copyright (C) 2020 Michael Noguera
# Permission to copy and modify is granted under the MIT license
#
# In order to manually run a test, use:
# $ diff <(./prodcomm <input.txt 2>/dev/null) <(bash mock.sh<input.txt)
#
# Output from each test (diffs) are put into the test.log file in the present
# directory. The file is created or overwritten as needed.

rm -f test.log

let "TOTAL=0"
let "OK=0"
let "FAIL=0"

for TEST in tests/*.txt
do
    let "TOTAL+=1" 
    echo -e '\n\n'$TOTAL. $TEST >> test.log

    # we ignore the last line of ./prodcomm's output because of the difficulty
    # in reproducing the newline-before-line-count behavior in our mock
    BUF=$(( diff -I "(STATISTICS.*|Enqueue.*|Dequeue.*)" -c <(./prodcomm <$TEST | head -n -1) <(cat tests/$(basename $TEST .txt).out) &>> test.log) 2>>test.log 1>>test.log)
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