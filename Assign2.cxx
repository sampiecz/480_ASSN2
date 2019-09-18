/***********************************************************
 CSCI 480 - Assignment 2 - Fall 2019
 
 Progammer: Sam Piecz
 Z-ID: Z1732715
 Section:  
 TA: 
 Date Due: Sep 17, 2019 
 Purpose: Pipes yo. 
 ************************************************************/
#include <sys/wait.h>             /* Needed to use wait() */
#include <stdio.h>                  
#include <stdlib.h>
#include <unistd.h>               /* UNIX and POSIX constants and functions (fork, pipe) */
#include <string.h>               /* Needed to use strlen() */

// Instantiate pipe vars
int pipeA[2];
int pipeB[2];
int pipeC[2];

// Parent function
int PWork()
{
  // Close uneeded pipes
  close(pipeA[0]);
  close(pipeB[0]);
  close(pipeB[1]);
  close(pipeC[1]);
  printf("The parent process is ready to proceed.\n");

  long M = 1;

  write(pipeA[1], "1", 2);

  while(M < 999999999)
  {
    char buffer[15] = "";
    char* buf;
    while (read(pipeC[0], &buf, 1) > 0 && buf[0] != '\0')
      strcat(buffer, buf);
    
    M = atol(buffer);
    M = 3 * M + 7;

    sprintf(buffer, "Parent:       Value =  %ld", M);
    write(pipeA[1], buffer, strlen(buffer) + 1);
  }

  close(pipeC[0]);
  close(pipeA[1]);

  return 1;
}

// Child function
int CWork()
{
  close(pipeA[1]);
  close(pipeB[0]);
  close(pipeC[0]);
  close(pipeC[1]);
  printf("The child process is ready to proceed.\n");

  long M = 1;

  while(M < 999999999)
  {
    char buffer[15] = "";
    char* buf;
    while (read(pipeA[0], &buf, 1) > 0 && buf[0] != '\0')
      strcat(buffer, buf);
    
    M = atol(buffer);
    M = 2 * M + 5;
    
    sprintf(buffer, "Child:       Value =  %ld", M);
    write(pipeB[1], buffer, strlen(buffer) + 1);
  }

  close(pipeA[0]);
  close(pipeB[1]);

  return 1;
}

// Grandchild function
int GWork()
{
  close(pipeA[0]);
  close(pipeA[1]);
  close(pipeB[1]);
  close(pipeC[0]);
  printf("The grandchild process is ready to proceed.\n");

  long M = 1;

  while(M < 999999999)
  {
    char buffer[15] = "";
    char* buf;
    while (read(pipeB[0], &buf, 1) > 0 && buf[0] != '\0')
      strcat(buffer, buf);
    
    M = atol(buffer);
    M = 5 * M + 1;
    
    sprintf(buffer, "Child:       Value =  %ld", M);
    write(pipeC[1], buffer, strlen(buffer) + 1);
  }

  close(pipeB[0]);
  close(pipeC[1]);

  return 1;
}

int main()
{
  
  // Pipe error handling
  if (pipe(pipeA) == -1)
  {
    fprintf(stderr, "%s", "Pipe #A error");           
    exit(EXIT_FAILURE);
  }
  else if (pipe(pipeB) == -1)
  {
    fprintf(stderr, "%s", "Pipe #B error");           
    exit(EXIT_FAILURE);
  }
  else if (pipe(pipeC) == -1)
  {
    fprintf(stderr, "%s", "Pipe #C error");           
    exit(EXIT_FAILURE);
  }

  // Start forking this is child now
  pid_t pid = fork();

  if(pid == -1)
  {
    fprintf(stderr, "%s", "Fork #1 error \n");
    exit(EXIT_FAILURE);
  }
  else if(pid == 0) 
  {
    // Grandchild pid now
    pid = fork();

    if(pid == -1)
    {
      fprintf(stderr, "%s", "Fork #2 error \n");
      exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
      // Grandchild
      GWork();
      exit(0);
    }
    else
    {
      // Child
      CWork();
      exit(0);
    }

  }
  else if(pid > 0)
  {
    // Parent
    PWork();
    exit(0);
  }

  return 0;
}
