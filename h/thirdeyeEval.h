#ifndef FILE_THIRDEYE_EVALUATION_H
#define FILE_THIRDEYE_EVALUATION_H

// OpenCV includes
#include <opencv2/core/core.hpp>

// Project includes
#include "thirdeyeMask.h"
#include "thirdeye.h"
#include "thirdeyeStats.h"

class CThirdEyeEvaluation
{
public:
  CThirdEyeEvaluation();

  ~CThirdEyeEvaluation();

  void  setParams( const float f_baseLine_f,
		   const float f_translationX_f, 
		   const float f_translationY_f,
		   const float f_translationZ_f,
		   const float f_m11_f, const float f_m21_f, 
		   const float f_m31_f, const float f_m12_f, 
		   const float f_m22_f, const float f_m32_f,
		   const float f_m13_f, const float f_m23_f, 
		   const float f_m33_f,
		   const float f_principalPointBaseX_f, 
		   const float f_principalPointBaseY_f,
		   const float f_focalLengthBase_f,     
		   const float f_focalLengthBaseY_f,
		   const float f_principalPointControlX_f,   
		   const float f_principalPointControlY_f,
		   const float f_focalLengthControlX_f,      
		   const float f_focalLengthControlY_f,
		   const float f_pixelSizeX_f, const float f_pixelSizeY_f );

  // Set the params that define the geometric transformation. They should be
  // packed in a suitable structure. See params.h
  inline void setParams( const SThirdEyeParams &f_params )
  {
    m_virtualImgGenerator.setParams( f_params );
  }

  // Set the parameters that define the mask
  inline void setMaskParams( const float f_thresholdDistance_f,
			     const float f_thresholdGradient_f  )
  {
    m_maskGenerator.setParams( f_thresholdDistance_f,
			       f_thresholdGradient_f );
  }

  // Set a RoI to discard obvious occluded regions
  inline void setEvaluationRoi( const unsigned f_x1_ui, const unsigned f_y1_ui,
				const unsigned f_x2_ui, const unsigned f_y2_ui  )
  {
    m_errorCalculator.setROI( f_x1_ui, f_y1_ui,
			      f_x2_ui, f_y2_ui );
  }

  // Pixels whose respective disparity has this value will be ignored
  // when generating the virtual image
  inline void setInvalidValue( const float f_invalid_f )
  {
    m_virtualImgGenerator.setInvalidValue( f_invalid_f  );
  }
		
  void  computeEvaluationIndices( const cv::Mat f_dispMap, const cv::Mat f_baseImg, 
				  const cv::Mat f_controlImg,
				  float &f_fullIndex_f, float &f_maskIndex_f );

  cv::Mat getVirtualImage( const cv::Mat f_dispMap, 
			   const cv::Mat f_baseImg );
  
  cv::Mat getMask( const cv::Mat f_controlImg,
		   const float f_thresholdGradient_f = -1.f, 
		   const float f_thresholdDistance_f = -1.f );

  void print();

private:

  // Virtual image
  cv::Mat m_virtualImage;

  /// Auxiliary objects
  // Mask generator   
  CThirdEyeMask  m_maskGenerator;

  // Virtual image generator   
  CThirdEye  m_virtualImgGenerator;

  // Error calculator
  CThirdEyeStats m_errorCalculator;

}; // end class CThirdEyeEvaluation


#endif /* FILE_THIRDEYE_EVALUATION_H */
