//#include "SDL.h"
//#include "cnvt.h"
//#include "expr.h"
#include "evolve.h"
//#include <stdio.h>
//#include <cmath>

SDL_Surface *screen;
int exprcnt;
FILE* fio;
char eestr[65536];
crab* ppl[POPULATION];
bool sel[POPULATION];

void printcrab(crab* c){
	if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            exit(0);
        }
    }

	memcpy(screen->pixels,c->fullbuf,3*EVO_SQLEN*EVO_SQLEN*sizeof(Uint8));

	if ( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }
	SDL_UpdateRect(screen,0,0,0,0);
}

void saveform(){
	exprcnt++;
	fio = fopen("gallery/cnt.txt", "w");
	fprintf(fio,"%d\n",exprcnt);
	fclose(fio);
	char filen[4096];
	sprintf(filen,"gallery/form-%d.bmp",exprcnt);
	SDL_SaveBMP(screen,filen);
	fio = fopen("gallery/expr.txt", "a");
	fprintf(fio,"%d\n%s\n",exprcnt,eestr);
	fclose(fio);
}

void cast_thumb(int x, int y, bool selected){
	if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            exit(0);
        }
    }

	//memcpy(screen->pixels,c->fullbuf,3*EVO_SQLEN*EVO_SQLEN*sizeof(Uint8));

	Uint8* dptr=((Uint8*)screen->pixels)+y*3*(EVO_SQLEN>>2)*EVO_SQLEN+x*3*(EVO_SQLEN>>2);
	int i, gray;
	if(selected) gray=255; else gray=175;
	for(i=0; i<5; i++){
		memset(dptr,gray,3*(EVO_SQLEN>>2));
		dptr+=3*EVO_SQLEN;
	}
	for(;i<(EVO_SQLEN>>2)-5;i++){
		memset(dptr,gray,15);
		memset(dptr+3*(EVO_SQLEN>>2)-15,gray,15);
		dptr+=3*EVO_SQLEN;
	}
	for(;i<(EVO_SQLEN>>2);i++){
		memset(dptr,gray,3*(EVO_SQLEN>>2));
		dptr+=3*EVO_SQLEN;
	}

	if ( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }
	SDL_UpdateRect(screen,x*(EVO_SQLEN>>2),y*(EVO_SQLEN>>2),(EVO_SQLEN>>2),(EVO_SQLEN>>2));
}

void disp_crab_thumb(int x, int y){
	if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            exit(0);
        }
    }

	//memcpy(screen->pixels,c->fullbuf,3*EVO_SQLEN*EVO_SQLEN*sizeof(Uint8));

	crab* cr = ppl[x+(y<<2)];

	Uint8* dptr=((Uint8*)screen->pixels)+y*3*(EVO_SQLEN>>2)*EVO_SQLEN+x*3*(EVO_SQLEN>>2);
	Uint8* sptr=cr->smallbuf;
	for(int i=0;i<(EVO_SQLEN>>2);i++){
		memcpy(dptr,sptr,3*(EVO_SQLEN>>2)*sizeof(Uint8));
		dptr+=3*EVO_SQLEN;
		sptr+=3*(EVO_SQLEN>>2);
	}

	if ( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }
	SDL_UpdateRect(screen,x*(EVO_SQLEN>>2),y*(EVO_SQLEN>>2),(EVO_SQLEN>>2),(EVO_SQLEN>>2));

	if(sel[x+(y<<2)]) cast_thumb(x,y,true);
}

void crab_ctrl(int i, int j){
	if(!(ppl[i+(j<<2)]->expanded)) expand_crab(ppl[i+(j<<2)]);
	printcrab(ppl[i+(j<<2)]);
	SDL_Event e;
	while(true){
		SDL_WaitEvent(&e);
		while((e.type!=SDL_QUIT)&&(e.type!=SDL_KEYUP)){
			SDL_WaitEvent(&e);
		}
		if(e.type==SDL_QUIT){
			exit(0);
		}else{
			if((e.key.keysym.sym==SDLK_q)||(e.key.keysym.sym==SDLK_RETURN)){
				break;
			}else if(e.key.keysym.sym==SDLK_s){
				int eel=expr2str(ppl[i+(j<<2)]->e,eestr);
				eestr[eel]=0;
				saveform();
			}else{
				continue;
			}
		}
	}
	return;
}

void thumb_ctrl(){
	int selcnt=0, x=0, y=0;
	for(int i=0;i<POPULATION;i++) sel[i]=false;
	SDL_Event e;
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			disp_crab_thumb(i,j);
		}
	}
	cast_thumb(0,0,false);
	while(true){
		SDL_WaitEvent(&e);
		while((e.type!=SDL_QUIT)&&(e.type!=SDL_KEYUP)){
			SDL_WaitEvent(&e);
		}
		if(e.type==SDL_QUIT){
			exit(0);
		}else{
			if(e.key.keysym.sym==SDLK_UP){
				disp_crab_thumb(x,y);
				y+=3;
				y%=4;
				cast_thumb(x,y,false);
			}else if(e.key.keysym.sym==SDLK_DOWN){
				disp_crab_thumb(x,y);
				y+=1;
				y%=4;
				cast_thumb(x,y,false);
			}else if(e.key.keysym.sym==SDLK_LEFT){
				disp_crab_thumb(x,y);
				x+=3;
				x%=4;
				cast_thumb(x,y,false);
			}else if(e.key.keysym.sym==SDLK_RIGHT){
				disp_crab_thumb(x,y);
				x+=1;
				x%=4;
				cast_thumb(x,y,false);
			}else if(e.key.keysym.sym==SDLK_SPACE){
				sel[x+(y<<2)]=!sel[x+(y<<2)];
				if(sel[x+(y<<2)]) selcnt++; else selcnt--;
			}else if(e.key.keysym.sym==SDLK_ESCAPE){
				exit(0);
			}else if(e.key.keysym.sym==SDLK_RETURN){
				crab_ctrl(x,y);
				for(int i=0;i<4;i++){
					for(int j=0;j<4;j++){
						disp_crab_thumb(i,j);
					}
				}
				cast_thumb(x,y,false);
			}else if(e.key.keysym.sym==SDLK_e){
				if(selcnt==WINNERCNT) break;
			}
		}
	}
	return;
}

int main(int argc, char **argv){
	fio = fopen("gallery/cnt.txt", "r");
	fscanf(fio,"%d",&exprcnt);
	fclose(fio);

	if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr,
                "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
	atexit(SDL_Quit);
	screen = SDL_SetVideoMode(EVO_SQLEN, EVO_SQLEN, 24, SDL_SWSURFACE);
    if ( screen == NULL ) {
        fprintf(stderr, "Couldn't set up crab window: %s\n",
                        SDL_GetError());
        exit(1);
    }

	int seed;
	__asm RDTSC;
	__asm xor eax, edx;
	__asm mov seed, eax;
	srand((unsigned int)seed);

	init_pool(ppl);
	for(int i=0;i<POPULATION;i++) sel[i]=false;

	while(true){
		thumb_ctrl();
		natural_selection(ppl,sel);
	}

	exit(0);
}