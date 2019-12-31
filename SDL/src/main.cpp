/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include "socket_client.h"

///////use for socket
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

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//use for socket
void socket_client();

//Loads individual image
SDL_Surface* loadSurface( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Begin image
SDL_Surface* gSurface = NULL;

//Start button image
SDL_Surface* gbuttonSurface = NULL;

//Running image
SDL_Surface* grunningSurface = NULL;

//sitting long image
SDL_Surface* gsittingSurface = NULL;

//left image
SDL_Surface* gleftSurface = NULL;

//right image
SDL_Surface* grightSurface = NULL;

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
	gbuttonSurface = loadSurface( "pic/start_icon.bmp" );
	if( gbuttonSurface == NULL )
	{
		printf( "Failed to load start_icon image!\n" );
		success = false;
	}
	grunningSurface = loadSurface( "pic/running.bmp" );
	if( grunningSurface == NULL )
	{
		printf( "Failed to load running image!\n" );
		success = false;
	}
	gsittingSurface = loadSurface( "pic/sitting_long.bmp" );
	if( gsittingSurface == NULL )
	{
		printf( "Failed to load sitting_long image!\n" );
		success = false;
	}
	gleftSurface = loadSurface( "pic/tilt_left.bmp" );
	if( gleftSurface == NULL )
	{
		printf( "Failed to load tilt_left image!\n" );
		success = false;
	}
	grightSurface = loadSurface( "pic/tilt_right.bmp" );
	if( grightSurface == NULL )
	{
		printf( "Failed to load tilt_right image!\n" );
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
    char message[] = {"Hi, I've heard you!"};
    char receiveMessage[100] = {};
    char state[1] = {};
    //state: 0: healthy, 1:too right, 2:too left, 3:sit too long, 4: leave
    while(state[0] != '4')
    {
		SDL_Rect stretchRect;
		stretchRect.x = 0;
		stretchRect.y = 0;
		stretchRect.w = SCREEN_WIDTH;
		stretchRect.h = SCREEN_HEIGHT;
        recv(sockfd,state,1,0);
        if(state[0] == '0')
        {
			SDL_BlitScaled( grunningSurface, NULL, gScreenSurface, &stretchRect );
        }
        else if(state[0] == '1')
        {
			cout<<"too right"<<endl;
			SDL_BlitScaled( grightSurface, NULL, gScreenSurface, &stretchRect );
        }
        else if(state[0] == '2')
        {
			SDL_BlitScaled( gleftSurface, NULL, gScreenSurface, &stretchRect );
        }
        else if(state[0] == '3') 
		{
			SDL_BlitScaled( gsittingSurface, NULL, gScreenSurface, &stretchRect );
		}
		
		send(sockfd,message,sizeof(message),0);
        //recv(sockfd,receiveMessage,sizeof(receiveMessage),0);
		cout<<receiveMessage<<endl;
		
		//Update the surface
		SDL_UpdateWindowSurface( gWindow );
    }

    //cout<<receiveMessage<<endl;
    cout<<"close Socket"<<endl;
    close(sockfd);
    return ;
}

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
			//Main loop flag
			bool quit = false;
			bool mouse = false;

			//Event handler
			SDL_Event e;

			//Start button view point
			SDL_Rect buttonRect;
			buttonRect.x = SCREEN_WIDTH*2/5;
			buttonRect.y = SCREEN_HEIGHT*2/3;
			buttonRect.w = SCREEN_WIDTH/5;
			buttonRect.h = SCREEN_HEIGHT/6;

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
						int x,y;
						SDL_GetMouseState( &x, &y );
						if( x>buttonRect.x && x<buttonRect.x+buttonRect.w && y>buttonRect.y && y<buttonRect.y+buttonRect.h )
						{
							//Apply the image stretched
							SDL_Rect runningRect;
							runningRect.x = 0;
							runningRect.y = 0;
							runningRect.w = SCREEN_WIDTH;
							runningRect.h = SCREEN_HEIGHT;
							SDL_BlitScaled( grunningSurface, NULL, gScreenSurface, &runningRect );
							//Update the surface
							SDL_UpdateWindowSurface( gWindow );
							socket_client();
							reset_screen(buttonRect);
						}
						mouse = false;
					}
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}