#ifndef _SETTING_H_
#define _SETTING_H_

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Loads individual image
//SDL_Surface* loadSurface( std::string path );

//The window we'll be rendering to
inline SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreenSurface ;//= NULL;

//Begin image
SDL_Surface* gSurface;// = NULL;

//Start button image
SDL_Surface* gbuttonSurface ;//= NULL;

//Running image
SDL_Surface* grunningSurface ;//= NULL;

//sitting long image
SDL_Surface* gsittingSurface ;//= NULL;

//left image
SDL_Surface* gleftSurface ;//= NULL;

//right image
SDL_Surface* grightSurface;// = NULL;

//exit image
SDL_Surface* gexitSurface ;//= NULL;

#endif