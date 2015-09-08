// Opencv includes
#include <opencv2/core/core.hpp>

// Regular includes
#include <string>

cv::Mat loadImageFile( const std::string &f_name_str, const unsigned f_bits_ui );

cv::Mat loadRawImage( const std::string &f_name_str );

void showImage( const cv::Mat f_img, const std::string &f_name_str = "Image display" );

cv::Mat	makeIt8bit( const cv::Mat f_img );
