#include "socket_server.h"

void socket_server(volatile char *recMessage)
{	
	//create a socket
    char message[] = {'9'};
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
    serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverInfo.sin_port = htons(8700);						//trans local endian to net endian

/*
    //connect is to get data from other, while bind is to bind own addr on socket (like tell other where we to connect)
    bind(socket_fd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));	
    //we need to continue checking if there are someone to send
    listen(socket_fd,5);	//it allows 5 people in the waiting array
*/   
    cout<<"start loop"<<endl;
    bool quit = false;

	//connect is to get data from other, while bind is to bind own ad
	bind(socket_fd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));

    while(!quit){
        
        //we need to continue checking if there are someone to send
        listen(socket_fd,5);	//it allows 5 people in the waiting array
    
    	//use accept to see the clinet. it will generate a new socket for it and remove the request from waiting array
        cout<<"loop"<<endl;
        forClientSockfd = accept(socket_fd,(struct sockaddr*) &clientInfo, &addrlen);	//clientInfo is empty, used to store the received info of client
        int recv_status;
		recMessage[0] = 'c';
        

		//state: left->right abcde
		while(!quit)
		{
			int sendbit = send(forClientSockfd,message,sizeof(message),0);
            if (sendbit != 0)
            {
                recv_status = recv(forClientSockfd,recMessage,sizeof(recMessage),0);
            
                if(recv_status == 0 || recv_status == -1)
                {
                    //create a pic of failed connect and restart
                    recMessage[0] == 'x';
                    quit = true;
                    close(forClientSockfd);
                    break;
                }
                else if(recMessage[0] == 'x') 
                {
                    quit = true;
                    close(forClientSockfd);
                }
                cout<<"The message is : "<<recMessage<<endl;
                sleep(1);
                sendbit = 0;
            }
		}
    }

    //cout<<receiveMessage<<endl;
    cout<<"close Socket"<<endl;
	
    close(socket_fd);
    return ;
}