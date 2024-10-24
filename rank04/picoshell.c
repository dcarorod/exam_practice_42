/*
Write the function

int    picoshell(char **cmds[])

You are given a main function. It converts received arguments into cmds array of strings. When there is a pipe the commands after the pipe are in the next array of strings. You have to create a pipeline using the cmds you receive from the main, and execute them. If there is any error the function should return 1. Close all FFS before returning. If the cmds executed successfully wait all child processes and return 0.

Functions to use: close fork wait, execvp, dup2 pipe

Examples: 
./picoshell /bin/ls "|" grep "picoshell"
./picoshell /bin/ls "|" grep "picoshell"
picoshell
./picoshell echo 'squalalala' | cat "|" sed something
./picoshell echo 'squalalala' | cat "|" sed something
*/
