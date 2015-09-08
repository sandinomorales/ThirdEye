// Corresponding header
#include "../h/thirdeyeEval.h"

// Common includes
#include <iostream>

using std::cout;
using std::endl;

/* *************************** METHOD ************************************** */
/* Standard constructor
 *
 * \brief      Standard constructor.
 *
 * \author     Sandino Morales.
 * \date       29.10.2010
 *
 * \param[in]  -
 * \param[out] -
 *
 * \return     -
 *************************************************************************** */
CThirdEyeEvaluation::CThirdEyeEvaluation()
  : m_maskGenerator( 5.f, 10.f ),
    m_virtualImgGenerator( ),
    m_errorCalculator( )
{
  /* Empty body */
}

/* *************************** METHOD ************************************** */
/* Standard destructor
 *
 * \brief      Standard destructor.
 *
 * \author     Sandino Morales.
 * \date       29.10.2010
 *
 * \param[in]  -
 * \param[out] -
 *
 * \return     -
 *************************************************************************** */
CThirdEyeEvaluation::~CThirdEyeEvaluation()
{
  /* Empty body */
}

/* *************************** METHOD ************************************** */
/* setParams
 *
 * \brief      Set the parameters required to compute the geometric transformation
 *             between the image plane of the base camera and that of the control
 *             camera. The params are feed one by one. See the comments in params.h
 *             for a description of each of the input params.
 *
 * \author     Sandino Morales.
 * \date       29.10.2010
 *
 * \param[in]  -
 * \param[out] -
 *
 * \return     -
 *************************************************************************** */
void CThirdEyeEvaluation::setParams( const float f_baseLine_f,
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
				     const float f_focalLengthBaseX_f,     
				     const float f_focalLengthBaseY_f,
				     const float f_principalPointControlX_f,   
				     const float f_principalPointControlY_f,
				     const float f_focalLengthControlX_f,      
				     const float f_focalLengthControlY_f,
				     const float f_pixelSizeX_f, 
				     const float f_pixelSizeY_f )
{
  m_virtualImgGenerator.setParams( SThirdEyeParams( f_baseLine_f,
				   f_translationX_f,  f_translationY_f,  f_translationZ_f,
				   f_m11_f,  f_m21_f,  f_m31_f,
				   f_m12_f,  f_m22_f,  f_m32_f,
				   f_m13_f,  f_m23_f,  f_m33_f,
		    f_principalPointBaseX_f, f_principalPointBaseY_f,
		       f_focalLengthBaseX_f, f_focalLengthBaseY_f,
		 f_principalPointControlX_f, f_principalPointControlY_f,
		    f_focalLengthControlX_f, f_focalLengthControlY_f,
		  	     f_pixelSizeX_f, f_pixelSizeY_f )	 );
}

/* *************************** METHOD ************************************** */
/* computeEvaluationIndices
 *
 * \brief      Computes the NCC indices for the third eye evaluation approach.
 *             This method requires as input a disparity map, the base image
 *             of the stereo pair used to compute the diparities and a control
 *             image for the evaluation. All the process of the evaluation is
 *             done in here, the generation of the virtual image (by calling
 *             CThirdEye::generateVirtualImage), and the calculation of the
 *             NCC indices (by calling CThirdEyeStats::evaluate). The mask
 *             required by the masked approach is also generated in here (by
 *             calling CThirdEyeMask::generateImageMask).
 *             The output evaluation indices are stored in two latter input
 *             arguments.
 *
 * \author     Sandino Morales.
 * \date       29.10.2010
 *
 * \param[in]  const cv::Mat f_dispMap: Input disparity map.
 * \param[in]  const cv::Mat f_baseImg: Base image from a stereo pair.
 * \param[in]  const cv::Mat f_controlImg: Control image, for evaluation.
 * \param[out] float &f_fullIndex_f: NCC index computed from the full approach.
 * \param[out] float &f_maskIndex_f: NCC index computed from the masked approach.
 *
 * \return     -
 *************************************************************************** */
void CThirdEyeEvaluation::computeEvaluationIndices( const cv::Mat f_dispMap, const cv::Mat f_baseImg,
						    const cv::Mat f_controlImg,
						    float &f_fullIndex_f, float &f_maskIndex_f )
{
  if( f_dispMap.empty() || f_baseImg.empty() || f_controlImg.empty() )
  {
    cout << "CThirdEyeEvaluation::computeEvaluationIndices: An input image is missing!\n";
    return;
  }
	
  // Generate the virtual image
  m_virtualImgGenerator.generateVirtualImage( f_dispMap, f_baseImg, m_virtualImage );

  // Generate the mask image. Required before computing the error
  m_maskGenerator.generateImageMask( f_controlImg );
	
  // Calculate the error indices
  m_errorCalculator.evaluate( f_controlImg, m_virtualImage, 
			      m_maskGenerator.getMask() );
  // Get the error indices
  f_fullIndex_f = m_errorCalculator.getNCC();
  f_maskIndex_f = m_errorCalculator.getNCCmask();
}


/* *************************** METHOD ************************************** */
/* getVirtualImage
 *
 * \brief      Returns the generated virtual image.
 * \author     Sandino Morales.
 * \date       01.09.2015
 *
 * \param[in]  const cv::Mat f_dispMap: Input disparity map (32 float).
 * \param[in]  const cv::Mat f_baseImg: Base image of the stereo pair.
 *
 * \return     cv::Mat: The generated virtual image.
 *************************************************************************** */
cv::Mat CThirdEyeEvaluation::getVirtualImage( const cv::Mat f_dispMap, 
					      const cv::Mat f_baseImg )
{
  // Generate the virtual image if necessary
  if( m_virtualImage.empty() )
  {
    m_virtualImgGenerator.generateVirtualImage( f_dispMap, f_baseImg, m_virtualImage );
  }

  return m_virtualImage;
}

/* *************************** METHOD ************************************** */
/* getMask
 *
 * \brief      Returns a masked image. If the last two parameters are given
 *             a value larger than zero, the mask will be generated using those
 *             values. Else, the mask will be generated usig the default params,
 *             or any other values that have been set previously.
 *
 * \author     Sandino Morales.
 * \date       01.09.2015
 *
 * \param[in]  const cv::Mat f_controlImg: Input image (32 float).
 * \param[in]  const float f_thresholdGradient_f: Defines the length of the gradient.
               This param, has a default value of -1.f.
 * \param[in]  const float f_thresholdDistance_f: Defines the required distance used by
 *             the distance transform. This param, has a default value of -1.f.
 *
 * \return     cv::Mat: The generated masked image.
 *************************************************************************** */
cv::Mat CThirdEyeEvaluation::getMask( const cv::Mat f_controlImg,
				      const float f_thresholdGradient_f, 
				      const float f_thresholdDistance_f )
{
  if( f_thresholdDistance_f > 0.f && f_thresholdGradient_f > 0.f )
  {
    m_maskGenerator.generateImageMask( f_controlImg,
				       f_thresholdGradient_f,
				       f_thresholdDistance_f   );
  }
  else
  {
    m_maskGenerator.generateImageMask( f_controlImg );
  }

  // This will return a masked image. The binary mask used to generated a masked
  // image can be accessed using CThirdEyeMask::getMask.
  return m_maskGenerator.maskImage( f_controlImg );  
}

/* *************************** METHOD ************************************** */
/* print
 *
 * \brief     Prints out all the parameters involved in the evaluation.
 *
 * \author     Sandino Morales.
 * \date       01.09.2015
 *
 * \param[in]  -
 * \param[out] -
 *
 * \return     -
 *************************************************************************** */
void CThirdEyeEvaluation::print()
{
  m_virtualImgGenerator.print();
  m_maskGenerator.print();
}
