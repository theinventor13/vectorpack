extern transform globaltransform;
extern int screenwidth;
extern int screenheight;
extern int halfscreenwidth;
extern int halfscreenheight;

#define defaulttransform  1.0, 1.0, 0.0, 0.0, 0.0
#define pi 3.1415926535

// # = datatype (vec, point, tri, mesh) 
// ! = transformation (scale, rotate, translate, normalize)

// set! functions overwrite transform

//    ! functions transform transform

//init# functions overwrite datatype position values
//      and overwrite local transform with default

// set# functions overwrite datatype position values

//set#! functions overwrite datatype local transform

//   !# functions transform datatype position values

//   #! functions transform datatype local transform


//miscellaneous

void setglobaltransform(void){
	
	settranslate(&globaltransform, (double)halfscreenwidth, (double)halfscreenheight);
	
	if(screenwidth > screenheight){
		setscale(&globaltransform, (double)(halfscreenwidth), (double)(-halfscreenwidth));
	}else{
		setscale(&globaltransform, (double)(halfscreenheight), (double)(-halfscreenheight));
	}
		
	setrotate(&globaltransform, 0.0);
}

double wraprotate(double a){
	if(a > (pi * 2.0) || a < -(pi * 2.0)){
		return fmod(a, pi * 2.0);
	}else{
		return a;
	}
}


//general transform functions

void setscale(transform * t, double dx, double dy){
	t->scale.x = dx;
	t->scale.y = dy;
}

void scale(transform * t, double dx, double dy){
	t->scale.x *= dx;
	t->scale.y *= dy;
}

void settranslate(transform * t, double dx, double dy){
	t->translate.x = dx;
	t->translate.y = dy;
}


void translate(transform * t, double dx, double dy){
	t->translate.x += dx;
	t->translate.y += dy;
}

void setrotate(transform * t, double a){
	t->rotate = wraprotate(a);
}

void rotate(transform * t, double a){
	t->rotate += a;
	t->rotate = wraprotate(t->rotate);
}


//vec2 transform functions

void setvec2(vec2 * v, double x, double y){
	v->x = x;
	v->y = y;	
}

void applytransformvec2(transform t, vec2 * v){
	v->x *= t.scale.x;
	v->y *= t.scale.y;
	double newx = cos(t.rotate) * v->x - sin(t.rotate) * v->y;
	double newy = sin(t.rotate) * v->x + cos(t.rotate) * v->y;
	v->x = newx + t.translate.x;
	v->y = newy + t.translate.y;	
}

void normalizevec2(vec2 * v){
	double mag = sqrt(v->x * v->x + v->y * v->y);
	v->x /= mag;
	v->y /= mag;
}

void scalevec2(vec2 * v, double dx, double dy){
	v->x *= dx;
	v->y *= dy;
}

void translatevec2(vec2 * v, double dx, double dy){
	v->x += dx;
	v->y += dy;
}

void rotatevec2(vec2 * v, double a){
	double newx = cos(a) * v->x - sin(a) * v->y;
	double newy = sin(a) * v->x + cos(a) * v->y;
	v->x = newx;
	v->y = newy;
}


//point transform functions

void setpoint(point * p, double x, double y){
	p->x = x;
	p->y = y;
}

void initpoint(point * p, double x, double y){
	setpoint(p, x, y);
	setpointtransform(p, defaulttransform);
}

void setpointtransform(point * p, double sx, double sy, double a, double tx, double ty){
	p->localtransform.scale.x = sx;
	p->localtransform.scale.y = sy;
	p->localtransform.scale.x = sx;
	p->localtransform.rotate = wraprotate(a);
	p->localtransform.translate.x = tx;
	p->localtransform.translate.y = ty;
}

void applypointtransform(transform t, point * p){
	p->x *= t.scale.x;
	p->y *= t.scale.y;
	double newx = cos(t.rotate) * p->x - sin(t.rotate) * p->y;
	double newy = sin(t.rotate) * p->x + cos(t.rotate) * p->y;
	p->x = newx + t.translate.x;
	p->y = newy + t.translate.y;	
}

void setpointscale(point * p, double dx, double dy){
	p->localtransform.scale.x = dx;
	p->localtransform.scale.y = dy;
}

void pointscale(point * p, double dx, double dy){
	p->localtransform.scale.x *= dx;
	p->localtransform.scale.y *= dy;
}

void setpointtranslate(point * p, double dx, double dy){
	p->localtransform.translate.x = dx;
	p->localtransform.translate.y = dy;
}

void pointtranslate(point * p, double dx, double dy){
	p->localtransform.translate.x += dx;
	p->localtransform.translate.y += dy;
}

void setpointrotate(point * p, double a){
	p->localtransform.rotate = wraprotate(a);
}

void pointrotate(point * p, double a){
	p->localtransform.rotate += a;
	p->localtransform.rotate = wraprotate(p->localtransform.rotate);
}

void scalepoint(point * p, double dx, double dy){
	p->x *= dx;
	p->y *= dy;
}

void translatepoint(point * p, double dx, double dy){
	p->x += dx;
	p->y += dy;
}

void rotatepoint(point * p, double a){
	double newx = cos(a) * p->x - sin(a) * p->y;
	double newy = sin(a) * p->x + cos(a) * p->y;
	p->x = newx;
	p->y = newy;
}

void normalizepoint(point * p){
	double mag = sqrt(p->x * p->x + p->y * p->y);
	p->x /= mag;
	p->y /= mag;
}


//line transform functions

void setline(line * l, double x1, double y1, double x2, double y2){
	l->vertex[0].x = x1;
	l->vertex[0].y = y1;
	l->vertex[1].x = x2;
	l->vertex[1].y = y2;
}

void initline(line * l, double x1, double y1, double x2, double y2){
	setline(l, x1, y1, x2, y2);
	setlinetransform(l, defaulttransform);
}

void setlinetransform(line * l, double sx, double sy, double a, double tx, double ty){
	l->localtransform.scale.x = sx;
	l->localtransform.scale.y = sy;
	l->localtransform.rotate = wraprotate(a);
	l->localtransform.translate.x = tx;
	l->localtransform.translate.y = ty;
}

void settransform(transform * t, double sx, double sy, double a, double tx, double ty){
	t->scale.x = sx;
	t->scale.y = sy;
	t->rotate = wraprotate(a);
	t->translate.x = tx;
	t->translate.y = ty;
}

void setlinescale(line * l, double dx, double dy){
	l->localtransform.scale.x = dx;
	l->localtransform.scale.y = dy;
}

void linescale(line * l, double dx, double dy){
	l->localtransform.scale.x *= dx;
	l->localtransform.scale.y *= dy;
}

void setlinetranslate(line * l, double dx, double dy){
	l->localtransform.translate.x = dx;
	l->localtransform.translate.y = dy;
}

void linetranslate(line * l, double dx, double dy){
	l->localtransform.translate.x += dx;
	l->localtransform.translate.y += dy;
}

void setlinerotate(line * l, double a){
	l->localtransform.rotate = wraprotate(a);
}

void linerotate(line * l, double a){
	l->localtransform.rotate += a;
	l->localtransform.rotate = wraprotate(l->localtransform.rotate);
}

void scaleline(line * l, double dx, double dy){
	l->vertex[0].x *= dx;
	l->vertex[0].y *= dy;
	l->vertex[1].x *= dx;
	l->vertex[1].y *= dy;
}

void translateline(line * l, double dx, double dy){
	l->vertex[0].x += dx;
	l->vertex[0].y += dy;
	l->vertex[1].x += dx;
	l->vertex[1].y += dy;
}

void rotateline(line * l, double a){
	double newx = cos(a) * l->vertex[0].x - sin(a) * l->vertex[0].y;
	double newy = sin(a) * l->vertex[0].x + cos(a) * l->vertex[0].y;
	l->vertex[0].x = newx;
	l->vertex[0].y = newy;
	newx = cos(a) * l->vertex[1].x - sin(a) * l->vertex[1].y;
	newy = sin(a) * l->vertex[1].x + cos(a) * l->vertex[1].y;
	l->vertex[1].x = newx;
	l->vertex[1].y = newy;
}

void normalizeline(line * l){
	double mag = sqrt(l->vertex[0].x * l->vertex[0].x + l->vertex[0].y * l->vertex[0].y);
	l->vertex[0].x /= mag;
	l->vertex[0].y /= mag;
	mag = sqrt(l->vertex[1].x * l->vertex[1].x + l->vertex[1].y * l->vertex[1].y);
	l->vertex[1].x /= mag;
	l->vertex[1].y /= mag;
}


//triangle transform functions

void settriangle(triangle * tri, double x1, double y1, double x2, double y2, double x3, double y3){
	tri->vertex[0].x = x1;
	tri->vertex[0].y = y1;
	tri->vertex[1].x = x2;
	tri->vertex[1].y = y2;
	tri->vertex[2].x = x3;
	tri->vertex[2].y = y3;
}

void inittriangle(triangle * tri, double x1, double y1, double x2, double y2, double x3, double y3){
	settriangle(tri, x1, y1, x2, y2, x3, y3);
	settriangletransform(tri, defaulttransform);
}

void settriangletransform(triangle * tri, double sx, double sy, double a, double tx, double ty){
	tri->localtransform.scale.x = sx;
	tri->localtransform.scale.y = sy;
	tri->localtransform.rotate = wraprotate(a);
	tri->localtransform.translate.x = tx;
	tri->localtransform.translate.y = ty;
}

void applytriangletransform(transform t, triangle * tri){
	tri->vertex[0].x *= t.scale.x;
	tri->vertex[0].y *= t.scale.y;
	double newx = cos(t.rotate) * tri->vertex[0].x - sin(t.rotate) * tri->vertex[0].y;
	double newy = sin(t.rotate) * tri->vertex[0].x + cos(t.rotate) * tri->vertex[0].y;
	tri->vertex[0].x = newx + t.translate.x;
	tri->vertex[0].y = newy + t.translate.y;
	
	tri->vertex[1].x *= t.scale.x;
	tri->vertex[1].y *= t.scale.y;
	newx = cos(t.rotate) * tri->vertex[1].x - sin(t.rotate) * tri->vertex[1].y;
	newy = sin(t.rotate) * tri->vertex[1].x + cos(t.rotate) * tri->vertex[1].y;
	tri->vertex[1].x = newx + t.translate.x;
	tri->vertex[1].y = newy + t.translate.y;
	
	tri->vertex[2].x *= t.scale.x;
	tri->vertex[2].y *= t.scale.y;
	newx = cos(t.rotate) * tri->vertex[2].x - sin(t.rotate) * tri->vertex[2].y;
	newy = sin(t.rotate) * tri->vertex[2].x + cos(t.rotate) * tri->vertex[2].y;
	tri->vertex[2].x = newx + t.translate.x;
	tri->vertex[2].y = newy + t.translate.y;
}

void settrianglescale(triangle * tri, double dx, double dy){
	tri->localtransform.scale.x = dx;
	tri->localtransform.scale.y = dy;
}

void trianglescale(triangle * tri, double dx, double dy){
	tri->localtransform.scale.x *= dx;
	tri->localtransform.scale.y *= dy;
}

void settriangletranslate(triangle * tri, double dx, double dy){
	tri->localtransform.translate.x = dx;
	tri->localtransform.translate.y = dy;
}

void triangletranslate(triangle * tri, double dx, double dy){
	tri->localtransform.translate.x += dx;
	tri->localtransform.translate.y += dy;
}

void settrianglerotate(triangle * tri, double a){
	tri->localtransform.rotate = wraprotate(a);
}

void trianglerotate(triangle * tri, double a){
	tri->localtransform.rotate += a;
	tri->localtransform.rotate = wraprotate(tri->localtransform.rotate);
}

void scaletriangle(triangle * tri, double dx, double dy){
	tri->vertex[0].x *= dx;
	tri->vertex[0].y *= dy;
	tri->vertex[1].x *= dx;
	tri->vertex[1].y *= dy;
	tri->vertex[2].x *= dx;
	tri->vertex[2].y *= dy;
}

void translatetriangle(triangle * tri, double dx, double dy){
	tri->vertex[0].x += dx;
	tri->vertex[0].y += dy;
	tri->vertex[1].x += dx;
	tri->vertex[1].y += dy;
	tri->vertex[2].x += dx;
	tri->vertex[2].y += dy;
}

void rotatetriangle(triangle * tri, double a){
	double newx = cos(a) * tri->vertex[0].x - sin(a) * tri->vertex[0].y;
	double newy = sin(a) * tri->vertex[0].x + cos(a) * tri->vertex[0].y;
	tri->vertex[0].x = newx;
	tri->vertex[0].y = newy;
	newx = cos(a) * tri->vertex[1].x - sin(a) * tri->vertex[1].y;
	newy = sin(a) * tri->vertex[1].x + cos(a) * tri->vertex[1].y;
	tri->vertex[1].x = newx;
	tri->vertex[1].y = newy;
	newx = cos(a) * tri->vertex[2].x - sin(a) * tri->vertex[2].y;
	newy = sin(a) * tri->vertex[2].x + cos(a) * tri->vertex[2].y;
	tri->vertex[2].x = newx;
	tri->vertex[2].y = newy;
}

void normalizetriangle(triangle * tri){
	double mag = sqrt(tri->vertex[0].x * tri->vertex[0].x + tri->vertex[0].y * tri->vertex[0].y);
	tri->vertex[0].x /= mag;
	tri->vertex[0].y /= mag;
	mag = sqrt(tri->vertex[1].x * tri->vertex[1].x + tri->vertex[1].y * tri->vertex[1].y);
	tri->vertex[1].x /= mag;
	tri->vertex[1].y /= mag;
	mag = sqrt(tri->vertex[2].x * tri->vertex[2].x + tri->vertex[2].y * tri->vertex[2].y);
	tri->vertex[2].x /= mag;
	tri->vertex[2].y /= mag;
}







