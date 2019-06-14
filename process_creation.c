/*
Operating Systems
Lab assignment 1

Authors:
	Vekrakis Emmanouil
	Daskos Rafail

Create a process tree with Father at root and two children processes C1, C2.
C1 has two leaves C3,C4 and C2 has another leaf, C5. Each process prints 10
times its pid and the pid of its father with 1 second interval.
------------------------------------------------------------------------------
*/

#include <stdio.h>					//To use printf()
#include <sys/wait.h>				//To use wait()
#include <unistd.h>					//To use getpid(), getppid(), fork(), sleep()
#include <sys/types.h>				//Same as <unistd.h>
#include <stdlib.h>					//To use exit()

int main() {
	int status;
	pid_t child, pid_f=getpid(), pid_1;

//	printf("Father pid is %d\n", pid_f);

	child = fork();					//C1 is created

	if(child < 0)
		printf("Process C1 failed to be created");
	if(child == 0) {				//For C1 process
		pid_1 = getpid();
		child = fork();				//C3 is created
		if(child < 0)
			printf("Process C3 failed to be created");
		if(child != 0) {
			child = fork();			//C4 is created
			if(child < 0)
				printf("Process C4 failed to be created");
		}
		for (int i=0; i<10; i++) {
			sleep(1);
			printf("Process %d is executed, my father is %d\n", getpid(), getppid());
		}
		if (getpid() != pid_1)
			exit(0);				//Terminate C3 and C4

		wait(&status);				//Wait for C3 and C4
		wait(&status);				//to terminate
		exit(0);					//Terminate C1
	}
	else {							//For F process
		child = fork();				//C2 is created
		if(child < 0)
			printf("Process C2 failed to be created");
		if (child == 0) {
			child = fork();			//C5 is created
			if(child < 0)
				printf("Process C5 failed to be created");
		}
		if(getpid() != pid_f) {
			for (int i=0; i<10; i++) {
				sleep(1);
				printf("Process %d is executed, my father is %d\n", getpid(), getppid());
			}
			if(child > 0) {
				wait(&status);		//Wait for C5 to terminate
				exit(0);			//Terminate C2
			}
			else	
				exit(0);			//Terminate C5
		}
		wait(&status);				//Wait for C1 and C2
		wait(&status);				//to terminate
		exit(0);					//Terminate F
	}

	return 0;
}
