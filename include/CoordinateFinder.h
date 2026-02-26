#ifndef COORDINATE_FINDER_H
#define COORDINATE_FINDER_H

#include <vector>
#include <X11/Xlib.h>
#undef Status // This will not be used here and causes conflict with the opencv2.

#include <string>
#include <opencv2/opencv.hpp>

/**
 * @brief This will be responsible to find the coordinates of the needed actions.
 * @brief That includes scanning through the pixmap of the game with openCV and matching certain figures or even maintaining the coordinates of fixed points if needed
 */
class CoordinateFinder {
public:
	CoordinateFinder();
	
	/**
	 * @brief get the coordinates of each matching of the pattern on the game screen
	 * @attention this function will not free the XImage
	 */
	static std::vector<cv::Point> getMatches(XImage *game_pixmap, std::string &pattern_local_address);
private:
	
};

#endif // COORDINATE_FINDER_H
