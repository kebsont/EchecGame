#include <stdio.h>
#include <stdlib.h>

typedef struct {
   char row;		//position ligne
   char col;		//position colonne
   char fond;		//couleur de la case b=blanc n=noir
   char piece;	//v=vide p=pion t=tour c=cheval f=fou d=dame r=roi
   char couleur;	//couleur de la pièce 'n'=noir, 'b'=blanc
} cases;

//boolean flag;
 //L'échiquier est une matrice de 8x8 soit 64 cases
cases *Echiquier[8][8];

void fenetrePrincipale(){
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      Echiquier[row][col] = (cases*) malloc(sizeof(cases));
      Echiquier[row][col]->row		= row;
      Echiquier[row][col]->col		= col;
      //Echiquier[row][col]->fond		= (flag)?'b':'n';
      Echiquier[row][col]->piece	= 'v'; // vide
      Echiquier[row][col]->couleur	= 'v'; // vide
      printf("%c\n", Echiquier[row][col]->piece);
    }
  }
}

int main(int argc, char const *argv[]) {

  fenetrePrincipale();
  return 0;
}
