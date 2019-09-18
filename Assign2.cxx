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
#include <iostream>
#include <iomanip>
#include <stdio.h>                  
#include <stdlib.h>
#include <unistd.h>               /* UNIX and POSIX constants and functions (fork, pipe) */
#include <string.h>               /* Needed to use strlen() */

using std::cerr;
using std::endl;
using std::left;
using std::right;
using std::setw;
using std::to_string;

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
 
  write(pipeA[1], "1", 2);


  // 2
  while(M < 999999999)
  {
		char buffer[15] = "";
		char value[15] = "1";

    // a
		int i = 0;
    do
    {
      read(pipeC[0], &value[i++], 1);
			strcat(buffer, &value[i]);
    }
    while (buffer[i-1] != '\0');

    // c
    M = atol(buffer);

    // d
    M = 3 * M + 7;

    // e
    sprintf(buffer, "%ld", M);
    cerr << setw(10) << left << "Parent" << "Value: " << setw(12) << right << value << endl;

    // f
    write(pipeB[1], buffer, strlen(buffer) + 1);
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
  char buffer[15] = "";
  char value[15] = "1";
  
  // 2
  while(M < 999999999)
  {
    // a
		int i = 0;
    do
    {
      read(pipeA[0], &buffer[i++], 1);
			strcat(buffer, &value[i]);
    }
    while (buffer[i-1] != '\0');

    // c
    M = atol(buffer);

    // d
    M = 2 * M + 5;

    // e
    sprintf(buffer, "%ld", M);
    cerr << setw(10) << left << "Parent" << "Value: " << setw(12) << right << value << endl;

    // f
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
  char buffer[15] = "";
  
  // 2
  while(M < 999999999)
  {
		// a
		int i = 0;
    do
    {
      read(pipeB[0], &buffer[i++], 1);
			strcat(buffer, &value[i]);
    }
    while (buffer[i-1] != '\0');

    // c
    M = atol(buffer);

    // d
    M = 5 * M + 1;

    // e
    sprintf(buffer, "%ld", M);
    cerr << setw(10) << left << "Parent" << "Value: " << setw(12) << right << value << endl;

    // f
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
    pid_t pid2 = fork();

    if(pid2 == -1)
    {
      fprintf(stderr, "%s", "Fork #2 error \n");
      exit(EXIT_FAILURE);
    }
    else if(pid2 == 0)
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
