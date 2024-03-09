#!/bin/bash
# how to use
# step1 : ./PmergeMe `jot -r 3000 1 10000 | tr '\n' ' '` > output.txt
# step2 : erase all strings without numbers in output.txt
# step3 : bash ex02_test.sh output.txt
# Function to check if an array of numbers is sorted
check_sorted() {
    # Read numbers into an array
    local numbers=($1)
    local prev_number=${numbers[0]}

    for number in "${numbers[@]:1}"; do
        if (( prev_number > number )); then
            echo "Not sorted."
            exit 1
        fi
        prev_number=$number
    done

    echo "All sorted."
}

# Check if file path is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

filename=$1

# Check if the file exists
if [ ! -f "$filename" ]; then
    echo "Error: File '$filename' not found."
    exit 1
fi

# Read the file content
file_content=$(<"$filename")

# Call the function and pass the file content
check_sorted "$file_content"