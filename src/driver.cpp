#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include "WindowManager.h"
#include "CoordinateFinder.h"

using namespace std;

string replay_button = "./images/replay_button.png";

int main(){
	
	Display * server = XOpenDisplay(NULL); // The display argument tells the hostname IP, server number and screen number; For this application, NULL tells it to use on the default for all arguments.
	// Later, this could be useful to set a virtual screen to run the game, as the macro needs the application to be the first focus (global) in order to work, a virtual screen could solve the background running requisite. 

	WindowManager wm(server); // Window manager will be used to capture the game screen
	

	XImage *game_image = wm.getImage();

	CoordinateFinder::getMatches(game_image, replay_button);


	XDestroyImage(game_image);
	XCloseDisplay(server);
	return 0;
}
