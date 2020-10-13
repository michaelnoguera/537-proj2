#!/bin/bash
# Mock of the program used for testing
# by Michael Noguera and Julien de Castelnau

sed -r  "/.{4096,}/d" | sed 's/ /\*/g' | sed 's/[a-z]/\U&/g'# | tee >(wc -l)
# the word count does not include the newline at the end of the last line of
# the output, so we created .out files for each test in the /tests directory