# Tideman Voting Algorithm

## Overview

This project implements the Tideman voting algorithm, a ranked-choice voting system, in C. The Tideman algorithm determines the winner of an election by considering the preferences (votes) of each voter.
It avoids the problems of the simple plurality voting system by allowing voters to express their preferences more fully.

The program takes in candidate names, collects votes from voters, calculates the preferences, creates pairs of candidates based on preferences, sorts these pairs, and then locks in pairs in a way that avoids cycles in the preference graph.
Finally, it prints out the winner(s) of the election. The project is a part of CS50x course.

## Usage

To use the program, compile the source code (`tideman.c`) using a C compiler such as `gcc`. Here's an example compilation command:
```
gcc -o tideman tideman.c
```
After compilation, you can run the program with the following command:

```
./tideman [candidate1] [candidate2] ...
```

Replace `[candidate1]`, `[candidate2]`, etc., with the names of the candidates participating in the election.

The program will prompt you to enter the number of voters and then proceed to collect their votes. Follow the instructions provided by the program to input each voter's preferences.

Once all votes are collected, the program will calculate the winner(s) of the election based on the Tideman algorithm and print the result.


