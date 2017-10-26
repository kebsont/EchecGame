/**
 * @Author: Moustapha KEBE <kebson>
 * @Date:   2017-10-12T11:55:27+02:00
 * @Email:  mktapha@gmail.com
 * @Last modified by:   kebson
 * @Last modified time: 2017-10-26T04:14:03+02:00
 */



#include "chesslib.h"


//prototypes pour les fonctions utilises ici

//Verifier si le Roi est menace par un epiece ennemie, returns true si c'est vrai*/
static bool KingEstMenace(const int, const int, const int, const int, const char, const int, templateEchiquier[][8]);

/*retirer king's life si une piece peut se deplacer autour de lui*/
static bool k_domain_ctrl(const int, const int, const int, const int, const int, const char);

/*vérifie si un roi est capturé dans le prochain coup, mettant ainsi fin au jeu
  ou s'il est sur le point d'être capturé en se déplaçant sur un certain carré */
static void echecEtMat(EtatDuRoi**, EtatDuRoi**);


/* est appelé deux fois dans findEtatKing avec les coordonnées de chaque roi et stocke
 * les mouvements possibles qu'un Roi peut faire, pendant le contrôle */
void getDeplacementKing(templateEchiquier [][8], int, int, int);


/* la vie de chaque roi est mesurée dans son domaine libre
 * Note: voir
 AIlist = AjouterDeplacement(AIlist, tmp);comment les valeurs de ces variables sont modifiées tout au long de
 * le jeu à comprendre; un bon débogueur aidera avec cela
 * 0 si le Roi peut se déplacer sur cette case
 * 1 si une attaque est imminente sur cette case
 * 2 si un carré adjacent a
}
if (chb[i-1][j+1].occ == true && chb[i-1][j+1].c != BLACK) {
	tmp[1] = j+1 + 'A';
	AIlist = AjouterDeplacement(AIlist, tmp);une position amicale
 * 3 pour les carrés qui ne sont pas visibles lorsque le roi est sur les bords */
static short WKingLife[3][3]; /*energy of white King*/
static short BKingLife[3][3]; /*energy of black King*/


/* si l'état d'un roi est check ou sauve_check ces chaînes stockent les mouvements possibles du Roi
 * peut faire des paires de caractères; par exemple "A8 H4 P3", "F2 B0", etc.) */
char *DeplacementWKing = NULL;
char *DeplacementBKing = NULL;

CastlingBool check_castling = ALL_CASTL_TRUE;


/* compteur pour le total des parties; un tour de jeu se termine lorsque Black termine son déplacement */
static unsigned short rc = 1;

bool cstl_is_enabled = false;

typedef struct KingDomain {
	int x;
	int y;
} KingDomain;


/********************
 *ChessLib functions*
 ********************/
extern void clear_buffer(void)
{
	char clbuf;
	while ((clbuf=getchar()) != '\n');
}

void _initiaiiserEchiquier(templateEchiquier chb[][8], unsigned k, char col)	/*k ligne / col  colonne*/
{
	if (k == 0 || k == 7) {
		if (col == 'A' || col == 'H')
			chb[k][col - 'A'].courant = 'R';
			//sprintf(chb[k][col - 'A'].ches ,"%s", "\u2657");
		else if (col == 'B' || col == 'G')
			chb[k][col - 'A'].courant = 'N';
		else if (col == 'C' || col == 'F')
			chb[k][col - 'A'].courant = 'B';
		else if (col == 'D')
			chb[k][col - 'A'].courant = 'Q';
		else
			chb[k][col - 'A'].courant = 'K';
		if (k == 0)
			chb[k][col - 'A'].c = WHITE;	/*colorier les pieces*/
		else
			chb[k][col - 'A'].c = BLACK;
		chb[k][col - 'A'].occ = true;
		chb[k][col - 'A'].square[0] = col;
		chb[k][col - 'A'].square[1] = k + '1';
		col = col + 1;
	} else if (k == 1 || k == 6) {
		if(k == 1)
			chb[k][col - 'A'].c = WHITE;
		else
			chb[k][col - 'A'].c = BLACK;
		chb[k][col - 'A'].courant = 'P';
		chb[k][col - 'A'].occ = true;
		chb[k][col - 'A'].square[0] = col;
		chb[k][col - 'A'].square[1] = k + '1';
		col = col + 1;
	} else if (k >= 2 && k <= 5) {
		chb[k][col - 'A'].c = EMPTY;
		chb[k][col - 'A'].courant = 'e';
		chb[k][col - 'A'].occ = false;
		chb[k][col - 'A'].square[0] = col;
		chb[k][col - 'A'].square[1] = k + '1';
		col = col + 1;
	}
	if (col == 'I') {
		++k;
		col = 'A';
	}
	if (k != 8)
		_initiaiiserEchiquier(chb, k, col);
}

void printBoard(templateEchiquier chb[][8], const char p)
{
#ifdef _WIN32
	HANDLE cmdhandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO cmdinfo;
	WORD sv_att;
	int i, j, max, y = 0, x = 0;

	GetConsoleScreenBufferInfo(cmdhandle, &cmdinfo);
	sv_att = cmdinfo.wAttributes;

	printf("    a   b   c   d   e   f   g   h  \n");
	for (i = 0; i < (max = (MOS%2)?MOS:(MOS-1)); i++) {
		if(i%2 == true) printf("%d ", y + 1);
		else printf("  ");
		for (j = 0; j < max; j++) {
			if (!i) {
				if(!j) printf("%c", 218);
				else if(j == (max-1)) printf("%c", 191);
				else if(j%2) printf("%c%c%c", 196,196,196);
				else if(!(j%2)) printf("%c", 194);
			} else if (i == max-1) {
				if(!j)printf("%c", 192);
				else if(j == (max-1)) printf("%c", 217);
				else if(j%2) printf("%c%c%c", 196,196,196);
				else if(!(j%2)) printf("%c", 193);
			} else if (i%2) {
				if(!(j%2)) printf("%c", 179);
				else {
					if (chb[y][x].occ == false)
						printf("   ");
					else {
						if (chb[y][x].c == BLACK)
							SetConsoleTextAttribute(cmdhandle, FOREGROUND_RED);
						else
							SetConsoleTextAttribute(cmdhandle, FOREGROUND_GREEN);
						printf(" %c ", chb[y][x].courant);
						SetConsoleTextAttribute(cmdhandle, sv_att);
					}
					x++;
				}
			} else if (!(i%2)) {
				if(!j) printf("%c", 195);
				else if(j == (max-1)) printf("%c", 180);
				else if(j%2) printf("%c%c%c", 196,196,196);
				else if(!(j%2)) printf("%c", 197);
			}
		}
		x = 0;
		if(i%2 == true) {
			printf(" %d", y + 1);
			y++;
		} else
			printf("  ");
		printf("\n");
	}
	printf("    a   b   c   d   e   f   g   h  \n");
	printf("\n");
#else
	int i, j, max, y = 0, x = 0;

		printf("   \033[1m a   b   c   d   e   f   g   h \033[0m \n");
	for (i = 0; i < (max = (MOS%2)?MOS:(MOS-1)); i++) {
		if(i%2 == true) printf("\033[1m%d\033[0m ", y + 1);
		else printf("  ");
		for (j = 0; j < max; j++) {
			if (!i) {
				if(!j) printf("\u250F");
				else if(j == (max-1)) printf("\u2513");
				else if(j%2) printf("\u2501\u2501\u2501");
				else if(!(j%2)) printf("\u2533");
			} else if (i == max-1) {
				if(!j)printf("\u2517");
				else if(j == (max-1)) printf("\u251B");
				else if(j%2) printf("\u2501\u2501\u2501");
				else if(!(j%2)) printf("\u253B");
			} else if (i%2) {
				if(!(j%2)) printf("\u2503");
				else {
					/*usleep(3000);
					fflush(stdout);*/
					if (chb[y][x].occ == false)
						printf("   ");
					else {
						if (p == 'a') {
							if (chb[y][x].c == BLACK)
								printf(" %s%c%s ", KRED, chb[y][x].courant, RESET);
							else
								printf(" %s%c%s ", KYEL, chb[y][x].courant, RESET);
						} else {
							if (chb[y][x].courant == 'P') {
								if (chb[y][x].c == BLACK)
									printf(" \u265F ");
								else
									printf(" \u2659 ");
							} else if (chb[y][x].courant == 'Q') {
								if (chb[y][x].c == BLACK)
									printf(" \u265B ");
								else
									printf(" \u2655 ");
							} else if (chb[y][x].courant == 'B') {
								if (chb[y][x].c == BLACK)
									printf(" \u265D ");
								else
									printf(" \u2657 ");
							} else if (chb[y][x].courant == 'R') {
								if (chb[y][x].c == BLACK)
									printf(" \u265C ");
								else
									printf(" \u2656 ");
							} else if (chb[y][x].courant == 'N') {
								if (chb[y][x].c == BLACK)
									printf(" \u265E ");
								else
									printf(" \u2658 ");
							} else if (chb[y][x].courant == 'K') {
								if (chb[y][x].c == BLACK)
									printf(" \u265A ");
								else
									printf(" \u2654 ");
							}
						}
					}
					x++;
				}
			} else if (!(i%2)) {
				if(!j) printf("\u2523");
				else if(j == (max-1)) printf("\u252B");
				else if(j%2) printf("\u2501\u2501\u2501");
				else if(!(j%2)) printf("\u254B");
			}
		}
		x = 0;
		if(i%2 == true) {
			printf(" \033[1m%d\033[0m", y + 1);
			y++;
		} else
			printf("  ");
		printf("\n");
	}
	printf("   \033[1m a   b   c   d   e   f   g   h \033[0m \n");
	printf("\n");
#endif
}

bool entreeValide(const char *input, int *errPtr)
{
	if (strlen(input) > 3) {
		return false;
	}
	if (!strchr("rnbqkpRNBQKP", *input)) {
		*errPtr = 5;
		return false;
	}
	if (!strchr("abcdefghABCDEFGH", input[1])) {
		*errPtr = 6;
		return false;
	}
	if (!strchr("12345678", input[2])) {
		*errPtr = 7;
		return false;
	}
	return true;
}

void printError(int i)
{
	char *error_out[] = { "\n",
	"Ligne de commande non reconnue.\n", "Mauvaise ENTREE\n",
	"Deplacement Illegal.\n", "Fichier de Log ne peut pas etre cree.\n"};

	if (i < 5)
		fprintf(stderr, "%s", error_out[i]);
	else {
		if (i == 5)
			fprintf(stderr, "%s\n%s%s\n\t%s\n\t%s\n\t%s\n",
			"Piece entree invalide. ",
			"Utilisez seulement R/r pour Roque, ", "N/n pour Chevalier,",
			"B/b pour Fou,", "Q/q pour Reine,", "K/k pour Roi and P/p pour Pion");
		else if (i == 6)
			fprintf(stderr, "%s\n",
			"Lettre de colonne invalide. Les lettres legales sont [a-h] ou [A-H].");
		else if (i == 7)
			fprintf(stderr, "%s\n",
			"Lettre de ligne invalide. Utilisez seulement les nombres de 1 a 8.");
		else if (i == 8)
			fprintf(stdout, "%s%s\n%s %s\n", "ChessLib-", CHESSGAMECLI_VERSION_STRING,
				"Fin ",
				"Test");
		else
			fprintf(stderr, "%s%d.\n",
			"Numero d'erreur non reconnu ", i);
	}
}

char *findPiece(templateEchiquier chb[][8], const char *input, int color)
{
	int i, j, k, l, count, conflict = 3;
	/*Note: Conflict is a bool to check for two pieces attacking the same piece.
	 *The chess board is scanned twice to see if another piece that can do the same move
	 *is found; on the first piece conflict is false, if it finds a second piece it becomes true.*/
	char *retvalue = calloc(3, sizeof(*retvalue));
	char *temp = calloc(3, sizeof(*temp));

	if (!retvalue)
		exit(0);
	retvalue[2] = '\0';
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (chb[i][j].courant == input[0] && chb[i][j].c == color) {
				retvalue[0] = chb[i][j].square[0];
				retvalue[1] = chb[i][j].square[1];
				if (input[0] == 'P'){
					l = input[2] - '1';
					k = input[1] - 'A';
					if (color == BLACK) {
						if ((i == 6) && (l == i - 2) && (j == k) && (chb[l][k].occ == false) && (chb[i-1][j].occ == false))
							return retvalue;	/*check for pawn's double first move*/
						if ((i-1) == 0 && chb[i-1][j].square[0] == input[1] && chb[i-1][j].square[1] == input[2] && chb[i-1][j].occ == false) {
							return retvalue;
						}
						if ((i-1) == 0 && chb[i-1][j+1].square[0] == input[1] && chb[i-1][j+1].square[1] == input[2] &&
							chb[i-1][j+1].occ == true && chb[i-1][j+1].c != color) {
							return retvalue;
						}
						if ((i-1) == 0 && chb[i-1][j-1].square[0] == input[1] && chb[i-1][j-1].square[1] == input[2] &&
							chb[i-1][j-1].occ == true && chb[i-1][j-1].c != color) {
							return retvalue;
						}
						if (chb[i-1][j].occ == false) {
							if (chb[i-1][j].square[0] == input[1] && chb[i-1][j].square[1] == input[2]) {
								return retvalue;
							}
						}
						if (chb[i-1][j+1].occ == true) {
							if (chb[i-1][j+1].square[0] == input[1] && chb[i-1][j+1].square[1] == input[2]) {
								if (temp[0] != retvalue[0] && temp[1] == retvalue[1]) {
									conflict = true;
									goto EXIT_LOOP;
								}
								conflict = false;
								memcpy(temp,retvalue,3);
							}
						}
						if (chb[i-1][j-1].occ == true) {
							if (chb[i-1][j-1].square[0] == input[1] && chb[i-1][j-1].square[1] == input[2]) {
								if (temp[0] != retvalue[0] && temp[1] == retvalue[1]) {
									conflict = true;
									goto EXIT_LOOP;
								}
								conflict = false;
								memcpy(temp,retvalue,3);
							}
						}
					} else {
						if ((i == 1) && (l == i + 2) && (j == k) && (chb[l][k].occ == false) && (chb[i+1][j].occ == false))
							return retvalue;	/*check for pawn's double first move*/
						if ((i+1) == 0 && chb[i+1][j].square[0] == input[1] && chb[i+1][j].square[1] == input[2] && chb[i+1][j].occ == false) {
							return retvalue;
						}
						if ((i+1) == 0 && chb[i+1][j+1].square[0] == input[1] && chb[i+1][j+1].square[1] == input[2] &&
							chb[i+1][j+1].occ == true && chb[i+1][j+1].c != color) {
							return retvalue;
						}
						if ((i+1) == 0 && chb[i+1][j-1].square[0] == input[1] && chb[i+1][j-1].square[1] == input[2] &&
							chb[i+1][j-1].occ == true && chb[i-1][j-1].c != color) {
							return retvalue;
						}
						if (chb[i+1][j].occ == false){
							if (chb[i+1][j].square[0] == input[1] && chb[i+1][j].square[1] == input[2]) {
								return retvalue;
							}
						}
						if (chb[i+1][j+1].occ == true){
							if (chb[i+1][j+1].square[0] == input[1] && chb[i+1][j+1].square[1] == input[2]) {
								if (temp[0] != retvalue[0] && temp[1] == retvalue[1]) {
									conflict = true;
									goto EXIT_LOOP;
								}
								conflict = false;
								memcpy(temp,retvalue,3);
							}
						}
						if (chb[i+1][j-1].occ == true){
							if (chb[i+1][j-1].square[0] == input[1] && chb[i+1][j-1].square[1] == input[2]) {
								if (temp[0] != retvalue[0] && temp[1] == retvalue[1]) {
									conflict = true;
									goto EXIT_LOOP;
								}
								conflict = false;
								memcpy(temp,retvalue,3);
							}
						}
					}
					continue;
				} else if (input[0] == 'K') {
					if (chb[i][j].c == BLACK && check_castling.KBlack) {
						if ((input[1] == 'G' && input[2] == '8') && check_castling.BR_right) {
							if(!piecesOverlap(chb, 7, ('H'-'A'), i, j, 'R')) {
								check_castling.KBlack = false;
								check_castling.BR_right = false;
								cstl_is_enabled = true;
								return retvalue;
							}
						} else if ((input[1] == 'C' && input[2] == '8') && check_castling.BR_left) {
							if(!piecesOverlap(chb, 7, ('A'-'A'), i, j, 'R')) {
								check_castling.KBlack = false;
								check_castling.BR_left = false;
								cstl_is_enabled = true;
								return retvalue;
							}
						}
					} else if (chb[i][j].c == WHITE && check_castling.KWhite) {
						if ((input[1] == 'G' && input[2] == '1') && check_castling.WR_right) {
							if(!piecesOverlap(chb, 0, ('H'-'A'), i, j, 'R')) {
								check_castling.KWhite = false;
								check_castling.WR_right = false;
								cstl_is_enabled = true;
								return retvalue;
							}
						} else if ((input[1] == 'C' && input[2] == '1') && check_castling.WR_left) {
							if(!piecesOverlap(chb, 0, ('A'-'A'), i, j, 'R')) {
								check_castling.KWhite = false;
								check_castling.WR_left = false;
								cstl_is_enabled = true;
								return retvalue;
							}
						}
					}
					for (k = i - 1; k < i + 2; k++){
						for (l = j - 1; l < j + 2; l++){
							if (chb[k][l].square[0] == input[1] && chb[k][l].square[1] == input[2]) {
								return retvalue;
							}
						}
					}
				} else if (input[0] == 'N') {
					int knightrow[] = {i-2,i-2,i-1,i-1,i+1,i+1,i+2,i+2};
					int knightcol[] = {j-1,j+1,j-2,j+2,j-2,j+2,j-1,j+1};
					l = input[2] - '1';
					k = input[1] - 'A';
					for (count = 0; count < 8; count++) {
						if (knightrow[count] == l && knightcol[count] == k) {
							if (conflict == false) {
								conflict = true;
								goto EXIT_LOOP;
							}
							conflict = false;
							memcpy(temp,retvalue,3);
						}
					}
				} else if (input[0] == 'R' || input[0] == 'Q') {
					k = i;
					for (l=0; l < 8; l++) {
						if (l == j)	/*to skip the piece itself*/
							continue;
						if (chb[k][l].square[0] == input[1] && chb[k][l].square[1] == input[2]) {
							if (input[0] == 'R') {
								if (conflict == false) {
									conflict = true;
									goto EXIT_LOOP;
								}
								conflict = false;
								memcpy(temp,retvalue,3);
							} else {
								return retvalue;
							}
						}
					}
					l = j;
					for (k = 0; k < 8; k++) {
						if (k == i)
							continue;
						if (chb[k][l].square[0] == input[1] && chb[k][l].square[1] == input[2]) {
							if (input[0] == 'R') {
								if (chb[i][j].square[0] == 'D') {
									if (chb[i][j].c == BLACK)
										check_castling.BR_left = false;
									else
										check_castling.WR_left = false;
								} else if (chb[i][j].square[0] == 'F') {
									if (chb[i][j].c == BLACK)
										check_castling.BR_right = false;
									else
										check_castling.WR_right = false;
								}
								if (conflict == false) {
									conflict = true;
									goto EXIT_LOOP;
								}
								conflict = false;
								memcpy(temp,retvalue,3);
							} else {
								return retvalue;
							}
						}
					}
				}
				/*different if, to check for queen's diagonal moves*/
				if (input[0] == 'B' || input[0] == 'Q') {
					k = i - 1;
					l = j - 1;
					while ((k <= 7 && k >= 0) && (l >= 0 && l <= 7)) {
						if (chb[k][l].square[0] == input[1] && chb[k][l].square[1] == input[2]) {
							if (input[0] == 'B') {
								if (conflict == false) {
									conflict = true;
									goto EXIT_LOOP;
								}
								conflict = false;
								memcpy(temp,retvalue,3);
							} else {
								return retvalue;
							}
						}
						k--;
						l--;
					}
					k = i - 1;
					l = j + 1;
					while ((k >= 0 && k <= 7) && (l <= 7 && l >= 0)) {
						if (chb[k][l].square[0] == input[1] && chb[k][l].square[1] == input[2]) {
							if (input[0] == 'B') {
								if (conflict == false) {
									conflict = true;
									goto EXIT_LOOP;
								}
								conflict = false;
								memcpy(temp,retvalue,3);
							} else {
								return retvalue;
							}
						}
						k--;
						l++;
					}
					k = i + 1;
					l = j - 1;
					while ((k <= 7 && k >= 0) && (l >= 0 && l <= 7)) {
						if (chb[k][l].square[0] == input[1] && chb[k][l].square[1] == input[2]) {
							if (input[0] == 'B') {
								if (conflict == false) {
									conflict = true;
									goto EXIT_LOOP;
								}
								conflict = false;
								memcpy(temp,retvalue,3);
							} else {
								return retvalue;
							}
						}
						k++;
						l--;
					}
					k = i + 1;
					l = j + 1;
					while ((k <= 7 && k >= 0) && (l <= 7 && l >= 0)) {
						if (chb[k][l].square[0] == input[1] && chb[k][l].square[1] == input[2]) {
							if (input[0] == 'B') {
								if (conflict == false) {
									conflict = true;
									goto EXIT_LOOP;
								}
								conflict = false;
								memcpy(temp,retvalue,3);
							} else {
								return retvalue;
							}
						}
						k++;
						l++;
					}
				}
			}
		}
	}
	EXIT_LOOP:
	if (conflict == true) {
		retvalue = realloc(retvalue, 5);
		retvalue[2] = temp[0];
		retvalue[3] = temp[1];
		retvalue[4] = '\0';
		free(temp);
		return retvalue;
	} else if(conflict == false) {
		return temp;
	}
	return NULL;
}

extern void printInstructions(void)
{
	printf("%s%s\n%s\n%s\n%s\n%s\n%s %s\n%s\n",
"Entrez votre mouvement dans le format suivant:", "\'xyz \'",
"x est la pièce que vous voulez déplacer",
"y est la lettre de la colonne et",
"z est le numéro de la ligne.",
"Les valeurs acceptables pour x sont: R/r pour Rook, N/n pour Knight, B/b pour Bishop, Q/q pour Queen, K/k pour Roi et P/p pour Pawn.",
"Les Pa6valeurs acceptables pour y sont des minuscules",
"de \'a \' à \'h \' et pour les nombres z de 1 à 8.",
"Par exemple, pour déplacer Bishop sur e2, tapez Be2 ou Pawn sur a4 tapez Pa4.");
}

extern void clear_screen(void)
{
#ifndef _WIN32
	system("clear");
#else
	system("cls");
#endif
	/*puts( "\033[2J" );
	 *Note: Effacer l'écran en utilisant ASCII; ça n'a pas l'air si bon.
	 * On lutiliz ke si on a pas installer libncurses et
	 * ne pas oublier de supprimer ou de commenter les lignes juste ci-dessus*/
}

void setCastling(templateEchiquier chb[][8], char *plInput, int color)
{
	int row, kcol_start = 4, kcol_end, rcol_start, rcol_end;

	if (color == WHITE)
		row = 0;
	else
		row = 7;
	if (plInput[1] == 'C' || plInput[1] == 'D') {
		kcol_end = 2;
		rcol_start = 0;
		rcol_end = 3;
	} else {
		kcol_end = 6;
		rcol_start = 7;
		rcol_end = 5;
	}
	chb[row][kcol_start].occ = chb[row][rcol_start].occ = false;
	chb[row][kcol_start].c = chb[row][rcol_start].c = EMPTY;
	chb[row][kcol_start].courant = chb[row][rcol_start].courant = 'e';
	chb[row][kcol_end].occ = chb[row][rcol_end].occ = true;
	chb[row][kcol_end].c = chb[row][rcol_end].c = color;
	chb[row][kcol_end].courant = 'K';
	chb[row][rcol_end].courant = 'R';
	/*cstl_is_enabled = false;*/
}

bool verifiValideDeplacement(templateEchiquier chb[][8], char *plInput, char piece[2], int color)
{
	templateEchiquier nxt_chb[8][8];
	EtatDuRoi nxtWK = safe, nxtBK = safe;
	int i = 0;
	for(; i < 8; i++)
		memcpy(&nxt_chb[i], &chb[i], sizeof(chb[i]));

	if (!DeplacerPiece(nxt_chb, plInput, piece, color))
		return false;
	findEtatKing(nxt_chb, &nxtWK, &nxtBK);
	if (nxtWK != check && nxtBK != check)
		return true;
	return false;
}

bool DeplacerPiece(templateEchiquier chb[][8], char *plInput, char piece[2], int color)
{
	int startx, starty, endx, endy;

	endx = plInput[2] - '1';
	endy = plInput[1] - 'A';
	startx = piece[1] - '1';
	starty = piece[0] - 'A';
	if (!piecesOverlap(chb, startx, starty, endx, endy, plInput[0])) {
		/*Note: Si un morceau de la même couleur occupe le carré de votre
		* la pièce est sur le point de bouger, le déplacement n'est pas valide et DeplacerPiece renvoie false.*/
		if (chb[endx][endy].c != color) {
			if (chb[startx][starty].courant == 'K' || chb[startx][starty].courant == 'R') {
				if (startx == 0) {
					if (starty == 0)
						check_castling.WR_left = false;
					else if (starty == 7)
						check_castling.WR_right = false;
				} else if (startx == 7) {
					if (starty == 0)
						check_castling.BR_left = false;
					else if (starty == 7)
						check_castling.BR_right = false;
				}
				if (plInput[0] == 'K') {
					if (color == BLACK)
						check_castling.KBlack = false;
					else
						check_castling.KWhite = false;
				}
			}
			chb[endx][endy].occ = true;
			chb[endx][endy].courant = chb[startx][starty].courant;
			chb[endx][endy].c = color;
			chb[startx][starty].occ = false;
			chb[startx][starty].c = EMPTY;
			chb[startx][starty].courant = 'e';
			if (plInput[0] == 'P' && ((startx == 1 && endx == 0) || (startx == 6 && endx == 7))) {
				/*options for the promotion of the pawn*/
				char *promote_selection;
				RETRY:
				printf("Quelle piece voulez-vous utiliser dans votre Pion ? ");
				promote_selection = getPlayerInput();
				if (!strcmp(promote_selection, "queen") || !strcmp(promote_selection, "Q") ||
					!strcmp(promote_selection, "QUEEN") || !strcmp(promote_selection, "q")) {
					chb[endx][endy].courant = 'Q';
				} else if (!strcmp(promote_selection, "bishop") || !strcmp(promote_selection, "B") ||
					!strcmp(promote_selection, "BISHOP") || !strcmp(promote_selection, "b")) {
					chb[endx][endy].courant = 'B';
				} else if (!strcmp(promote_selection, "knight") || !strcmp(promote_selection, "N") ||
					!strcmp(promote_selection, "KNIGHT") || !strcmp(promote_selection, "n")) {
					chb[endx][endy].courant = 'N';
				} else {
					free(promote_selection);
					printError(2);
					goto RETRY;
				}
				free(promote_selection);
			}
			return true;
		}
	}
	return false;
}

bool piecesOverlap(templateEchiquier chb[][8], const int sx, const int sy,
		const int ex, const int ey, const char piece)
{
	int tempx = sx, tempy = sy;

	if (piece == 'R' || (piece == 'Q' && (sx == ex || sy == ey))) {
		if (!(sx-ex)) {
			if (sy > ey) {
				tempy--;
				for (; tempy > ey; tempy--) {
					if (chb[sx][tempy].occ)
						return true;
				}
			} else {
				tempy++;
				for (; tempy < ey; tempy++) {
					if (chb[sx][tempy].occ)
						return true;
				}
			}
		} else {
			if (sx > ex) {
				tempx--;
				for (; tempx > ex; tempx--) {
					if (chb[tempx][sy].occ)
						return true;
				}
			} else {
				tempx++;
				for (; tempx < ex; tempx++) {
					if (chb[tempx][sy].occ)
						return true;
				}
			}
		}
	}
	if (piece == 'B' || piece == 'Q') {
		tempx = ex;
		tempy = ey;
		if (ex>sx && ey>sy) {
			tempx--;
			tempy--;
			while (tempx > sx && tempy > sy) {
				if (chb[tempx--][tempy--].occ)
					return true;
			}
		} else if (ex<sx && ey>sy) {
			tempx++;
			tempy--;
			while (tempx < sx && tempy > sy) {
				if (chb[tempx++][tempy--].occ)
					return true;
			}
		} else if (ex>sx && ey<sy) {
			tempx--;
			tempy++;
			while (tempx > sx && tempy < sy) {
				if (chb[tempx--][tempy++].occ)
					return true;
			}
		} else {
			tempx++;
			tempy++;
			while (tempx < sx && tempy < sy) {
				if (chb[tempx++][tempy++].occ)
					return true;
			}
		}
	}
	return false;
}

void findEtatKing(templateEchiquier chb[][8], EtatDuRoi *WK, EtatDuRoi *BK)
{
	int i, j, WKx = -1, WKy, BKx = -1, BKy;

	/*Note: Les domaines des deux rois sont remis à zéro après chaque tour
* recalculer leurs valeurs en fonction du nouvel état du tableau.*/
	memset(WKingLife, 0, 9*sizeof(short));
	memset(BKingLife, 0, 9*sizeof(short));
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (chb[i][j].courant == 'K') {
				if (chb[i][j].c == WHITE) {
					WKx = i;
					WKy = j;
				} else {
					BKx = i;
					BKy = j;
				}
			}
		}
	}
	if (WKx == -1) {	/*hack temporaire pour mettre fin au jeu si le roi est capturé*/
		*WK = checkmate;
		return;
	} else if (BKx == -1) {
		*BK = checkmate;
		return;
	}

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (chb[i][j].c == BLACK) {
				if (chb[i][j].courant == 'P') {
					k_domain_ctrl(i,j,WKx,WKy,chb[i][j].c, 'e');
					k_domain_ctrl(i-1,j+1,WKx,WKy,chb[i][j].c, 'e');
					k_domain_ctrl(i-1,j-1,WKx,WKy,chb[i][j].c, 'e');
					k_domain_ctrl(i,j,BKx,BKy,chb[i][j].c, 'f');
				}
				if (chb[i][j].courant == 'R' || chb[i][j].courant == 'Q') {
					if ((WKx == i) && (piecesOverlap(chb,i,j,WKx,WKy,chb[i][j].courant) == false)) {
						*WK = check;
					}
					if ((WKy == j) && (piecesOverlap(chb,i,j,WKx,WKy,chb[i][j].courant) == false)) {
						*WK = check;
					}
					KingEstMenace(WKx, WKy, i, j, chb[i][j].courant, chb[i][j].c, chb);
				}
				if (chb[i][j].courant == 'B' || chb[i][j].courant == 'Q' ||
					chb[i][j].courant == 'N' || chb[i][j].courant == 'K') {
					if (KingEstMenace(WKx, WKy, i, j, chb[i][j].courant, chb[i][j].c, chb))
						*WK = safe_check;
				}
				if (chb[i][j].courant != 'K')
					k_domain_ctrl(i, j, BKx, BKy, chb[i][j].c, 'f');
			} else if (chb[i][j].c == WHITE) {
				if (chb[i][j].courant == 'P') {
					k_domain_ctrl(i,j,BKx,BKy,chb[i][j].c, 'e');
					k_domain_ctrl(i+1,j+1,BKx,BKy,chb[i][j].c, 'e');
					k_domain_ctrl(i+1,j-1,BKx,BKy,chb[i][j].c, 'e');
					k_domain_ctrl(i,j,WKx,WKy,chb[i][j].c, 'f');
				}
				if (chb[i][j].courant == 'R' || chb[i][j].courant == 'Q') {
					if ((BKx == i) && (piecesOverlap(chb,i,j,BKx,BKy,chb[i][j].courant) == false))
						*BK = check;
					if ((BKy == j) && (piecesOverlap(chb,i,j,BKx,BKy,chb[i][j].courant) == false))
						*BK = check;
					KingEstMenace(BKx, BKy, i, j, chb[i][j].courant, chb[i][j].c, chb);
				}
				if (chb[i][j].courant == 'B' || chb[i][j].courant == 'Q' ||
					chb[i][j].courant == 'N' || chb[i][j].courant == 'K') {
					if (KingEstMenace(BKx, BKy, i, j, chb[i][j].courant, chb[i][j].c, chb) == true)
						*BK = safe_check;
				}
				if (chb[i][j].courant != 'K')
					k_domain_ctrl(i, j, WKx, WKy, chb[i][j].c, 'f');
			}
		}
	}
	for (i = 0; i < 3; i++) {
		if (WKx == 0) {
			WKingLife[0][i] = 3;
		} else if (WKx == 7) {
			WKingLife[2][i] = 3;
		}
		if (WKy == 0) {
			WKingLife[i][0] = 3;
		} else if (WKy == 7) {
			WKingLife[i][2] = 3;
		}
		if (BKx == 0) {
			BKingLife[0][i] = 3;
		} else if (BKx == 7) {
			BKingLife[2][i] = 3;
		}
		if (BKy == 0) {
			BKingLife[i][0] = 3;
		} else if (BKy == 7) {
			BKingLife[i][2] = 3;
		}
	}

	if (WKingLife[1][1] == 1) {
		*WK = check;
	} else if (WKingLife[1][1] == 0) {
		*WK = safe;
	}
	if (BKingLife[1][1] == 1) {
		*BK = check;
	} else if (BKingLife[1][1] == 0) {
		*BK = safe;
	}
	echecEtMat(&WK, &BK);
	getDeplacementKing(chb, WKx, WKy, WHITE);
	getDeplacementKing(chb, BKx, BKy, BLACK);
}

bool KingEstMenace(const int Kx, const int Ky, const int xpiece,
			const int ypiece, const char c, const int color, templateEchiquier chb[][8])
{
	int k, l, max, ovlap_flag = false;

	if (c == 'R' || c == 'Q') {
		if (ypiece > Ky) {
			if (Ky >= 1)
				max = Ky-1;
			else
				max = Ky;
			for (l = ypiece-1; l >= max; l--) {
				if (chb[xpiece][l].occ == true && l!=Ky && ovlap_flag == false) {
					if (chb[xpiece][l].c != color)	/*encore en train de tester*/
						k_domain_ctrl(xpiece, l, Kx, Ky, color, 'e');
					ovlap_flag = true;
				}
				if (ovlap_flag == false)
					k_domain_ctrl(xpiece, l, Kx, Ky, color, 'e');
			}
		} else if (ypiece < Ky) {
			if (Ky <= 6)
				max = Ky+1;
			else
				max = Ky;
			for (l = ypiece+1; l <= max; l++) {
				if (chb[xpiece][l].occ == true && l!=Ky && ovlap_flag == false) {
					if (chb[xpiece][l].c != color)
						k_domain_ctrl(xpiece, l, Kx, Ky, color, 'e');
					ovlap_flag = true;
				}
				if (ovlap_flag == false)
					k_domain_ctrl(xpiece, l, Kx, Ky, color, 'e');
			}
		}
		ovlap_flag = false;	/*réinitialiser le drapeau juste au cas où*/
		if (xpiece > Kx) {
			if (Kx >= 1)
				max = Kx-1;
			else
				max = Kx;
			for (k = xpiece-1; k >= max; k--) {
				if (chb[k][ypiece].occ == true && k!=Kx && ovlap_flag == false) {
					if (chb[k][ypiece].occ != color)
						k_domain_ctrl(k, ypiece, Kx, Ky, color, 'e');
					ovlap_flag = true;
				}
				if (ovlap_flag == false)
					k_domain_ctrl(k, ypiece, Kx, Ky, color, 'e');
			}
		} else if (xpiece < Kx) {
			if (Kx <= 6)
				max = Kx+1;
			else
				max = Kx;
			for (k = xpiece+1; k <= max; k++) {
				if (chb[k][ypiece].occ == true && k!=Kx && ovlap_flag == false) {
					if (chb[k][ypiece].occ != color)
						k_domain_ctrl(k, ypiece, Kx, Ky, color, 'e');
					ovlap_flag = true;
				}
				if (ovlap_flag == false)
					k_domain_ctrl(k, ypiece, Kx, Ky, color, 'e');
			}
		}
	}
	if (c == 'B' || c == 'Q') {
		if (Kx == xpiece || Ky == ypiece) {
			return false;
		}
		ovlap_flag = false;	/*réinitialiser le flag ici aussi, dans le cas où c est Queen*/
		if (Kx > xpiece && Ky > ypiece) {
			k = xpiece + 1;
			l = ypiece + 1;
			while ((k <= 7 && k >= 0) && (l <= 7 && l >= 0)) {
				if (chb[k][l].occ == true && (k != Kx && l != Ky) && ovlap_flag == false) {
					if (chb[k][l].c != color)
						k_domain_ctrl(k, l, Kx, Ky, color, 'e');
					ovlap_flag = true;
				}
				if (ovlap_flag == false)
					k_domain_ctrl(k, l, Kx, Ky, color, 'e');
				if ((Kx > 0 && Kx < 7) && (Ky > 0 && Ky < 7)) {
					if (k == (Kx+1) && l == (Ky+1)) {
						return true;
					}
				} else {
					if (k == Kx && l == Ky) {
						return true;
					}
				}
				k++;
				l++;
			}
		} else if (Kx < xpiece && Ky > ypiece) {
			k = xpiece - 1;
			l = ypiece + 1;
			while ((k >= 0 && k <= 7) && (l <= 7 && l >= 0)) {
				if (chb[k][l].occ == true && (k != Kx && l != Ky) && ovlap_flag == false) {
					if (chb[k][l].c != color)
						k_domain_ctrl(k, l, Kx, Ky, color, 'e');
					ovlap_flag = true;
				}
				if (ovlap_flag == false)
					k_domain_ctrl(k, l, Kx, Ky, color, 'e');
				if ((Kx > 0 && Kx < 7) && (Ky > 0 && Ky < 7)) {
					if (k == (Kx-1) && l == (Ky+1)) {
						return true;
					}
				} else {
					if (k == Kx && l == Ky) {
						return true;
					}
				}
				k--;
				l++;
			}
		} else if (Kx > xpiece && Ky < ypiece) {
			k = xpiece + 1;
			l = ypiece - 1;
			while ((k <= 7 && k >= 0) && (l >= 0 && l <= 7)) {
				if (chb[k][l].occ == true && (k != Kx && l != Ky) && ovlap_flag == false) {
					if (chb[k][l].c != color)
						k_domain_ctrl(k, l, Kx, Ky, color, 'e');
					ovlap_flag = true;
				}
				if (ovlap_flag == false)
					k_domain_ctrl(k, l, Kx, Ky, color, 'e');
				if ((Kx > 0 && Kx < 7) && (Ky > 0 && Ky < 7)) {
					if (k == (Kx+1) && l == (Ky-1)) {
						return true;
					}
				} else {
					if (k == Kx && l == Ky) {
						return true;
					}
				}
				k++;
				l--;
			}
		} else {
			k = xpiece - 1;
			l = ypiece - 1;
			while ((k <= 7 && k >= 0) && (l >= 0 && l <= 7)) {
				if (chb[k][l].occ == true && (k != Kx && l != Ky) && ovlap_flag == false) {
					if (chb[k][l].c != color)
						k_domain_ctrl(k, l, Kx, Ky, color, 'e');
					ovlap_flag = true;
				}
				if (ovlap_flag == false)
					k_domain_ctrl(k, l, Kx, Ky, color, 'e');
				if ((Kx > 0 && Kx < 7) && (Ky > 0 && Ky < 7)) {
					if (k == (Kx-1) && l == (Ky-1)) {
						return true;
					}
				} else {
					if (k == Kx && l == Ky) {
						return true;
					}
				}
				k--;
				l--;
			}
		}
	}
	if (c == 'N') {
		int knightrow[] = {xpiece-2,xpiece-2,xpiece-1,xpiece-1,
			xpiece+1,xpiece+1,xpiece+2,xpiece+2};
		int knightcol[] = {ypiece-1,ypiece+1,ypiece-2,ypiece+2,
			ypiece-2,ypiece+2,ypiece-1,ypiece+1};
		for (k = 0; k < 8; k++) {
			k_domain_ctrl(knightrow[k], knightcol[k], Kx, Ky, color, 'e');
			if (knightrow[k] == Kx && knightcol[k] == Ky) {
				return true;
			}
		}
	}
	if (c == 'K') {
		for (k = xpiece - 1; k < xpiece + 2; k++){
			for (l = ypiece - 1; l < ypiece + 2; l++){
				k_domain_ctrl(k, l, Kx, Ky, color, 'e');
				if (k == Kx && l == Ky) {
					return true;
				}
			}
		}
	}
	return false;
}

bool k_domain_ctrl(const int x_p, const int y_p, const int Kx,
				   const int Ky, const int color, const char flag)
{
	KingDomain KD[3][3] = {{{Kx-1, Ky-1},{Kx-1, Ky},{Kx-1, Ky+1}},
			{{Kx, Ky-1},{Kx, Ky},{Kx, Ky+1}},
			{{Kx+1, Ky-1},{Kx+1, Ky},{Kx+1, Ky+1}}};
	int k, l, retvalue = false;

	for (k = 0; k < 3; k++) {
		for (l = 0; l < 3; l++) {
			if (KD[k][l].x == x_p && KD[k][l].y == y_p) {
				if (color == BLACK) {
					if (flag == 'e') {
						WKingLife[k][l] = 1;
						if (k == 1 && l ==1)
							retvalue = true;
					}
					else if (flag == 'f')
						BKingLife[k][l] = 2;
				} else {
					if (flag == 'e') {
						BKingLife[k][l] = 1;
						if (k == 1 && l ==1 )
							retvalue = true;
					}
					else if (flag == 'f')
						WKingLife[k][l] = 2;
				}
			}
		}
	}
	return retvalue;
}

void getDeplacementKing(templateEchiquier chb[][8], int Kx, int Ky, int color)
{
	KingDomain KD[3][3] = {{{Kx-1, Ky-1},{Kx-1, Ky},{Kx-1, Ky+1}},
			{{Kx, Ky-1},{Kx, Ky},{Kx, Ky+1}},
			{{Kx+1, Ky-1},{Kx+1, Ky},{Kx+1, Ky+1}}};
	int i, j, str_index = 0, tempx, tempy;

	if (color == BLACK)
		DeplacementBKing = malloc(22);
	else
		DeplacementWKing = malloc(22);
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			tempx = KD[i][j].x;
			tempy = KD[i][j].y;
			if (color == BLACK) {
				if (BKingLife[i][j] == 0) {
					sprintf(&DeplacementBKing[str_index], "%c%c ",
						chb[tempx][tempy].square[0],
						chb[tempx][tempy].square[1]);
					str_index+=3;
				}
			} else {
				if (WKingLife[i][j] == 0) {
					sprintf(&DeplacementWKing[str_index], "%c%c ",
						chb[tempx][tempy].square[0],
						chb[tempx][tempy].square[1]);
					str_index+=3;
				}
			}
		}
	}
	if (color == BLACK && !str_index) {
		free(DeplacementBKing);
		DeplacementBKing = NULL;
		/*Trucs bizarres: segfaults de jeu si DeplacementBKing et DeplacementWKing ne sont pas définis
* à NULL immédiatement après la libération; J'ai reproduit la même situation
* dans un petit programme de test et cela a fonctionné en libérant les cordes
* (sans réglage à NULL). Encore une fois si vous avez une explication contactez-moi.*/
	}
	if (color == WHITE && !str_index) {
		free(DeplacementWKing);
		DeplacementWKing = NULL;
	}
}

void echecEtMat(EtatDuRoi **WK, EtatDuRoi **BK)
{
	int i, j, Wcounter = 0, Bcounter = 0;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (WKingLife[i][j] == 1)
				Wcounter++;
			if (BKingLife[i][j] == 1)
				Bcounter++;
		}
	}
	if (Wcounter > 0) {
		**WK = safe_check;
		if (WKingLife[1][1] == 1)
			**WK = check;
		if (Wcounter == 9)
			**WK = checkmate;
	}
	if (Bcounter > 0) {
		**BK = safe_check;
		if (BKingLife[1][1] == 1)
			**BK = check;
		if (Bcounter == 9)
			**BK = checkmate;
	}
}

void date_filename(char *buf, int ln)
{
	time_t t_epc = time(NULL);
	struct tm t;

	t = *localtime(&t_epc);
	strftime(buf, ln, "%a %Y-%m-%d %H%M%S.txt", &t);
}

void ecrireDansLog(int round, FILE* logf, char *plInput, char piece[2])
{

	if (!strncmp(piece, CSTL_LEFTROOK, 2)) {
		if (round == WHITE) {
			fprintf(logf, "Round  #%d:\tBlanc déplace Rook de A1 à D1 et King de E1 à C1\n", rc);
		} else {
			fprintf(logf, "           \tBlack déplace Rook de A8 à D8 et King de E8 à C8\n");
			rc++;
		}
		return;
	} else if (!strncmp(piece, CSTL_RIGHTROOK, 2)) {
		if (round == WHITE)
			fprintf(logf, "Round  #%d:\tBlanc déplace Rook de H1 à F1 et King de E1 à G1\n", rc);
		else {
			fprintf(logf, "           \tBlack déplace Rook de H8 à F8 et King de E8 à G8\n");
			rc++;
		}
		return;
	}
	if (round == WHITE) {
		fprintf(logf, "Round  #%d:\tDéplacements Blanc ", rc);
	} else {
		fprintf(logf, "           \tDéplacements Noir  ");
		rc++;
	}
	if (plInput[0] == 'P') {
		fprintf(logf, "Pawn ");
	} else if (plInput[0] == 'R') {
		fprintf(logf, "Rook ");
	} else if (plInput[0] == 'N') {
		fprintf(logf, "Knight ");
	} else if (plInput[0] == 'B') {
		fprintf(logf, "Bishop ");
	} else if (plInput[0] == 'Q') {
		fprintf(logf, "Queen ");
	} else if (plInput[0] == 'K') {
		fprintf(logf, "King ");
	}
	fprintf(logf, "de %c%c à %c%c\n", piece[0], piece[1], plInput[1], plInput[2]);
}

char *getPlayerInput(void)
{
	size_t len = 0, max = 1;
	int count = 0;
	char c = 0;
	char *str_in = calloc(max, 1);
	char *str_temp = str_in;

	if(!str_in)
		return str_in;
	while ((c = fgetc(stdin)) != '\n') {
		str_in[len++] = c;
		if (len == max) {
			max++;
			str_temp = realloc(str_in, max);
			if (!str_temp) {
				return str_in;
			} else {
				str_in = str_temp;
			}
		} else {
			clear_buffer();
			break;
		}
		count++;
	}
	str_in[len] = '\0';
	return str_in;
}

char *pieceConflict(const char *piece_pos, const char p)
{
	static char *temp, fpiece[3], name[7];

	switch (p) {
		case 'P':
			strcpy(name, "Pawn");
			break;
		case 'B':
			strcpy(name, "Bishop");
			break;
		case 'R':
			strcpy(name, "Rook");
			break;
		case 'N':
			strcpy(name, "Knight");
			break;
		default:
			strcpy(name, " ");
			break;
	}
	if (piece_pos[0] != piece_pos[2]) {
		printf("%s %s %s %s?\n%s", "Vouliez-vous vous déplacer à gauche",
				name, "ou à droite", name,
				"Spécifiez le avec soit 'left'/'LEFT' ou 'right'/'RIGHT': ");
		do {
			temp = getPlayerInput();
		} while (strcmp(temp, "left") && strcmp(temp, "right")
		&& strcmp(temp, "LEFT") && strcmp(temp, "RIGHT"));

		if (((!strcmp(temp, "right") || !strcmp(temp, "RIGHT")) && piece_pos[0] < piece_pos[2]) ||
			((!strcmp(temp, "left") || !strcmp(temp, "LEFT")) && piece_pos[0] > piece_pos[2])) {
			fpiece[0] = piece_pos[2];
			fpiece[1] = piece_pos[3];
		} else {
			memcpy(fpiece, piece_pos, 2);
		}
	} else {
		printf("%s %s %s %s %s\n%s", "Voulez-vous vous déplacer",
				name, "ci-dessus ou", name, "en dessous?",
				"Spécifiez le avec soit avec soit 'up'/'UP' ou 'down'/'DOWN': ");
		do {
			temp = getPlayerInput();
		} while (strcmp(temp, "up") && strcmp(temp, "down")
		&& strcmp(temp, "UP") && strcmp(temp, "DOWN"));

		if (((!strcmp(temp, "up") || !strcmp(temp, "UP")) && piece_pos[1] > piece_pos[3]) ||
			((!strcmp(temp, "down") || !strcmp(temp, "DOWN")) && piece_pos[1] < piece_pos[3])) {
			fpiece[0] = piece_pos[2];
			fpiece[1] = piece_pos[3];
		} else {
			memcpy(fpiece, piece_pos, 2);
		}
	}
	fpiece[2] = '\0';
	return fpiece;
}

extern void printMignon(const char *banner)
{
	int i, j, c = 0, len = (int)strlen(banner);
#if !defined (__MINGW32__) || !defined(_WIN32)
	struct timespec *start_t = alloca(sizeof(struct timespec));
	struct timespec *end_t = alloca(sizeof(struct timespec));

	*start_t = (struct timespec){0, (BANNER_SPEED)*1000000};
	*end_t = (struct timespec){0, (BANNER_SPEED)*1000000};
#endif
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 2*len; j++) {
			if (!j || !i || i == 2 || j == (2*len - 1))
				printf("*");
			else if (j >= (int)((2*len)/4) && c < len && i == 1)
				printf("%c", banner[c++]);
			else
				printf(" ");
			fflush(stdout);
#if !defined(__MINGW32__) || !defined(_WIN32)
			nanosleep(start_t, end_t);
#else
			Sleep(BANNER_SPEED);
#endif
		}
		printf("\n");
	}
}
