#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <sys/wait.h>
#include <time.h>

int randomNumber = 0;

/* clear standart text mode window */

void clrscr() {
	unsigned char esc[11];

	/* obtain set cursor to left-top window corner */

	esc[0] = 27; esc[1] = '[';
	esc[2] = 'H';
	write(1,esc,3);

	/* obtain clear escape sequence */

	esc[2] = '2'; esc[3] = 'J';
	write(1,esc,4);

	return;

}

/* position cursor in standart text window */

void gotoxy(int dist, int ty, char c) {
	unsigned char esc[11];
	static unsigned char ystr[3];
	static unsigned char xstr[3];

	static int notAvailableX[100];

	unsigned char s;
	int i;
	int j;

	if(ty > 99)
	  ty = 99;
	if(ty < 1)
	  ty = 1;

	srand((unsigned) time(NULL) * getpid());
	int pos = rand() % dist;

	/*while (notAvailableX[pos]) {
		srand((unsigned) time(NULL) * getpid());
		pos = rand() % dist;
	}
	notAvailableX[pos] = 1;*/

	if (notAvailableX[pos]) {
		pos += randomNumber;
		pos = rand() % 10;
	}
	notAvailableX[pos] = 1;


	xstr[0] = xstr[1] = xstr[2] = '\0';
	ystr[0] = ystr[1] = ystr[2] = '\0';

	sprintf((char *) ystr,"%d",ty);
	sprintf((char *) xstr,"%d",pos);

	esc[0] = 27; esc[1] = '[';
	i = 2; j = 0;
	while(ystr[j])
		esc[i++]=ystr[j++];
	j = 0;
	esc[i++]=';';
	while(xstr[j])
	  esc[i++]=xstr[j++];
	esc[i++] = 'H';
	esc[i++] = c;
	esc[i] = '\0';
	write(1,esc,i);

	return;

}

int main(int argc, char* argv[]) {
	int x = 1;
	int status;
	int i;
	int j = 0;
	int PROCNUM = atoi(argv[1]);
	int* pid = sbrk((PROCNUM+1)*sizeof(int));
	char* lead = sbrk((PROCNUM+1)*sizeof(char));
	int dist = atoi(argv[2]);
	int p;
	char bell = '\007';
	struct timeb tp[1];
	int jump;

	srand((unsigned) time(NULL) * getpid());
	randomNumber = rand();

	clrscr();
	while(j < PROCNUM) {
		if((pid[j] = fork()) == 0) {
			usleep(PROCNUM - j);
	    	while(x < dist) {
	      		gotoxy(dist, j + 1, 'A' + j);
	      		ftime(tp);
	      		if((tp[0].millitm % (j + 'A')) != j)
	        		continue;
	      	x++;
	     	for(i=0; i<100000000; i++);
	    	}
	      	
	    	exit('A' + j);
	  	}
	  	j++;
	}

	j = 0;

	while((p = wait(&status)) != (-1)) {
  		for(i = 0; i < PROCNUM; i++)
    	if(pid[i] == p)
      		lead[j++] = (char) ((status >> 8) & '\377');
  		write(1,&bell,1);
	}

	lead[j] = '\n';
	sleep(1);
	gotoxy(1, PROCNUM + 3, '\n');
	write(1, lead, PROCNUM + 1);
	exit(0);
}
