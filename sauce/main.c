#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2\SDL.h>
#include "types.h"
#include "transform.h"
#define defaulttransform  1.0, 1.0, 0.0, 0.0, 0.0
#define pi 3.1415926535
#define defaultscreenwidth 640
#define defaultscreenheight 480

int screenwidth = 1000;
int screenheight = 800;
int halfscreenwidth;
int halfscreenheight;
SDL_Window * window;
SDL_Renderer * renderer;
SDL_Event event;
SDL_Color clearcolor;

transform globaltransform;
transform3d globaltransform3d;
vec2 mouseposition;
double arrowsize = 10.0;
double dt = 0.0;
double screenratio = (double)defaultscreenwidth / (double)defaultscreenheight;
bool disableglobal = false;
bool disablelocal = false;
bool disableglobal3d = false;
bool disablelocal3d = false;
bool init = true;
bool screenchanged = false;
vec3 data[100][100];

void loop(void){
	
	//declare vars

	static vec3 v1;
	static vec3 v0;
	static transform3d t0;
	int iz = 0;
	int ix = 0;
	//end declare vars
	
	if(init){ //init vars
		iz = 0;
		ix = 0;
		for(double z = -1.0; iz < 100; z += .02, iz++){ //z from -1.0 to 1.0
			for(double x = -1.0; ix < 100; x += .02, ix++){ //x from -1.0 to 1.0
				data[ix][iz].y = x * (x*x - 3*z*z);
				data[ix][iz].x = x;
				data[ix][iz].z = z;
				printf("(%f, %f)\n", data[ix][iz].x, data[ix][iz].z);
			}
		}
		
		setclearcolor(0,0,0);
		srand(time(0));
		
		setscale3d(&t0, .4, .4, .4);
		settranslate3d(&t0, 0.0, 0.0, 0.0);
		setrotate3d(&t0, 0.0, 0.0, 0.0);
		
	} //end init vars
	
	//draw
	clear();
	rotate3d(&t0, .3 * dt, .2 * dt, .6 * dt);
	setcolor(0, 255, 0);
	for(iz = 0; iz < 100; iz++){
		for(ix = 0; ix < 100; ix++){
			
			setvec3(&v1, data[ix][iz].x, data[ix][iz].y, data[ix][iz].z);
			applytransformvec3(t0, &v1);
			drawvec3(v1);
			if(ix < 99){
				setvec3(&v0, data[ix+1][iz].x, data[ix+1][iz].y, data[ix+1][iz].z);
				applytransformvec3(t0, &v0);
				drawlinevec3(v1, v0);
			}
			if(iz < 99){
				setvec3(&v0, data[ix][iz+1].x, data[ix][iz+1].y, data[ix][iz+1].z);
				applytransformvec3(t0, &v0);
				drawlinevec3(v1, v0);
			}
		}
	}
	
	flip();
	//end draw
	
	return;
}

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
	
	halfscreenwidth = screenwidth / 2;
	halfscreenheight = screenheight / 2;
	clearcolor = (SDL_Color){.r = 255, .g = 255, .b = 255, .a = 255};
	clear();
	setglobaltransform();
	setglobaltransform3d();
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
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
						screenchanged = true;
						break;
					default:
						break;
				}
			}
		}
		//do stuff
		updatemouse();
		setdt();
		loop();
		init = false;
		screenchanged = false;
	}
	
	return 0;
}

//main loop


//render functions

void drawvec2(vec2 v){ //renders pixel
	if(!disableglobal){
		applytransformvec2(globaltransform, &v);
	}	
	SDL_RenderDrawPoint(renderer, (int)v.x, (int)v.y);
};

void drawvec3(vec3 v){ //renders pixel
	if(!disableglobal3d){
		applytransformvec3(globaltransform3d, &v);
	}	
	SDL_RenderDrawPoint(renderer, (int)v.x, (int)v.y);
};

void drawpoint(point p){ //renders pixel
	if(!disablelocal){
		applypointtransform(p.localtransform, &p);
	}
	if(!disableglobal){
		applypointtransform(globaltransform, &p);
	}
	SDL_RenderDrawPoint(renderer, (int)p.x, (int)p.y);
};


void drawlinevec2(vec2 v1, vec2 v2){ //renders point line
	if(!disableglobal){
		applytransformvec2(globaltransform, &v1);
		applytransformvec2(globaltransform, &v2);
	}
	SDL_RenderDrawLine(renderer, (int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y);
};

void drawlinevec3(vec3 v1, vec3 v2){ //renders point line
	if(!disableglobal3d){
		applytransformvec3(globaltransform3d, &v1);
		applytransformvec3(globaltransform3d, &v2);
	}
	SDL_RenderDrawLine(renderer, (int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y);
};

void drawpointline(point p1, point p2){ //renders point line
	if(!disablelocal){
		applypointtransform(p1.localtransform, &p1);
		applypointtransform(p2.localtransform, &p2);
	}	
	if(!disableglobal){
		applypointtransform(globaltransform, &p1);
		applypointtransform(globaltransform, &p2);
	}
	SDL_RenderDrawLine(renderer, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
};

void drawline(line l){ //renders line
	if(!disablelocal){
		applytransformvec2(l.localtransform, &(l.vertex[0]));
		applytransformvec2(l.localtransform, &(l.vertex[1]));
	}
	if(!disableglobal){
		applytransformvec2(globaltransform, &(l.vertex[0]));
		applytransformvec2(globaltransform, &(l.vertex[1]));
	}
	//printf("(%f, %f) -> (%f, %f)\n", l.vertex[0].x, l.vertex[0].y, l.vertex[1].x, l.vertex[1].y);
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
	scalevec2(&right, arrowsize/globaltransform.scale.x, arrowsize/globaltransform.scale.x); //scale arrowhead size to "arrowsize" pixels
	scalevec2(&left, arrowsize/globaltransform.scale.x, arrowsize/globaltransform.scale.x); //TODO: make arrowhead size variable
	translatevec2(&right, l.vertex[dir].x, l.vertex[dir].y); //move arrowhead back to selected line vertex
	translatevec2(&left, l.vertex[dir].x, l.vertex[dir].y);
	//draw arrowhead
	temp.vertex[0] = center;
	temp.vertex[1] = right;
	drawline(temp);
	temp.vertex[1] = left;
	drawline(temp);
};

void drawtriangle(triangle t){
	line temp;
	temp.localtransform = t.localtransform;
	temp.vertex[0].x = t.vertex[0].x;
	temp.vertex[0].y = t.vertex[0].y;
	temp.vertex[1].x = t.vertex[1].x;
	temp.vertex[1].y = t.vertex[1].y;
	drawline(temp);
	temp.vertex[0].x = t.vertex[1].x;
	temp.vertex[0].y = t.vertex[1].y;
	temp.vertex[1].x = t.vertex[2].x;
	temp.vertex[1].y = t.vertex[2].y;
	drawline(temp);
	temp.vertex[0].x = t.vertex[2].x;
	temp.vertex[0].y = t.vertex[2].y;
	temp.vertex[1].x = t.vertex[0].x;
	temp.vertex[1].y = t.vertex[0].y;
	drawline(temp);
}

void drawfilledtriangle(triangle t){
	//pretransform triangle verts to avoid problems
	applytriangletransform(t.localtransform, &t);
	applytriangletransform(globaltransform, &t);
	
	//sort points by y;
	if(t.vertex[1].y < t.vertex[0].y){swapvec2(&(t.vertex[0]), &(t.vertex[1]));}
	if(t.vertex[2].y < t.vertex[1].y){swapvec2(&(t.vertex[1]), &(t.vertex[2]));}
	if(t.vertex[1].y < t.vertex[0].y){swapvec2(&(t.vertex[0]), &(t.vertex[1]));}
	
	//get line slopes
	t.vertex[0].y = round(t.vertex[0].y);
	t.vertex[0].x = round(t.vertex[0].x);
	t.vertex[1].y = round(t.vertex[1].y);
	t.vertex[1].x = round(t.vertex[1].x);
	t.vertex[2].y = round(t.vertex[2].y);
	t.vertex[2].x = round(t.vertex[2].x);
	
	double v0tov2 = (t.vertex[2].x - t.vertex[0].x) / (t.vertex[2].y - t.vertex[0].y);
	double v0tov1 = (t.vertex[1].x - t.vertex[0].x) / (t.vertex[1].y - t.vertex[0].y);
	double v1tov2 = (t.vertex[2].x - t.vertex[1].x) / (t.vertex[2].y - t.vertex[1].y);
	
	//loop vars
	bool switchslope = false;
	line temp;
	temp.vertex[0].x = temp.vertex[1].x = t.vertex[0].x; //set line vert x's equal to top point x
	disablelocal = true;
	disableglobal = true;
	for(temp.vertex[0].y = temp.vertex[1].y = t.vertex[0].y;	//set line vert y's equal to top point y
		temp.vertex[0].y < t.vertex[2].y; 						//iterate while we havent passed the bottom of the triangle
		temp.vertex[0].y++, 									//step down a pixel at a time
		temp.vertex[1].y = temp.vertex[0].y){					//scanline is horizontal, set the y's equal
		
		if(temp.vertex[0].y < t.vertex[1].y){ 
			temp.vertex[0].x += v0tov1;
			temp.vertex[1].x += v0tov2;	
		}else{
			if(!switchslope){ //this needs to be here or else there will be slop left from the top half slope
				temp.vertex[0].x = t.vertex[1].x;
				switchslope = true;
			}else{ //just remove else and brackets to observe the effect
				temp.vertex[0].x += v1tov2;	
				temp.vertex[1].x += v0tov2;
			}
		}
		drawline(temp);
	}
	disableglobal = false;
	disablelocal = false;
}

void drawlinemesh(mesh m){
	line temp;
	setlinetransform(&temp, m.localtransform.scale.x, 
							m.localtransform.scale.y, 
							m.localtransform.rotate, 
							m.localtransform.translate.x, 
							m.localtransform.translate.x);
							
	for(int iter = 0; iter < m.edgecount; iter++){
		temp.vertex[0] = m.vertex[m.edge[iter].v1];
		temp.vertex[1] = m.vertex[m.edge[iter].v2];
		drawline(temp);
	}
}

void drawpointmesh(mesh m){
	point temp;
	setpointtransform(&temp, m.localtransform.scale.x, 
							 m.localtransform.scale.y, 
							 m.localtransform.rotate, 
							 m.localtransform.translate.x, 
							 m.localtransform.translate.x);
							
	for(int iter = 0; iter < m.vertexcount; iter++){
		temp.x = m.vertex[iter].x;
		temp.y = m.vertex[iter].y;
		drawpoint(temp);
	}
}

//utility

void updatemouse(void){
	int dx;
	int dy;
	SDL_GetMouseState(&dx, &dy);
	mouseposition.x = (double) dx;
	mouseposition.y = (double) dy;
	translatevec2(&mouseposition, -globaltransform.translate.x, -globaltransform.translate.y);
	scalevec2(&mouseposition, 1.0/globaltransform.scale.x, 1.0/globaltransform.scale.y);
};

void setdt(void){
	static Uint32 oldtime;
	static bool init = true;
	if(init){
		oldtime = SDL_GetTicks();
		init = false;
	}
	dt = (double)(SDL_GetTicks() - oldtime) / 1000.0;
	oldtime = SDL_GetTicks();
};

void setcolor(Uint8 r, Uint8 g, Uint8 b){
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
};

void setcolorwithalpha(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
};

void setclearcolor(Uint8 r, Uint8 g, Uint8 b){
	clearcolor.r = r;
	clearcolor.g = g;
	clearcolor.b = b;
	clearcolor.a = 255;
};


void clear(void){
	static SDL_Rect temp;
	setcolor(clearcolor.r,clearcolor.g,clearcolor.b);
	SDL_RenderClear(renderer);
};

void flip(void){
	SDL_RenderPresent(renderer);
};

void delay(int s){
	SDL_Delay(s);
};
