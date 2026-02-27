#include "ActionManager.h"
#include <chrono>
#include <thread>

// Initializes the window manager to use it
ActionManager::ActionManager(const WindowManager &window_manager): WindowManager(window_manager){
}


void ActionManager::left_click(int x, int y){
	wait_for_focus(); // just to be sure
	XTestFakeMotionEvent(server, -1, x, y, CurrentTime); // move the mouse to the position
	
	XTestFakeButtonEvent(server, 1, true, CurrentTime); // hold left click
	XFlush(server); // flush the queue buffer so that the key is pressed now

	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	XTestFakeButtonEvent(server, 1, false, CurrentTime); // release left click
	XFlush(server); 

	std::this_thread::sleep_for(std::chrono::milliseconds(30)); // wait again to make sure 

	
}

void ActionManager::key_press(int key){
	wait_for_focus(); // just to be sure
	KeyCode k = XKeysymToKeycode(server, key);
	XTestFakeKeyEvent(server, k, true, 0); // Press the key down
	XFlush(server); // flush the queue buffer so that the key is pressed now

	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	XTestFakeKeyEvent(server, k, false, 0); // Relase the key after 30ms 
	XFlush(server); // flush the queue buffer

	std::this_thread::sleep_for(std::chrono::milliseconds(30)); // wait again to make sure 
}