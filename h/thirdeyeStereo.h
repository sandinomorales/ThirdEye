/* ******************************** FILE *********************************** */
/** \file    virtualView.h
 *
 *  \author  Sandino Morales
 *  \date    10.02.2010
 *  \note    .enpeda.. group. The University of Auckland
 *
 *************************************************************************** */
#ifndef FILE_THIRDEYE_STEREO_H
#define FILE_THIRDEYE_STEREO_H

// Common includes
#include <string>

// OpenCV includes
#include <opencv2/core/core.hpp>

// Project includes
#include "params.h"

class CThirdeyeStereo 
{    
public:

  // Constructors
  CThirdeyeStereo ( );

  CThirdeyeStereo( const SThirdEyeParams &f_params, 
		   const cv::Mat f_dispMap, const cv::Mat f_baseImg, 
		   const cv::Mat f_controlImage );

  // Destructor
  ~CThirdeyeStereo ( );
    
  void setParams( const float f_baseLine_f,
		  const float f_translationX_f, const float f_translationY_f, 
		  const float f_translationZ_f, const float f_m11_f, 
		  const float f_m21_f, const float f_m31_f,
		  const float f_m12_f, const float f_m22_f, const float f_m32_f,
		  const float f_m13_f, const float f_m23_f, const float f_m33_f,
		  const float f_principalPointBaseX_f, 
		  const float f_principalPointBaseY_f,
		  const float f_focalLengthBase_f,     
		  const float f_focalLengthBaseY_f,
		  const float f_principalPointControlX_f,   
		  const float f_principalPointControlY_f,
		  const float f_focalLengthControlX_f,      
		  const float f_focalLengthControlY_f,
		  const float f_pixelSizeX_f, const float f_pixelSizeY_f );
    
  void setParams( const SThirdEyeParams &f_params );

  void setMaskParams();
    
  cv::Mat getVirtualImageStereo( cv::Mat f_disparityMap_p, cv::Mat f_baseImage_p );

  void print();
		
  void getIndices( float& f_full_f, float& f_mask_f );
    
  float getFullIndex();

  float getMaskIndex();

private:

  bool			m_params_b;
  bool			m_images_b;

  // Internal and external camera params
  SThirdEyeParams	m_params;
			
  // Image dimensions
  unsigned int		m_imageWidth_ui,
    m_imageHeight_ui;


  float			m_fullIndex_f;

  float			m_maskIndex_f;

  // Virtual image
  cv::Mat createVirtualImageStereo( cv::Mat f_disparityMap_p, 
				    cv::Mat f_baseImage_p );
	
}; // end class CThirdeyeStereo



#endif // FILE_THIRDEYE_STEREO_H
