#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class WindowManager {
public:
	/**
	 * @brief Initializes the WindowManager with the X11 server display connection
	 */
    WindowManager(Display* server);

	 /**
	  * @brief Retrives the window of the game.
	  * @brief It's criteria is the WM_CLASS which is basically the "name" of the process. A process can have multiple windows, so the biggest of them is returned (Which is the useful one, generally).
	  */
    Window getWindow();

	 /**
	  * @brief just get the current raw pixels for the portion of the screen defined by the window. It's safe to return the pointer because XGetImage allocates the memory on the heap, just need to free it after.
	  */
	 XImage *getImage(Window *window);

	 /**
	  * @brief plot the raw pixels in to a visible format (PPM, the simplest one). Useful for debug
	  */
	 void plotImage(XImage *image);

private:
    Display* server;
};

#endif // WINDOW_MANAGER_H
