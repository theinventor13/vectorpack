#include <stdio.h>
#include <stdbool.h>
#include <SDL2\SDL.h>
#include "types.h"
#include "transform.h"
#define defaulttransform  1.0, 1.0, 0.0, 0.0, 0.0
#define pi 3.1415926535

int screenwidth = 640;
int screenheight = 480;
int halfscreenwidth = 320;
int halfscreenheight = 240;
SDL_Window * window;
SDL_Renderer * renderer;
SDL_Event event;
SDL_Color clearcolor;
bool init = true;

transform globaltransform;

void loop(void);

void drawpoint(point p);

void drawpointline(point p1, point p2);

void drawline(line l);

void drawvector(line l, bool dir);

int main(int argc, char ** argv){
	
	window = SDL_CreateWindow("Vector Sandbox", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenwidth, screenheight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if(!window){
		printf("SDL WINDOW ERROR\n");
		return 1;
	}
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!renderer){
		printf("SDL RENDERER ERROR\n");
		return 1;
	}
	
	clearcolor = (SDL_Color){.r = 255, .g = 255, .b = 255, .a = 255};
	setglobaltransform();
	
	
	bool quit = false;
	while(!quit){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				quit = true;
				break;
			}else if(event.type == SDL_WINDOWEVENT){
				switch(event.window.event){
					case SDL_WINDOWEVENT_CLOSE:
						quit = true;
						break;
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_MINIMIZED:
						SDL_GetWindowSize(window, &screenwidth, &screenheight);
						halfscreenwidth = screenwidth / 2; 
						halfscreenheight = screenheight / 2;
						setglobaltransform();
						break;
					default:
						break;
				}
			}
		}
		//do stuff
		loop();
	}
	
	return 0;
}

//main loop

void loop(void){
	
	//declare vars
	static line vector;
	//end declare vars
	
	if(init){ //init vars
		initline(&vector, .1, .2, .4, .2);
		init = false;
	} //end init vars
	
	//draw
	clear();
	linerotate(&vector, .1);
	setcolor(255,0,0);drawvector(vector, false);
	flip();
	delay(60);
	//end draw
	
	return;
}

//render functions

void drawpoint(point p){ //renders pixel
	applypointtransform(p.localtransform, &p);
	applypointtransform(globaltransform, &p);
	SDL_RenderDrawPoint(renderer, (int)p.x, (int)p.y);
};

void drawlinevec2(vec2 v1, vec2 v2){ //renders point line
	applytransformvec2(globaltransform, &v1);
	applytransformvec2(globaltransform, &v2);
	SDL_RenderDrawLine(renderer, (int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y);
};

void drawpointline(point p1, point p2){ //renders point line
	applypointtransform(p1.localtransform, &p1);
	applypointtransform(globaltransform, &p1);
	applypointtransform(p2.localtransform, &p2);
	applypointtransform(globaltransform, &p2);
	SDL_RenderDrawLine(renderer, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
};

void drawline(line l){ //renders line
	applytransformvec2(l.localtransform, &(l.vertex[0]));
	applytransformvec2(globaltransform, &(l.vertex[0]));
	applytransformvec2(l.localtransform, &(l.vertex[1]));
	applytransformvec2(globaltransform, &(l.vertex[1]));
	SDL_RenderDrawLine(renderer, (int)l.vertex[0].x, (int)l.vertex[0].y, (int)l.vertex[1].x, (int)l.vertex[1].y);
};

void drawvector(line l, bool dir){ //renders line indicating direction from vertex[!dir] to vertex[dir]
	vec2 left; //left point of arrowhead
	vec2 center; //center point of arrowhead
	vec2 right; //right point of arrowhead
	line temp; //for drawing the arrowhead
	temp.localtransform = l.localtransform; 
	drawline(l);
	center.x = l.vertex[dir].x; //arrowhead displays at the line vertex specified by dir, and away from the other
	center.y = l.vertex[dir].y;
	left.x = l.vertex[!dir].x - l.vertex[dir].x; //get opposite vector
	left.y = l.vertex[!dir].y - l.vertex[dir].y;
	right = left;
	rotatevec2(&right, (pi/4.0)); //rotate out sides of arrowhead
	rotatevec2(&left, -(pi/4.0)); 
	normalizevec2(&right);
	normalizevec2(&left);
	scalevec2(&right, 10.0/globaltransform.scale.x, 10.0/globaltransform.scale.x); //set arrowhead sides to be 10 pixels long
	scalevec2(&left, 10.0/globaltransform.scale.x, 10.0/globaltransform.scale.x); //TODO: make arrowhead size variable
	translatevec2(&right, l.vertex[dir].x, l.vertex[dir].y); //move arrowhead back to selected line vertex
	translatevec2(&left, l.vertex[dir].x, l.vertex[dir].y);
	//draw arrowhead
	temp.vertex[0] = center;
	temp.vertex[1] = right;
	drawline(temp);
	temp.vertex[1] = left;
	drawline(temp);
	temp.vertex[0] = right;
	drawline(temp);
};

//implement drawtriangle 



//utility

void setcolor(Uint8 r, Uint8 g, Uint8 b){
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
};

void setcolorwithalpha(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
};

void clear(void){
	setcolorwithalpha(clearcolor.r,clearcolor.g,clearcolor.b,clearcolor.a);
	SDL_RenderClear(renderer);
}

void flip(void){
	SDL_RenderPresent(renderer);
}

void delay(int s){
	SDL_Delay(s);
}
