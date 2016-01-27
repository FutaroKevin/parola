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
* 	Ultima Modifica: 10/01/2016 20:07
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

}

SCRIVIFILE_() {   
// mi servono due parametri il primo il file e il secondo la stringa da scrivere all'interno preferibilmente in due posizioni contigue dello stack
	Val n=PCV, *ap=(sp+=n)-1;
	for (f=stab + *sp++; *f; f++)
		if (*f=='%') printf("%d", (int)*ap--);
		else if (*f=='$') printf("%s", (char*)*ap--);
		else putchar(*f);
}


APRIFILELETTURA_() {
	FILE *fp = NULL;
	fp = fopen((char*)*ap-- , "r");
	return fp;
}

SCRIVIFILEINCODA