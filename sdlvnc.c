/* 
	sdlvnc SDL Based VNC Client - Zonyl

	- Based code from on   SDL_vnc.c - VNC client implementation  LGPL (c) A. Schiffler, aschiffler@appwares.com
        - Uses SDL_vnc lib and SDL_image for parameter fetching

*/

#ifdef WIN32
 #include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include <SDL/SDL.h>

#include "SDL/SDL_vnc.h"
#include "SFont.h"

#define DEFAULT_W	320
#define DEFAULT_H	480

/* Commandline configurable items */

char *vnc_server = NULL;
int   vnc_port = 5900;
char *vnc_method = NULL;
char *vnc_password = NULL;
int   vnc_framerate = 12;

char GetTextKey(SDL_Surface *screen)
{
 char hostname;
 int  inloop=1;
 SDL_Event event; 
 Uint32 key;

  /* Check for events */
  while ( SDL_PollEvent(&event) ) {
   switch (event.type) {

    case SDL_KEYDOWN:
    case SDL_KEYUP:
     /* Map SDL key to VNC key */
     key = event.key.keysym.sym;
     switch (event.key.keysym.sym) {
      case SDLK_BACKSPACE: key=0xff08; break;
      case SDLK_RETURN: 
		key=0xff0d; 
		break;
      case SDLK_LSHIFT: key=0xffe1; break;
      case SDLK_RSHIFT: key=0xffe2; break;
      case SDLK_LCTRL: key=0xffe3; break;
      case SDLK_RCTRL: key=0xffe4; break;
      case SDLK_LMETA: key=0xffe7; break;
      case SDLK_RMETA: key=0xffe8; break;
      case SDLK_LALT: key=0xffe9; break;
      case SDLK_RALT: key=0xffea; break;
      default: 
	key = event.key.keysym.sym;
      }
     /* Handle upper case letters. */
     if (event.key.keysym.mod & KMOD_SHIFT) {
      key=toupper(key);
     }
     /* Add client event */
//     vncClientKeyevent(vnc, (event.type==SDL_KEYDOWN), key);
     break;
    }
  }
	return (char)key & 0x00ff;
}

/* Very hacky! Just want to get something that doesnt requrie command line for now */
char* GetHostname(SDL_Surface *screen)
{
        SFont_Font* Font;
	int	inloop=1;
	char*	hostName;
	char	key;

	fprintf(stderr,"Blah6");
	hostName = calloc(50,sizeof(char));

	 /* Black screen */
	 SDL_FillRect(screen,NULL,0);
 	SDL_UpdateRect(screen,0,0,0,0);

        // Load and Prepare the font - You don't have to use the IMGlib for this
        Font = SFont_InitFont(IMG_Load("24P_Copperplate_Blue.png"));
        if(!Font) {
                fprintf(stderr, "An error occured while loading the font.");
                exit(1);
        }

        // a simple text blit to (0/0)
        SFont_Write (screen, Font, 0,0,"sdlvnc - WebOS");
	SFont_Write (screen, Font, 0,100,"Enter Hostname:");
	fprintf(stderr,"Blah3");
        // Update the screen
        SDL_UpdateRect(screen, 0, 0, 0, 0);

	while (inloop)
	{
		key=GetTextKey(screen);
		fprintf(stderr,"BlahKey:%d\n",key);
		fprintf(stderr,"Key:%d\r",key);
		if (key > 0) // MOdifiers are mostly negative
		{
		switch (key) {
			case 0x0d:
				printf("blah8");
				inloop=0;
				break;
			// Catch anything that isnt text -- Horrid code but I dont have the time to clean up
			case 0:
			case 32:
			case 24:
			case 16: //Nothing
				break;
			default:
				strncat(hostName,&key,1);
				SFont_Write (screen, Font, 0,200,hostName);
			        SDL_UpdateRect(screen, 0, 0, 0, 0);
		}
		}
	        SDL_Delay(1000);

	}

        // Don't forget to free our font
        SFont_FreeFont(Font);
	return hostName;
}

/* Drawing loop */
void Draw(SDL_Surface *screen, tSDL_vnc *vnc)
{
 SDL_Event event; 
 SDL_Rect updateRect;
 int inloop;
 Uint8 mousebuttons, buttonmask;
 int mousex, mousey;
 Uint32 key;
   
 /* Black screen */
 SDL_FillRect(screen,NULL,0);
 SDL_UpdateRect(screen,0,0,0,0);
  
 inloop=1;
 while (inloop) {
    
  /* Check for events */
  while ( SDL_PollEvent(&event) ) {
   switch (event.type) {

    case SDL_KEYDOWN:
    case SDL_KEYUP:
     /* Map SDL key to VNC key */
     key = event.key.keysym.sym;
     switch (event.key.keysym.sym) {
      case SDLK_BACKSPACE: key=0xff08; break;
      case SDLK_TAB: key=0xff09; break;
      case SDLK_RETURN: key=0xff0d; break;
      case SDLK_ESCAPE: key=0xff1b; break;
      case SDLK_INSERT: key=0xff63; break;
      case SDLK_DELETE: key=0xffff; break;
      case SDLK_HOME: key=0xff50; break;
      case SDLK_END: key=0xff57; break;
      case SDLK_PAGEUP: key=0xff55; break;
      case SDLK_PAGEDOWN: key=0xff56; break;
      case SDLK_LEFT: key=0xff51; break;
      case SDLK_UP: key=0xff52; break;
      case SDLK_RIGHT: key=0xff53; break;
      case SDLK_DOWN: key=0xff54; break;
      case SDLK_F1: key=0xffbe; break;
      case SDLK_F2: key=0xffbf; break;
      case SDLK_F3: key=0xffc0; break;
      case SDLK_F4: key=0xffc1; break;
      case SDLK_F5: key=0xffc2; break;
      case SDLK_F6: key=0xffc3; break;
      case SDLK_F7: key=0xffc4; break;
      case SDLK_F8: key=0xffc5; break;
      case SDLK_F9: key=0xffc6; break;
      case SDLK_F10: key=0xffc7; break;
      case SDLK_F11: key=0xffc8; break;
      case SDLK_F12: key=0xffc9; break;
      case SDLK_LSHIFT: key=0xffe1; break;
      case SDLK_RSHIFT: key=0xffe2; break;
      case SDLK_LCTRL: key=0xffe3; break;
      case SDLK_RCTRL: key=0xffe4; break;
      case SDLK_LMETA: key=0xffe7; break;
      case SDLK_RMETA: key=0xffe8; break;
      case SDLK_LALT: key=0xffe9; break;
      case SDLK_RALT: key=0xffea; break;
      default: key = event.key.keysym.sym;
     }
     /* Handle upper case letters. */
     if (event.key.keysym.mod & KMOD_SHIFT) {
      key=toupper(key);
     }
     /* Add client event */
     vncClientKeyevent(vnc, (event.type==SDL_KEYDOWN), key);
     break;

    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEMOTION:
     /* Get current mouse state */
     mousebuttons=SDL_GetMouseState(&mousex,&mousey);
     /* Map SDL buttonmask to VNC buttonmask */
     buttonmask=0;
     if (mousebuttons & SDL_BUTTON(SDL_BUTTON_LEFT)) buttonmask       |= 1;
     if (mousebuttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) buttonmask     |= 2;
     if (mousebuttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) buttonmask      |= 4;
     if (mousebuttons & SDL_BUTTON(SDL_BUTTON_WHEELUP)) buttonmask    |= 8;
     if (mousebuttons & SDL_BUTTON(SDL_BUTTON_WHEELDOWN)) buttonmask  |= 16;
     /* Add client event */
     vncClientPointerevent(vnc, buttonmask, mousex, mousey);
     break;

    case SDL_QUIT:
     inloop=0;
     break;
    }
   }

   /* Blit VNC screen */
   if (vncBlitFramebuffer(vnc, screen, &updateRect) ) {
    /* Display by updating changed parts of the display */
    SDL_UpdateRect(screen,updateRect.x,updateRect.y,updateRect.w,updateRect.h);
   }
    
   /* Delay to limit rate */                   
   SDL_Delay(1000/vnc_framerate);
   
 }
}

void PrintUsage()
{
 fprintf (stderr,"Usage: sdlvnc [SDL parameters] -server servername [VNC parameters]\n");
 fprintf (stderr," SDL parameters\n");
 fprintf (stderr,"  -width [i]		Set screen width (default: %i)\n",DEFAULT_W);
 fprintf (stderr,"  -height [i]		Set_screen height (default: %i)\n",DEFAULT_H);
 fprintf (stderr,"  -bpp [i]		Set [i] bits per pixel\n");
 fprintf (stderr,"  -warp			Use hardware palette\n");
 fprintf (stderr,"  -hw			Use hardware surface\n");
 fprintf (stderr,"  -fullscreen		Go into fullscreen mode\n");
 fprintf (stderr," VNC\n");
 fprintf (stderr,"  -server [s]		VNC server to connect to\n");
 fprintf (stderr,"  -port [i]		VNC port to connect to\n");
 fprintf (stderr,"  -method [s]		Method to use, first to last.\n");
 fprintf (stderr,"            		Implemented: hextile,rre,copyrect,raw,cursor\n");
 fprintf (stderr,"            		Missing/Problems: corre,zrle,tight,desktop\n");
 fprintf (stderr,"  -password [s]		VNC password to use\n");
 fprintf (stderr,"  -framerate [i]	Target framerate for RFB requests and redraws\n");
}

#ifdef WIN32
 extern char ** __argv;
 extern int __argc;
 int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
#else // non WIN32
 int main ( int argc, char *argv[] )
#endif
{
	SDL_Surface *screen;
	int w, h;
	int desired_bpp;
	Uint32 video_flags;
#ifdef WIN32
	int argc;
	char **argv;

	argv = __argv;
	argc = __argc;
#endif
	tSDL_vnc vnc;
	int result;

	/* Title */
	fprintf (stderr,"sdlvnc - Based on SDL_vnc Sample VNC Client - LGPL, A. Schiffler, aschiffler@appwares.com\n\n");

        if (argc==1) {
//         PrintUsage();
//         exit(0);
        }
        
	/* Set default options and check command-line */
	w = DEFAULT_W;
	h = DEFAULT_H;
	desired_bpp = 0;
	video_flags = 0;
	while ( argc > 1 ) {

		/* SDL specific arguments */
		
		if ( strcmp(argv[1], "-width") == 0 ) {
			if ( argv[2] && ((w = atoi(argv[2])) > 0) ) {
				argv += 2;
				argc -= 2;
			} else {
				fprintf(stderr,
				"The -width option requires an argument\n");
				exit(1);
			}
		} else
		if ( strcmp(argv[1], "-height") == 0 ) {
			if ( argv[2] && ((h = atoi(argv[2])) > 0) ) {
				argv += 2;
				argc -= 2;
			} else {
				fprintf(stderr,"The -height option requires an argument\n");
				exit(1);
			}
		} else
		if ( strcmp(argv[1], "-bpp") == 0 ) {
			if ( argv[2] ) {
				desired_bpp = atoi(argv[2]);
				argv += 2;
				argc -= 2;
			} else {
				fprintf(stderr,"The -bpp option requires an argument\n");
				exit(1);
			}
		} else
		if ( strcmp(argv[1], "-warp") == 0 ) {
			video_flags |= SDL_HWPALETTE;
			argv += 1;
			argc -= 1;
		} else
		if ( strcmp(argv[1], "-hw") == 0 ) {
			video_flags |= SDL_HWSURFACE;
			argv += 1;
			argc -= 1;
		} else
		if ( strcmp(argv[1], "-fullscreen") == 0 ) {
			video_flags |= SDL_FULLSCREEN;
			argv += 1;
			argc -= 1;
		} else

		
		/* VNC specific arguments */

		if ( strcmp(argv[1], "-server") == 0 ) {
			if (argv[2]) {
				vnc_server = strdup(argv[2]);
				argv += 2;
				argc -= 2;
			} else {
				fprintf(stderr,"The -server option requires an argument\n");
				exit(1);
			}
		} else
		if ( strcmp(argv[1], "-port") == 0 ) {
			if (argv[2]) {
				vnc_port = atoi(argv[2]);
				argv += 2;
				argc -= 2;
			} else {
				fprintf(stderr,"The -port option requires an argument\n");
				exit(1);
			}
		} else
		if ( strcmp(argv[1], "-method") == 0 ) {
			if (argv[2]) {
				vnc_method = strdup(argv[2]);
				argv += 2;
				argc -= 2;
			} else {
				fprintf(stderr,"The -method option requires an argument\n");
				exit(1);
			}
		} else
		if ( strcmp(argv[1], "-password") == 0 ) {
			if (argv[2]) {
				vnc_password = strdup(argv[2]);
				argv += 2;
				argc -= 2;
			} else {
				fprintf(stderr,"The -password option requires an argument\n");
				exit(1);
			}
		} else
		if ( strcmp(argv[1], "-framerate") == 0 ) {
			if (argv[2]) {
				vnc_framerate = atoi(argv[2]);
				argv += 2;
				argc -= 2;
			} else {
				fprintf(stderr,"The -framerate option requires an argument\n");
				exit(1);
			}
		} else {
			PrintUsage();
			exit(0);
			break;
		}
	}

	/* Force double buffering */
//	video_flags |= SDL_DOUBLEBUF;

	/* Check/Adjust VNC parameters */
	if (vnc_server==NULL) {
	 fprintf (stderr,"Need VNC server.\n");
//	 PrintUsage();
//         exit(1);
        }
        if (vnc_method==NULL) {	
         vnc_method=strdup("hextile,rre,copyrect,raw");
        }
        if (vnc_password==NULL) {	
         vnc_method=strdup("");
        }
        if ((vnc_framerate<1) || (vnc_framerate>100)) {
	 fprintf (stderr,"Bad framerate (%i). Use a value from 1 to 100.\n",vnc_framerate);
	 PrintUsage();
         exit(1);
	}        
        
	/* Initialize SDL */
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);			/* Clean up on exit */

	/* Initialize the display */
	screen = SDL_SetVideoMode(w, h, desired_bpp, video_flags);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n",
					w, h, desired_bpp, SDL_GetError());
		exit(1);
	}

	/* Check for double buffering */
	if ( screen->flags & SDL_DOUBLEBUF ) {
		printf("Double-buffering enabled - good!\n");
	}

	/* Set the window manager title bar */
	SDL_WM_SetCaption("sdlvnc", "sdlvnc");

	printf("blah10");
	/* Get Parameters */
	if (vnc_server == NULL)
	{
		vnc_server=GetHostname(screen);
	}

	/* Open vnc connection */
	result =vncConnect(&vnc,vnc_server,vnc_port,"hextile,rre,copyrect,raw,cursor","lyoner",10);
	fprintf(stderr,"blah19");	
	if (result != 0)
	{
		/* Do all the drawing work */
		Draw (screen, &vnc);
	}		
		/* Close connection */
		vncDisconnect(&vnc);

	free(vnc_server);	
	return(0);
}
