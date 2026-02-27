#ifndef ACTION_MANAGER_H
#define ACTION_MANAGER_H

#include <opencv2/opencv.hpp>

#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include "WindowManager.h"

/**
 * @brief The action motor of the macro. Clicks, keyboard strokes etc.. Uses XTest to perform it.
 * @brief Extends the class WindowManager, having the same private attributes.
 */
class ActionManager : public WindowManager {
public:
	/**
	 * @brief initialize the ActionManager from a WindowManager
	 */
	ActionManager(const WindowManager& window_manager);
	
	/**
	 * @brief performs a click on given coordinates
	 */
	void left_click(int x, int y);

	/**
	 * @brief perform a key press. Please pass the key as the define "XK_{key}" 
	 */
	void key_press(int key);

private:

};

#endif // ACTION_MANAGER_H
