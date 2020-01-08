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
    char message[16][21] = {{'0','0','0',',','4','1','3','2','1',',','1','4','3','2','1',',','4','4','3','2','1'},
                            {'2','0','0',',','4','5','2','2','1',',','2','4','3','2','1',',','4','7','3','2','1'},
                            {'2','0','0',',','4','5','2','2','1',',','2','4','3','2','1',',','4','7','3','2','1'},
                            {'2','0','0',',','4','5','2','2','1',',','2','4','3','2','1',',','4','7','3','2','1'},
                            {'2','0','0',',','4','5','2','2','1',',','2','4','3','2','1',',','4','7','3','2','1'},
                            {'3','0','0',',','6','4','3','2','1',',','5','4','3','2','1',',','4','7','3','2','1'},
                            {'3','1','0',',','8','4','3','2','1',',','6','4','3','2','1',',','2','4','3','2','1'},
                            {'3','3','1',',','8','4','3','2','1',',','6','4','3','2','1',',','2','4','3','2','1'},
                            {'3','2','0',',','8','4','6','2','1',',','2','3','3','2','1',',','2','4','3','2','1'},
                            {'3','3','0',',','1','4','6','2','1',',','4','3','3','2','1',',','9','4','3','2','1'},
                            {'3','4','0',',','2','4','7','2','1',',','4','4','3','2','1',',','9','4','9','2','1'},
                            {'3','5','0',',','4','4','2','2','1',',','4','4','3','2','1',',','4','1','9','2','1'},
                            {'3','5','0',',','4','4','2','2','1',',','4','4','3','2','1',',','4','1','9','2','1'},
                            {'3','5','0',',','4','4','2','2','1',',','4','4','3','2','1',',','4','1','9','2','1'},
                            {'3','5','0',',','4','4','2','2','1',',','4','4','3','2','1',',','4','1','9','2','1'},
                            {'3','3','1',',','4','4','3','2','1',',','4','4','3','2','1',',','4','4','2','2','1'}};
    

    char receiveMessage[100] = {};
    int send_status, receive_status;
    

    int i=0;
    while(1){
        
        receive_status = recv(sockfd,receiveMessage,sizeof(receiveMessage),0);
        cout << "receive : \"" << receiveMessage << "\", status: "<< receive_status<< endl;
        //message[0] = 'd';
        
        send_status = send(sockfd,message[i],sizeof(message[i]),0);
        cout << "send_status : " << send_status<<" , i = "<<i<<endl;
        
        if(i<16) i++;
        else i=0;

    }
    cout<<"close Socket"<<endl;
    close(sockfd);
    return 0;

}
