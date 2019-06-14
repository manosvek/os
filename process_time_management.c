/*
Operating Systems
Lab assignment 2

Authors:
	Vekrakis Emmanouil
	Daskos Rafail

Father process creates 5 children which are paused. He then unpauses each
one with an order given by the user from the command line, lets them run
for 3 seconds and pauses them again, repeating this task 4 times. Each 
child prints its id, its pid and how many times it has printed with 1 
second interval.
------------------------------------------------------------------------------
*/


#include <stdio.h>					//To use printf()
#include <sys/types.h>				//Same as <unistd.h>
#include <sys/wait.h>				//To use wait()
#include <unistd.h>					//To use getpid(), getppid(), fork(), sleep()
#include <stdlib.h>					//To use exit()
#include <signal.h>

void handleCont() {}

void handlePause() {
	pause();
}

int main (int argc, char **argv) {
	int priority[5], numbers[5], i=0, valid=1;
	pid_t children[5], pid_f=getpid(), child=1, id;

// Get initial arguements and error control

// Check if 5 additional arguements were given
	if (argc != 6) {
		printf("Wrong number of arguements.\n");
		exit(0);
	}

	for (int i=0; i<5; i++) {
		priority[i] = atoi(argv[i+1]);
		numbers[i] = 0;
	}

// Check if the arguements range from 1 to 5
	while (valid == 1 && i < 5) {
		if (priority[i] > 5 || priority[i] < 1)
			valid = 0;
		if (valid)
			numbers[priority[i] - 1]++;
		i++;
	}

// Check if all numbers are taken only once
	for (int i=0; i<5; i++)
		if (numbers[i] != 1)
			valid = 0;

// If the arguements are invalid, exit
	if (valid == 0) {
		printf("Arguements were invalid.\n");
		exit(0);
	}



// Create children and pause them
	for (int i=0; i<5; i++) {
		if (child > 0) {
//			printf("Creating child %d\n", i+1);
			child = fork();
			if (child < 0)
				printf("Child %d failed to be created\n", i+1);
			else if (child == 0) {
				id = i + 1;
				signal(SIGCONT, handleCont);
				signal(SIGSTOP, handlePause);
				pause();
			}
			else
				children[i] = child;
		}
	}


// Code for father

	if (getpid() == pid_f) {
		usleep(1000);	// For signal handlers' setup time
		for (int i=0; i<4; i++) {
			for (int j=0; j<5; j++) {
				kill(children[priority[j] - 1], SIGCONT);
				sleep(3);
				kill(children[priority[j] - 1], SIGSTOP);
			}
		}

	// Kill children
		for (int j=0; j<5; j++) {
//			printf("Killing child %d\n", priority[j]);
			kill(children[priority[j] - 1], SIGTERM);
		}

		exit(0);
	}


// Code for each child

	else {
		int n=1;
		while(1) {
			printf("Child%d %d is executed (%d)\n", id, getpid(), n);
			sleep(1);
			n++;
		}
	}
}
