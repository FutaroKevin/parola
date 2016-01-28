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

/*
*	Autore: Kevin Tafuro
*	Contatto: tafurok@gmail.com  k.tafuro@studenti.uniba.it
* 	Ultima Modifica: 28/01/2016 12:00
*	Ultima Modifica da: Kevin Tafuro
*/

STAMPA_() {
	printf("%ld\n",*sp++);
}

STAMPAFORMATTATA_() {
	char *f;
	Val n=PCV, *ap=(sp+=n)-1;
	for (f=stab + *sp++; *f; f++)
		if (*f=='%') printf("%d", (int)*ap--);
		else if (*f=='$') printf("%s", (char*)*ap--);
		else putchar(*f);
	putchar('\n'); STEP;
}

LEGGIDATASTIERA_() {
	char buffer[STRSZ];
	fscanf(stdin , "%s" , buffer);
	return buffer; 
}

LEGGIDAFILE_() {
	if(!fileOpened) {
		printf("File non Aperto!\n");
		return "";
	} else {
		char *fcontent;
		fseek(fileOpened, 0, SEEK_END);
        fsize = ftell(fileOpened);
        rewind(fileOpened);
		fcontent = (char*) malloc(sizeof(char) * fsize);
        fread(fcontent, 1, fsize, fileOpened);
        return fcontent;
	}
}

SCRIVIFILE_() {   
// mi servono due parametri il primo il file e il secondo la stringa da scrivere all'interno preferibilmente in due posizioni contigue dello stack
	Val n=PCV, *ap=(sp+=n)-1;
	for (f=stab + *sp++; *f; f++)
		if (*f=='%') fprintf(fileOpened , "%d", (int)*ap--);
		else if (*f=='$') fprintf(fileOpened , "%s", (char*)*ap--);
		else fputc(*f, fileOpened);
}


APRIFILELETTURA_() {
	fileOpened = fopen((char*)*sp++ , "r");
	if(!fileOpened) {
		printf("Apertura File impossibile!\nVerifica che il file esiste!\n");
	}
}

APRIFILESCRITTURA_() {
	fileOpened = fopen((char*)*sp++ , "w");
	if(!fileOpened) {
		printf("Impossibile aprire il file!\nVerifica se hai i permessi per scrivere nella cartella.\n");
	}
}

APRIFILEPERCODA_() {
	fileOpened = fopen((char*)*sp++ , "a");
	if(!fileOpened) {
		printf("Impossibile aprire il file!\nVerifica se il file esiste o hai i permessi per scrivere nella cartella.\n");
	}
}

