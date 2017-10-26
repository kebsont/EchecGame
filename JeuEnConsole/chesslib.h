/**
 * @Author: Moustapha KEBE <kebson>
 * @Date:   2017-10-12T11:55:27+02:00
 * @Email:  mktapha@gmail.com
 * @Last modified by:   kebson
 * @Last modified time: 2017-10-26T04:16:18+02:00
 */



#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#if defined(__MINGW32__) || defined(_WIN32)
# ifndef UNICODE
#  define UNICODE
# endif
# ifndef _UNICODE
#  define _UNICODE
# endif
# include <windows.h>
#elif defined(__FreeBSD__) \
|| defined(__linux__) \
|| defined(__APPLE__) \
|| defined(__gnu_linux__)
# define KRED  "\x1B[31m"
# define KYEL  "\x1B[33m"
# define RESET "\033[0m"
# include <termcap.h>
# include <alloca.h>
#else
# error Non-compatible OS or compiler
#endif

#define initiaiiserEchiquier(x) _initiaiiserEchiquier(x, 0, 'A')

#define MOS 17	/*celuici contrôle la taille du jeu d'échecs, on plaisante pas avec lui*/
#define WHITE 2
#define BLACK 1
#define EMPTY 0
#define BANNER_SPEED 100 - R_SPEED
#define R_SPEED 80	/*vitesse de la bannière animée, plus grande valeur plus grande vitesse; la valeur maximale est de 99*/
#define s_l 26	/*Longueur du nom de fichier, string*/
#define ALL_CASTL_TRUE {true, true, true, true, true, true}
#define CSTL_LEFTROOK "l"
#define CSTL_RIGHTROOK "r"
#define AI_IS_ENABLED argtmp=='c'?1:0

/*versioning*/
#define CHESSGAMECLI_MAJOR 0
#define CHESSGAMECLI_MINOR 2
#define CHESSGAMECLI_PATCH 0

#define SSTR(x) STR(x)
#define STR(x) #x

#define CHESSGAMECLI_VERSION_STRING SSTR(CHESSGAMECLI_MAJOR)"."SSTR(CHESSGAMECLI_MINOR)"."SSTR(CHESSGAMECLI_PATCH)

/*echiquier test*/
	typedef struct templateEchiquier {
	char *ches ;
	char courant ;	/*lettre courante, v pour carré vide*/
	char square[2];	/*carré actuel sur la carte, par exemple A1, H4*/
	bool occ;	/*un flag a checker si un carre est occupe*/
	int c;	/*couleur piece, 0 s'il nya pas de piece, 1 pour black, 2 pour white*/
} templateEchiquier;

/*struct de boolean pour verifier si castling est possible pour chak piece*/
typedef struct CastlingBool {
	bool WR_left;	/*white roque sur A1*/
	bool WR_right;	/*white roque sur H1*/
	bool BR_left;	/*Roque black sur A8*/
	bool BR_right;	/*black roque sur H8*/
	bool KBlack;	/*king noir*/
	bool KWhite;	/*king blanc*/
} CastlingBool;

/*
énumérateur pour les états possibles qu'un roi peut avoir*/
typedef enum EtatDuRoi {
	check,
	checkmate,
	safe,	/*King est sauve  (pas menace dans son 3x3 environ*/
	safe_check	/*King
n'est pas en échec mais n'est pas non plus autorisé à se déplacer vers certaines cases*/
} EtatDuRoi;

/*
les mouvements possibles que chaque roi peut faire après une situation de contrôle
 * par exemple DeplacementWKing= "A8 H4 B3"*/
extern char *DeplacementWKing;
extern char *DeplacementBKing;

/*si un joueur a choisi de faire du roque et du roque est possible
 * pour ce tour, la valeur de ce booléen devient vraie*/
extern bool cstl_is_enabled;
extern CastlingBool check_castling;


/******************************************
 *prototypes function pour la librarie main */


/*remplit un Échiquier[8][8], Échiquier avec des pièces d'échecs, récursivement
 * appelez initiaiiserEchiquier () si on a juste besoin de remplir un échiquier standard; ne pas
 * utiliser cette fonction
 * k: la ligne oû la fonction commence à remplir l'echiquier
 * col: même chose que k sauf que c'est la colonne et non la ligne*/
void _initiaiiserEchiquier(templateEchiquier chb[][8], unsigned k, char col);

/*fonction pour imprimer le tableau à un moment donné du jeu
 * p: caractère qui détermine le type de pièce d'échec qui sera imprimé; 'a' est pour
 * majuscules et "p" pour les pièces d'échecs Unicode (ne fonctionne pas sous Windows)*/
void printBoard (templateEchiquier chb[][8], const char p);

/*traverse l'échiquier, trouve et retourne la pièce qui est capable
 * pour effectuer le mouvement entré par le joueur; si plus d'une pièce peut bouger
 * au carré inscrit par le joueur qu'il trouve et renvoie les deux
 * entrée: chaîne avec la pièce et le carré à déplacer, par ex. PH3 NA5 KC1
 * couleur: la couleur de la pièce du joueur actuel*/
char *findPiece (templateEchiquier chb[][8], const char *input, int color);

/*déplacer la pièce si aucune autre pièce ne gêne; vérifie également la promotion du pion
 * renvoie faux si le déplacement n'est pas légal
 * plInput: chaîne avec la pièce et le carré à déplacer, par ex. PH3 NA5 KC1
 * piece: tableau de deux caractères pour la colonne et la ligne du carré le
 * pièce est allumée, par ex. H3 C7*/
bool DeplacerPiece (templateEchiquier chb[][8], char *plInput, char piece[2], int color);

/*vérifie si un déplacement est valide selon que la pièce chevauche d'autres pièces ou non
 * il prend les coordonnées de deux pièces (la première doit toujours être Q, R ou B),
 * comme arguments, et calcule s'il y a d'autres éléments entre eux
 * sx, sy: la rangée et la colonne de la première pièce
 * ex, ey: la rangée et la colonne de la deuxième pièce
 * pièce: la lettre de la première pièce*/
bool piecesOverlap (templateEchiquier chb[][8], const int sx, const int sy, const int ex, const int ey, const char piece);

/*dans le cas où le domaine d'un roi est menacé de quelque façon (check ou safe_check) cette fonction
 * vérifie si le prochain coup entré enlève le Roi d'une situation menaçante;
 * les arguments sont les mêmes que DeplacerPiece */
bool verifiValideDeplacement(templateEchiquier chb[][8], char *plInput, char piece[2], int color);

/*vérifier la validité de l'entrée du joueur; renvoie false pour la mauvaise entrée et true sinon
 * input: tampon d'entrée entré par le joueur; vérifiez si la chaîne est NULL avant
 * en utilisant cette fonction
 * errPtr: référence au code d'erreur entier dans la fonction principale*/
bool entreeValide (const char *input, int *errPtr);

/*créer un string avec la date actuelle à utiliser comme nom de fichier du fichier journal*/
void date_filename (char*, int);

/*écrire les mouvements de chaque joueur dans un fichier journal
 * rond: NOIR ou BLANC
 * logf: descripteur de fichier du fichier journal
 * plInput: entrée par le joueur; contient la pièce et le carré dans lequel la pièce s'est déplacée
 * piece: le carré initial de la pièce était dedans*/
void ecrireDansLog (int round, FILE* logf, char *plInput, char piece[2]);

/* erreur basique affichant les erreurs; ecrit dans stderr*/
void printError (int);

/*copier l'input l'entree buffer dans un string et return string*/
char *getPlayerInput (void);

/*gère le conflit de mouvement: si deux pièces du même type et de la même couleur
 * sont capables de se déplacer dans la même case entrée par le joueur, au même tour
 * piece_pos: chaîne avec 4 caractères qui contient l'emplacement des deux carrés, par ex. H6A4
 * p: le morceau à déplacer*/
char *pieceConflict (const char *piece_pos, const char p);

/*trouve et sauve l'état de chaque Roi pour le tour en cours; voir
 * l'enum d'EtatDuRoi pour tous les états qu'un Roi peut avoir*/
void findEtatKing (templateEchiquier chb[][8], EtatDuRoi *WK, EtatDuRoi *BK);

/*prend soin de tous les mouvements qui se produisent pendant le roque; appelez seulement si cstl_is_enabled est vrai
 * plInput: la pièce de roque qui a été retournée par findPiece
 * couleur: la couleur de la pièce du joueur*/
void setCastling (templateEchiquier chb[][8], char *plInput, int color);

/*Petites foncitons pour rendre joli le jeu. */
inline void clear_screen(void);
inline void clear_buffer(void);
inline void printMignon(const char*);
inline void printInstructions(void);
char *DeplacementOrdinateur(templateEchiquier chb[][8]);
