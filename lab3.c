#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <math.h>

int firstOperand;
int secondOperand;
int result;

int textmode(int mode) {
	static struct termios con[2];
	if(mode > 0)
	  return(tcsetattr(0, TCSAFLUSH, &con[1]));
	tcgetattr(0, &con[0]);
	tcgetattr(0, &con[1]);
	con[0].c_lflag &= ~(ICANON | ECHO | ISIG);
	con[0].c_iflag &= ~(ISTRIP | IXOFF | IXANY | IXON);
	con[0].c_oflag |= CS8;
	con[0].c_cc[VMIN] = 2;
	con[0].c_cc[VTIME] = 1;
	return(tcsetattr(0, TCSAFLUSH, &con[0]));
}

int RandNumber(int len) {
	int i = 0;
	int r;
	srand(getpid());
  	firstOperand = rand() % (int)pow(10, len);
  	//printf("%d\n", firstOperand );
  	secondOperand = rand() % (int)pow(10,len);
  	result = firstOperand / secondOperand;
  	//printf("**%d**", result);
	return(0);
}

int getch() {
	unsigned char c;
	c = 0;
	read(0, &c, 1);
	return(c);
}

int main(int argc, char* argv[]) {
	int len;
	int i=0;
	int err=0;
	unsigned char c;
	if (argc < 2 || (atoi(argv[1]) > 9))
		len = 2;
	else
		len = atoi(argv[1]);
	RandNumber(len);

	char firstOperandChar[len + 1];
	char secondOperandChar[len + 1];
	char resultChar[len + 1];

	snprintf(firstOperandChar, len + 1, "%d", firstOperand);
	snprintf(secondOperandChar, len + 1, "%d", secondOperand);
	int resultLen = snprintf(resultChar, len + 1, "%d", result);

	write(1, &firstOperandChar, len);
	write(1, " / ", 3);
	write(1, &secondOperandChar,len);
	write(1, "\n", 1);
	textmode(0);
	while(i < resultLen) {
	  	switch(c = getch()) {
		    case 0:  c = '\007';
		             switch(getch()) {
		               case 67: c = resultChar[i];
		                        break;
		               case 68: if(i == 0)
		                          break;
		                        i--;
		                        write(1, "\b", 1);
		                        continue;
		               default: break;
		              }
		             break;
		    case 27: i = len;
		             c = '\007';
		             break;
		    default: if(c != resultChar
		    	[i])
		               c = '\007';
		             break;
	 	}
	  	(c == '\007') ? err++ : i++;
	  	write(1, &c, 1);
	}
write(1, "\n", 1);
textmode(1);
return(err);
}	