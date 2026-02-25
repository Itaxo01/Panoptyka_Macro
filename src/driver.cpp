#include <iostream>
#include <X11/Xlib.h>
#include <unistd.h>
#include "WindowManager.h"

using namespace std;


int main(){
	
	Display * server = XOpenDisplay(NULL); // The display argument tells the hostname IP, server number and screen number; For this application, NULL tells it to use on the default for all arguments.
	// Later, this could be useful to set a virtual screen to run the game, as the macro needs the application to be the first focus (global) in order to work, a virtual screen could solve the background running requisite. 

	WindowManager wm(server);

	sleep(5);
	Window w = wm.getWindow();
	if(w != None){
		wm.plotImage(wm.getImage(&w)); 
	}

	XCloseDisplay(server);
	return 0;
}
