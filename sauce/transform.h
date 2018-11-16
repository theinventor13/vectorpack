extern transform globaltransform;
extern int screenwidth;
extern int screenheight;
extern int halfscreenwidth;
extern int halfscreenheight;

#define defaulttransform  1.0, 1.0, 0.0, 0.0, 0.0

void setvec2(vec2 * v, double x, double y){
	v->x = x;
	v->y = y;	
}

void setpoint(point * p, double x, double y){
	p->x = x;
	p->y = y;
}

void initpoint(point * p, double x, double y){
	p->x = x;
	p->y = y;
	setpointtransform(p, defaulttransform);
}

void setline(line * l, double x1, double y1, double x2, double y2){
	l->vertex[0].x = x1;
	l->vertex[0].y = y1;
	l->vertex[1].x = x2;
	l->vertex[1].y = y2;
}

void initline(line * l, double x1, double y1, double x2, double y2){
	l->vertex[0].x = x1;
	l->vertex[0].y = y1;
	l->vertex[1].x = x2;
	l->vertex[1].y = y2;
	setlinetransform(l, defaulttransform);
}

void applytransformvec2(transform t, vec2 * v){
	v->x *= t.scale.x;
	v->y *= t.scale.y;
	double newx = cos(t.rotate) * v->x - sin(t.rotate) * v->y;
	double newy = sin(t.rotate) * v->x + cos(t.rotate) * v->y;
	v->x = newx + t.translate.x;
	v->y = newy + t.translate.y;	
}

void applypointtransform(transform t, point * p){
	p->x *= t.scale.x;
	p->y *= t.scale.y;
	double newx = cos(t.rotate) * p->x - sin(t.rotate) * p->y;
	double newy = sin(t.rotate) * p->x + cos(t.rotate) * p->y;
	p->x = newx + t.translate.x;
	p->y = newy + t.translate.y;	
}

void settransform(transform * t, double sx, double sy, double a, double tx, double ty){
	t->scale.x = sx;
	t->scale.y = sy;
	t->rotate = a;
	t->translate.x = tx;
	t->translate.y = ty;
}

void setpointtransform(point * p, double sx, double sy, double a, double tx, double ty){
	p->localtransform.scale.x = sx;
	p->localtransform.scale.y = sy;
	p->localtransform.scale.x = sx;
	p->localtransform.rotate = a;
	p->localtransform.translate.x = tx;
	p->localtransform.translate.y = ty;
}

void setlinetransform(line * l, double sx, double sy, double a, double tx, double ty){
	l->localtransform.scale.x = sx;
	l->localtransform.scale.y = sy;
	l->localtransform.rotate = a;
	l->localtransform.translate.x = tx;
	l->localtransform.translate.y = ty;
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

void scale(transform * t, double dx, double dy){
	t->scale.x *= dx;
	t->scale.y *= dy;
}

void setscale(transform * t, double dx, double dy){
	t->scale.x = dx;
	t->scale.y = dy;
}

void translate(transform * t, double dx, double dy){
	t->translate.x += dx;
	t->translate.y += dy;
}

void settranslate(transform * t, double dx, double dy){
	t->translate.x = dx;
	t->translate.y = dy;
}

void rotate(transform * t, double a){
	t->rotate += a;
}

void setrotate(transform * t, double a){
	t->rotate = a;
}

void pointscale(point * p, double dx, double dy){
	p->localtransform.scale.x *= dx;
	p->localtransform.scale.y *= dy;
}

void setpointscale(point * p, double dx, double dy){
	p->localtransform.scale.x = dx;
	p->localtransform.scale.y = dy;
}

void pointtranslate(point * p, double dx, double dy){
	p->localtransform.translate.x += dx;
	p->localtransform.translate.y += dy;
}

void setpointtranslate(point * p, double dx, double dy){
	p->localtransform.translate.x = dx;
	p->localtransform.translate.y = dy;
}

void pointrotate(point * p, double a){
	p->localtransform.rotate += a;
}

void setpointrotate(point * p, double a){
	p->localtransform.rotate = a;
}

void linescale(line * l, double dx, double dy){
	l->localtransform.scale.x *= dx;
	l->localtransform.scale.y *= dy;
}

void setlinescale(line * l, double dx, double dy){
	l->localtransform.scale.x = dx;
	l->localtransform.scale.y = dy;
}

void linetranslate(line * l, double dx, double dy){
	l->localtransform.translate.x += dx;
	l->localtransform.translate.y += dy;
}

void setlinetranslate(line * l, double dx, double dy){
	l->localtransform.translate.x = dx;
	l->localtransform.translate.y = dy;
}

void linerotate(line * l, double a){
	l->localtransform.rotate += a;
}

void setlinerotate(line * l, double a){
	l->localtransform.rotate = a;
}

void setglobaltransform(void){
	
	settranslate(&globaltransform, (double)halfscreenwidth, (double)halfscreenheight);
	
	if(screenwidth > screenheight){
		setscale(&globaltransform, (double)(halfscreenwidth), (double)(-halfscreenwidth));
	}else{
		setscale(&globaltransform, (double)(halfscreenheight), (double)(-halfscreenheight));
	}
		
	setrotate(&globaltransform, 0.0);
}


