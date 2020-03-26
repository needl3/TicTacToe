
#include <stdio.h>
#include <stdlib.h>

//LOADS LIBRARIES ACCORDING TO SYSTEM ARCH
#ifdef _WIN32
#include <windows.h>
#define clear system("cls")
#define sleepTime 1000
#else
#include <unistd.h>
#include <ctype.h>
#define sleepTime 1
#define clear system("clear")
#endif

//CONSTANTS USED
#define hturn 0
#define mturn 1

// //color constants
// #define Black '\033[0;30m'    
// #define Red '\033[0;31m'      
// #define Green '\033[0;32m'    
// #define Yellow '\033[0;33m'   
// #define Blue '\033[0;34m'   
// #define Purple '\033[0;35m'
// #define Cyan '\033[0;36m' 
// #define White '\033[0;37m'
// #define Reset '\033[0;0m'

//FUNCTION DECLARATIONS
int menu();
void game_mode_frame(int);
void frame(char (*) []);
void line(int);
void space(int);
void human();
void Bot(int);
int readMove(char (*)[],char,int);
char best_spot(char (*)[]);
int best_move(char (*)[], int);
char check_winner(char (*) []);
int askAgain();
char check_for_immediate_winning_move(char (*) [],int,char *);
//FUNCTION DEFINITIONS
int main(void){
    startAgain:
    clear;
    switch(menu()){
        case 1:
            human();
            break;
        case 2:
            Bot(2);
            break;
        case 3:
            Bot(3);
            break;
        case 4:
            clear;
            printf("\nThank you for playing....\n");
            sleep(sleepTime);
            return 0;
            break;
        default:
            printf("\n\aEnter Valid option....\n");
            sleep(sleepTime);
            break;
    }
    goto startAgain;
}
void human(){
    int turn,readStat;
    char boxNo,players[2][20];
    clear;
    game_mode_frame(1);
    printf("Enter player1's name: ");
    scanf("%s",players[0]);
    printf("\nEnter player2's name: ");
    scanf("%s",players[1]);
    turn = hturn;
    startAgain: ;
    char spot[][3]={"123","456","789"};
    //--------------main game area-------------
    for(int i=0;i<9;i++){
        clear;
        game_mode_frame(1);
        frame(spot);
        printf("\t\t\t\t\t\t\t\t\t\t%s\'s Turn....\nEnter box Number: ==>\n",players[turn]);
        enterAgain: ;
        boxNo = getchar();
        readStat = readMove(spot,boxNo,turn);
        if(check_winner(spot) != 'E' && check_winner(spot) != 'F'){
            clear;
            frame(spot);
            printf("\n%s is the winner....\n",players[turn]);
            sleep(sleepTime);
            break;
        }
        if(readStat == 0){
            printf("\nInvalid move.....Enter again\n");
            sleep(sleepTime);
            goto enterAgain;
        }
        printf("\n%d\n",turn);
        turn = (turn+1)%2;
        clear;
    }
    if(check_winner(spot) == 'F'){
        clear;
        frame(spot);
        printf("\nGame Draw.....\n");
    }
    //----------------game area finishes---------------
    getchar();
    if (askAgain() == 1)
        goto startAgain;
    else
        return;
}
void Bot(int mode){
    //VARIABLE DECLARATIONS AND INITIALIZATIONS
    char dec,boxNo,pos,players[2][10]={"HUMAN","MACHINE"};
    int turn,stat;
    
    //SETTING THE TURN
    setTurn: ;
    clear;
    game_mode_frame(mode);
    char spot[][3]={"123","456","789"};
    printf("You want to go first?(Y/N)\n==>");
    getchar();
    scanf("%c",&dec);
    dec = toupper(dec);
    turn = (dec == 'Y')?hturn:mturn;
    //SWITCHING TO PROVIDED TURN 
    for(int i=0; i<9;i++){
        switch(turn){
            case hturn:     //HUMAN PLAYER
                clear;
                game_mode_frame(mode);
                frame(spot);
                chooseAgain:
                printf("\nEnter the box number: ");
                scanf("%c",&boxNo);
                stat = readMove(spot,boxNo,hturn);
                if(stat == 0){
                    printf("Invalid spot choosen....");
                    goto chooseAgain;
                }
                frame(spot);
                break;
            case mturn:     //MACHINE PLAYER
                clear;
                game_mode_frame(mode);
                frame(spot);
                printf("Machines Turn...\n");
                sleep(sleepTime);
                switch (mode){
                    case 2://mode for beatable bot
                    //first immediate winning move is calculated
                        pos = check_for_immediate_winning_move(spot,mturn,&boxNo);
                        if(pos == 'X')
                            readMove(spot,boxNo,mturn);
                        else{    // defending move
                            if(check_for_immediate_winning_move(spot,hturn,&boxNo)== '0')
                                readMove(spot,boxNo,mturn);
                            else{
                                for(int k=0;k<3;k++){
                                    for(int l=0;l<3;l++){
                                        if(spot[k][l] != 'X' && spot[k][l] != '0'){
                                            stat = 1;
                                            spot[k][l] = 'X';
                                            break;
                                        }
                                    }
                                    if(stat == 1)
                                        break;
                                }
                            }
                        }
                        break;
                    case 3://unbeatable bot
                        readMove(spot,best_spot(spot),mturn);
                        getchar();
                        break;
                }
                break;
            default:
                printf("\nInvlid choice...Choose Again...\n");
                goto setTurn;
                break;
        }
        clear;
        frame(spot);
        if(check_winner(spot) == 'X'){
            printf("\nMachine is the winner...\n");
            break;
        }
        else if(check_winner(spot) == '0'){
            printf("\nHuman is the winner....\n");
            break;
        }
        turn = (turn+1)%2;
        stat =0;
    }

    if(check_winner(spot) == 'F'){
        clear;
        frame(spot);
        printf("\n\t\t\t\t\tGame draw\n");
        sleep(sleepTime);
    }
    if(askAgain() == 1)
        goto setTurn;
    else
        return;    
}
int best_move(char (*subFrame)[3],int palo){
    char temp;
    int score,prevScore;
    if(check_winner(subFrame) == 'X')//if machine wins
            return 1;
    else if(check_winner(subFrame) == '0')//if human wins
            return -1;
    else if(check_winner(subFrame) == 'F')//if game is draw
            return 0;
    else{
        prevScore = (palo == mturn)?-1:1;//sets score corresponding to the turn
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(subFrame[i][j] != 'X' && subFrame[i][j] != '0'){
                    if(palo ==  mturn){
                        temp = subFrame[i][j];
                        subFrame[i][j] = 'X';
                        score = best_move(subFrame,hturn);
                        subFrame[i][j] = temp;
                        if(score > prevScore){
                            prevScore = score;
                        }
                    }
                    else if(palo == hturn){
                        temp = subFrame[i][j];
                        subFrame[i][j] = '0';
                        score = best_move(subFrame,mturn);
                        subFrame[i][j] = temp;
                        if(score < prevScore){
                            prevScore = score;
                        }
                    }
                }
            }
        }
        return prevScore;
    }
}
char best_spot(char (*frame)[3]){
    char winningPosition,temp;
    int prevScore=-10,score;
    //FIRST CHECK FOR IMMEDIATE WIN
    if(check_for_immediate_winning_move(frame,mturn,&winningPosition) == 'X')
        return winningPosition;
    else{//if no win then return the optimal spot
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(frame[i][j] != 'X' && frame[i][j] != '0'){//if current spot is not claimed
                    temp = frame[i][j];                        //store current spot value
                    frame[i][j] = 'X';                      //put X to test the score at this move
                    score = best_move(frame,hturn);                    //store the score in this spot
                    frame[i][j] = temp;                     //put the original value back in the spot
                    if(score > prevScore){                  //if the previous score is less than the score in this spot
                        prevScore = score;                  //update previous score to new score
                        winningPosition = temp;             //store current winning position to this spot
                    }
                }
            }
        }
        return winningPosition;
    }
}
char check_for_immediate_winning_move(char (*frame)[3], int checkFor,char *spotNo){
    char temp;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(frame[i][j] != 'X' && frame[i][j] != '0'){   //if there is unclaimed space
                if(checkFor == mturn){      //if passed turn is machines
                    temp = frame[i][j];     //copies original spot no
                    frame[i][j] = 'X';      //puts X for testing
                    if(check_winner(frame) == 'X'){     //if X wins
                        frame[i][j] = temp;     //returns original position value
                        *spotNo = frame[i][j];  //stores the winning position
                        return 'X';             //returns the winning stat
                    }
                    else
                        frame[i][j] = temp;     //if doesnt win and returns sets original value
                }
                else{                       //if passed turn is of human
                    temp = frame[i][j];
                    frame[i][j] = '0';
                    if(check_winner(frame) == '0'){
                        frame[i][j] = temp;
                        *spotNo = frame[i][j];
                        return '0';
                    }
                    else
                        frame[i][j] = temp;    
                }
            }
        }
    }
    *spotNo = 'Z';          //if doesnt win stores not won value Z
    return 'Z';             //if doesnt win turns not won value Z
}
char check_winner(char (*object)[3]){
    int stat=0,i,j;
    //CHECK FOR HORIZONTAL WIN CASES
     for(i=0;i<3;i++){
        for(j=0;j<2;j++){
            if(object[i][j]==object[i][j+1])
                stat++;
        }
        if (stat == 2)
            return object[i][j];        //returns winning group wither X or Y
        stat=0;
    }
    //CHECK FOR VERTICAL WIN CASES
    for(j=0;j<3;j++){
        for(i=0;i<2;i++){
            if(object[i][j]==object[i+1][j])
                stat++;
        }
        if(stat == 2)
            return object[i][j];
        stat=0;
    }
    //CHECK FOR DIAGONAL WIN CASES
    if((object[0][0]==object[1][1]&&object[0][0]==object[2][2])||(object[0][2]==object[1][1]&&object[0][2]==object[2][0]))
        return object[1][1];//returns the winning group identification using [1][1] because it is common for both wins
    else{
        //check if the game is tied or there are empty spots aviliable
        for(i=0;i<3;i++){
            for(j=0;j<3;j++){
                if(object[i][j] != 'X' && object[i][j] != '0')
                    return 'E';     //returns unclaimed spot status
            }
        }
        return ('F');               //returns tied status
        }
}
void frame(char (*count) [3]){
    int i,j;
    printf("\t\t\t");
    line(24);
    for (i = 0; i < 3; i++){
        printf("\t\t\t");
        for (j = 0; j < 3; j++){
            //add is else condition to determine colors
            printf("|   %c   ",count[i][j]);
        }
        printf("|\n\t\t\t");
        line(24);
    }
}
void line(int num){
    for(int i=0;i<num;i++){
        printf("-");
    }
    printf("\n");
}
void space(int num){
    for(int i=0;i<num;i++){
        printf(" ");
    }
}
int readMove(char (*temp_plane)[3],char spotNo, int palo){
    int stat = 0;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(temp_plane[i][j] != 'X' && temp_plane[i][j] != '0' && temp_plane[i][j] == spotNo){
                if(palo == hturn){
                    temp_plane[i][j] = '0';
                }

                else{
                    temp_plane[i][j] = 'X';
                }                
                stat =1;
                break;
            }
        }
        if(stat == 1)
        break;
    }
    if(stat == 0)
        return 0;
    else
        return 1;
        
}
int askAgain(){
    char dec;
    printf("\nDo you want to play again?(Y/N)\n==>");
    //getchar();//to clear buffer
    scanf("%c",&dec);
    dec=toupper(dec);
    if(dec == 'Y')
        return 1;
    else
        return 0;
}
int menu(){
    line(68);
    line(68);
    printf("|\t\t\t     TICTACTOE\t\t\t\t   |\n");
    line(68);
    line(68);
    printf("|\t\t\t\t\t\t\t\t   |\n||  1) Human vs Human\t\t  ||  2) Human vs Bot(Defeatable) ||\n\n||  3) Everwinning Bot vs Human\t  ||  4) Exit Game \t\t  ||\n|");
    line(66);
    line(68);
    printf("==>");
    int dec;
    scanf("%d",&dec);
    return dec;
}
void game_mode_frame(int Mode){
    printf("\t\t\t\t");
    line(15);
    printf("\t\t\t\t| TIC TAC TOE |\n\t\t");        
    switch(Mode){
        case 1:
            line(41);
            printf("\t\t|----------Mode: Human vs Human----------|\n\t\t");
            line(41);
            printf("\n");
            break;
        case 2:
            line(41);
            printf("\t\t|-------Mode: Human vs BeatableBot------|\n\t\t");
            line(41);
            printf("\n");
            break;
        case 3:
            line(41);
            printf("\t\t|------Mode: Human vs UnBeatableBot-----|\n\t\t");
            line(41);
            printf("\n");
            break;
        default:
            break;
    }
}
