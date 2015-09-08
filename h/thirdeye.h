/* ******************************** FILE *********************************** */
/** \file    virtualView.h
 *
 *  \author  Sandino Morales
 *  \date    10.02.2010
 *  \note    .enpeda.. group. The University of Auckland
 *
 *************************************************************************** */
#ifndef FILE_THIRDEYE_H
#define FILE_THIRDEYE_H

// OpenCV includes
#include <opencv2/core/core.hpp>

// Project includes
#include "thirdeyeMask.h"
#include "params.h"

class CThirdEye 
{    
public:

  // Defeault constructor 
  CThirdEye( );

  // Overloaded constructor. 
  CThirdEye( const SThirdEyeParams &f_params );

  // Destructor
  ~CThirdEye ( );
    
  
  /* *************************** METHOD ************************************** */
  /* setParams
   *
   * \brief      Set all the params requiered by the geometric transformation
   *
   * \author     Sandino Morales
   * \date       29.100.2010
   *
   * \param[in]  const SThirdEyeParams f_params: Params packed in a struct.
   * \param[out] const SThirdEyeParams f_params: Params packed in a struct.
   *
   * \return     -
   *************************************************************************** */
  inline void setParams( const SThirdEyeParams &f_params )
  {
    m_params = f_params;
    m_params_b = true;
  }
    
  void setMaskParams( const float f_thresholdDistance_f,
		      const float f_thresholdGradient_f  );

  // Pixels whose respective disparity has this value will be ignored
  // when generating the virtual image
  inline void setInvalidValue( const float f_invalid_f )
  {
    m_invalid_f = f_invalid_f;
  }

  void  print();

  bool  generateVirtualImage( const cv::Mat f_disparityMap, const cv::Mat f_baseImg,
			      cv::Mat &f_virtualImg );
		
private:

  SThirdEyeParams m_params;

  bool  m_params_b;

  float m_background_f;

  //
  float m_invalid_f;

  /// Methods
  bool computeNewPosition( const float f_xPos_f, const float f_yPos_f, 
			   const float f_disparity_f,
			   int& f_xNewPos_f, int& f_yNewPos_f );

  /* *************************** METHOD ************************************** */
  /* myRound
   *
   * \brief      Round a float to the nearest integer.
   *
   * \author     Sandino Morales
   * \date       29.10.2010
   *
   * \param[in]  const float f_value_f: Input float value.
   * \param[out] -
   *
   * \return     The rounded value as an integer.
   *************************************************************************** */
  inline int myRound( const float f_value_f )
  {
    if( f_value_f == 0.f )
    {
      return 0;
    }
    else if( f_value_f > 0.f )
    {
      return static_cast<int>( f_value_f + 0.5f );
    }
    else
    {
      return static_cast<int>( f_value_f - 0.5f );
    }

  }
};  /* end class CThirdEye */

#endif /* FILE_THIRDEYE_H */
