#include <iostream>
#include <unistd.h>
#include <opencv2/opencv.hpp>


#include "WindowManager.h"
#include "ActionManager.h"
#include "CoordinateFinder.h"
#include <X11/keysym.h>
#include <chrono>
#include <thread>

using namespace std;

#define PATTERN_LOCAL_ADDRESS "./images/replay_button1.png"
#define REPLAY_BUTTON_SIZE 36 // This can change, will be used in order to center the button press


void center_button(int &x, int &y){
	x += REPLAY_BUTTON_SIZE/2;
	y += REPLAY_BUTTON_SIZE/2;
}

void perform_actions(WindowManager &wm, ActionManager &am, CoordinateFinder &cf){
	XImage *game_image = wm.getImage();	

	cv::Mat alpha_game_matrix = cv::Mat(game_image->height, game_image->width, CV_8UC4, game_image->data, game_image->bytes_per_line);
	
	cv::Mat game_matrix;
	cv::cvtColor(alpha_game_matrix, game_matrix, cv::COLOR_BGRA2BGR); // strip the alpha layer
	
	auto matches = cf.getMatchesReplay(game_matrix);
	XDestroyImage(game_image);
	
	
	
	am.key_press(XK_F4); // press f4
	
	for(auto e: matches){
		auto [x, y] = wm.absolute_coordinates(e.x, e.y);
		center_button(x, y);
		am.left_click(x, y); 
	}

	am.key_press(XK_v); // press v
}

const char* get_screen(){
	const char *env_screen = std::getenv("DISPLAY");
	return env_screen ? env_screen : NULL;
}

int main(){

	const char* screen = get_screen();
	cout<<"Screen detected: "<<screen<<endl;

	Display * server = XOpenDisplay(screen); // The display argument tells the hostname IP, server number and screen number; For this application, NULL tells it to use on the default for all arguments.
	// Later, this could be useful to set a virtual screen to run the game, as the macro needs the application to be the first focus (global) in order to work, a virtual screen could solve the background running requisite. 
	
	WindowManager wm(server, screen); // Window manager will be used to capture the game screen
	ActionManager am(wm);
	CoordinateFinder cf;
	
	KeyCode pause_key = XKeysymToKeycode(server, XK_semicolon); // uses ';' as the pause button
	XGrabKey(server, pause_key, AnyModifier, DefaultRootWindow(server), True, GrabModeAsync, GrabModeAsync);
	
	bool is_paused = false;
	
	cout<<"Macro start paused, press ';' with the game on focus to resume and the same key to pause again"<<endl;
	while(true){
		XEvent event;
		while(XPending(server) > 0){
			XNextEvent(server, &event);

			if(event.type == KeyPress && event.xkey.keycode == pause_key){
				is_paused = !is_paused; 

				if(is_paused){
					cout<<"Macro Paused: Press ';' to resume"<<endl;
				} else {
					cout<<"Macro unpaused"<<endl;
				}
			}
		}

		if(is_paused){
			this_thread::sleep_for(chrono::milliseconds(500)); // sleep for now
			continue;
		}

		perform_actions(wm, am, cf);
		this_thread::sleep_for(chrono::milliseconds(500)); // sleep for now
	}
		
	XCloseDisplay(server);
	return 0;
}
