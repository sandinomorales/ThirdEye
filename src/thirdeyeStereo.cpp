/* ******************************** FILE *********************************** */
/** \file    virtualView.cpp
 *
 *  \author  Sandino Morales
 *  \date    18.06.2012
 *  \note    
 *
 *************************************************************************** */

// Corresponding header
#include "../h/thirdeyeStereo.h"


/* *************************** METHOD ************************************** */
/* Standard constructor.
 *
 *
 * \Brief          Standard constructor.
 * \author         Sandino Morales
 * \date           18.06.2012
 *
 * \return         -
 *************************************************************************** */
CThirdeyeStereo::CThirdeyeStereo()
  :	m_params_b( false ),
	m_images_b( false ),
	m_params(		  ),
	m_imageWidth_ui(  640 ),
	m_imageHeight_ui( 480 )
	
{
  /* Empty body */
}

/* *************************** METHOD ************************************** */
/* Overload constructor
 *
 *
 * \brief	This calculates a one frame virtual view and error. Are expected the
 *		required transformation params and all the images
 *
 * \author      Sandino Morales
 * \date        18.06.2012
 *
 * \input	const std::string f_name_s: Path of the sequence or the path of
 *		the params file
 *		const bool f_path_b: Set to true if the input string is the path
 *		of the sequence or to false is it is the params file
 *
 * \return      -
 *************************************************************************** */
CThirdeyeStereo::CThirdeyeStereo( const SThirdEyeParams &f_params, 
				  const cv::Mat f_dispMap, const cv::Mat  f_baseImg, 
				  const cv::Mat f_controlImage )
{
  /// Set the transformation params
  setParams( f_params );

  /// Get the virtual image
  cv::Mat virtualView = getVirtualImageStereo( f_dispMap, f_baseImg );
}


void CThirdeyeStereo::setParams( const SThirdEyeParams &f_params )
{
  m_params = f_params;
  m_params_b = true;
}

cv::Mat	CThirdeyeStereo::getVirtualImageStereo( cv::Mat f_disparityMap, cv::Mat f_baseImage )
{
  cv::Mat virtualView;

  return virtualView;
}
