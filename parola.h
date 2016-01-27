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

*   You should have received a copy of the GNU General Public License
*   along with Parola.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <setjmp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IO.h"             /* libreria per le funzioni di I/O */
#include "mate.h"  			/* libreria per le funzioni matematiche */
#include "standard.h"		/* libreria per le funzioni standard */
#include "definizioni.h"    /* libreria per le definizioni del linguaggio */

Val *bound(Val *mem, int n) { if (n<1 || n>*mem) err("BOUNDS"); return mem+n;  }

int	(*kwdhook)(char *kwd);		/* parole chiavi */
int	(*funhook)(char *kwd, int n);	/* richiama funzioni parole chiavi */

initparola(int comp) { pc=prg; sp=stk+STKSZ; csp=cstk+STKSZ; stabp=stab; compile=comp; }
bad(char *msg) { printf("ERRORE %d: %s\n", lnum, msg); longjmp(trap,1); }
err(char *msg) { printf("ERRORE %p: %s\n",lmap[pc-prg-1],msg); longjmp(trap,2); }
emit(int opcode()) { lmap[cpc]=lnum; prg[cpc++]=opcode; }
inst(int opcode(), Val x) { emit(opcode); emit((Code)x); }



find(char *var) {
	int	i;
	for (i=0; i<nvar && strcmp(var,name[i]); i++);
	if (i==nvar) strcpy(name[nvar++], var);
	return i;
}
read() {	/* leggi token */
	char *p,*d,**k, *pun="(),+-*/\\=<>", *dub="<><==>";
	if (ungot) return ungot=0, tok;	/* precedente non preso */
	while (isspace(*lp)) lp++;	/* evita spazi bianchi */
	if (!*lp || *lp=='#') return tok=0; /* EOF */
	if (isdigit(*lp))		/* legge numeri */
		return tokv=strtol(lp,&lp,0), tok=NUMBER;
	if ((p=strchr(pun,*lp)) && lp++) { /* gestione punteggiatura */
		for (d=dub; *d && strncmp(d,lp-1,2); d+=2);
		if (!*d) return tok=(p-pun)+LP;
		return lp++, tok=(d-dub)/2+DIVERSO;
	} else if (isalpha(*lp)) {	/* identificatore */
		for (p=tokn; isalnum(*lp); ) *p++=toupper(*lp++);
		for (*p=0, k=kwd; *k && strcmp(tokn,*k); k++);
		if (*k) return tok=(k-kwd)+E;
		return tokv=find(tokn), tok=NOME;
	} else if (*lp=='"' && lp++) {	/* stringhe */
		for (p=stabp; *lp && *lp!='"'; ) *stabp++=*lp++;
		return *stabp++=0, lp++, tokv=p-stab, tok=STRING;
	} else	return bad("Token non valido!");
}
want(int type) { return !(ungot=read()!=type); }
need(int type) { if (!want(type)) bad("Errore di sintassi"); }
#define LIST(BODY) if (!want(0)) do {BODY;} while (want(COMMA))
base() {		/* Espressioni Parola */
	int neg=want(SOTTRAI)? (inst(NUMBER_,0),1): 0;
	if (want(NUMBER)) inst(NUMBER_, tokv);
	else if (want(STRING)) inst(NUMBER_, (Val)(stab+tokv));
	else if (want(NOME)) {
		int var=tokv;
		if (want(LP))
			if (mode[var]==1) /* Definizione varibili (DIM) */
				expr(), need(RP), inst(LOAD_, var);
			else {
				int n=0;
				LIST(if (tok==RP) break; expr(); n++);
				need(RP);
				if (!funhook || !funhook(name[var],n)) {
					if (mode[var]!=2 || n!=sub[var][1])
						bad("Subroutine non valida o passaggi di parametri errati!");
					inst(RICHIAMA_, var);
					emit(RV_);
				}
			}
		else		inst(LOAD_, var);
	} else if (want(LP))	expr(), need(RP);
	else if (want(VALORE))	need(LP),need(NOME),need(RP),inst(VALORE_,tokv);
	else			bad("Espressione non valida");
	if (neg)		emit(SOTTRAI_);	/* negazione */
}
int (*bin[])()={SOMMA_,SOTTRAI_,MOLTIPLICA_,DIVIDI_,MODULO_,UGUALE_,MIN_,MAGG_,DIVERSO_,MINUGUALE_,MAGGUGUALE_,E_,O_};
#define BIN(NOME,LO,HI,ELEM)  NOME() { int (*o)(); \
	ELEM(); \
	while (want(0), LO<=tok && tok<=HI) \
		o=bin[tok-SOMMA], read(), ELEM(), emit(o); \
	return 0; }
BIN(factor,MOLTIPLICA,MODULO,base)
BIN(addition,SOMMA,SOTTRAI,factor)
BIN(relation,UGUALE,MAGGUGUALE,addition)
BIN(expr,E,O,relation)
stmt() {	/* STATEMENT */
	int	n,var;
	switch (read()) {
	case LEGGIDATASTIERA:
		inst(LEGGIDATASTIERA_ , 0);
		break;
	case STAMPAFORMATTATA:
		need(STRING), inst(NUMBER_, tokv);
		n=0; if (want(COMMA)) LIST(expr(); n++);
		inst(STAMPAFORMATTATA_, n);
		break;
	case SOTTOPROGRAMMA:	/* CSTK: {SUB,INDEX,JMP} */
		if (!compile) bad("Le subroutine devono essere compilate");
		compile++;			/* deve essere bilanciate alla fine */
		need(NOME), mode[cursub=var=tokv]=2; /* nome subroutine */
		n=0; LIST(need(NOME); sub[var][n+++2]=tokv); /* parametri */
		*--csp=cpc+1, inst(JMP_,0);	/* salta alla fine del codice */
		sub[var][0]=sub[var][1]=n;	/* LOCAL=PARAM COUNT */
		value[var]=cpc;			/* indirizzo memoria */
		*--csp=var, *--csp=SOTTOPROGRAMMA;		/* Fine subroutine */
		break;
	case LOCALE:
		LIST(need(NOME); sub[cursub][sub[cursub][0]+++2]=tokv;);
		break;
	case RITORNA:
		if (temp) inst(DROP_, temp);
		if (!want(0))	expr(), emit(SETRET_);
		inst(RITORNA_, cursub);
		break;
	case CICLA:	/* CSTK: {WHILE,TEST-FALSE,TOP} */
		compile++;			/* corpo del while compilato */
		*--csp=cpc, expr();
		*--csp=cpc+1, *--csp=CICLA, inst(FALSE_, 0);
		break;
	case PER:	/* CSTK: {FOR,TEST-FALSE,I,TOP}; STK:{HI} */
		compile++;			/* corpo del for compilato */
		need(NOME), var=tokv, temp++;
		need(UGUALE), expr(), inst(STORE_,var);
		need(FINOA), expr();
		*--csp=cpc, inst(PER_,var), emit(0);
		*--csp=var, *--csp=cpc-1, *--csp=PER;
		break;
	case SE:	/* CSTK: {IF,N,ENDS...,TEST-FALSE} */
		expr(), inst(FALSE_,0), *--csp=cpc-1;
		if (want(ALLORA)) { stmt(); prg[*csp++]=(Code)cpc; }
		else	compile++, *--csp=0, *--csp=SE;
		break;
	case ALTRIMENTI:
		n=csp[1]+1;
		inst(JMP_,0);			/* salta alla fine di "ALTRIMENTI" */
		*--csp=SE, csp[1]=n, csp[2]=cpc-1; /* fix simpatico */
		prg[csp[2+n]]=(Code)cpc;	/* PATCH "ALTRIMENTI" */
		csp[2+n]=!want(SE)? 0:		/* "ALTRIMENTI SE" */
			(expr(), inst(FALSE_,0), cpc-1);
		break;
	case FINE:
		need(*csp++), compile--;		/* controllo blocco */
		if (csp[-1]==SOTTOPROGRAMMA) {
			inst(RITORNA_, *csp++);
			prg[*csp++]=(Code)cpc;		/* patch jmp */
		} else if (csp[-1]==CICLA) {
			prg[*csp++]=(Code)(cpc+2);	/* patch per test */
			inst(JMP_, *csp++);		/* loop per test */
		} else if (csp[-1]==PER) {
			prg[*csp++]=(Code)(cpc+4);	/* patch per test */
			inst(NEXT_, *csp++);		/* INC asm */
			inst(JMP_, *csp++);		/* loop test */
			temp--;				/* onetime temp */
		} else if (csp[-1]==SE) {
			for (n=*csp++; n--; )		/* fine blocco patch */
				prg[*csp++]=(Code)cpc;
			if (n=*csp++) prg[n]=(Code)cpc; /* PATCH "ALTRIMENTI" */
		}
		break;
	case NOME:
		var=tokv;
		if (want(UGUALE)) expr(), inst(STORE_, var);
		else if (want(LP))
			expr(),need(RP),need(UGUALE),expr(),inst(STOREI_,var);
		else if (!kwdhook || !kwdhook(tokn)) {
			int n=0; LIST(expr(); n++);
			if (!funhook || !funhook(name[var],n)) {
				if (mode[var]!=2 || n!=sub[var][1])
					bad("Subroutine non valida o parametri non corretti!");
				inst(RICHIAMA_, var);
			}
		}
		break;
	case 
                        DEFINISCI:
		need(NOME), mode[var=tokv]=1;	/* gestione pulita delle variabili */
		need(LP), expr(), need(RP), inst(DEFINISCI_, var);
		break;
	case RIPRENDI: if (!want(0)) expr(); emit(RIPRENDI_); break;
	case INTERROMPI:	emit(INTERROMPI_); break;
	case CIAO:		emit(CIAO_); break;
	case STAMPA:		expr(); emit(STAMPA_); break;
	default:		if (tok) bad("Parola non valida");
	}
	if (!want(0))		bad("Controlla nel caso non hai chiuso qualche parentesi o qualche blocco!");
}
interp(FILE *sf) {	/* loop interprete */
	for (;;) {
		int code=setjmp(trap);			/* ritorna un errore */
		if (code==1 && sf!=stdin) return 1;	/* errore di sintassi */
		if (code==2) opc=pc;			/* FAULT */
		if (code==3) pc=opc?opc:pc, cpc=ipc;	/* "BREAK" */
		if (code==4) return 0;			/* "CIAO" */
		for (;;) {
			if (sf==stdin) printf("%d> ",lnum+1);
			if (!fgets(lp=lbuf,sizeof lbuf,sf)) break;
			lnum++, ungot=0, stmt();	/* parsato e compilato */
			if (compile) continue;		/* continua compilazione */
			opc=pc, pc=prg+ipc;		/* avvio immediato */
			emit(INTERROMPI_); DRIVER;		/* esecuzione simbolo */
		}
		ipc=cpc+1, compile=0, fclose(sf), sf=stdin; /* compilazione effettuata con successo */
		emit(CIAO_); DRIVER;			/* esecuzione programma */
	}
}