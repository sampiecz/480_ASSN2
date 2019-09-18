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



// Parent function
void PWork(int read1, int write1)
{
 	
	char buffer[15];
	char value[15] = "1";
  cerr << "The parent process is ready to proceed.\n";

  long int M = 1;
 
	sprintf(buffer, "%ld", M);
  write(write1, value, strlen(value) + 1);
	cerr << setw(10) << left << "Parent" << "Value: " << setw(12) << right << value << endl;

  // 2
  while(M < 99999999999)
  {

    // a
		char temp;
		long int i = 0;
		while(read(read1, &temp , 1) > 0 && temp != '\0')
		{
		  value[i] = temp;	
			i++;
		}

		value[i] = '\0';

    // c
    M = atol(value);

    // d
    M = 3 * M + 7;

    // e
    sprintf(buffer, "%ld", M);
    write(write1, buffer, strlen(buffer) + 1);
    cerr << setw(10) << left << "Parent" << "Value: " << setw(12) << right << value << endl;
  }

}


// Child function
void  CWork(int read1, int write1)
{

	char buffer[15];
	char value[15] = "1";
  cerr << "The child process is ready to proceed.\n";

  long int M = 1;

  // 2
  while(M < 99999999999)
  {

    // a
		char temp;
		long int i = 0;
		while(read(read1, &temp , 1) > 0 && temp != '\0')
		{
		  value[i] = temp;	
			i++;
		}

		value[i] = '\0';

    // c
    M = atol(value);

    // d
    M = 2 * M + 5;

    // e
    sprintf(buffer, "%ld", M);
    write(write1, buffer, strlen(buffer) + 1);
    cerr << setw(10) << left << "Child" << "Value: " << setw(12) << right << value << endl;

  }

}

// Grandchild function
void GWork(int read1, int write1)
{

	char buffer[15];
	char value[15] = "1";
  cerr << "The grandchild process is ready to proceed.\n";

  long int M = 1;

  // 2
  while(M < 99999999999)
  {

    // a
		char temp;
		long int i = 0;
		while(read(read1, &temp , 1) > 0 && temp != '\0')
		{
		  value[i] = temp;	
			i++;
		}

		value[i] = '\0';

    // c
    M = atol(value);

    // d
    M = 5 * M + 1;

    // e
    sprintf(buffer, "%ld", M);
    write(write1, buffer, strlen(buffer) + 1);
    cerr << setw(10) << left << "Grandchild" << "Value: " << setw(12) << right << value << endl;

  }

}

int main()
{
  // Instantiate pipe vars
	int pipeA[2];
	int pipeB[2];
	int pipeC[2];

  // Pipe error handling
  if (pipe(pipeA) == -1)
  {
    fprintf(stderr, "%s", "Pipe #A error");           
    exit(-5);
  }
  if (pipe(pipeB) == -1)
  {
    fprintf(stderr, "%s", "Pipe #B error");           
    exit(-5);
  }
  if (pipe(pipeC) == -1)
  {
    fprintf(stderr, "%s", "Pipe #C error");           
    exit(-5);
  }

  // Start forking this is child now
  pid_t pid = fork();

  if(pid == -1)
  {
    fprintf(stderr, "%s", "Fork #1 error \n");
    exit(-5);
  }
  if(pid == 0) 
  {
    // Grandchild pid now
    pid_t pid2 = fork();

    if(pid2 == -1)
    {
      fprintf(stderr, "%s", "Fork #2 error \n");
      exit(-5);
    }

    if( pid2 == 0)
    {
      // Grandchild
			close(pipeA[0]);
			close(pipeA[1]);
			close(pipeB[1]);
			close(pipeC[0]);

      GWork(pipeB[0], pipeC[1]);

			close(pipeB[0]);
			close(pipeC[1]);
    }
    else
    {
      // Child

			close(pipeA[1]);
			close(pipeB[0]);
			close(pipeC[0]);
			close(pipeC[1]);

      CWork(pipeA[0], pipeB[1]);

			close(pipeA[0]);
			close(pipeB[1]);

			wait(0);
			exit(0);
    }

  }
  else 
  {
    // Parent
		close(pipeA[0]);
		close(pipeB[0]);
		close(pipeB[1]);
		close(pipeC[1]);

    PWork(pipeC[0], pipeA[1]);

		close(pipeC[0]);
		close(pipeA[1]);

		wait(0);
		exit(0);

  }

  return 0;

}
