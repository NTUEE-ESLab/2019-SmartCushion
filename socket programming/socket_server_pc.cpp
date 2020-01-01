#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;
//#include <winsock.h>

int main(int argc , char *argv[])
{
	//create a socket
    char inputBuffer[256] = {};
    char message[] = {"Hi,this is server.\n"};
    int socket_fd = 0, forClientSockfd;
    socket_fd = socket(AF_INET , SOCK_STREAM , 0);		//AF_INET(IPv6) means two computer communicate with the net. 0 is the defalut value
    												//socket name includes IP, port, protocal
    cout << "socket_fd = "<<socket_fd<<endl;
    
    if (socket_fd == -1){
        cout<<"Fail to create a socket."<<endl;
    }

    //connection of socket
    struct sockaddr_in serverInfo, clientInfo;
    socklen_t addrlen = sizeof(clientInfo);
    bzero(&serverInfo,sizeof(serverInfo));						//initialize, set bits to 0
    //memset(&serverInfo, 0, sizeof(serverInfo));

    serverInfo.sin_family = PF_INET;						//sockaddr_in is IPv4
    //serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");	//IP address. inet_addr is convert address from string to int
    serverInfo.sin_addr.s_addr = inet_addr("192.168.43.8");
    serverInfo.sin_port = htons(8700);						//trans local endian to net endian

    while(1)
    {
    //connect is to get data from other, while bind is to bind own addr on socket (like tell other where we to connect)
    bind(socket_fd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));	
    //we need to continue checking if there are someone to send
    listen(socket_fd,5);	//it allows 5 people in the waiting array
    
    cout<<"start loop"<<endl;
    
    while(1){
    	//use accept to see the clinet. it will generate a new socket for it and remove the request from waiting array
        cout<<"loop"<<endl;
        forClientSockfd = accept(socket_fd,(struct sockaddr*) &clientInfo, &addrlen);	//clientInfo is empty, used to store the received info of client
        while(1){
            char state[] = {"I am server"};

            //int checkRec = recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0);

            //cin>>state;
            int checkRec = recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0);

            send(forClientSockfd,state,sizeof(state),0);

            //send(forClientSockfd,message,sizeof(message),0);
            cout<<"Get: "<<inputBuffer<<endl;
	    }
    }
    }
    return 0;
}
