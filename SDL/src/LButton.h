#ifndef LBUTTON_H_INCLUDED
#define LBUTTON_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "LTexture.h"

//Button constants
const int BUTTON_WIDTH = 120;
const int BUTTON_HEIGHT = 110;
const int TOTAL_BUTTONS = 2;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4 ,
	BUTTON_SPRITE_MOUSE_OVER = 5
};

extern SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];

extern LTexture gTexture[ TOTAL_BUTTONS ];
//button0 = start
//button1 = help
//button2 = quit
//button3 = back

//The mouse button
class LButton
{
	public:
		//Initializes internal variables
		LButton();

		//Sets top left position
		void setPosition( int x, int y );

		//Handles mouse event
		bool handleEvent( SDL_Event* e );

		//Shows button sprite
		void render(int);


	private:
		//Top left position
		SDL_Point mPosition;

		//Currently used global sprite
		LButtonSprite mCurrentSprite;
};



#endif // LBUTTON_H_INCLUDED
