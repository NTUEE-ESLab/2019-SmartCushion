//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <SDL2/SDL_ttf.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/time.h>
#include <cstring>

//#include "LTexture.h"
//#include "socket_server.h"

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//check if mouse in rectangle
bool checkmousepos(SDL_Rect &Rect);

//refresh the screen with current state
void set_state(SDL_Surface *surface, SDL_Surface *dataSurface);

//Loads individual image
SDL_Surface* loadSurface( string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Begin image
SDL_Surface* gSurface = NULL;

//Connect image
SDL_Surface* gconnectSurface = NULL;

//empty image
SDL_Surface* gemptySurface = NULL;

//move up and down image
SDL_Surface* gmoveSurface = NULL;

//too left image
SDL_Surface* gtooleftSurface = NULL;

//left image
SDL_Surface* gleftSurface = NULL;

//sit well image
SDL_Surface* gsitwellSurface = NULL;

//right image
SDL_Surface* grightSurface = NULL;

//too right image
SDL_Surface* gtoorightSurface = NULL;

//moving right left image
SDL_Surface* gtiltSurface = NULL;

//sitting long image
SDL_Surface* gsitlongSurface = NULL;

//button
//exit image
SDL_Surface* gexitSurface = NULL;

//Start button image
SDL_Surface* gbuttonSurface = NULL;

//Globally used font
TTF_Font *gFont;

SDL_Color textColor = { 0, 0, 0, 0xFF };


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "Smart Cusion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load stretching surface
	gSurface = loadSurface( "pic/smart_cushion.bmp" );
	if( gSurface == NULL )
	{
		printf( "Failed to load smart_cushion image!\n" );
		success = false;
	}
	gconnectSurface = loadSurface( "pic/connect.bmp" );
	if( gconnectSurface == NULL )
	{
		printf( "Failed to load connect image!\n" );
		success = false;
	}
	gemptySurface = loadSurface( "pic/empty.bmp" );
	if( gemptySurface == NULL )
	{
		printf( "Failed to load empty image!\n" );
		success = false;
	}
	gmoveSurface = loadSurface( "pic/move.bmp" );
	if( gmoveSurface == NULL )
	{
		printf( "Failed to load move image!\n" );
		success = false;
	}
	gtooleftSurface = loadSurface( "pic/too_left.bmp" );
	if( gtooleftSurface == NULL )
	{
		printf( "Failed to load too_left image!\n" );
		success = false;
	}
	gleftSurface = loadSurface( "pic/tilt_left.bmp" );
	if( gleftSurface == NULL )
	{
		printf( "Failed to load tilt_left image!\n" );
		success = false;
	}
	gsitwellSurface = loadSurface( "pic/sitwell.bmp" );
	if( gsitwellSurface == NULL )
	{
		printf( "Failed to load sitwell image!\n" );
		success = false;
	}
	grightSurface = loadSurface( "pic/tilt_right.bmp" );
	if( grightSurface == NULL )
	{
		printf( "Failed to load tilt_right image!\n" );
		success = false;
	}
	gtoorightSurface = loadSurface( "pic/too_right.bmp" );
	if( gtoorightSurface == NULL )
	{
		printf( "Failed to load too_right image!\n" );
		success = false;
	}
	gtiltSurface = loadSurface( "pic/tilt.bmp" );
	if( gtiltSurface == NULL )
	{
		printf( "Failed to load tilt image!\n" );
		success = false;
	}
	gsitlongSurface = loadSurface( "pic/sitting_long.bmp" );
	if( gsitlongSurface == NULL )
	{
		printf( "Failed to load sitting_long image!\n" );
		success = false;
	}
	
	//load button
	gbuttonSurface = loadSurface( "pic/start_icon.bmp" );
	if( gbuttonSurface == NULL )
	{
		printf( "Failed to load start_icon image!\n" );
		success = false;
	}
	gexitSurface = loadSurface( "pic/exit.bmp" );
	if( gexitSurface == NULL )
	{
		printf( "Failed to load exit image!\n" );
		success = false;
	}
	return success;
}

void close()
{
	//Free loaded image
	SDL_FreeSurface( gSurface );
	gSurface = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

SDL_Surface* loadSurface( string path )
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
		if( optimizedSurface == NULL )
		{
			printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return optimizedSurface;
}

//set the start view
void reset_screen(SDL_Rect &buttonRect)
{
	//Apply the image stretched
	SDL_Rect stretchRect;
	stretchRect.x = 0;
	stretchRect.y = 0;
	stretchRect.w = SCREEN_WIDTH;
	stretchRect.h = SCREEN_HEIGHT;
	SDL_BlitScaled( gSurface, NULL, gScreenSurface, &stretchRect );
	SDL_BlitScaled( gbuttonSurface, NULL, gScreenSurface, &buttonRect );

	//Update the surface
	SDL_UpdateWindowSurface( gWindow );
}

bool checkmousepos(SDL_Rect &Rect)
{
	//cout<<"checkmouse"<<endl;
	int x,y;
	SDL_GetMouseState( &x, &y );
	cout<<"x = "<<x<<",y ="<<y<<endl;
	cout<<"rec.x_1 = "<<Rect.x<<",rec.y_1 ="<<Rect.y<<endl;
	cout<<"rec.x_2 = "<<Rect.x+Rect.w<<",rec.y_2 ="<<Rect.y+Rect.h<<endl;
	if( x>Rect.x && x<Rect.x+Rect.w && y>Rect.y && y<Rect.y+Rect.h )
		return false;
	else return true;
}

void update(char *recMessage)
{
	
	//set screen
	//convert data to string
	
	string dataone = "", datatwo = "" , data = ""; 
    for (int i = 0; i < 5; i++) { 
        dataone = dataone + recMessage[4+i];
		datatwo = datatwo + recMessage[10+i];
    }
	data = "left pressure = "+dataone+", right pressure = "+datatwo;
	cout << data <<endl;
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, data.c_str(), textColor );

	//0: nonstart; 1:empty
	if(recMessage[0] == '0' || recMessage[0] == '1')
	{
		cout<<"in update function, state is "<<recMessage[0]<<endl;
		set_state(gemptySurface, textSurface);
	}
	//2: moving up and down
	else if(recMessage[0] == '2')
	{
		cout<<"in update function, state is "<<recMessage[0]<<endl;
		set_state(gmoveSurface, textSurface);
	}
	//3: sitting
	else if(recMessage[0] == '3')
	{
		//0: too left
		if(recMessage[2] == '0')
		{
			cout<<"in update function, state is "<<recMessage[2]<<endl;
			set_state(gtooleftSurface, textSurface);
		}
		//1: left
		else if(recMessage[2] == '1')
		{
			cout<<"in update function, state is "<<recMessage[2]<<endl;
			set_state(gleftSurface, textSurface);
		}
		//2: sit well
		else if(recMessage[2] == '2')
		{
			cout<<"in update function, state is "<<recMessage[2]<<endl;
			set_state(gsitwellSurface, textSurface);
		}
		//3: right
		else if(recMessage[2] == '3') 
		{
			cout<<"in update function, state is "<<recMessage[2]<<endl;
			set_state(grightSurface, textSurface);
		}
		//4: too right
		else if(recMessage[2] == '4') 
		{
			cout<<"in update function, state is "<<recMessage[2]<<endl;
			set_state(gtoorightSurface, textSurface);
		}
		//5: move left and right
		else if(recMessage[2] == '5') 
		{
			cout<<"in update function, state is "<<recMessage[2]<<endl;
			set_state(gtiltSurface, textSurface);
		}
	}
	//4: sitting too long
	else if(recMessage[0] == '4') 
	{
		cout<<"in update function, state is "<<recMessage[0]<<endl;
		set_state(gsitlongSurface, textSurface);
	}
	return;
}

void set_state(SDL_Surface *surface, SDL_Surface *dataSurface)
{
	//set screen
	SDL_Rect stretchRect;
	stretchRect.x = 0;
	stretchRect.y = 0;
	stretchRect.w = SCREEN_WIDTH;
	stretchRect.h = SCREEN_HEIGHT;

	SDL_Rect exitRect;
	exitRect.x = SCREEN_WIDTH*5/6;
	exitRect.y = SCREEN_HEIGHT*3/4;
	exitRect.w = SCREEN_WIDTH/6;
	exitRect.h = SCREEN_HEIGHT/4;

	SDL_Rect dataRect;
	dataRect.x = 0;
	dataRect.y = SCREEN_HEIGHT*3/4;
	dataRect.w = SCREEN_WIDTH*5/6;
	dataRect.h = SCREEN_HEIGHT/4;

	SDL_BlitScaled( surface, NULL, gScreenSurface, &stretchRect );
	SDL_BlitScaled( gexitSurface, NULL, gScreenSurface, &exitRect );
	if(dataSurface != NULL)
	{
		SDL_BlitScaled( dataSurface, NULL, gScreenSurface, &dataRect );
	}
	//Update the surface
	SDL_UpdateWindowSurface( gWindow );
	return;
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//create a socket
			char recMessage[20] = {};
			char message[] = {"request"};
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
			serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");	//IP address. inet_addr is convert address from string to int
			//serverInfo.sin_addr.s_addr = inet_addr("192.168.43.8");
			serverInfo.sin_port = htons(8700);						//trans local endian to net endian

			//connect is to get data from other, while bind is to bind own ad
			bind(socket_fd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
			
			//Main loop flag
			bool quit = false;
			bool mouse = false;
			//state, false for start, true for running
			bool state = false;

			//Event handler
			SDL_Event e;

			//Start button view point
			SDL_Rect buttonRect;
			buttonRect.x = SCREEN_WIDTH*2/5;
			buttonRect.y = SCREEN_HEIGHT*2/3;
			buttonRect.w = SCREEN_WIDTH/5;
			buttonRect.h = SCREEN_HEIGHT/6;
			//exit button
			SDL_Rect exitRect;
			exitRect.x = SCREEN_WIDTH*5/6;
			exitRect.y = SCREEN_HEIGHT*3/4;
			exitRect.w = SCREEN_WIDTH/6;
			exitRect.h = SCREEN_HEIGHT/4;

			reset_screen(buttonRect);

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_MOUSEBUTTONDOWN )
					{
						mouse = true;
					}
					else if( e.type == SDL_MOUSEBUTTONUP && mouse )
					{
						cout<<"mouse up"<<endl;
						if( !checkmousepos(buttonRect) && !state)
						{
							cout<<"starting state"<<endl;
							set_state(gconnectSurface, NULL);
							state = true;
							//we need to continue checking if there are someone to send
        					listen(socket_fd,5);	//it allows 5 people in the waiting array
							forClientSockfd = accept(socket_fd,(struct sockaddr*) &clientInfo, &addrlen);	//clientInfo is empty, used to store the received info of client
						}
						else if(state)
						{
							cout<<"running socket"<<endl;
							cout<<checkmousepos(exitRect);
							if(!checkmousepos(exitRect))
							{
								cout<<"close client"<<endl;
								close(forClientSockfd);
								state = false;
								reset_screen(buttonRect);
							}
						}
						mouse = false;
					}
				}
				if(state)
				{
					int sendbit = send(forClientSockfd,message,sizeof(message),0);
					cout<<"sendbit = "<<sendbit<<endl;
					int recv_status;
					if (sendbit != 0)
					{
						recv_status = recv(forClientSockfd,recMessage,sizeof(recMessage),0);
						cout << "recv_status = " << recv_status << endl ;
						if(recv_status == 0 || recv_status == -1)
						{
							//create a pic of failed connect and restart
							cout<<"not recieve anything"<<endl;
							recMessage[0] == 'x';
							quit = true;
							close(forClientSockfd);
							break;
						}
						else if(recMessage[0] == 'x') 
						{
							cout<<"not recieve anything"<<endl;
							quit = true;
							close(forClientSockfd);
						}
						cout<<"The message is : "<<recMessage<<endl;
						update(recMessage);
						sleep(1);
						sendbit = 0;
					}
				}
			}
			cout<<"close server"<<endl;
			close(socket_fd);
		}
	}
	//Free resources and close SDL
	close();

	return 0;
}