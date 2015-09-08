/* ******************************** FILE *********************************** */
/** \file		mask.cpp
 *
 *  \brief		Definition of the CThirdEyeMask class. 
 *
 *  \author		Sandino Morales
 *  \date		25.06.2012
 *  \note		.enpeda.. group. The University of Auckland
 *
 *************************************************************************** */
// Corresponding header
#include "../h/thirdeyeMask.h"

// OpenCV includes
#include <opencv2/imgproc/imgproc.hpp>

// Regular includes
#include <iostream>

/* *************************** METHOD ************************************** */
/* Standard constructor.
 *
 * \brief          Standard constructor.
 * \author         Sandino Morales
 * \date           25.06.2012
 *
 * \return         -
 *************************************************************************** */
CThirdEyeMask::CThirdEyeMask()
  : m_thresholdDistance_f( 10.f ),
    m_thresholdGradient_f(  5.f ), 
    m_kernelSize_ui(        3   ),
    
    m_gradientImage(            ),
    m_distanceImage(            ),
    m_trueMask(                 )
{
  /// Create the kernels to compute the gradient images;
  setKernels();
}

/* *************************** METHOD ************************************** */
/* Overloaded constructor.
 *
 * \brief	The paramerters used to generate the mask are passed as 
 *		arguments.
 *
 * \author	andino Morales
 * \date	25.06.2012
 *
 * \param[in]	const float f_thresholdGradient_f: Defines the lenght of the
 *		gradient
 * \param[in]	const float f_thresholdDistance_f: Defines the threslhold for
 *		the distance between edge pixels and regular ones
 *
 * \return      -
 *************************************************************************** */
CThirdEyeMask::CThirdEyeMask( const float f_thresholdGradient_f, 
			      const float f_thresholdDistance_f )
  : m_thresholdDistance_f( f_thresholdDistance_f ),
    m_thresholdGradient_f( f_thresholdGradient_f  ),
    m_kernelSize_ui( 3 ),
 
    m_gradientImage(    ),
    m_trueMask(         )
{
  /// Create the kernels to compute the gradient images;
  setKernels();
}

/* *************************** METHOD ************************************** */
/* Standard destructor.
 *
 * \brief          Standard destructor
 * \author         Sandino Morales
 * \date           25.06.2012
 *
 * \return         -
 *************************************************************************** */
CThirdEyeMask::~CThirdEyeMask()
{
  /* Empty body */
}

/* *************************** METHOD ************************************** */
/* generateImageMask
 *
 *
 * \brief       Generates and returns the image mask. The mask is created
 *		by calling the method generateImageMask().
 *
 * \author	Sandino Morales
 * \date	25.06.2012
 *
 * \param[in]	cv::Mat f_img: Input image to generate the mask
 *
 * \return	cv::Mat: The image mask
 *************************************************************************** */
bool CThirdEyeMask::generateImageMask( const cv::Mat f_img )
{
  /// Generate the image mask
  if( !generateImageMaskAuxiliar( f_img ) )
  {
    return false;
  }

  return true;
}

/* *************************** METHOD ************************************** */
/* generateImageMask (overloaded)
 *
 *
 * \brief	Overloaded version of the above method. The required params 
 *              can be defined in here.
 *
 * \author	Sandino Morales
 * \date       	25.06.2012
 *
 * \param[in]	cv::Mat f_img: Input image to generate the mask
 * \param[in]	const float f_thresholdGradient_f: Defines the lenght of the
 *		gradient
 * \param[in]   const float f_thresholdDistance_f: Defines the threslhold for
 *		the distance between edge pixels and regular ones
 *
 * \return			cv::Mat: The image mask
 *************************************************************************** */
bool CThirdEyeMask::generateImageMask( const cv::Mat f_img, 
			               const float f_thresholdGradient_f, 
			               const float f_thresholdDistance_f )
{
  /// Set the new params
  setParams( f_thresholdGradient_f, f_thresholdDistance_f );
	
  /// Generate the image mask
  if( !generateImageMaskAuxiliar( f_img ) )
  {
    return false;
  }

  return true;
}

/* *************************** METHOD ************************************** */
/* centralDifferenceKernels
 *
 *
 * \brief	TODO
 *              
 *
 * \author	Sandino Morales
 * \date       	25.06.2012
 *
 * \param[in]   -
 *
 * \return	cv::Mat: The image mask
 *************************************************************************** */
void CThirdEyeMask::centralDifferenceKernels()
{
  // Coefficient to define central differnces
  const float coeff_f{ 0.5f };

  const cv::Size kernelSize( m_kernelSize_ui, m_kernelSize_ui );

  // Initilize to zero the 3x3 "kernel images"
  m_horizontalKernel.create( kernelSize, CV_32FC1 );
  m_horizontalKernel.setTo( 0.f );
  m_verticalKernel.create(   kernelSize, CV_32FC1 );
  m_verticalKernel.setTo( 0.f );
  
  // Set the horizontal kernel values
  m_horizontalKernel.ptr<float>( 1 )[ 0 ] =  coeff_f;
  m_horizontalKernel.ptr<float>( 1 )[ 2 ] = -coeff_f;

  // Set the vertical kernel values
  m_verticalKernel.ptr<float>( 0 )[ 1 ] =  coeff_f;
  m_verticalKernel.ptr<float>( 2 )[ 1 ] = -coeff_f;
}

void CThirdEyeMask::setKernels()
{
  /// For now, just one option, central differences.
  /// Future work: forward differences, etc, etc.
  centralDifferenceKernels();
}

/* *************************** METHOD ************************************** */
/* generateBinaryGradientImage
 *
 *
 * \brief	Generates the binary image based on the gradient of the image
 *		using central differences to calculate the deriatives. To define
 *		the binarization threshold it is used the value of the variable
 *		m_thresholdGradient_f
 *
 * \author	Sandino Morales
 * \date	25.06.2012
 *
 * \param[in]	cv::Mat f_img: Input image data
 *
 * \return	cv::Mat: Binarized gradient image
 *************************************************************************** */
void CThirdEyeMask::generateBinaryGradientImage( const cv::Mat f_img )
{
  /// Allocate images for the horizontal and vertical derivatives
  const cv::Size imgSize( f_img.size() );
  cv::Mat centralDiffX( imgSize, CV_32FC1 );
  cv::Mat centralDiffY( imgSize, CV_32FC1 );

  
  cv::filter2D( f_img, centralDiffX, CV_32FC1, m_horizontalKernel );
  cv::filter2D( f_img, centralDiffY, CV_32FC1, m_verticalKernel   );

  /// Just in case, initilize or reset
  m_gradientImage.create( imgSize, CV_8UC1 );


  for ( unsigned y = 0; y < static_cast<unsigned>( imgSize.height ); ++y )
  {
    const float* yDir_p = centralDiffY.ptr<float>( y );
    const float* xDir_p = centralDiffX.ptr<float>( y );
    for ( unsigned x = 0; x < static_cast<unsigned>( imgSize.width ); ++x )
    {
      // Compute the length of the gradient
      const float lengthGradient_f = sqrt( (xDir_p[x]*xDir_p[x]) + (yDir_p[x]*yDir_p[x]) );

      // We kepth the points with large value. It is assigned the value zero
      // so we can use the cvDistTransform function
      if ( lengthGradient_f > m_thresholdGradient_f )
      {
	m_gradientImage.ptr<uchar>( y )[ x ] = 0;
      }
      else
      {
	m_gradientImage.ptr<uchar>( y )[ x ] = 255;
      }
    } // end for x
  } // end for y

}



/* *************************** METHOD ************************************** */
/* generateImageMask
 *
 * \brief	Generates the binary image mask. Note that the output image
 *              is not a binary image, it is a 8bit image with only two
 *              values, 0 or 255
 *
 * \author	Sandino Morales.
 * \date	25.06.2012.
 *
 * \param[in]	cv::Mat f_img: Input image to generate the mask from.
 *
 * \return	cv::Mat: The image mask
 *************************************************************************** */
bool CThirdEyeMask::generateImageMaskAuxiliar( cv::Mat f_img )
{
  /// Get the image dimensions
  const cv::Size imgSize( f_img.size() );
  
  /// Generate the binarized gradient image
  generateBinaryGradientImage( f_img );
  
  /// Allocate the distance transformed image
  m_distanceImage.create( imgSize, CV_32FC1 );
	
  /// Compute the distance transform image
  cv::distanceTransform( m_gradientImage, m_distanceImage, 
			 CV_DIST_L2, CV_DIST_MASK_PRECISE );
 
  /// Allocate space for the mask
  m_trueMask.create( imgSize, CV_32FC1 );
  for ( unsigned y = 0; y < static_cast<unsigned>( imgSize.height ); ++y )
  {
    for ( unsigned x = 0; x < static_cast<unsigned>( imgSize.width ); ++x )
    {			
      if ( m_distanceImage.ptr<float>( y )[ x ] > m_thresholdDistance_f )
      {
	m_trueMask.ptr<float>( y )[ x ] = 0.f;
      }
      else
      {
	m_trueMask.ptr<float>( y )[ x ] = 255.f;
      }
    } // end for y
  } // end for x

  return true;
}

/* *************************** METHOD ************************************** */
/* getBinarizedGradient
 *
 *
 * \brief	Returns the binarized gradient image stored in the member
 *		variable m_gradientImage
 *
 * \author	Sandino Morales
 * \date	25.06.2012
 *
 * \param[in]	-
 *
 * \return       cv::Mat m_gradientImage
 *************************************************************************** */
cv::Mat	CThirdEyeMask::getBinarizedGradient()
{
  return m_gradientImage;
}

/* *************************** METHOD ************************************** */
/* getDistanceImage
 *
 *
 * \brief	Returns the distance image generated form the binarized
 *		gradient image
 *
 * \author	Sandino Morales
 * \date	25.06.2012
 *
 * \param[in]	-
 *
 * \return      cv::Mat m_distanceImage
 *************************************************************************** */
cv::Mat	CThirdEyeMask::getDistanceImage()
{
  return m_distanceImage;
}

/* *************************** METHOD ************************************** */
/* getMask
 *
 *
 * \brief	Returns the binary mask needed for the mask trinocular 
 *		analysis
 *
 * \author	Sandino Morales
 * \date	25.06.2012
 *
 * \param[in]	-
 *
 * \return	cv::Mat m_trueMask
 *************************************************************************** */
cv::Mat	CThirdEyeMask::getMask()
{
  return m_trueMask;
}

/* *************************** METHOD ************************************** */
/* getMask
 *
 * \brief       Mask the input image.
 *
 * \author	Sandino Morales
 * \date	25.06.2012
 *
 * \param[in]	cv::Mat f_img: Image to mask.
 *
 * \return	A copy of the input image after being "maksed".
 *************************************************************************** */
cv::Mat	CThirdEyeMask::maskImage( const cv::Mat f_img, 
				  const float f_thresholdGradient_f, 
			          const float f_thresholdDistance_f )
{
  // Maybe not that efficient to initilize the image to something...
  // maybe try it later in the loop.
  cv::Mat maskedImage( f_img.size(), CV_32FC1, cv::Scalar( 255.f ) );

  // If the mask has not been created yet, create it!
  if( m_trueMask.empty() )
  {
    generateImageMaskAuxiliar( f_img );

    if( f_thresholdGradient_f > 0.f && f_thresholdDistance_f > 0.f )
    {
      generateImageMask( f_img,
			 f_thresholdGradient_f,
			 f_thresholdDistance_f );
    }
  }

  // Mask it!
  for ( unsigned y = 0; y < static_cast<unsigned>( f_img.rows ); ++y )
  {
    for ( unsigned x = 0; x < static_cast<unsigned>( f_img.cols ); ++x )
    {			
	if( m_trueMask.ptr<float>( y )[ x ] != 0.f )
	{
	  maskedImage.ptr<float>( y )[ x ] = f_img.ptr<float>( y )[ x ];
	}
      }
   } 
  
  return maskedImage;
}

/* *************************** METHOD ************************************** */
/* getMask
 *
 * \brief       Prints out the current state of the member data.
 *
 * \author	Sandino Morales
 * \date	25.06.2012
 *
 * \param[in]	-
 * \param[out]	-
 *
 * \return	-
 *************************************************************************** */
 void CThirdEyeMask::print()
 { 
   std::cout << "***************** Mask params **********************\n"
	<< "Gradient threshold = " << m_thresholdGradient_f << std::endl
	<< "Distance threshold = " << m_thresholdDistance_f << std::endl
	<< "****************************************************\n";
 }
