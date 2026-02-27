#include "CoordinateFinder.h"

using namespace std; 

vector<cv::Point> CoordinateFinder::getMatches(const cv::Mat &game_matrix, const cv::Mat &pattern_matrix){
	if(game_matrix.empty())	throw runtime_error("Couldn't extract the alpha layer from the game matrix");
	if(pattern_matrix.empty())	throw runtime_error("Couldn't open the pattern image");
	
	// Here both matrices should have the same format (BGR, 8bit3channels);

	cv::Mat result;	
	cv::matchTemplate(game_matrix, pattern_matrix, result, cv::TM_CCOEFF_NORMED);
	
	double threshold = 0.98; // A perfect match would likely be 1.00
	cv::threshold(result, result, threshold, 1.0, cv::THRESH_BINARY); // Peak the heatmap, anything below the threshold will be set to 0 and anything above to 1.00


	result.convertTo(result, CV_8U, 255); // convert it to 8bit 
	vector<cv::Point> matches;
	cv::findNonZero(result, matches); // find (x, y) of every point set to 1 on the heatmap.
	
	return matches;
}

CoordinateFinder::CoordinateFinder(){}

std::vector<cv::Point> CoordinateFinder::getMatchesReplay(const cv::Mat &game_matrix){
	return getMatches(game_matrix, this->replay_pattern);
}
