/*
Operating Systems
Lab assignment 3

Authors:
	Vekrakis Emmanouil
	Daskos Rafail

The user provides how many processes to be created and the number whose
factorial he wants to calculate from the command line and the father
process creates that many processes, writes 1 through a pipe to the first
chlid and then the chlidren start cycling through pipes, each making a 
multiplication until they calculate the factorial of the given number.
--------------------------------------------------------------------------
*/


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	int fact, procs, id, iter=0, status, zero=0;
	pid_t child=1, pid_f=getpid();
	long num;

// Get initial arguements and error control

// Check if 2 additional arguements were given
	if (argc != 3) {
		printf("Wrong number of arguements.\n");
		exit(0);
	}

	if (*argv[2] == '0')
		zero = 1;

	procs = atoi(argv[1]);
	fact = atoi(argv[2]);

// Check if the arguements are numbers
	if (!zero && !fact) {
		printf("Non numeric character.\n");
		exit(0);
	}

// Check if the processes are a positive number
	if (procs <= 0 || procs > 10) {
		printf("Wrong number of processes.\n");
		exit(0);
	}
	
// Check if the factorial is a non negative number
	if (procs < 0) {
		printf("Negative factorial.\n");
		exit(0);
	}


// Main program

// Factorial of 0 is 1
	if (!fact) {
		printf("0! = 1\n");
		exit(0);
	}

	int pd[2*(procs+1)];

	for (int i=0; i<=procs; i++)
		pipe(&pd[2*i]);


// Create children and close pipes
	for (int i=0; i<procs; i++) {
		if (child > 0) {
			child = fork();
			if (child < 0)
				printf("Child %d failed to be created\n", i+1);
			else if (child == 0) {
				id = i;
				close(pd[2*(i+1)]);
				close(pd[2*(i+1) -1]);
				if (i == 1) {
					close(pd[2*procs]);
				}
			}
		}
	}
// Pass 1 to the first child and wait for them to finish
	if (getpid() == pid_f) {
		num = 1;
		printf("F -> 1: 1\n");
		write(pd[1], &num, sizeof(num));
		for (int i=0; i<procs; i++)
			wait(&status);
		exit(0);
	}

// Children code
	else {
		if (id == 0) {
			read(pd[0], &num, sizeof(num));
		}

		while ((iter*procs + id) < fact) {
			if (id != 0) {
				read(pd[2*id], &num, sizeof(num));
			}

			num = num * ((iter)*procs + id+1);
			printf("%d -> %d: %ld\n", id+1, (id+1)%procs+1, num);
			write(pd[2*id+3], &num, sizeof(num));

			if (id == 0 && (iter+1)*procs < fact) { 
				read(pd[2*procs], &num, sizeof(num));
			}

			iter ++;
		}
		exit(0);
	}
}

