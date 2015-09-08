/* ******************************** FILE *********************************** */
/** \file    virtualViewError.cpp
 *
 *  \author  Sandino Morales
 *  \date    17.11.2010
 *  \note    The .enpeda.. Project.
 *
 *************************************************************************** */
// Corresponding header
#include "../h/thirdeyeStats.h"

// Common includes
#include <iostream>

using std::cout;

/* *************************** METHOD ************************************** */
/* Standard constructor.
 *
 *
 * \brief          Standard constructor.
 * \author         Sandino Morales
 * \date           17.11.2010
 *
 * \return         -
 *************************************************************************** */
CThirdEyeStats::CThirdEyeStats()
  : m_x1_ui(    0 ), 
    m_y1_ui(    0 ),
    m_x2_ui(  640 ), 
    m_y2_ui(  480 ),

    m_ncc_f(     -32000.f ),
    m_nccMask_f( -32000.f )
{
  /* Empty body */
}

/* *************************** METHOD ************************************** */
/* Standard destructor.
 *
 *
 * \brief          Standard destructor.
 * \author         Sandino Morales
 * \date           17.11.2010
 *
 * \return         -
 *************************************************************************** */
CThirdEyeStats::~CThirdEyeStats()
{
  /* Empty body */
}

/* *************************** METHOD ************************************** */
/* setROI
 *
 * \brief      Sets the evaluation region of interest.
 *
 * \author     Sandino Morales
 * \date       17.11.2010
 *
 * \param[in]  const unsigned f_x1_ui: Top left corner horz coordinate of RoI.
 * \param[in]  const unsigned f_y1_ui: Top left corner vert coordinate of RoI.
 * \param[in]  const unsigned f_x2_ui: Bottom right corner horz coordinate of RoI.
 * \param[in]  const unsigned f_y2_ui: Bottom right corner vert coordinate of RoI.
 *
 * \return     -
 *************************************************************************** */
void CThirdEyeStats::setROI( const unsigned f_x1_ui, const unsigned f_y1_ui,
			     const unsigned f_x2_ui, const unsigned f_y2_ui  )
{
  // TODO. Error check on the dimensions
  m_x1_ui = f_x1_ui;
  m_y1_ui = f_y1_ui;
  m_x2_ui = f_x2_ui;
  m_y2_ui = f_y2_ui;
}

/* *************************** METHOD ************************************** */
/* calculateNCCindex
 *
 * \brief      Computes the NCC index given a control, virtual and a mask image.
 *             The actual computation of the index is done by calling the method
 *             CThirdEyeStats::normalizedCrossCorrelation.
 *
 * \author     Sandino Morales
 * \date       17.11.2010
 *
 * \param[in]  const cv::Mat f_controlImg: Control image of the third eye analysis.
 * \param[in]  const cv::Mat f_virtualImg: Virtual image of the third eye analysis.
 * \param[in]  const cv::Mat f_maskImg: Mask image of the third eye analysis.
 *
 * \return     True if everything went well. False otherwise.
 *************************************************************************** */
bool CThirdEyeStats::evaluate( const cv::Mat f_controlImg, const cv::Mat f_virtualImg,
			       const cv::Mat f_maskImg )
{	
  // NOTE: Put the control image as the first argument
  if ( !normalizedCrossCorrelation( f_controlImg, f_virtualImg, f_maskImg ) )
  {
    return false;
  }
  
  return true;
}


/* *************************** METHOD ************************************** */
/* mean
 *
 * \brief      Computes the mean of all the images involved in the third eye
 *             analysis. 
 *
 * \author     Sandino Morales
 * \date       17.11.2010
 *
 * \param[in]  const cv::Mat f_controlImg: Control image of the third eye analysis.
 * \param[in]  const cv::Mat f_virtualImg: Virtual image of the third eye analysis.
 * \param[in]  const cv::Mat f_maskImg: Mask image of the third eye analysis.
 * \param[out] float& f_meanControl_f: Mask image of the third eye analysis.
 * \param[out] float& f_meanVirtual_f: Mask image of the third eye analysis.
 * \param[out] float& f_meanControlMask_f: Mask image of the third eye analysis.
 * \param[out] float& f_meanVirtualMask_f: Mask image of the third eye analysis.
 *
 * \return     True if everything went well. False otherwise.
 *************************************************************************** */
bool CThirdEyeStats::mean( const cv::Mat f_controlImg, const cv::Mat f_virtualImg,
			   const cv::Mat f_mask,
			   float& f_meanControl_f, float& f_meanVirtual_f,
			   float& f_meanControlMask_f, 
			   float& f_meanVirtualMask_f	)
{
  float addControl_f = 0.f,
        addVirtual_f = 0.f;
  float addControlMask_f = 0.f,
        addVirtualMask_f = 0.f;

  unsigned size_ui     = 0;
  unsigned sizeMask_ui = 0;
	
  for( unsigned y = m_y1_ui; y < m_y2_ui; ++y )
  {
    for( unsigned x = m_x1_ui; x < m_x2_ui; ++x )
    {
      addControl_f += f_controlImg.ptr<float>( y )[ x ];
      addVirtual_f += f_virtualImg.ptr<float>( y )[ x ];
      size_ui++;
      
      if( f_mask.ptr<float>( y )[ x ] > 0.f )
      {
	addControlMask_f += f_controlImg.ptr<float>( y )[ x ];
	addVirtualMask_f += f_virtualImg.ptr<float>( y )[ x ];
	sizeMask_ui++;
      }
			
    } // end for x
  } // end for y

  // Just in case
  if ( size_ui <= 0 || sizeMask_ui <= 0 )
  {
    cout << "ERROR CThirdEyeStats::mean: Calculation error (size_ui)!\n";
    return false;
  }

  f_meanControl_f = addControl_f / static_cast<float>( size_ui );
  f_meanVirtual_f = addVirtual_f / static_cast<float>( size_ui );
  
  f_meanControlMask_f = addControlMask_f / static_cast<float>( sizeMask_ui );
  f_meanVirtualMask_f = addVirtualMask_f / static_cast<float>( sizeMask_ui );

  return true;
	    	
}

/* *************************** METHOD ************************************** */
/* normalizedCrossCorrelation
 *
 * \brief      Computes the required normalized cross correlation for the
 *             third eye analysis. 
 *
 * \author     Sandino Morales
 * \date       17.11.2010
 *
 * \param[in]  const cv::Mat f_controlImg: Control image of the third eye analysis.
 * \param[in]  const cv::Mat f_virtualImg: Virtual image of the third eye analysis.
 * \param[in]  const cv::Mat f_maskImg: Mask image of the third eye analysis.
 *
 * \return    -
 *************************************************************************** */
bool CThirdEyeStats::normalizedCrossCorrelation( const cv::Mat f_controlImg,
						 const cv::Mat f_virtualImg,
						 const cv::Mat f_mask )  
{
  //Calulate the mean
  float meanControl_f = 0.f;
  float meanVirtual_f = 0.f;
  float meanControlMask_f = 0.f;
  float meanVirtualMask_f = 0.f;
  if( !mean( f_controlImg, f_controlImg, f_mask, 
	     meanControl_f, meanVirtual_f,
	     meanControlMask_f, meanVirtualMask_f )        )
  {
    return false;
  }

  // Debug
  //printf( "meanControl=%f  meanVirtual=%f\n",  meanControl_f, meanVirtual_f );	

  unsigned size_ui	  = 0;      
  float	   numeratorNCC_f  = 0.f;
  float	   addSDcontrol_f  = 0.f;
  float	   addSDvirtual_f  = 0.f;
  
  unsigned sizeMask_ui     = 0;
  float	numeratorNCCmask_f = 0.f;
  float	addSDcontrolMask_f = 0.f;
  float	addSDvirtualMask_f = 0.f;

  // main loop
  for( unsigned y = m_y1_ui; y < m_y2_ui; ++y )
  {
    const float* controlImg_p = f_controlImg.ptr<float>( y );
    const float* virtualImg_p = f_virtualImg.ptr<float>( y );
    for( unsigned x = m_x1_ui; x < m_x2_ui; ++x )
    {
      size_ui++;
			
      //Calculating the numerator of NCC
      numeratorNCC_f += numeratorNCC( controlImg_p[ x ], meanControl_f,
				      virtualImg_p[ x ], meanVirtual_f );
				
      // Addition for the SD
      addSDcontrol_f += addSD( controlImg_p[ x ], meanControl_f );
      addSDvirtual_f += addSD( virtualImg_p[ x ], meanVirtual_f );

      // Do the same stuff for the mask analysis
      if( f_mask.ptr<float>( y )[ x ] > 0.f )
      {
	//Calculating the numerator of NCC
	numeratorNCCmask_f += numeratorNCC( controlImg_p[ x ], meanControl_f,
					    virtualImg_p[ x ], meanVirtual_f  );

	// Addition for the SD
	addSDcontrolMask_f += addSD( controlImg_p[ x ], meanControl_f );
	addSDvirtualMask_f += addSD( virtualImg_p[ x ], meanVirtual_f );
	sizeMask_ui++;
      } // end if mask

    } // end for x
  } // end for y

  // Compute the values for the regular approach
  if( !computeNCC( addSDcontrol_f, addSDvirtual_f, static_cast<float>( size_ui ),
		   numeratorNCC_f, m_ncc_f ) )
  {
    return false;
  }

  // Compute the values for the mask approach
  if( !computeNCC( addSDcontrolMask_f, addSDvirtualMask_f, static_cast<float>( sizeMask_ui ),
		   numeratorNCCmask_f,   m_nccMask_f ) )
  {
    return false;
  }

  return  true;
}


/* *************************** METHOD ************************************** */
/* computeNCC
 *
 * \brief      Computes the final NCC value from the input values. The input
 *             values represent the additions (over all the involved images)
 *             required to compute NCC.
 *
 * \author     Sandino Morales
 * \date       17.11.2010
 *
 * \param[in]  const float f_addSDControl_f: Addition required for computing the SD of
 *             the control image.
 * \param[in]  const float f_addSDVirtual_f: Addition required for computing the SD of
 *             the virtual image.
 * \param[in]  const float f_size_f: Number of elements considered for calculatin the SD
 *             and the NCC.
 * \param[in]  const float f_numeratorNCC_f: Numerator for the NCC. Computed previouslly.
 * \param[out] float &f_ncc_f: NCC value (* 100, so that it looks nicer).
 *
 * \return     False, if there were no divisions by zero. True otherwise.
 *************************************************************************** */
bool CThirdEyeStats::computeNCC( const float f_addSDControl_f, const float f_addSDVirtual_f,
				 const float f_size_f, const float f_numeratorNCC_f,
				 float &f_ncc_f )
{
  // Compute the SD
  const float SDControl_f = SD( f_addSDControl_f, f_size_f );
  const float SDVirtual_f = SD( f_addSDVirtual_f, f_size_f );

  // Compute NCC's denominators
  const float denominatorNCC_f = denominatorNCC( f_size_f, SDControl_f, SDVirtual_f );

  // Just in case
  if( denominatorNCC_f == 0.f )
  {
    cout << "ERROR CThirdEyeStats::computeNCC: It is intended to divide by zero!\n";
    f_ncc_f = 32000.f;
    return false;
  }

  // Compute the final value
  f_ncc_f = ( f_numeratorNCC_f / denominatorNCC_f ) * 100.0f;

  // One more final check
  if( fabs( f_ncc_f  ) > 100.01f )
  {
    cout << "ERROR CThirdEyeStats::computeNCC: Calculation error!\n";
    return false;
  }
  
  return true;
}
