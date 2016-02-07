/*
*   This file is part of Parola.
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
* Autore: Kevin Tafuro
* Contatto: tafurok@gmail.com  k.tafuro@studenti.uniba.it
*   Ultima Modifica: 28/01/2016 13:00
* Ultima Modifica da: Kevin Tafuro
*/

#ifdef _WIN32
  
  #include "winNet.h"

  typedef enum istanza {server , client};  // tipo definito per la gestione del socket

  istanza i = NULL;

  _DEFINISCISOCKET() {
    i = (istanza) A;   // imposto tipo


    if(i  == server) {
      impostaServer();
    } else if(tipo == client) {
      impostaClient();
    } else {
      puts("ERRORE! tipo non definito!\n");
    }
  }


#else
  
  #include "unixNet.h"

#endif