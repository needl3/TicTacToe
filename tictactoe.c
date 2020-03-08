#include <stdio.h>
#include <windows.h>
///#include <conio.h>
//#include <string.h>
//colour definitions
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

void line(int);
void frame();
void space(int);
char player(int *);
void updateFrame(int *,char *data);
int checkWinner();

//global variables
char count[3][3]={"123","456","789"};

void main(){
	int playerNum=1;
	char ch;
	while(playerNum<=9 || ch == 'Y'){
		frame();
		ch = player(&playerNum);
		updateFrame(&playerNum,&ch);
		if(checkWinner() == 1){
			system("cls");
			frame();
			if((playerNum%2) == 0)
				playerNum=2;
			else
				playerNum=1;
			printf("\a\n\n\t\t\tPlayer %d is the winner\n",playerNum);
			Sleep(1000);
				printf(GREEN"\a\n\n\t\tThank you for playing...\n\n"RESET);
				Sleep(1000);
				return;
		}
	playerNum++;
	}

}
void frame(){
	int i,j;
	printf("\t\t\t");
	line(24);
	for (i = 0; i < 3; ++i){
		printf("\t\t\t");
		for (j = 0; j < 3; ++j){
			printf(GREEN"|"RESET"   %c   ",count[i][j]);
		}

		printf(GREEN"|\n\t\t\t"RESET);
		line(24);
	}
}
void line(int l){
	int i;
	for(i=0;i<=l;i++){
		printf(GREEN"*"GREEN);
	}
	printf("\n");
}
void space(int l){
	int i;
	for(i=0;i<=l;i++){
		printf(" ");
	}
}
char player(int *turn){
	char ch;
	if(*turn%2==0)
		printf(YELLOW"Player 2:"RESET);
	else
		printf(BLUE"Player 1:"RESET);	
	babe:
	fflush(stdin);
	printf("\n\tEnter the box number..\n\t==>");
	scanf("%c",&ch);

	if (ch<'1' || ch>'9'){
	printf(RED"\n\aEnter valid number.."RESET);
	Sleep(1000);
	goto babe;
	}

	else{
		system("cls");
		return(ch);
		return;
	}
}
void updateFrame(int *turn,char *data){
	int i,j,stat=0;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if(count[i][j] == *data && (*turn%2 == 1)){
				count[i][j]='X';
				stat=1;
				break;
			}
			else if(count[i][j] == *data && (*turn%2 == 0)){
				count[i][j]='0';
				stat=1;
				break;
			}
		}
		if(stat==1)
			break;
	}
}
int checkWinner(){
	int i,j,stat=0;
	//checks horizontal elements
	for(i=0;i<3;i++){
		for(j=0;j<2;j++){
			if(count[i][j]==count[i][j+1])
				stat++;
		}
		if(stat==2)
			return(1);
		stat=0;
	}
	//checks vertical elements
	for(j=0;j<3;j++){
		for(i=0;i<2;i++){
			if(count[i][j]==count[i+1][j])
				stat++;
		}
		if(stat == 2)
			return (1);
		stat=0;
	}

	if((count[0][0]==count[1][1]&&count[0][0]==count[2][2])||(count[0][2]==count[1][1]&&count[0][2]==count[2][0]))
		return(1);
	
	else
		return (0);
}
