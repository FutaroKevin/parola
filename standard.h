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
* 	Ultima Modifica: 10/01/2016 20:23
*	Ultima Modifica da: Kevin Tafuro
*/

CIAO_() {
	longjmp(trap,4);
}

INTERROMPI_() {
	longjmp(trap,3);
}

RIPRENDI_() {
	pc=opc? opc:pc;
	opc=pc;
	cpc=ipc;
	STEP;
}
NUMBER_() {
	*--sp=PCV;
	STEP;
}

CARICA_() {
	*--sp=value[PCV];
	STEP;
}

STORE_() {
	value[PCV]=*sp++;
	STEP;
}

UGUALE_() {
	A=(A==B)? -1: 0;
	sp++; STEP;
};

MIN_() {
	A=(A<B)? -1: 0;
	sp++;
	STEP;
};

MAGG_() {
	A=(A>B)? -1: 0;
	sp++;
	STEP;
};

DIVERSO_() {
	A=(A!=B)? -1: 0;
	sp++;
	STEP;
};

MINUGUALE_() {
	A=(A<=B)? -1: 0;
	sp++;
	STEP;
};

MAGGUGUALE_() {
	A=(A>=B)? -1: 0;
	sp++;
	STEP;
};

E_() {
	A&=B;
	sp++;
	STEP;
};

O_() {
	A|=B;
	sp++;
	STEP;
};

JMP_() {
	pc=prg+(int)*pc;
	STEP;
}

FALSE_() {
	if (*sp++)
		pc++;
	else 
		pc=prg+(int)*pc;
	STEP;
}

PER_() {
	if (value[PCV] >= *sp)
		pc=prg+(int)*pc, sp++;
	else
		PCV;
	STEP;
}

NEXT_() {
	value[PCV]++;
	STEP;
}

RICHIAMA_() {
	Val v=PCV, n=sub[v][1], x, *ap=sp;
	while (n--) {
		x=LOC(n); 
		LOC(n)=*ap; 
		*ap++=x; 
	}
	for (n=sub[v][1]; n<sub[v][0]; n++)
		*--sp=LOC(n);
	*--sp=pc-prg;
	pc=prg+value[v];
	STEP;
}

RITORNA_() { int v=PCV, n=sub[v][0];
	pc=prg+*sp++;
	while (n--) LOC(n)=*sp++;
	STEP;
}

SETRET_() {
	ret=*sp++;
	STEP;
}

RV_() {
	*--sp=ret;
	STEP;
}

DROP_() {
	sp+=PCV;
	STEP;
}

DEFINISCI_() {
	int v=PCV, n=*sp++;
	Val *mem=calloc(sizeof(Val),n+1);
	mem[0]=n;
	value[v]=(Val)mem;
}

LOAD_() {
	Val x=*sp++;
	x=*bound((Val*)value[PCV],x);
	*--sp=x;
	STEP;
}

STOREI_() {
	Val x=*sp++, i=*sp++;
	*bound((Val*)value[PCV],i)=x;
	STEP;
}