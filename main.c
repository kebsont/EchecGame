#include <stdio.h>
#include <stdlib.h>


void init(){
    char echiquier[8][8];
    int i, j;
    printf("AVANT\n");
    for(i = 0;i < 8;i++){
        for(j = 0;j < 8;j++){
            echiquier[i][j] = 'o';
        }
    }
        printf("APRES\n");

    printf("%c\t", echiquier[i][j]);
        printf("APREs TABLEAU");

}
/*0
void deplacement(char typePion[]){
    switch(typePion){
    case "Tour":

    }
}*/

int main()
{
    /*char echiquier[8][8];
    int i,j;
    for(j = 0;j < 8;j++){
        echiquier[1][j] = 'P';
    }

    for(i = 0;i < 8;i++){
        echiquier[6][i] = 'P';
    }

    for(i = 2;i < 6;i++){
        for(j = 0;j < 8;j++){
            echiquier[i][j] = '_';
        }
    }

    echiquier[0][0] = 'T';
    echiquier[0][1] = 'C';
    echiquier[0][2] = 'F';
    echiquier[0][3] = 'K';
    echiquier[0][4] = 'Q';
    echiquier[0][5] = 'F';
    echiquier[0][6] = 'C';
    echiquier[0][7] = 'T';

    echiquier[7][0] = 'T';
    echiquier[7][1] = 'C';
    echiquier[7][2] = 'F';
    echiquier[7][3] = 'K';
    echiquier[7][4] = 'Q';
    echiquier[7][5] = 'F';
    echiquier[7][6] = 'C';
    echiquier[7][7] = 'T';

    for(i = 0;i < 8;i++){
        for(j = 0;j < 8;j++){
            printf("%c\t", echiquier[i][j]);
        }
        printf("\n");

    }*/

    init();
    return 0;
}


