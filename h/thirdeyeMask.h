/* ******************************** FILE *********************************** */
/** \file	thirdeyeMask.h
 *
 *  \brief	Declaration of the CThirdEyeMask class. This class is used to create
 *		the mask used for analyising the non-homogeneous regions 
 *		within an image. It is intended to be used for the trinocular
 *		stereo evaluation
 *
 *  \author	Sandino Morales
 *  \date	25.06.2012
 *  \note	The .enpeda.. group. The University of Auckland
 *
 *************************************************************************** */
#ifndef FILE_THIRDEYE_MASK_H
#define FILE_THIRDEYE_MASK_H

// OpenCv includes
#include <opencv2/core/core.hpp>

class CThirdEyeMask
{
public:

  // Default constructor
  CThirdEyeMask();

  // Overloaded contructor. The input arguments redefine the member variables
  CThirdEyeMask( const float f_thresholdGradient_f, const float f_thresholdDistance_f );

  // Destructor
  ~CThirdEyeMask();
	
   
  /* *************************** METHOD ************************************** */
  /* setParams
  *
  *
  * \brief	Sets the member data variables.
  *
  * \author	Sandino Morales.
  * \date	25.06.2012
  *
  * \param[in]	const float f_thresholdGradient_f: Defines the lenght of the
  *		gradient.
  * \param[in]	const float f_thresholdDistance_f: Defines the threslhold for
  *		the distance between edge pixels and regular ones.
  *
  * \return         -
  *************************************************************************** */
  inline void setParams( const float f_thresholdGradient_f, 
			 const float f_thresholdDistance_f )
  {
    /// Set the new params
    setThresholdGradient( f_thresholdGradient_f );
    setThresholdDistance( f_thresholdDistance_f );    
  }

  inline void setThresholdDistance( const float f_thresholdDistance_f )
  { m_thresholdDistance_f = f_thresholdDistance_f; };

  inline void setThresholdGradient( const float f_thresholdGradient_f )
  { m_thresholdGradient_f = f_thresholdGradient_f; };

  bool  generateImageMask( const cv::Mat f_img );

  bool  generateImageMask( const cv::Mat f_img, 
			   const float f_thresholdGradient_f, 
			   const float f_thresholdDistance_f );
	
  cv::Mat  maskImage( cv::Mat f_inputImage_p, 
		      const float f_thresholdGradient_f = -1.f, 
		      const float f_thresholdDistance_f = -1.f );

  cv::Mat  getBinarizedGradient();

  cv::Mat  getDistanceImage();

  cv::Mat  getMask( );

  inline float getThresholdDistance()
  { return m_thresholdDistance_f; };

  inline float getThresholdGradient()
  { return m_thresholdGradient_f; };

  void print();

private:

  void  generateBinaryGradientImage( const cv::Mat f_img );

  void  centralDifferenceKernels();
	
  void  setKernels();

  bool  generateImageMaskAuxiliar( const cv::Mat f_img );
	
  // Data members
	
  // Params
  float  m_thresholdDistance_f;

  float  m_thresholdGradient_f;

  unsigned m_kernelSize_ui;

  // Kernels for generating the gradient
  cv::Mat m_horizontalKernel;

  cv::Mat m_verticalKernel;

  // Images in between and final mask
  cv::Mat  m_gradientImage;

  cv::Mat  m_distanceImage;

  cv::Mat  m_trueMask;
};
#endif /* FILE_THIRDEYE_MASK_H */
