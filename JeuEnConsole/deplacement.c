/**
 * @Author: Moustapha KEBE <kebson>
 * @Date:   2017-10-12T11:33:21+02:00
 * @Email:  mktapha@gmail.com
 * @Last modified by:   kebson
 * @Last modified time: 2017-10-26T04:16:12+02:00
 */



#include "chesslib.h"

static unsigned deplcmtTotal;

typedef struct PDeplacementNoeud {
	char move[4];
	struct PDeplacementNoeud *nxt;
} PDeplacementNoeud;


PDeplacementNoeud *AjouterDeplacement(PDeplacementNoeud *racine, char m[4])
{
	deplcmtTotal++;
	if (!racine) {
		racine = malloc(sizeof(PDeplacementNoeud));
		memcpy(racine->move, m, 4);
		racine->nxt = NULL;
		return racine;
	}
	PDeplacementNoeud *new = malloc(sizeof(PDeplacementNoeud));
	memcpy(new->move, m, 4);
	new->nxt = racine;
	return new;
}

#ifndef NDEBUG
void afficherList(PDeplacementNoeud *racine)
{
	if (!racine)
		return;
	do {
		printf("%s\n", racine->move);
		racine = racine->nxt;
	} while (racine);
}
#endif

void freeList(PDeplacementNoeud *racine)
{
	PDeplacementNoeud *temp;
	while (racine != NULL) {
		temp = racine;
		racine = racine->nxt;
		free(temp);
	}
}
// un peu d'intelligence artif
PDeplacementNoeud *getDeplacements(PDeplacementNoeud *AIlist, templateEchiquier chb[][8])
{
	int i, j, k, l;
	char tmp[4];

	tmp[3] = '\0';
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if(chb[i][j].c == BLACK) {
				tmp[0] = chb[i][j].courant;
				if (chb[i][j].courant == 'P') {
					tmp[2] = i-1 + '1';
					if (chb[i-1][j].occ == false) {
						tmp[1] = j + 'A';
						AIlist = AjouterDeplacement(AIlist, tmp);
					}
					if (chb[i-1][j+1].occ == true && chb[i-1][j+1].c != BLACK) {
						tmp[1] = j+1 + 'A';
						AIlist = AjouterDeplacement(AIlist, tmp);
					}
					if (chb[i-1][j-1].occ == true && chb[i-1][j-1].c != BLACK) {
						tmp[1] = j-1 + 'A';
						AIlist = AjouterDeplacement(AIlist, tmp);
					}
				}
				if (chb[i][j].courant == 'B' || chb[i][j].courant == 'Q') {
					k = i - 1;
					l = j - 1;
					while ((k <= 7 && k >= 0) && (l >= 0 && l <= 7)) {
						if (chb[k][l].c != chb[i][j].c) {
							tmp[1] = l + 'A';
							tmp[2] = k + '1';
							AIlist = AjouterDeplacement(AIlist, tmp);
						}
						if (chb[k][l].occ == true) break;
						k--;
						l--;
					}
					k = i - 1;
					l = j + 1;
					while ((k >= 0 && k <= 7) && (l <= 7 && l >= 0)) {
						if (chb[k][l].c != chb[i][j].c) {
							tmp[1] = l + 'A';
							tmp[2] = k + '1';
							AIlist = AjouterDeplacement(AIlist, tmp);
						}
						if (chb[k][l].occ == true) break;
						k--;
						l++;
					}
					k = i + 1;
					l = j - 1;
					while ((k <= 7 && k >= 0) && (l >= 0 && l <= 7)) {
						if (chb[k][l].c != chb[i][j].c) {
							tmp[1] = l + 'A';
							tmp[2] = k + '1';
							AIlist = AjouterDeplacement(AIlist, tmp);
						}
						if (chb[k][l].occ == true) break;
						k++;
						l--;
					}
					k = i + 1;
					l = j + 1;
					while ((k <= 7 && k >= 0) && (l <= 7 && l >= 0)) {
						if (chb[k][l].c != chb[i][j].c) {
							tmp[1] = l + 'A';
							tmp[2] = k + '1';
							AIlist = AjouterDeplacement(AIlist, tmp);
						}
						if (chb[k][l].occ == true) break;
						k++;
						l++;
					}
				}
				if (chb[i][j].courant == 'N') {
					int knightrow[] = {i-2,i-2,i-1,i-1,i+1,i+1,i+2,i+2};
					int knightcol[] = {j-1,j+1,j-2,j+2,j-2,j+2,j-1,j+1};
					int count = 0;
					for (; count < 8; count++) {
						if (chb[knightrow[count]][knightcol[count]].c != chb[i][j].c) {
							if (knightrow[count] > -1 && knightrow[count] < 8
								&& knightcol[count] > -1 && knightcol[count] < 8) {
								tmp[1] = knightcol[count] + 'A';
								tmp[2] = knightrow[count] + '1';
								AIlist = AjouterDeplacement(AIlist, tmp);
							}
						}
					}
				}
				if (chb[i][j].courant == 'R' || chb[i][j].courant == 'Q') {
					k = i;
					for (l = j+1; l <= 7; l++) {
						if (chb[k][l].c == chb[i][j].c)
							break;
						tmp[1] = chb[k][l].square[0];
						tmp[2] = chb[k][l].square[1];
						AIlist = AjouterDeplacement(AIlist, tmp);
						if (chb[k][l].occ == true)
							break;
					}
					for (l = j-1; l >= 0; l--) {
						if (chb[k][l].c == chb[i][j].c)
							break;
						tmp[1] = chb[k][l].square[0];
						tmp[2] = chb[k][l].square[1];
						AIlist = AjouterDeplacement(AIlist, tmp);
						if (chb[k][l].occ == true)
							break;
					}

					l = j;
					for (k = i+1; k <= 7; k++) {
						if (chb[k][l].c == chb[i][j].c)
							break;
						tmp[1] = chb[k][l].square[0];
						tmp[2] = chb[k][l].square[1];
						AIlist = AjouterDeplacement(AIlist, tmp);
						if (chb[k][l].occ == true)
							break;
					}
					for (k = i-1; k >= 0; k--) {
						if (chb[k][l].c == chb[i][j].c)
							break;
						tmp[1] = chb[k][l].square[0];
						tmp[2] = chb[k][l].square[1];
						AIlist = AjouterDeplacement(AIlist, tmp);
						if (chb[k][l].occ == true)
							break;
					}
				}
				if (chb[i][j].courant == 'K') {
					for (k = i - 1; k < i + 2; k++){
						for (l = j - 1; l < j + 2; l++){
							if ((i == k && j == l) || (chb[k][l].c == chb[i][j].c)
								|| k > 7 || k < 0 || l > 7 || l < 0)
								continue;
							tmp[1] = chb[k][l].square[0];
							tmp[2] = chb[k][l].square[1];
							AIlist = AjouterDeplacement(AIlist, tmp);
						}
					}
				}
				if (check_castling.KBlack) {
					if (check_castling.BR_right && !piecesOverlap(chb, 7, ('H'-'A'), 7, 4, 'R'))
						AIlist = AjouterDeplacement(AIlist, "KG8");
					if (check_castling.BR_left && !piecesOverlap(chb, 7, ('A'-'A'), 7, 4, 'R'))
						AIlist = AjouterDeplacement(AIlist, "KC8");
				}
			}
		}
	}
	return AIlist;
}

char *getDeplcmtAleatoire(PDeplacementNoeud *racine)
{
	char *d = malloc(4);
	unsigned short t = 1, s;
	srand(time(NULL));
	s = rand()%deplcmtTotal;
	while (racine!=NULL) {
		if (s == t) {
			memcpy(d, racine->move, 4);
			break;
		}
		racine = racine->nxt;
		t++;
	}
	return d;
}

char *DeplacementOrdinateur(templateEchiquier chb[][8])
{
	PDeplacementNoeud *AIl = NULL;
	char *valeurDeRetour = malloc(4), *temp = malloc(4);

	deplcmtTotal = 0;
	AIl = getDeplacements(AIl, chb);
	memcpy(temp, getDeplcmtAleatoire(AIl), 4);
	strcpy(valeurDeRetour,temp);
	free(temp);
	freeList(AIl);
	return valeurDeRetour;
}
