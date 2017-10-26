/**
 * @Author: Moustapha KEBE <kebson>
 * @Date:   2017-10-12T11:55:27+02:00
 * @Email:  mktapha@gmail.com
 * @Last modified by:   kebson
 * @Last modified time: 2017-10-26T04:16:56+02:00
 */



#include "chesslib.h"

int main(int argc, char *argv[])
{
	templateEchiquier echiquier[8][8];
	char *playerInput = NULL, piece_a_deplacer[2], fn[s_l], attak_gardien[5], temp_cpiece, chbflag = 'a';
	int round = 0, round_compteur = 1, p_err = 0, compteur_boucle = 1, argtmp;
		EtatDuRoi roi_blanc = safe, roi_noir = safe;
	FILE *logfile;

	do {
		argtmp = getopt(argc, argv, "c");
		if (argtmp == 'c')
			break;
	} while (argtmp != -1);
	initiaiiserEchiquier(echiquier);
	date_filename(fn, s_l);
	clear_screen();

	while (1) {
		if (round_compteur == 1) {
			printf("\n");
			printMignon("Bienvenue !");
			printf("\n");
			round = WHITE;
		} else {
			if (round_compteur%2 == 1)
				round = WHITE;
			else
				round = BLACK;
			printf("\n\n\n\n\n");
		}

		printf("Appuyer sur 'Help' puis Entrer pour voir les instructions\n");

		LOOP:
		do {
			if (compteur_boucle > 1) {
				clear_screen();
				printf("\n\n\n\n\n\n");
				printBoard(echiquier, chbflag);
				if (!strcmp(playerInput, "help")) {
					printInstructions();
					p_err = 0;
				}
			} else
				printBoard(echiquier, chbflag);
			printError(p_err);
			if (roi_blanc == checkmate) {
				printf("Joueur Noir a gagne!\n\t\t\n");
				goto ENDGAME;
			} else if (roi_noir == checkmate) {
				printf("Joueur Blanc a gagne!\n\t\t\n");
				goto ENDGAME;
			}
			if (roi_blanc == check || roi_blanc == safe_check) {
				if (!DeplacementWKing)
					printf("Roi blanc ne peut pas se deplacer.\n");
				else {
					if (roi_blanc == check)
						printf("Roi blanc est en danger\n");
#ifndef NDEBUG
					printf("Deplacements possibles pour le Roi Blanc: %s\n", DeplacementWKing);
#endif
				}
			}
			if (roi_noir == check || roi_noir == safe_check) {
				if (!DeplacementBKing)
					printf("Roi noir peut pas se deplacer.\n");
				else {
					if (roi_noir == check)
						printf("Roi blanc est en danger!\n");
#ifndef NDEBUG
					printf("Deplacements possibles pour le Roi Blanc: %s\n", DeplacementBKing);
#endif
				}
			}
			if (round == BLACK) {
				if (AI_IS_ENABLED) {
					memcpy(playerInput, DeplacementOrdinateur(echiquier), 4);
#ifndef NDEBUG
					printf("%s ", playerInput);
#endif
				}
				else {
					printf("C'est au tour du NOIR: ");
					playerInput = getPlayerInput();
				}
			} else {
				printf("C'est au tour du blanc: ");
				playerInput = getPlayerInput();
			}

			if (!playerInput) {	/*eviter segfaulting pour strlen(NULL) plus tard*/
				compteur_boucle++;
				continue;
			}
			if (!strcmp(playerInput, "quitter") || !strcmp(playerInput, "sortir")
				|| !strcmp(playerInput, "QUITTER") || !strcmp(playerInput, "EXIT")) {
				goto ENDGAME;
			}
			if (strlen(playerInput) > 4 || playerInput[1] == '\0') {	/*change error code for bad input*/
				compteur_boucle++;
#if !defined (__MINGW32__) || !defined(_WIN32)
				if (!strncmp(playerInput, "pieces", 7) || !strncmp(playerInput, "PIECES", 7)) {
					chbflag = 'p';
					p_err = 0;
					goto LOOP;
				}
				if (!strncmp(playerInput, "lettres", 8) || !strncmp(playerInput, "LETTRES", 8)) {
					chbflag = 'a';
					p_err = 0;
					goto LOOP;
				}
#endif
				if (!strncmp(playerInput, "version", 8) || !strncmp(playerInput, "VERSION", 8)) {
					p_err = 8;
					goto LOOP;
				}
				p_err = 2;
				goto LOOP;
			}
			compteur_boucle++;
		} while (entreeValide(playerInput, &p_err) == false);
		playerInput[0] = (char)toupper(playerInput[0]);
		playerInput[1] = (char)toupper(playerInput[1]);

		if (!findPiece(echiquier, playerInput, round)) {
			p_err = 3;
			goto LOOP;
		}
		if (cstl_is_enabled) {
			setCastling(echiquier, playerInput, round);
			goto LOG;
		}
		strncpy(attak_gardien,findPiece(echiquier, playerInput, round), 4);
		if (strlen(attak_gardien) < 3) {
			memcpy(piece_a_deplacer, attak_gardien, 2);
		} else if (piecesOverlap(echiquier, (attak_gardien[1]-'1'), (attak_gardien[0]-'A'),
				(playerInput[2]-'1'), (playerInput[1]-'A'), playerInput[0]) == true && playerInput[0] != 'N') {
			piece_a_deplacer[0] = attak_gardien[2];
			piece_a_deplacer[1] = attak_gardien[3];
		} else if (piecesOverlap(echiquier, (attak_gardien[3]-'1'), (attak_gardien[2]-'A'),
				(playerInput[2]-'1'), (playerInput[1]-'A'), playerInput[0]) == true && playerInput[0] != 'N') {
			memcpy(piece_a_deplacer, attak_gardien, 2);
		} else {
			temp_cpiece = playerInput[0];
			memcpy(piece_a_deplacer, pieceConflict(attak_gardien, temp_cpiece), 2);
		}

		if (roi_blanc == check  || roi_noir == check ||
			(playerInput[0] == 'K' && (roi_blanc == safe_check || roi_noir == safe_check))) {
			if (!verifiValideDeplacement(echiquier, playerInput, piece_a_deplacer, round)) {
				p_err = 3;
				goto LOOP;
			}
		}
		if (DeplacerPiece(echiquier, playerInput, piece_a_deplacer, round) == false) {
			p_err = 3;
			goto LOOP;
		}
		LOG:
		if (!(logfile = fopen(fn, "a"))) {
			printError(3);
		} else {
			if (cstl_is_enabled) {
				cstl_is_enabled = false;
				if (playerInput[1] == 'C' || playerInput[1] == 'D')
					ecrireDansLog(round, logfile, playerInput, CSTL_LEFTROOK);
				else
					ecrireDansLog(round, logfile, playerInput, CSTL_RIGHTROOK);
			} else {
				ecrireDansLog(round, logfile, playerInput, piece_a_deplacer);
			}
		}
		fclose(logfile);
		free(playerInput);
		round_compteur++;
		p_err = 0;
		compteur_boucle = 2;
		findEtatKing(echiquier, &roi_blanc, &roi_noir);
	}
	ENDGAME:
	if (roi_blanc == checkmate || roi_noir == checkmate) {
#if !defined(__MINGW32__) || !defined(_WIN32)
		sleep(4);
#else
		Sleep(4000);
#endif
	}
	playerInput = NULL;
	return 0;
}
