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


void socket_client()
{
	//socket的建立
    int sockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd == -1){
		cout<<"Fail to create a socket.";
    }

    //socket的連線

    struct sockaddr_in info;
    bzero(&info,sizeof(info));
    info.sin_family = PF_INET;

    //localhost test
    //info.sin_addr.s_addr = inet_addr("192.168.43.207");
    //info.sin_addr.s_addr = inet_addr("192.168.43.8");
    info.sin_addr.s_addr = inet_addr("127.0.0.1");
	info.sin_port = htons(8700);


    int err = connect(sockfd,(struct sockaddr *)&info,sizeof(info));
	if(err==-1){
        cout<<"Connection error"<<endl;
    }


    //Send a message to server
    char message[] = {"Hi there"};
    char receiveMessage[100] = {};
    char state[1] = {};
    //state: 0: healthy, 1:too right, 2:too left, 3:sit too long, 4: leave
    while(state[0] != '4')
    {
        send(sockfd,message,sizeof(message),0);
        recv(sockfd,state,1,0);
        recv(sockfd,receiveMessage,sizeof(receiveMessage),0);
        if(state[0] == '0')
        {

        }
        else if(state[0] == '1')
        {

        }
        else if(state[0] == '2')
        {

        }
        else if(state[0] == '3') 
    }

    cout<<receiveMessage<<endl;
    cout<<"close Socket"<<endl;
    close(sockfd);
    return ;
}
