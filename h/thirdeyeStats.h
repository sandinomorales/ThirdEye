/* ******************************** FILE *********************************** */
/** \file    thirdeyeStats.h
 *
 *  \author  Sandino Morales
 *  \date    10.02.2010
 *  \note    .enpeda.. group. The University of Auckland
 *
 *************************************************************************** */
#ifndef FILE_THIRDEYE_STATS_H
#define FILE_THIRDEYE_STATS_H

// OpenCV includes
#include <opencv2/core/core.hpp>

class CThirdEyeStats 
{    
public:
  CThirdEyeStats ();
  ~CThirdEyeStats ();
    
  bool evaluate( const cv::Mat f_controlImg, const cv::Mat f_virtualImg,
		 const cv::Mat f_maskImg  );

  // TO DO
  bool evaluate( const cv::Mat f_controlImg, const cv::Mat f_virtualImg );

  void setROI( const unsigned f_x1_ui, const unsigned f_y1_ui,
	       const unsigned f_x2_ui, const unsigned f_y2_ui  );

  inline float getNCC()
  { return m_ncc_f; };

  inline float getNCCmask()
  { return m_nccMask_f; };

private:

  bool normalizedCrossCorrelation( cv::Mat f_controlImg, cv::Mat f_virtualImg,
				   cv::Mat f_mask );

  bool computeNCC( const float f_addSDControl_f, const float f_addSDVirtual_f,
		   const float f_size_f, const float f_numeratorNCC_f,
		   float &f_ncc_f );


  bool mean( cv::Mat f_controlImg, cv::Mat f_virtualImg,
	     cv::Mat f_mask,
	     float& f_meanControl_f, float& f_meanVirtual_f,
	     float& f_meanControlMask_f, float& f_meanVirtualMask_f );

  inline float  numeratorNCC( const float f_imageValue1_f, const float f_mean1_f,
			      const float f_imageValue2_f, const float f_mean2_f    )
  { return ( ( f_imageValue1_f - f_mean1_f ) * ( f_imageValue2_f - f_mean2_f ) ); };

  inline float denominatorNCC( const float f_size_f, const float f_sd1_f, const float f_sd2_f )
  { return ( ( f_size_f - 1.f ) * f_sd1_f * f_sd2_f ); }

  inline float  addSD( const float f_imageValue_f, const float f_mean_f )
  { return ( ( f_imageValue_f - f_mean_f ) * ( f_imageValue_f - f_mean_f ) ); };

  inline float SD( const float f_num_f, const float f_den_f )
  { return sqrt( ( f_num_f / f_den_f ) ); };

  //Roi
  unsigned m_x1_ui, m_y1_ui,
           m_x2_ui, m_y2_ui;

  float    m_ncc_f;

  float    m_nccMask_f;

}; // end class CThirdEyeStats



#endif // FILE_THIRDEYE_STATS_H
