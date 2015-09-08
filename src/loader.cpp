// Corresponding header
#include "../h/loader.h"

// Opencv headers
#include <opencv2/highgui/highgui.hpp>

// Project includes
#include "../h/rawImageIO.h"

// Regular includes
#include <iostream>

using std::cout;
using std::endl;

cv::Mat loadImageFile( const std::string &f_name_str, const unsigned f_bits_ui )
{
  // Load the image, as is.
  cv::Mat input = cv::imread( f_name_str, -1 );
  
  if( input.empty() )
  {
    cout << "ERROR loadImageFile: Could not load " << f_name_str << endl;
    return input;
  }

  double scaleFactor_d;
  switch( f_bits_ui )
  {
    case 8:
      scaleFactor_d = 1.0;
    break;	
    case 10:
      scaleFactor_d = 255.0 / 1023.0;
    break;
    case 12:
      scaleFactor_d = 255.0 / 4095.0;
    break;
    case 16:
      scaleFactor_d = 255.0 / 65535.0;
    break;
    default:
      scaleFactor_d = -1.0;
    break;
  }

  cv::Mat output;

  // Error checking
  if( scaleFactor_d == -1.0 )
  {
    cout << "ERROR loadImageFile: Image depth of " <<  f_bits_ui 
	 << " bits not supported yet! Try with 8, 10, 12 or 16 bit images\n";
    //system("pause");
		
    output.release();
    return output;
  }
	
  // Convert the image into a 32 float one, reescaled into the 
  // [0,255] interval
  if( input.channels() == 1 )
  {
    input.convertTo( output, CV_32FC1, scaleFactor_d );
  }
  else if( input.channels() == 3 )
  {
    input.convertTo( output, CV_32FC3, scaleFactor_d );
  } 
	
  return output;
}

cv::Mat loadRawImage( const std::string &f_fileInName_str )
{
  CImageSize tempSize;
  CRawImageIO rawLoader;
  cv::Mat tempImg_p;

  // Get image data
  char* outputData_p = rawLoader.loadRawDataImage( f_fileInName_str, tempSize );

  // If no data returned, clean up and return the empty image
  if( !outputData_p )
  {
    delete[] outputData_p;
    return tempImg_p;
  }

  // Set the image type
  int imgType_i = 0;
  if( tempSize.m_pixelDepth_ui == IO_DATATYPE_32F )
  {
    imgType_i = CV_32FC1;
  }
  else if( tempSize.m_pixelDepth_ui == IO_DATATYPE_64F )
  {
    imgType_i = CV_64FC1;
  }
		
  // Create image
  tempImg_p.create( tempSize.m_height_ui, tempSize.m_width_ui, imgType_i );

  // Copy image data to image
  for( unsigned y = 0 ; y < tempSize.getNumberBytes(); ++y )
  {
    tempImg_p.data[y] = outputData_p[y];
  }

  delete [] outputData_p;
  return tempImg_p;
}

void showImage( const cv::Mat f_img, const std::string &f_name_str )
{
  // Set the name of the window
  std::string title_s = f_name_str;

  // Create the window for displing the image		
  // The second argument is set to zero to allow resizing of the window
  cv::namedWindow( f_name_str, CV_WINDOW_AUTOSIZE );
	
  // Make the input image an 8bit one if necessary
  cv::Mat img8bit = makeIt8bit( f_img );
		
  // Show image
  cv::imshow( f_name_str, img8bit );
  //Wait for a key
  cv::waitKey( 0 );
  //Clean data
  cv::destroyWindow( f_name_str );
}

cv::Mat	makeIt8bit( const cv::Mat f_img )
{
  if( f_img.depth() > CV_8S )
  {
    // Create a target image
    cv::Mat img8bit;
    // Convert the input image into an 8 bit one
    f_img.convertTo( img8bit, CV_8U ); 
		
    return img8bit;		
  } 
  else
  {
    return f_img;
  }
}
