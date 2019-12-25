#include<stdio.h>
#include<sys/socket.h>

int main(int argc , char *argv[])
{
	//create a socket
	char inputBuffer[256] = {};
    char message[] = {"Hi,this is server.\n"};
    int sockfd = 0, Clientsockfd;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);		//AF_INET(IPv6) means two computer communicate with the net. 0 is the defalut value

    if (socket_fd == -1){
        printf("Fail to create a socket.");
    }

    //connection of socket
    struct sockaddr_in serverInfo, clientInfo;
    int addrlen = sizeof(clientinfo)
    bzero(&serverInfo,sizeof(serverInfo));						//initialize, set bits to 0

    serverInfo.sin_family = PF_INET;						//sockaddr_in is IPv4
    //serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");	//IP address. inet_addr is convert address from string to int
    serverInfo.sin_addr.s_addr = inet_addr("168.192.43.8");
    serverInfo.sin_port = htons(8700);						//trans local endian to net endian

    //connect is to get data from other, while bind is to bind own addr on socket (like tell other where we to connect)
    bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));	
    //we need to continue checking if there are someone to send
    listen(sockfd,5)	//it allows 5 people in the waiting array
    
    while(1){
    	//use accept to see the clinet. it will generate a new socket for it and remove the request from waiting array
        forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);	//clientInfo is empty, used to store the received info of client
        send(forClientSockfd,message,sizeof(message),0);
        recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0);
        printf("Get:%s\n",inputBuffer);
    }
    return 0;
}
