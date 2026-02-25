#include "WindowManager.h"
#include <queue>
#include <vector>
#include <string>

#define GAME_CLASS_NAME "steam_app_3882730" // This was obtained through testing, I'm not sure if it will hold the same through different environments 

using namespace std;

WindowManager::WindowManager(Display* server) : server(server) {}

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
			if(window_attributes.width > current_width){
				current_width = window_attributes.width; // Pick the biggest window, there will be several.
				current_window = w;
			}
		}
	}
	return current_window;
}
