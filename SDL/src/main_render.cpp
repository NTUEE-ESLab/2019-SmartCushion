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
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close_SDL();

//set the start view
void reset_screen(bool start);

//refresh the screen with current state
void update(char *recMessage);
void message();


//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

LTexture gbegin;		//Begin image
LTexture gconnect;		//Connect image
LTexture gerror;		//error image
LTexture gempty;		//empty image
LTexture gmove[10];			//move up and down image
LTexture gtooleft;		//too left image
LTexture gleft;			//left image
LTexture gsitwell;		//sit well image
LTexture gright;		//right image
LTexture gtooright;		//too right image
LTexture gtilt[9];			//moving right left image
LTexture gsitlong;		//sitting long image
LTexture gfront;		//sit toward front
LTexture gdata[3];			//data

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
	string pathone = "pic/", pathtwo = ".bmp";
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
	if(!gerror.loadFromFile( "pic/error.bmp" ))
	{
		printf( "Failed to load error image!\n" );
		success = false;
	}
	if(!gempty.loadFromFile( "pic/empty.bmp" ))
	{
		printf( "Failed to load empty image!\n");
		success = false;
	}
	for(int i=0 ; i<10 ; i++)
	{
		string path =pathone + "move" + to_string(i) + pathtwo;
		if(!gmove[i].loadFromFile( path ))
		{
			printf( "Failed to load move%d image!\n",i);
			success = false;
		}
	}
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
	for(int i=0 ; i<9 ; i++)
	{
		string path =pathone + "tilt" + to_string(i) + pathtwo;
		if(!gtilt[i].loadFromFile( path ))
		{
			printf( "Failed to load tilt %d image!\n",i);
			success = false;
		}
	}
	if(!gsitlong.loadFromFile( "pic/sitting_long.bmp" ))
	{
		printf( "Failed to load sitting_long image!\n" );
		success = false;
	}
	if(!gfront.loadFromFile( "pic/front.bmp" ))
	{
		printf( "Failed to load front image!\n" );
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
	gstartButton.setPosition( 390, 350);
    gexitButton.setPosition( 780, 510);
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
	for(int i=0 ; i<10 ;i++)	gmove[i].free();
	gtooleft.free();
	gleft.free();
	gsitwell.free();
	gright.free();
	gtooright.free();
	for(int i=0 ; i<9 ;i++)	gtilt[i].free();
	gsitlong.free();
	gTexture[0].free();
	gTexture[1].free();

	//Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

//set the start or error view
void reset_screen(bool start)
{
	SDL_RenderClear( gRenderer );
	//Apply the image stretched
	if (start)
	{
		gbegin.render(0,0,0);
		gstartButton.render(0);
	}
	else 
		gerror.render(0,0,0);

	//Update screen
	SDL_RenderPresent( gRenderer );
}

void update(char *recMessage)
{
	//set screen
	//convert data to string
	bool showdata = true;
	string data[3]; 
    for (int i = 0; i < 5; i++) { 
        data[0] = data[0] + recMessage[4+i];
		data[1] = data[1] + recMessage[10+i];
		data[2] = data[2] + recMessage[16+i];
    }
	string datacon = "left pressure = "+data[0]+", right pressure = "+data[1]+", front pressure = "+data[2];
	cout << datacon <<endl;

	//0: nonstart; 1:empty
	if(recMessage[0] == '0' || recMessage[0] == '1')
		gempty.render(0,0,0);
	//2: moving up and down
	else if(recMessage[0] == '2')
	{
		for(int i=0 ; i<20 ; i++)
		{
			int j = i%10 ;
			gmove[j].render(0,0,0);
			gexitButton.render(1);
			SDL_RenderPresent( gRenderer );
			SDL_Delay(75);
		}
		showdata = false ;
	}
	//3: sitting
	else if(recMessage[0] == '3')
	{
		//1: too front
		if(recMessage[2] == '1') 
			gfront.render(0,0,0);
		//0: too left
		else if(recMessage[1] == '0')
			gtooleft.render(0,0,0);
		//1: left
		else if(recMessage[1] == '1')
			gleft.render(0,0,0);
		//2: sit well
		else if(recMessage[1] == '2' && recMessage[2] == '0')
			gsitwell.render(0,0,0);
		//3: right
		else if(recMessage[1] == '3') 
			gright.render(0,0,0);
		//4: too right
		else if(recMessage[1] == '4') 
			gtooright.render(0,0,0);
		//5: move left and right
		else if(recMessage[1] == '5') 
		{
			for(int i=0 ; i<20 ; i++)
			{
				int j = i%8 ;
				gtilt[j].render(0,0,0);
				gexitButton.render(1);
				SDL_RenderPresent( gRenderer );
				SDL_Delay(125);
			}
			showdata = false;
		}
	}
	//4: sitting too long
	else if(recMessage[0] == '4') 
		gsitlong.render(0,0,0);
	gexitButton.render(1);
	if (showdata)
		for(int i=0 ; i<3 ; i++)
		{
			gdata[i].loadFromRenderedText(data[i],textColor);
			gdata[i].render(775,150+48*i,0);
		}
	
	SDL_RenderPresent( gRenderer );
	if(recMessage[1] != '2' || recMessage[2] != '0')
		message();
	return;
}

void message()
{
	const SDL_MessageBoxButtonData button = { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "I know it" };
    
    const SDL_MessageBoxColorScheme colorScheme = {
        { /* .colors (.r, .g, .b) */
            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
            { 211, 211, 211 },
            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
            {   0,   0,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
            {  70, 130, 180 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
            { 173, 216, 230 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
            {  70, 130, 180 }
        }
    };
    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        gWindow, /* .window */
        "Warning", /* .title */
        "WARNNING!!!\nYou are sitting with unhealth posture\nPlease go check your status", /* .message */
        1, /* .numbuttons */
        &button, /* .buttons */
        &colorScheme /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("error displaying message box");
    }
    if (buttonid == -1) {
        SDL_Log("no selection");
    } else {
        SDL_Log("selection was %s", button.text);
    }
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
				reset_screen(false);
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

			reset_screen(true);

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
						reset_screen(true);
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