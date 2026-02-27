#include "WindowManager.h"
#include <queue>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <thread>

using namespace std;

Window WindowManager::getWindow() {
	queue<Window> q; 
	q.push(DefaultRootWindow(server)); // Push the root window 
	
	auto checkWindow = [](Display *server, Window *w) { // Check if the window is the correct one
		Atom property = XInternAtom(server, "WM_CLASS", false);
		long offset = 0, long_length = ~0L;
		bool _delete = false;
		Atom req_type = AnyPropertyType;
		
		// Return parameters 
		Atom actual_type_return; int actual_format_return; unsigned long nitems_return, bytes_after_return;
		unsigned char *prop_return = nullptr;

		int status = XGetWindowProperty(server, *w, property, offset, long_length, _delete, req_type, &actual_type_return, &actual_format_return, &nitems_return, &bytes_after_return, &prop_return);

		if(status == Success && prop_return != nullptr){
			char * str = reinterpret_cast<char*>(prop_return); // Just cast it to char * safely
			string s(str);
			if(s == GAME_CLASS_NAME){
				XFree(prop_return);
				return true;
			} 
		}
		XFree(prop_return);
		return false;
	};

	vector<Window> allow_list;
	while(!q.empty()){
		Window u = q.front(); q.pop();
		if(checkWindow(server, &u)){
			allow_list.push_back(u);
		}

		Window root_return, parent_return, *children_return = nullptr;
		unsigned int nchildren_return;
		// These pointers exists because of C pass by reference change. As it can only return one argument, it changes the income arguments when they are passed as pointers.

		Status s = XQueryTree(server, u, &root_return, &parent_return, &children_return, &nchildren_return);
		if(s){
			for(uint i = 0; i < nchildren_return; i++){
				Window v = children_return[i];
				q.push(v);
			}
		}
		XFree(children_return);
	}

	int current_width = 0;
	Window current_window = None;
	for(Window w: allow_list){
		XWindowAttributes window_attributes;
		Status s = XGetWindowAttributes(server, w, &window_attributes);
		if(s){
			cout<<window_attributes.x<<" "<<window_attributes.y<<endl;
			if(window_attributes.width > current_width){
				current_width = window_attributes.width; // Pick the biggest window, there will be several.
				current_window = w;
			}
		}
	}
	return current_window;
}


void WindowManager::wait_for_focus() {
	Window focus_window;
	int revert_to;
	while(True){ // Just to make sure that the game is focused
		XGetInputFocus(server, &focus_window, &revert_to);
		if(focus_window != game_window){
			std::cout<<"Waiting for the game to gain focus"<<std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		} else break;
	}
}

WindowManager::WindowManager(Display* server, bool display = false) : server(server) {
	if(display){
		cout<<"Proceeding with default window"<<endl;
		this->game_window = DefaultRootWindow(this->server);
	} else {
		this->game_window = getWindow();
		if(this->game_window == None){
			throw runtime_error("Game window isn't open or couldn't be found");
		}
	}

	Window _; // useless

	XTranslateCoordinates(
		this->server,
		this->game_window, 
		DefaultRootWindow(this->server), 
		0, 0, 
		&offset_x, // Get the coordinate offset 
		&offset_y,
		&_
	);
}


XImage* WindowManager::getImage(){
	XWindowAttributes window_attributes;
	Status s = XGetWindowAttributes(server, game_window, &window_attributes); // It's best to ask this every time, as the window shape could change.
	if(!s){
		throw runtime_error("Failed to capture window attributes: getImage()");
	}

	wait_for_focus();
	
	XImage *image = XGetImage(server, game_window, 0, 0, window_attributes.width, window_attributes.height, AllPlanes, ZPixmap);
	
	if(!image) {
		throw runtime_error("Failed to capture window image.2");
	}
	
	// This is raw pixels on a pixmap
	return image;
}


void WindowManager::plotImage(){
	XImage *image = getImage();
	ofstream file("image.ppm", ios::binary);
	
	// header declaration
	file << "P6\n" << image->width << " " << image->height << "\n255\n";

	// image->data is a 1D array, where each pixel uses 4 8bit values (BGRA).
	// They are disposed in chars (8bit), so each color gets one.
	for(unsigned int i = 0; i<image->width*image->height*4u; i+=4){
		char blue = image->data[i], green = image->data[i+1], red = image->data[i+2]; // ignores alpha for ppm 
		file.put(red);
		file.put(green);
		file.put(blue); // switch to RGB
	}
	XDestroyImage(image);
}


std::pair<int, int> WindowManager::absolute_coordinates(int x, int y){
	return pair(x+offset_x, y+offset_y);
}
