/***********************************************************
 CSCI 480 - Assignment 2 - Fall 2019
 
 Progammer: Sam Piecz
 Z-ID: Z1732715
 Section:  
 TA: 
 Date Due: Sep 17, 2019 
 Purpose: Pipes yo. 
 ************************************************************/
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using std::cout;
using std::endl;
using std::cerr;

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
  cout << "The parent process is ready to proceed." << endl;


  long M = 1;
  int in = pipeA[1];
  int out = pipeC[0];

  write(out, "1", 2);

  while(M < 999999999)
  {
    char buffer[15] = "";
    char* ptr; 
    do
    {
      read(in, ptr, 1);
      strcat(buffer, ptr);
    } 
    while(ptr[0] != '\0');

    M = atol(buffer);
    M = 3 * M + 7;

    cout << "Parent:       Value =  " << M << endl; 
    sprintf(buffer, "%ld", M);
    write(out, buffer, strlen(buffer) + 1);
  }

  close(pipeA[1]);
  close(pipeC[0]);

  return 1;
}

// Child function
int CWork()
{
  close(pipeA[1]);
  close(pipeB[0]);
  close(pipeC[0]);
  close(pipeC[1]);
  cout << "The child process is ready to proceed." << endl;

  long M = 1;
  int in = pipeA[0];
  int out = pipeB[1];

  while(M < 999999999)
  {
    char buffer[15] = "";
    char* ptr; 
    do
    {
      read(in, ptr, 1);
      strcat(buffer, ptr);
    } 
    while(ptr[0] != '\0');

    M = atol(buffer);
    M = 2 * M + 5;

    cout << "Child:       Value =  " << M << endl; 
    sprintf(buffer, "%ld", M);
    write(out, buffer, strlen(buffer) + 1);
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
  cout << "The grandchild process is ready to proceed." << endl;

  long M = 1;
  int in = pipeB[0];
  int out = pipeC[1];

  while(M < 999999999)
  {
    char buffer[15] = "";
    char* ptr; 
    do
    {
      read(in, ptr, 1);
      strcat(buffer, ptr);
    } 
    while(ptr[0] != '\0');

    M = atol(buffer);
    M = 5 * M + 1;

    cout << "Grandchild:       Value =  " << M << endl; 
    sprintf(buffer, "%ld", M);
    write(out, buffer, strlen(buffer) + 1);
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
    cerr << "Pipe #A error" << endl;
    exit(-5);
  }
  else if (pipe(pipeB) == -1)
  {
    cerr << "Pipe #B error" << endl;
    exit(-5);
  }
  else if (pipe(pipeC) == -1)
  {
    cerr << "Pipe #C error" << endl;
    exit(-5);
  }

  // Start forking
  pid_t pid = fork();

  if(pid < 0)
  {
    cerr << "Fork #1 error" << endl;
    exit(-5);
  }
  else if(pid == 0) 
  {
    int pid = fork();

    if(pid < 0)
    {
      cerr << "Fork #2 error" << endl;
      exit(-5);
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
