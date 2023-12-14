#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <stdint.h>
#include "helperTokenize.h"


void help_buildIn() {
  
  printf("Project 1: Shell - Task 2.1: Built-in Commands - Help\n");
  printf("cd: This command should change the current working directory of the shell to the path specified as the argument. Tip: You can check what the current working directory is using the pwd command (not a built-in).\n");
  printf("source: Execute a script. Takes a filename as an argument and processes each line of the file as a command, including built-ins. In other words, each line should be processed as if it was entered by the user at the prompt.\n");
  printf("prev: Prints the previous command line and executes it again, without becomming the new command line. You do not have to support combining prev with other commands on a command line.\n");
  printf("help: Explains all the built-in commands available in your shell\n\n");

}

void shell() {
  printf("Welcome to mini-shell\n");
  char prev[256] = "";
  while (1) {
    printf("shell $ ");
    char args[256];
    char* temp[256];

    // Ctrl-D (end of file)
    char *isNull = fgets(args, 256, stdin);
    if(isNull == NULL) {
      printf("\nBye bye.\n");
      exit(0);
    }

    if(strcmp(args, "prev\n") == 0 && prev[0] != '\0') {
      printf("%s", prev);
      strcpy(args, prev);
      continue;
    } else if (strcmp(args, "prev\n") == 0 && prev[0] == '\0') {
      printf("First command cannot be prev\n");
      continue;
    
    } else {
      strcpy(prev, args);
    }


    char **info_token = token_retrieve(args);

    if(info_token[0] == NULL) {
      continue;
    
    } else if(strcmp(info_token[0], "exit") == 0) {
      printf("Bye bye.\n");
      break;
    
    } else if(strcmp(info_token[0], "cd") == 0 && info_token[1] == NULL) {
      printf("cd: missing argument\n");
    
    } else if(strcmp(info_token[0], "cd") == 0 && chdir(info_token[1]) != 0) {
      printf("cd: no such dir found");
    }


    int i = 0;
    int j = 0;
    int redir_in = 0;
    int redir_out = 0;
    char *file_in = NULL;
    char *file_out = NULL;
    while (info_token[i] != NULL) {

      int input_redir = strcmp("<", info_token[i]);
      int output_redir = strcmp(">", info_token[i]);
      int exit_comm = strcmp("exit", info_token[i]);
      int help_comm = strcmp("help", info_token[i]);
      int prev_comm = strcmp("prev", info_token[i]);  

      if (input_redir == 0) {
        file_in = info_token[i + 1];
        i = i + 2;
        redir_in = 1;
        continue;
      } else if (output_redir == 0) {
        file_out = info_token[i+1];
        i = i + 2;
        redir_out = 1;
        continue;
      }

      if (exit_comm = 0) {
        printf("Bye bye.\n");
        exit(0);

      } else if (help_comm = 0) {
        help_buildIn();
        i = i + 1;
        continue;

      } else if (prev_comm = 0) {
        if (prev[0] == '\0') { 
          printf("Previous command not found\n");
        
        } else {
          printf("%s", prev); 
          strcpy(args, prev);  
          i = i + 1;
          continue;
        } 
      }       

      if(strcmp(info_token[i], ";") == 0) {

        temp[j] = NULL;
        if(fork() == 0) {

          if (redir_in) {
            FILE *fp = freopen(file_in, "r", stdin);
            if (fp == NULL) {
              printf("File not found\n");
              exit(1);
            }
          }

          if (redir_out) {
            FILE *fp = freopen(file_out, "w", stdout);
            if (fp == NULL) {
              printf("Opening file failed\n");
              exit(1);
            }
          }

          

          execvp(temp[0], temp);
          printf("%s: ", info_token[i - 1]);
          printf("command not found\n");

          exit(0);

        } else {

          wait(NULL);
          j = 0;
          i += 1;
        }

      } else {
        temp[j] = info_token[i];
        j += 1;
        i += 1;
      }
    }
    temp[j] = NULL;
    // executes here
    if(fork() == 0) {

      if (redir_in) {
        FILE *fp = freopen(file_in, "r", stdin);
        if (fp == NULL) {
          printf("File not found\n");
          exit(1);
        }
      }

      if (redir_out) {
        FILE *fp = freopen(file_out, "w", stdout);
        if (fp == NULL) {
          printf("Opening file failed\n");
          exit(1);
        }
      }  

      if(strcmp(temp[0], "help") == 0) {
        help_buildIn();
        break;
      }

      execvp(temp[0], temp);
      printf("%s: ", temp[0]);
      printf("command not found\n");
      
      exit(1);
    } else {
      wait(NULL);
    }
  }
}

int main(int argc, char **argv) {
  shell();
  return 0;
}
