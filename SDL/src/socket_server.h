#ifndef _SOCKET_SERVER_H_
#define _SOCKET_SERVER_H_
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/time.h>
#include <cstring>
//#include "setting.h"

using namespace std;



//use for socket
void socket_server(volatile char *recMessage);


#endif