#include "CoordinateFinder.h"

using namespace std; 

vector<cv::Point> CoordinateFinder::getMatches(XImage *game_pixmap, string &pattern_local_address){
	cv::Mat alpha_game_matrix = cv::Mat(game_pixmap->height, game_pixmap->width, CV_8UC4, game_pixmap->data, game_pixmap->bytes_per_line);
	if(alpha_game_matrix.empty())	throw runtime_error("Couldn't start the game image matrix");

	cv::Mat game_matrix;
	cv::cvtColor(alpha_game_matrix, game_matrix, cv::COLOR_BGRA2BGR); // strip the alpha layer
	if(game_matrix.empty())	throw runtime_error("Couldn't extract the alpha layer from the game matrix");

	cv::Mat pattern_matrix = cv::imread(pattern_local_address, cv::IMREAD_COLOR);
	if(pattern_matrix.empty())	throw runtime_error("Couldn't open the pattern image");
	
	// cout<<"Matrizes criadas"<<endl;
	// Here both matrices should have the same format (BGR, 8bit4channels);

	cv::Mat result;	
	cv::matchTemplate(game_matrix, pattern_matrix, result, cv::TM_CCOEFF_NORMED);
	// cout<<"Match feito"<<endl;
	
	double threshold = 0.98; // A perfect match would likely be 1.00
	cv::threshold(result, result, threshold, 1.0, cv::THRESH_BINARY); // Peak the heatmap, anything below the threshold will be set to 0 and anything above to 1.00
	// cout<<"Threshold aplicado"<<endl;


	result.convertTo(result, CV_8U, 255); // convert it to 8bit 
	vector<cv::Point> matches;
	cv::findNonZero(result, matches); // find (x, y) of every point set to 1 on the heatmap.
	// cout<<"Converção feita"<<endl;

	// cv::imwrite("heatmap_debug.png", result);
	// cout<<"Heatmap criado"<<endl;

	// for(auto p: matches){
	// 	cout<<"("<<p.x<<", "<<p.y<<")"<<endl;
	// }
	
	
	return matches;
}