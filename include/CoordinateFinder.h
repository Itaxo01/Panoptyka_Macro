#ifndef COORDINATE_FINDER_H
#define COORDINATE_FINDER_H

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#define PATTERN_LOCAL_ADDRESS "./images/replay_button1.png"

/**
 * @brief This will be responsible to find the coordinates of the needed actions.
 * @brief That includes scanning through the pixmap of the game with openCV and matching certain figures or even maintaining the coordinates of fixed points if needed
 */
class CoordinateFinder {
public:
	CoordinateFinder();
	
	/**
	 * @brief get the coordinates of each matching of the pattern on the game screen
	 * @attention Both the game_matrix and pattern_matrix should be BGR (3 layers)!!! 
	 */
	static std::vector<cv::Point> getMatches(const cv::Mat &game_matrix, const cv::Mat &pattern_matrix);

	std::vector<cv::Point> getMatchesReplay(const cv::Mat &game_matrix);

private:
	const cv::Mat replay_pattern = cv::imread(PATTERN_LOCAL_ADDRESS, cv::IMREAD_COLOR);
};

#endif // COORDINATE_FINDER_H
