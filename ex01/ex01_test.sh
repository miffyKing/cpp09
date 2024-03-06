#!/bin/bash
# how to use : bash ex01.test.sh ./ex01.execute
# Define a list of valid test cases as arguments.
VALID_TEST_CASES=(
	"8 9 * 9 - 9 - 9 - 4 - 1 +"
	"7 7 * 7 -"
	"1 2 * 2 / 2 * 2 4 - +"
	"1 2 +"
	"4 3 -"
	"3 8 + 2 *"
	"9 3 - 1 +"
	"2 2 1 + *"
	"6 2 / 3 +"
	"1 4 + 3 /"
	"4 6 * 2 /"
	"1 5 + 2 3 + *"
	"3 3 * 3 /"
	"4 5 + 3 2 - *"
	"2 9 * 3 /"
	"5 5 * 2 2 + /"
	"9 7 - 1 3 + *"
	"1 2 + 3 4 + - "
	# ...add more valid cases here as needed
)

# Define a list of erroneous test cases as arguments.
ERROR_TEST_CASES=(
	"(1 + 1)"
	"(1)"
	"()"
	"a"
	"*"
	"+*"
	"+ *"
	"1 2"
	"-4 5 +"
	"7 +"
	"9 5 - 2"
	"a b +"
	"1 -"
	"2 2 &"
	"23 3 +"
	"3 / 5"
	"8 *"
	"c d +"
	"5 4 !"
	""
	"6 5 4 +"
	"3 3 %"
	"14 2 +"
	"3 -5 +"
	"2 * 7"
	"* 8"
	"e f *"
	"6 9 ("
	"12"
	"7 8 2"
	"4 4 |"
	"28 3 /"
	"-8 2 +"
	"9 + 3"
	"/ 2"
	"g h -"
	"3 7 )"
	"11 11"
	"5 +"
	"3 -"
	"m n +"
	"( 1 2 + 3 4 +)"
	"1 2 + 3 4 + - -"
	"1 2 + 3 4 + | 4"
	# ...add more error cases here as needed
)

# Check if the number of arguments is correct
if [ "$#" -ne 1 ]; then
	echo "Usage: $0 <path_to_executable>"
	exit 1
fi

# Path to the executable
EXECUTABLE="./$1"

# Check if the executable exists in the current directory
if [ ! -f "$EXECUTABLE" ]; then
	echo "Error: Executable $EXECUTABLE does not exist."
	exit 1
fi

echo "Running valid test cases..."
echo "-------"

# Iterate over the valid test cases and pass them to the executable
for TEST in "${VALID_TEST_CASES[@]}"; do
	echo "Test case: $TEST"
	RESULT=$($EXECUTABLE "$TEST")  # Execute the command and capture its output
	echo "Result: $RESULT"
	echo "-------" # For visual separation of test cases
done

echo "Running error test cases..."
echo "-------"

# Iterate over the error test cases and pass them to the executable
for TEST in "${ERROR_TEST_CASES[@]}"; do
	echo "Test case: $TEST"
	RESULT=$($EXECUTABLE "$TEST")  # Execute the command and capture its output
	echo "Result: $RESULT"
	echo "-------" # For visual separation of test cases
done
~