3#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[] = "shell > ";
char delimiters[] = " \t\r\n";
char **environ;

int main() {
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];

    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];
    
    while (true) {
        // Print the shell prompt.
        printf("%s", prompt);
        fflush(stdout);
        // Read input from stdin and store it in command_line. If there's an
        // error, exit immediately. (If you want to learn more about this line,
        // you can Google "man fgets")
        if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
            fprintf(stderr, "fgets error");
            exit(0);
        }
            
        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }
        // tokenize the command in the command line input
        char *token = strtok(command_line,delimiters);
  
        //populating the arguments array with the tokenized command 
        char *temp;
        int i = 0;
        temp = strtok(NULL,delimiters);
        while (temp != NULL){
          arguments[i] = temp;
          i+=1;
          temp = strtok(NULL,delimiters);
        }
        
        int pid = fork();
        if (pid == 0){
          //Child process
          if (execve(token, arguments, environ) < 0 ){
            perror("execution error ");   // error is only returned when input is not executable
            exit(1); 
          }
          exit(0);  // exit after successful execution
        }
        else if (pid < 0){
          perror("Fork error!\n");
          exit(1);
        }  else {
          // Parent process
          wait(NULL);  // Wait  for the child process
        }
    }
    
    // This should never be reached.
    return -1;
}