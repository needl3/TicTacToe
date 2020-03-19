#include <stdio.h>
#include <conio.h>
void line(int l){
    int i;
    for(i=0;i<=l;i++){
        printf("*");
    }
    printf("\n");
}
void space(int l){
    int i;
    for(i=0;i<=l;i++){
        printf(" ");
    }
}
void frame(char count[][3]){
    int i,j;
    printf("\t\t\t");
    line(24);
    for (i = 0; i < 3; i++){
        printf("\t\t\t");
        for (j = 0; j < 3; j++){
            printf("|   %c   ",count[i][j]);
        }
        printf("|\n\t\t\t");
        line(24);
    }
}
void read(int *t,char count[][3],char name[][20]){
    char boxNo;
    int i,j,stat=0;
    line(12);
    printf("%s\'s turn|\n",name[*t]);
    line(12);
    enterBox:
    printf("\nEnter Box Number: \n");
    fflush(stdin);
    scanf("%c",&boxNo);

    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            if(count[i][j] != 'X' || count[i][j] != '0'){
                if(count[i][j] == boxNo && (*t == 0)){    
                    count[i][j] ='X';
                    stat=1;
                    break;
                }
                else if(count[i][j] == boxNo && (*t == 1)){        //checks if the player is second
                    count[i][j] = '0';
                    stat = 1;
                    break;
                }
            }
            else{
                printf("\a\n\nNot a valid box...");
                goto enterBox;
            }
        }
        if(stat==1)
            break;
    }
}
int checkWinner(char count[][3]){
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
void main(){
    char dec,player[2][20];
    int i,counter=0,palo,winner=0;
    for(i=0;i<2;i++){
        printf("Enter Player%d's name: ",(i+1));
        scanf("%s",player[i]);                  //ASK PLAYER NAME
    }
    startAgain:                             //GAME RESTART POINT
    system("cls");
    palo=0;
    char spot[][3]={"123","456","789"};
    
    //------------------MAIN GAME AREA-------------------------------
    for(counter=0;counter<9;counter++,palo=(++palo%2)){
        system("cls");
        frame(spot);
        if(checkWinner(spot) == 1){
            printf("\a%s is the Winner...",player[(palo+1)%2]);
            Sleep(3000);
            break;
        }
        read(&palo,spot,player);
    }
    if(checkWinner(spot) != 1){
        printf("\a\nGame Draw...\n");
        Sleep(1000);
    }
    //---------------------------------------------------------------

    //ASK TO PLAY AGAIN AREA
    ask:
    fflush(stdin);
    printf("\n\t\t\t\t\tPlay Again?(Y/N)\n\t\t\t\t\t==>");
    scanf("%c",&dec);
    if(dec == 'y'|| dec == 'Y'){
        goto startAgain;
    }
    else if (dec == 'n' || dec == 'N'){
        system("cls");
        printf("\n\n\n\n\n\n\t\t\t\tThank you for playing");
        Sleep(1000);
        return;
    }
    else{
        system("cls");
        printf("\aEnter valid option...");
        Sleep(1000);
        system("cls");
        goto ask;
    } 
}
