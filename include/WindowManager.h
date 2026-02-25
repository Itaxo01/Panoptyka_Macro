#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <X11/Xlib.h>

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

private:
    Display* server;
};

#endif // WINDOW_MANAGER_H
