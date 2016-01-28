/*
* 	This file is part of Parola.
*
*   Parola is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   Parola is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with Parola.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
*	Autore: Kevin Tafuro
*	Contatto: tafurok@gmail.com  k.tafuro@studenti.uniba.it
* 	Ultima Modifica: 10/01/2016 19:30
*	Ultima Modifica da: Kevin Tafuro
*/

#define SYMSZ	16			/* dimensione simbolo */
#define PRGSZ	65536			/* dimensione massima programma */
#define STKSZ	256			/* dimensione massima stack */
#define STRSZ	4096			/* dimensione massima stringhe */
#define VARS	512			/* numero massimo variabili */
#define LOCS	8			/* contatore locale */

typedef ptrdiff_t	Val;		/* puntatore istruzioni */
typedef int		(*Code)();	/* BYTE-CODE */

enum {	NOME=1,NUMBER,STRING,LP,RP,COMMA, SOMMA,SOTTRAI,MOLTIPLICA,DIVIDI,MODULO,
	UGUALE,MIN,MAGG,DIVERSO,MINUGUALE,MAGGUGUALE,E,O,STAMPAFORMATTATA,SOTTOPROGRAMMA,FINE,RITORNA,LOCALE,
	CICLA,PER,FINOA,SE,ALTRIMENTI,ALLORA,DEFINISCI,VALORE,CIAO,INTERROMPI,RIPRENDI,LEGGIDATASTIERA,STAMPA };
char	*kwd[]={ "E","O","STAMPA","SOTTOPROGRAMMA","FINE","RITORNA","LOCALE","CICLA",
	"PER","FINOA","SE","ALTRIMENTI","ALLORA","DEFINISCI","VALORE","CIAO","INTERROMPI","RIPRENDI","LEGGIDATASTIERA", 0 };

char	lbuf[256],tokn[SYMSZ],*lp;	/* stato di lexer (ch) */
int	lnum,tok,tokv,ungot;		/* stato di lexer (int) */
int	(*prg[PRGSZ])(),(**pc)(),cpc,lmap[PRGSZ]; /* puntatore programma compilato */
Val	stk[STKSZ],*sp;			/* RUN-TIME STACK */
Val	value[VARS];			/* valore variabili */
char	name[VARS][SYMSZ];		/* nome variabili */
int	sub[VARS][LOCS+2];		/* locazione variabili */
int	mode[VARS];			/* 0=NONE, 1=DIM, 2=SUB */
Val	ret;				/* funzione "ritorna" */
int	cstk[STKSZ], *csp;		/* stack compilatore */
int	nvar,cursub,temp,compile,ipc,(**opc)(); /* stato compilatore */
char	stab[STRSZ], *stabp;		/* tabella stringhe */
jmp_buf	trap;				/* errore trappola */
FILE *fileOpened;		/* array file aperti */


#define A	sp[1]			/* operando sinistro */
#define B	sp[0]			/* operando destro */
#define PCV	((Val)*pc++)		/* prendi successico (PC) */
#define STEP	return 1		/* continua esecuzione */
#define DRIVER	while ((*pc++)())	/* avvia programma */
#define LOC(N) value[sub[v][N+2]]	/* subroutine */