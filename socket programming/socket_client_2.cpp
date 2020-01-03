#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>


using namespace std;

int main(int argc , char *argv[])
{
	cout<<"hi"<<endl;

	//socket的建立
    int sockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd == -1){
		cout<<"Fail to create a socket.";
    }
	cout<<"sockfd = " <<sockfd<<endl;

    //socket的連線

    struct sockaddr_in info;
    bzero(&info,sizeof(info));
    info.sin_family = PF_INET;
	cout << "PF_INET = "<<PF_INET<<endl;

    //localhost test
    //info.sin_addr.s_addr = inet_addr("192.168.43.207");
    info.sin_addr.s_addr = inet_addr("127.0.0.1");
    info.sin_port = htons(8700);


    int err = connect(sockfd,(struct sockaddr *)&info,sizeof(info));
	if(err==-1){
        cout<<"Connection error"<<endl;
    }


    //Send a message to server
    char message[15] = {'3','2','1',',','4','4','3','2','1',',','4','4','3','2','1'};
    char receiveMessage[100] = {};
    int send_status, receive_status;
    


    while(1){
        
        receive_status = recv(sockfd,receiveMessage,sizeof(receiveMessage),0);
        cout << "receive : \"" << receiveMessage << "\", status: "<< receive_status<< endl;
        //cin>> message;
        //message[0] = 'd';
        
        send_status = send(sockfd,message,sizeof(message),0);
        cout << "send_status" << send_status<<endl;
        
        

    }
    cout<<"close Socket"<<endl;
    close(sockfd);
    return 0;

}
