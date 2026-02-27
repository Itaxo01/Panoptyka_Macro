#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <utility>

#define GAME_CLASS_NAME "steam_app_3882730" // This was obtained through testing, I'm not sure if it will hold the same through different environments 

/**
 * @brief This one is responsible for one job: getting a screenshot of the game when needed
 */
class WindowManager {
public:
	/**
	 * @brief Initializes the WindowManager with the X11 server display connection
	 * @brief Also initialize with the game window.
	 */
    WindowManager(Display* server, bool display);
	 
	 /**
	  * @brief just get the current raw pixels for the portion of the screen defined by the window. It's safe to return the pointer because XGetImage allocates the memory on the heap, just need to free it after.
	  * @attention Don't forget to free it with XDestroyImage() 
	  */
	 XImage *getImage();
	 
	 /**
	  * @brief wait while the game isn't focused
	  */
	 void wait_for_focus();

	 /**
	  * @brief turn relative coordinates to absolute coordinates (considering the window padding)
	  */
	 std::pair<int, int> absolute_coordinates(int x, int y);
	 
	 /**
	  * @brief plot the raw pixels in to a visible format (PPM, the simplest one). Useful for debug
	  */
	 void plotImage();
	 
protected:
	 
Display* server;
	 Window game_window; // Doesn't need to be a pointer, is just an ID
	 int offset_x, offset_y; // offset of the window 
	 
	 /**
	  * @brief Retrives the window of the game.
	  * @brief It's criteria is the WM_CLASS which is basically the "name" of the process. A process can have multiple windows, so the biggest of them is returned (Which is the useful one, generally).
	  */
	 Window getWindow();
};

#endif // WINDOW_MANAGER_H
