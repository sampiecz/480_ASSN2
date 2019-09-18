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

using std::cerr;

// Instantiate pipe vars
int pipeA[2];
int pipeB[2];
int pipeC[2];

// Parent function
int PWork()
{
  close(pipeA[0]);
  close(pipeB[0]);
  close(pipeB[1]);
  close(pipeC[1]);
  cerr << "The parent process is ready to proceed.\n";

  long M = 1;
  char buffer[15];
  char* buf = buffer;
  char value[15] = "1";
  char* val = value;

  
  write(pipeA[1], val, 1);

  // 2
  while(M < 999999999)
  {
    // a
    do
    {
      read(pipeC[0], buf, 1);
    }
    while (buf[0] != '\0')

    // b
    val[strlen(*val)] = '\0';

    // c
    M = atol(buffer);

    // d
    M = 3 * M + 7;

    // e
    sprintf(buf, "%d", M);
    sprintf(Value, "Parent:      Value: %d\n", M);
    cerr << Value;

    // f
    write(pipeB[1], buf, strlen(buf) + 1);
  }

  close(pipeC[0]);
  close(pipeA[1]);

  wait(0);
  exit(0);
}

// Child function
int CWork()
{
  close(pipeA[1]);
  close(pipeB[0]);
  close(pipeC[0]);
  close(pipeC[1]);
  cerr << "The child process is ready to proceed.\n";

  long M = 1;

  while(M < 999999999)
  {
    char buffer[15] = "";
    char* buf = buffer;
    while (read(pipeA[0], buf, 1) > 0 && buf[0] != '\0')
      strcat(buffer, buf);
    M = atol(buffer);
    M = 2 * M + 5;
    cerr << "Child:       Value = " + to_string(M) + "\n";
    write(pipeB[1], buffer, strlen(buffer) + 1);
  }

  close(pipeA[0]);
  close(pipeB[1]);

  wait(0);
  exit(0);
}

// Grandchild function
int GWork()
{
  close(pipeA[0]);
  close(pipeA[1]);
  close(pipeB[1]);
  close(pipeC[0]);
  cerr << "The grandchild process is ready to proceed.\n";

  long M = 1;

  while(M < 999999999)
  {
    char buffer[15] = "";
    char* buf = buffer;
    while (read(pipeB[0], buf, 1) > 0 && buf[0] != '\0')
      strcat(buffer, buf);
    M = atol(buffer);
    M = 5 * M + 1;
    cerr << "Grandchild:       Value = " + to_string(M) + "\n";
    write(pipeB[1], buffer, strlen(buffer) + 1);
  }

  close(pipeB[0]);
  close(pipeC[1]);

  exit(0);
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
    }
    else
    {
      // Child
      CWork();
    }

  }
  else if(pid > 0)
  {
    // Parent
    PWork();
  }

  return 0;
}
