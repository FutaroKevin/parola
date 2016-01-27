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
* 	Ultima Modifica: 10/01/2016 20:02
*	Ultima Modifica da: Kevin Tafuro
*/

SOMMA_() {
	A+=B;
	sp++;
	STEP; 
};

SOTTRAI_() {
	A-=B;
	sp++; 
	STEP; 
};

MOLTIPLICA_() {
	A*=B;
	sp++;
	STEP;
};

DIVIDI_() {
	if (!B) sp+=2,err("Sei stupido? vioi fare la divisione per zero?");
	A/=B;
	sp++;
	STEP; 
};

MODULO_() {
	if (!B) sp+=2,err("Vuoi fare il modulo di zero? Ma sei coglione!");
	A%=B;
	sp++;
	STEP;
};

VALORE_() {
	*--sp=*(Val*)value[PCV];
	STEP;
}

