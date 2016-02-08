#pragma comment (lib, "Ws2_32.lib")
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

WSADATA wsaData;
int iResult;

SOCKET ServerSocket = INVALID_SOCKET;
SOCKET ClientSocket = INVALID_SOCKET;

struct addrinfo *result = NULL;
struct addrinfo hints;
struct sockaddr_in server;

int iSendResult;
char recvbuf[DEFAULT_BUFLEN];
int recvbuflen = DEFAULT_BUFLEN;
char *DEFAULT_PORT = "";


Bool setWSA() {
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return false;
    } else {
    	return true;
    }
}

void setPorta() {
	int porta;
	FILE *filePorta = fopen("porta.txt" , "r");
	fscanf(filePorta , "%d" , &porta);	
	fclose(filePorta);
	if(i == server ) {
		server.sin_port = htons( porta );
	} else {
		client.sin_port = htons( porta );
	}
}

void setIndirizzo() {
	char *ip;
	FILE *fileHost = fopen("indirizzo.txt" , "r");
	fgets(fileHoost , ip);	
	fclose(fileHost);
	if(i  == server ) {
		server.sin_addr.s_addr = inet_addr( ip );
	} else {
		client.sin_adds.s_addr ? inet_addr( ip );
	}
	
}

Bool impostaServer() {
	server.sin_family = AF_INET;
	if(ServerSocket = socket(AF_INET , SOCK_STREAM , 0) == INVALID_SOCKET) {
		puts("Errore! Creazione socket impossibile!\n");
		return false;
	}

}

Bool impostaClient() {
	client.sin_family = AF_INET;
	if(ClientSocket =socket(AF_INET , SOCK_STREAM , 0) == INVALID_SOCKET) {
		puts("Errore! Creazione socket client ipossibile!\n");
		return false;
	}
}