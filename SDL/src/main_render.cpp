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

#include "LTexture.h"
#include "LButton.h"

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close_SDL();

//set the start view
void reset_screen();
void update(char *recMessage);
//refresh the screen with current state
void set_state(LTexture currentTexture, string data);


//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

LTexture gbegin;		//Begin image
LTexture gconnect;		//Connect image
LTexture gempty;		//empty image
LTexture gmove;			//move up and down image
LTexture gtooleft;		//too left image
LTexture gleft;			//left image
LTexture gsitwell;		//sit well image
LTexture gright;		//right image
LTexture gtooright;		//too right image
LTexture gtilt;			//moving right left image
LTexture gsitlong;		//sitting long image
//LTexture gstart;		//start button
//LTexture gexit;			//exit button

//Globally used font
TTF_Font *gFont;

SDL_Color textColor = { 0, 0, 0, 0xFF};

//Buttons objects
LButton gstartButton;
LButton gexitButton;


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO  ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Smart Cusion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				/*
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
				*/
				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;

				}

			}
		}
	}
	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Loading image
	if(!gbegin.loadFromFile( "pic/smart_cushion.bmp" ))
	{
		printf( "Failed to load smart_cushion image!\n" );
		success = false;
	}
	if(!gconnect.loadFromFile( "pic/connect.bmp" ))
	{
		printf( "Failed to load connect image!\n" );
		success = false;
	}
	if(!gempty.loadFromFile( "pic/empty.bmp" ))
	{
		printf( "Failed to load empty image!\n");
		success = false;
	}
	if(!gmove.loadFromFile( "pic/move.bmp" ))
	{
		printf( "Failed to load move image!\n" );
		success = false;
	};
	if(!gtooleft.loadFromFile( "pic/too_left.bmp" ))
	{
		printf( "Failed to load too_left image!\n" );
		success = false;
	}
	if(!gleft.loadFromFile( "pic/tilt_left.bmp" ))
	{
		printf( "Failed to load tilt_left image!\n" );
		success = false;
	}
	if(!gsitwell.loadFromFile( "pic/sitwell.bmp" ))
	{
		printf( "Failed to load sitwell image!\n" );
		success = false;
	}
	if(!gright.loadFromFile( "pic/tilt_right.bmp" ))
	{
		printf( "Failed to load tilt_right image!\n" );
		success = false;
	}
	if(!gtooright.loadFromFile( "pic/too_right.bmp" ))
	{
		printf( "Failed to load too_right image!\n" );
		success = false;
	}
	if(!gtilt.loadFromFile( "pic/tilt.bmp" ))
	{
		printf( "Failed to load tilt image!\n" );
		success = false;
	}
	if(!gsitlong.loadFromFile( "pic/sitting_long.bmp" ))
	{
		printf( "Failed to load sitting_long image!\n" );
		success = false;
	}
	if(!gTexture[0].loadFromFile( "pic/start_icon.bmp" ))
	{
		printf( "Failed to load start_icon image!\n" );
		success = false;
	}
	if(!gTexture[1].loadFromFile( "pic/exit.bmp" ))
	{
		printf( "Failed to load exit image!\n" );
		success = false;
	}

	/////set sprite

	//set buttons
	gstartButton.setPosition( SCREEN_WIDTH*2/5, SCREEN_HEIGHT*2/3);
    gexitButton.setPosition( SCREEN_WIDTH*5/6, SCREEN_HEIGHT*3/4);
	gTexture[0].setHeight(BUTTON_HEIGHT);
	gTexture[0].setWidth(BUTTON_WIDTH);
	gTexture[1].setHeight(BUTTON_HEIGHT);
	gTexture[1].setWidth(BUTTON_WIDTH);

	return success;
}

void close_SDL()
{
	//Free loaded images
	gbegin.free();
	gempty.free();
	gmove.free();
	gtooleft.free();
	gleft.free();
	gsitwell.free();
	gright.free();
	gtooright.free();
	gtilt.free();
	gsitlong.free();
	gTexture[0].free();
	gTexture[1].free();


	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

//set the start view
void reset_screen()
{
	SDL_RenderClear( gRenderer );
	//Apply the image stretched
	gbegin.render(0,0,0);
	gstartButton.render(0);

	//Update screen
	SDL_RenderPresent( gRenderer );
}

void update(char *recMessage)
{
	//set screen
	//convert data to string
	
	string dataone = "", datatwo = "" , datathree = "" , data = ""; 
    for (int i = 0; i < 5; i++) { 
        dataone = dataone + recMessage[4+i];
		datatwo = datatwo + recMessage[10+i];
		datathree = datathree + recMessage[16+i];

    }
	data = "left pressure = "+dataone+", right pressure = "+datatwo+", front pressure = "+datathree;
	cout << data <<endl;
	gFont = TTF_OpenFont( "text/lazy.ttf", 28 );
	if(!gFont) cout<<"gFont error, "<<TTF_GetError()<<endl;
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, data.c_str(), textColor );
	if(!textSurface) cout<<"error"<<endl;

	//0: nonstart; 1:empty
	if(recMessage[0] == '0' || recMessage[0] == '1')
	{
		cout<<"in update function, state is "<<recMessage[0]<<endl;
		//set_state(gempty, data);
		gempty.render(0,0,0);
	}
	//2: moving up and down
	else if(recMessage[0] == '2')
	{
		cout<<"in update function, state is "<<recMessage[0]<<endl;
		//set_state(gmove, data);
		gmove.render(0,0,0);
	}
	//3: sitting
	else if(recMessage[0] == '3')
	{
		//0: too left
		if(recMessage[2] == '0')
		{
			cout<<"in update function, state is "<<recMessage[2]<<endl;
			//set_state(gtooleft, data);
			gtooleft.render(0,0,0);
		}
		//1: left
		else if(recMessage[2] == '1')
		{
			cout<<"in update function, state is "<<recMessage[2]<<endl;
			//set_state(gleft, data);
			gleft.render(0,0,0);
		}
		//2: sit well
		else if(recMessage[2] == '2')
		{
			cout<<"in update function, state is "<<recMessage[2]<<endl;
			//set_state(gsitwell, data);
			gsitwell.render(0,0,0);
		}
		//3: right
		else if(recMessage[2] == '3') 
		{
			cout<<"in update function, state is "<<recMessage[2]<<endl;
			//set_state(gright, data);
			gright.render(0,0,0);
		}
		//4: too right
		else if(recMessage[2] == '4') 
		{
			cout<<"in update function, state is "<<recMessage[2]<<endl;
			//set_state(gtooright, data);
			gtooright.render(0,0,0);
		}
		//5: move left and right
		else if(recMessage[2] == '5') 
		{
			cout<<"in update function, state is "<<recMessage[2]<<endl;
			//set_state(gtilt, data);
			gtilt.render(0,0,0);
		}
	}
	//4: sitting too long
	else if(recMessage[0] == '4') 
	{
		cout<<"in update function, state is "<<recMessage[0]<<endl;
		//set_state(gsitlong, data);
		gsitlong.render(0,0,0);
	}
	gexitButton.render(1);
	cout<<"render exit button"<<endl;
	SDL_RenderPresent( gRenderer );
	return;
}

void set_state(LTexture currentTexture, string data)
{
	cout<<"set state"<<endl;
	SDL_RenderClear( gRenderer );
	cout<<"clear"<<endl;
	currentTexture.render(0,0,0);
	cout<<"render current texture"<<endl;
	gexitButton.render(1);
	cout<<"render exit button"<<endl;
	SDL_RenderPresent( gRenderer );
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

			serverInfo.sin_family = PF_INET;						//sockaddr_in is IPv4
			serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");	//IP address. inet_addr is convert address from string to int
			//serverInfo.sin_addr.s_addr = inet_addr("192.168.43.8");
			serverInfo.sin_port = htons(8700);						//trans local endian to net endian

			//connect is to get data from other, while bind is to bind own ad
			bind(socket_fd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
			
			//Main loop flag
			bool quit = false;
			//state, false for start, true for running
			bool state = false;

			//Event handler
			SDL_Event e;

			reset_screen();

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
					//Handle button events
					//if in start stage and someone click start
					if(!state && gstartButton.handleEvent(&e))
					{
						cout<<"in start stage and someone click start"<<endl;
						state = true;
						//set_state(gconnect, "0");
						gconnect.render(0,0,0);
						SDL_RenderPresent( gRenderer );
						listen(socket_fd,5);	//it allows 5 people in the waiting array
						forClientSockfd = accept(socket_fd,(struct sockaddr*) &clientInfo, &addrlen);	//clientInfo is empty, used to store the received info of client
					}
					//if in running stage and someone click exit
					else if(state && gexitButton.handleEvent(&e)) 
					{
						cout<<"in running stage and someone click exit"<<endl;
						cout<<"close client"<<endl;
						close(forClientSockfd);
						state = false;
						reset_screen();
					}
				}
				if(state)
				{
					char recMessage[21] = {};
					int sendbit = send(forClientSockfd,message,sizeof(message),0);
					cout<<"sendbit = "<<sendbit<<endl;
					int recv_status = 0;
					if (sendbit != 0)
					{
						recv_status = recv(forClientSockfd,recMessage,sizeof(recMessage),0);
						cout << "recv_status = " << recv_status << endl ;
						if(recv_status == 0 || recv_status == -1)
						{
							//create a pic of failed connect and restart
							cout<<"not recieve anything"<<endl;
							state = false;
							close(forClientSockfd);
							break;
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
	close_SDL();

	return 0;
}