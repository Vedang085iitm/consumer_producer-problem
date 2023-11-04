#!/bin/bash

# Explanation:
# This script compiles and runs a C program that implements a multi-threaded
# producer-consumer problem. The producer-consumer problem is a classic
# synchronization issue in concurrent programming where multiple threads
# collaborate to produce and consume data from a shared buffer.

# Function to compile and run the program
run_program() {
    # Compile the program
    gcc 1.c -o 1 -pthread

    # Check if the compilation was successful
    if [ $? -eq 0 ]; then
        # Compilation was successful

        # Ask the user for command-line arguments
        read -p "Enter command in this format (e.g., -p <int> -c <int> -s <int> -n <int>): " user_args

        # Run the program with user-provided arguments
        ./1 $user_args
    else
        # Compilation failed
        echo "Compilation failed."
    fi
}

echo "Explanation:"
echo "This script compiles and runs a C program that implements a multi-threaded"
echo "producer-consumer problem. The producer-consumer problem is a classic"
echo "synchronization issue in concurrent programming where multiple threads"
echo "collaborate to produce and consume data from a shared buffer."
echo

# Run the program
run_program

# Ask the user if they want to run the program again
while true; do
    read -p "Do you want to provide another input? (y/n): " choice
    case $choice in
        [Yy]*)
            run_program
            ;;
        [Nn]*)
            echo "Exiting..."
            exit 0
            ;;
        *)
            echo "Please enter 'y' for yes or 'n' for no."
            ;;
    esac
done
