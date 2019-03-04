#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <assert.h>


/******************************************************************************************
outline for the script:

1. need to declear the varibles
2. helper funtion/buildin fution
3.in main()
 a.declear varibles
 b.funtion to stop with ctrl+C
 c.set end
 d.loop for typein
    :> =< something like this, make funtion
    :pid funtion
    :buffer
    :clean
**********************************************************************************************/

// Global Variables

struct sigaction action1 = {0};
struct sigaction action2 = {0};


/***************************************************************************************
                       code about the signals 
****************************************************************************************/
//from grade script, I know I have to catch the signal, ctrl+c or z or stop the prgram.
//I need to set up 2 things, one is to catch the signal, and one is to react with if.

void catchsig(){  // To catch signals,taking from the lecture. 
  action1.sa_handler = SIG_IGN; // Set to ignore
  action1.sa_flags = 0; 
  sigaction(sigINT, &action1, NULL);
    
  action2.sa_handler = SIG_IGN; 
  action2.sa_flags = 0;
  sigaction(sigTSTP, &action2, NULL);}

//crtl+c include the sleep funtion.
void sigINT(int signal) {
  printf("\nCaught SIGINT.\n");
  action1.sa_handler = SIG_IGN; // Set to ignore
  sigaction(SIGINT, &action1, NULL);
}
//source from http://www.csl.mtu.edu/cs4411.choi/www/Resource/signal.pdf
 
// This function checks for Ctrl-Z
void sigTSTP(int signo) {
  char *message = "\nCaught SigStop.\n";
  write(STDOUT_FILENO, message, 25);
} //from lecture

/***************************************************************************************
                    end of code about the signals 
****************************************************************************************/






int main() {
  //list the varibles are in main funtion    
  char readLine[500];
  char *argument[200];
  int status = 0; //init to 0
  char *inputFile=0;
  char *outputFile=0;
  catchsig(); /*from the grade script, I know I have to catch the signal.ctrl+c should worl at least, I list this one as the first funtion because it needs to be worked at the begining. */ 

 
    
    
    
 /***************************************************************************************
                    user input
 *****************************************************************************************/
  //set a while loop to run, make sure I can type many times until hits exit.
  while(1) {
    //list varibles are used in user input funtion
    int numArguments;
    numArguments = 0;
    char *specialinput;
    int background  =0;
    //if only next prompt is displayed next(5pts)
    printf(": ");
    fflush(stdout);

    // Reached end, exit funtion,taking from the lecture 
    if(fgets(readLine, 500, stdin) == NULL) {
    
      return 0;
    }

      
      
/*****************************************************************************************
                       special input
******************************************************************************************/
      
   //on the grade script, it shows like wc < junk > junk2. So we can defince the funtion with those non chartere.
    int pos = 0;
    specialinput = strtok(readLine, " \n");
    while(specialinput != NULL) {

      if(strcmp(specialinput, "<") == 0) {
        specialinput = strtok(NULL, " \n");
        inputFile = strdup(specialinput);
        specialinput = strtok(NULL, " \n");
      }

      else if(strcmp(specialinput, ">") == 0) {
        specialinput = strtok(NULL, " \n");
        outputFile = strdup(specialinput);
        specialinput = strtok(NULL, " \n");
      }
     
      
        
      else if(strcmp(specialinput, "&") == 0) {
      
          background = 1;
        
        break;
      }

      else {
                           
        // Store argument in an array
        argument[pos] = strdup(specialinput);

        specialinput = strtok(NULL, " \n");

        ++pos;
          numArguments++;
      }
    }
    argument[pos] = NULL;

/*****************************************************************************************
                     end of special input
******************************************************************************************/

/****************************************************************************************
                      getpid
*****************************************************************************************/
//from the grade script, we know we need to getpid, and also from $$.
  char *pch;
  int i = 0;
  int j = 0;
  int argumentLength = 0;
  while(argument[i] != NULL) {
    pch = strstr(argument[i], "$$");
    if(pch != NULL) {
      argumentLength = strlen(argument[i]);
      char *buffer = malloc(sizeof(char) + (argumentLength + 5));
      char *tmp1 = malloc(sizeof(char) + (argumentLength + 5));

      while(argument[i] + sizeof(char) * j != pch) {
        tmp1[j] = argument[i][j];
        j++;
      }
      tmp1[j] = '\0';
      j += 2;



      sprintf(buffer, "%s%ld",tmp1 , (long)getpid());

      argument[i] = buffer;
    }
    i++;
  }
//from piazza and chegg.com I did not figure out this part
/*************************************************************************************
                        end of pid
*****************************************************************************************/
      
      
      
/*****************************************************************************************
                       buffer input
******************************************************************************************/

    if(argument[0] == NULL ||*(argument[0] )== '#'){
    }
      
    else if(strcmp(argument[0], "cd") == 0) {
             if(argument[1] == NULL)   
                 chdir(getenv("HOME"));
            
            else{
                chdir(argument[1]);
            }    
    }

    else if(strcmp(argument[0], "status") == 0) {
         
          if(WIFEXITED(status)){          //Taken from lecture
        printf("exit value %i\n", WEXITSTATUS(status));
    } 
          else{
        printf("terminated by signal %i\n", status);
    }    }
    
    else if(strcmp(argument[0], "exit") == 0) {
      exit(0);
    }

   else {
      int FD;

      // Call fork
      pid_t spawnPID = fork();

      if(spawnPID == 0) {
        // Child process
        // insertPID(argument, getpid());
        if(!background) {
          // If process isn't in background
          // Can be interrupted with Ctrl-C
          action1.sa_handler = SIG_DFL; // Set to default
          action1.sa_flags = 0;
          sigfillset(&action1.sa_mask);
          sigaction(SIGINT, &action1, NULL);
        }
        if(inputFile != NULL) {
          // If input file exists
          // Open with readonly permissions
          FD = open(inputFile, O_RDONLY);

          if(FD == -1) {
            // File is invalid or doesn't exist, exit
            fprintf(stderr, "File is invalid or doesn't exist: %s\n", inputFile);
            exit(1);
          }
          else if(dup2(FD, 0) == -1) { // Redirect the input
            fprintf(stderr, "DUP2 error\n");
            exit(1);
          }
          // After finished, close the file
          close(FD);
        }

        /* Otherwise, if process is in the background, redirect the
           input to /dev/null if the input file is not specified */
        else if(background) {

          FD = open("/dev/null", O_RDONLY);

          if(FD == -1) {
            // Could not open file
            fprintf(stderr, "Could not open.\n");
            exit(1);
          }
          else if(dup2(FD, 0) == -1) {
            fprintf(stderr, "DUP2 error\n");
            exit(1);
          }
          // After finished, close the file
          close(FD);
        }

        // If output file exists
        else if(outputFile != NULL) {
          // Open the file
          FD = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

          if(FD == -1) {
            // Could not open file
            fprintf(stderr, "Could not open file %s\n", outputFile);
            fflush(stdout);
            exit(1);
          }

          if(dup2(FD, 1) == -1) {
            // Error redirecting output
            fprintf(stderr, "DUP2 error\n");
            exit(1);
          }
          // After finished, close the file
          close(FD);
        }

        // Execute the buffer in userArguments[0]
        if(execvp(argument[0], argument)) {
          // buffer not recognized
          fprintf(stderr, "buffer not found: %s\n", argument[0]);
          fflush(stdout);
          exit(1);
        }
      }

      else if(spawnPID < 0) {
        // There was a fork() error
        fprintf(stderr, "There was an error with fork()\n");
        status = 1;
      }

      else {
        // The parent
        if(!background) {
          // If process isn't in the background, wait for foreground process to finish
          waitpid(spawnPID, &status, 0);
        }
        else {
          // If process is in background, print its PID
          printf("Background PID: %d\n", spawnPID);
        }
      }

      for(int i = 0; i< numArguments; i++){
            free(argument[i]);
            argument[i]= NULL;
        }
           
        free(inputFile);            //Some cleanup
        inputFile = NULL;
        free(outputFile);
        outputFile = NULL;

      // Check to see if background processes have completed
      spawnPID = waitpid(-1, &status, WNOHANG);
      while(spawnPID > 0) {
        // Show that process is completed
        printf("Background PID %d is completed. \n", spawnPID);

        spawnPID = waitpid(-1, &status, WNOHANG);
      }

    }
  }

  return 0;
}




