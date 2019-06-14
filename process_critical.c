/*
Operating Systems
Lab assignment 4

Authors:
	Vekrakis Emmanouil
	Daskos Rafail

Create 3 processes with different critical and non-critical sections, which
can be seen at each process' code and create a function which checks if a
process is in critical mode via the use of a pipe and forbids all other
processes to enter their critical section. All processes print whether they
are executing a critical (5 times) or a non-critical (7 times)  section.
------------------------------------------------------------------------------
*/


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int pd[2];


// Critical section of each process
void crit(int id, pid_t pid) {
	int flag=0;

// Read from pipe until no process is in critical section
	do {
		read(pd[0], &flag, sizeof(flag));
	}
	while (flag == 1);

// Raise flag that a process is in critical section
	flag = 1;
	write(pd[1], &flag, sizeof(flag));

// Critical section code
	for (int i=0; i<5; i++)
		printf("Child%d %d executes a critical section\n", id+1, pid);

// Write that the process has exited critical section
	flag = 0;
	write(pd[1], &flag, sizeof(flag));
}



// Non-critical section of each process
void non_crit(int id, pid_t pid) {
	for (int i=0; i<7; i++)
		printf("Child%d %d executes a non critical section\n", id+1, pid);
}



int main() {
	int id, flag=0, status;
	pid_t child=1, pid_f=getpid();
	pipe(&pd[0]);

// Create children
	for (int i=0; i<3; i++) {
		if (child > 0) {
			child = fork();
			if (child < 0)
				printf("Child %d failed to be created\n", i+1);
			else if (child == 0)
				id = i;
		}
	}

	if (getpid() == pid_f) {	// Father's code
		write(pd[1], &flag, sizeof(flag));

		for (int i=0; i<3; i++)
			wait(&status);
		exit(0);
	}

	else {						// Children's code
		if (id == 0) {			// Code of 1st child
			pid_t pid = getpid();

			crit(id, pid);
			non_crit(id, pid);
			non_crit(id, pid);
			crit(id, pid);
			non_crit(id, pid);
			crit(id, pid);
		}
		else if (id == 1) {		// Code of 2nd child
			pid_t pid = getpid();

			non_crit(id, pid);
			crit(id, pid);
			non_crit(id, pid);
			crit(id, pid);
			non_crit(id, pid);
			crit(id, pid);
		}
		else if (id == 2) {		// Code of 3rd child
			pid_t pid = getpid();

			crit(id, pid);
			non_crit(id, pid);
			crit(id, pid);
			non_crit(id, pid);
			crit(id, pid);
			non_crit(id, pid);
		}

		exit(0);
	}
}
