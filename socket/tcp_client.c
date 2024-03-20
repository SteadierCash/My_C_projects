#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main()
{
    //create a socket
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    //specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002); //converting data to a correct data format
    server_address.sin_addr.s_addr = INADDR_ANY; // local addres

    int connect_status = connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    //check for errors
    if (connect_status == -1){
        printf("There was an error making a connection to a remote socket\n\n");
    }

    //recive data from the server
    char server_response[256];
    recv(network_socket, &server_response, sizeof(server_response), 0);
    
    //printing the data
    printf("The server sent the data: %s\n", server_response);

    //closing the socket
    pclose(network_socket);
    return 0;
}