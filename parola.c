/*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include "parola.h"

PRINTS_() { puts((char*)*sp++); STEP; }

kwdhook_(char *msg) {
	if (!strcmp(msg,"STAMPA"))
		expr(), emit(PRINTS_);
	else	return 0;
	return 1;
}

main(int argc, char **argv) {
	FILE *sf=stdin;
	initparola(0);
	kwdhook=kwdhook_;
	if (argv[1]) {
            sf = fopen(argv[1] , "r");
            if (sf) {
      		compile++;
            } else {
		printf("Impossibile aprire: %s\n", argv[1]);
		return 255;
            }
        }
	return interp(sf);
}