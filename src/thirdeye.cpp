/* ******************************** FILE *********************************** */
/** \file       virtualView.cpp
 *
 * \brief	This file contains the definition of the class CThirdEye.
 *
 *  \author	Sandino Morales
 *  \date	29.100.2010
 *  \note	The .enpeda.. group.
 *
 *************************************************************************** */
// Corresponding header
#include "../h/thirdeye.h"

// Common includes
#include <iostream>

using std::cout;
using std::endl;

/* *************************** METHOD ************************************** */
/* Standard constructor.
 *
 * \brief          Standard constructor.
 *
 * \author         Sandino Morales
 * \date           16.05.2012
 *
 * \return         -
 *************************************************************************** */
CThirdEye::CThirdEye( )
  : m_params( ),
    m_params_b( false ),

    m_background_f( 127.f ),
    m_invalid_f( -1.f )
{
  /* Empty body */
}

/* *************************** METHOD ************************************** */
/* Overloaded constructor.
 *
 *
 * \brief	Overloaded constructor that takes as inuput  the
 *              params required for performing the geometric transformation.
 *
 * \author      Sandino Morales.
 * \date        16.05.2012
 *
 * \param[in]   SThirdEyeParams f_params: Struct that hold the params.
 *
 * \return      -
 *************************************************************************** */
CThirdEye::CThirdEye( const SThirdEyeParams &f_params )
  :  m_params( f_params ),
     m_params_b( true ),
     
     m_background_f( 127.f ),
     
     m_invalid_f( -1.f )
{
  /* Empty body */
}


/* *************************** METHOD ************************************** */
/* Standard destructor.
 *
 * \brief          Standard destructor.
 *
 * \author         Sandino Morales
 * \date           29.100.2010
 *
 * \return         -
 *************************************************************************** */
CThirdEye::~CThirdEye() 
{
  /* Empty body */
}

/* *************************** METHOD ************************************** */
/* createVirtualView
 *
 * \brief      Generates a virtual image given a disparity map and a base image.
 *             The new position of each "pixel" in the base image is computed 
 *             by calling the method CThirdEye::computeNewPosition. In case two
 *             "pixles" are mapped into the same position in the virtual image, 
 *             the "pixel" closer to the camera (i.e. with the largest disparity)
 *             will be kept.
 *
 * \author     Sandino Morales.
 * \date       29.10.2010
 *
 * \param[in]  const cv::Mat f_disparityMap: Input disparity map.
 * \param[in]  const cv::Mat f_baseImg: Base image of the stareo pair.
 * \param[out] cv::Mat &f_virtualImg: Generated virtual image.
 *
 * \return     True, if the virtual image was succesfully generated. False
 *             otherwise.
 *************************************************************************** */
bool CThirdEye::generateVirtualImage( const cv::Mat f_disparityMap, 
				      const cv::Mat f_baseImg,
				      cv::Mat &f_virtualImg )
{	
  if ( f_disparityMap.empty() || f_baseImg.empty() )
  {
    cout << "ERROR CThirdEye::generateVirtualImage: No enough input data!\n";
    return false;
  }
  
  if ( !m_params_b )
  {
    cout << "ERROR CThirdEye::generateVirtualImage: Set up first the transformation parameters!\n";
    return false;
  }

  // Reallocate the virtual image, just in case
  f_virtualImg.create( f_baseImg.size(), f_baseImg.type() );
  f_virtualImg.setTo( m_background_f );

  // Use this 2-channel image to solve the problem when two positions (wrt the base image)
  // are mapped in the same position in the virtual image. The intensity in the virtual image
  // will be that with corresponding larger disparity. I.e., that of the 3D point closer to
  // the camera.
  const float free_f = -1.f;
  cv::Mat hold( f_baseImg.size(), CV_32FC2, cv::Scalar( free_f )  );
  
  // Compute the virtual image
  for ( unsigned y = 0; y < static_cast<unsigned>( f_baseImg.rows ); ++y ) 
  {	
    for ( unsigned x = 0; x < static_cast<unsigned>( f_baseImg.cols ); ++x )  
    {
      // Get the current disparity
      const float disparity_f = f_disparityMap.ptr<float>( y )[ x ];
      // Get the current intensity of the base image
      const float intensity_f = f_baseImg.ptr<float>( y )[ x ];

      // Ignore invalid values
      if( disparity_f == m_invalid_f )
      {
	continue;
      }
      
      // Compute the new position
      int virtualX_i = 0;
      int virtualY_i = 0;
      computeNewPosition( static_cast<float>( x ), static_cast<float>( y ), disparity_f,
			  virtualX_i, virtualY_i           );

      if( 0 <= virtualX_i && virtualX_i < f_baseImg.cols )
      {
	if(  0 <= virtualY_i && virtualY_i < f_baseImg.rows ) 
	{
	  // To access the 2-channel image hold.
	  // The + 1 in the following, is also beacause the 2 channel image
	  const unsigned xPos_ui = virtualX_i*2;
	  
	  // If position (virtualX_i, virtualY_i) has not been visitied before,
	  // set that position's intinsity of the virtual image to the intensity
	  // of the position (x,y) of the base image.
	  if( hold.ptr<float>( virtualY_i )[ xPos_ui ] == free_f  )
	  {
	    // Keep the disparity. We would need to compare it with the disparity of
	    // another (x,y) that is also mapped into (virtualX_i, virtualY_i).
	    hold.ptr<float>( virtualY_i )[ xPos_ui ]            = disparity_f;
	    // Save to the intensity, just in case.
	    hold.ptr<float>( virtualY_i )[ xPos_ui + 1]         = intensity_f;
	    // Set the value of the the virtual image
	    f_virtualImg.ptr<float>( virtualY_i )[ virtualX_i ] = intensity_f;
	  }
	  // If the position has alredy been visited, we keep the intensity of the
	  // point in the base image whose corresponding disparity is larger.
	  // I want to keep the 3D point which is closer to the camera.
	  else
	  {
	    // If the "old" disparity is smaller, use the current intensity
	    // (from the base imag)
	    if( hold.ptr<float>( virtualY_i )[ xPos_ui ] < disparity_f )
	    {
	      // The value in the virtual image is the one with less disparity
	      f_virtualImg.ptr<float>( virtualY_i )[ virtualX_i ] = intensity_f;

	      // Update the values of hold....
	      hold.ptr<float>( virtualY_i )[ xPos_ui ]     = disparity_f;
	      hold.ptr<float>( virtualY_i )[ xPos_ui + 1 ] = intensity_f;

	    }
	    // If the "old" disparity is larger, use the intensity from the "old"
	    // position 
	    else
	    {
	      f_virtualImg.ptr<float>( virtualY_i )[ virtualX_i ] =
	  	hold.ptr<float>( virtualY_i )[ xPos_ui + 1];

	      // No update of the values of hold is required, as the disparity
	      // in there is larger, so in case of another comparison we would
	      // the larger value so far.
	    }
	    
	  } // end else hold == free
	  
	} // endif virtualX_i whithin bounds
	
      } // endif virtualY_i whithin bounds
      
    
    } //endif x
    
  } //endif y

  return true;
}

/* *************************** METHOD ************************************** */
/* computeNewPosition
 *
 * \brief      Computes the position in the virtual image of a position given
 *             from the base image and the corresponding disparity value.
 *
 * \author     Sandino Morales.
 * \date       29.10.2010
 *
 * \param[in]  const float f_xPos_f: Horz position (wrt the base image).
 * \param[in]  const float f_yPos_f: Vert position (wrt the base image).
 * \param[in]  const float f_disp_f: Corresponding disparity value (wrt base image).
 * \param[out] int& f_xNewPos_i: Horz position (wrt the virtual image).
 * \param[out] int& f_yNewPos_i: Vert position (wrt the virtual image).
 *
 * \return     True, if the new position was succesfully Calculated. False
 *             otherwise.
 *************************************************************************** */
inline bool CThirdEye::computeNewPosition( const float f_xPos_f, 
					   const float f_yPos_f, 
					   const float f_disp_f,
					   int& f_xNewPos_i, 
					   int& f_yNewPos_i	)
{
  // Coordinates with respect to the image plane of the 
  // base camera of the stereo camera (origin in upper left corner)
  const float coordinateCameraX_f = f_xPos_f - m_params.m_principalPointBaseX_f;
  const float coordinateCameraY_f = f_yPos_f - m_params.m_principalPointBaseY_f;

  //Ratio of the pixel size
  const float ratio_f = m_params.m_pixelSizeX_f / m_params.m_pixelSizeY_f;

  //Calculation of common variables
  const float Xterm = ( coordinateCameraX_f * m_params.m_baseLine_f )           -
                      ( f_disp_f * m_params.m_translationX_f );
  const float Yterm = ( ratio_f * coordinateCameraY_f * m_params.m_baseLine_f ) - 
                      ( f_disp_f * m_params.m_translationY_f );
  const float Zterm = ( m_params.m_focalLengthBaseX_f * m_params.m_baseLine_f ) -
                      ( f_disp_f * m_params.m_translationZ_f );

  //Calculation of the common denominator
  const float denominator_f  =  ( m_params.m_m31_f*Xterm ) +
                                ( m_params.m_m32_f*Yterm ) + 
                                ( m_params.m_m33_f*Zterm );

  // Just in case
  if ( denominator_f == 0.f )
  {
    cout << "Error CThirdEye::computeNewPosition: A division by zero is attempted in the point ("
	 << myRound( f_xPos_f ) << "," << myRound( f_yPos_f ) << ")\n";
    return false;
  }

  //Calculation of virtual x
  const float numeratorX_f = ( m_params.m_m11_f*Xterm ) + 
                             ( m_params.m_m12_f*Yterm ) + 
                             ( m_params.m_m13_f*Zterm );
  f_xNewPos_i = myRound(  ( ( ( m_params.m_focalLengthControlX_f * numeratorX_f ) / denominator_f ) + 
			      m_params.m_principalPointControlX_f ) );

  //Calculation of virtual y
  const float numeratorY_f = ( m_params.m_m21_f*Xterm ) + 
                             ( m_params.m_m22_f*Yterm ) + 
                             ( m_params.m_m23_f*Zterm );
  f_yNewPos_i = myRound( ( ( ( m_params.m_focalLengthControlY_f * numeratorY_f ) / denominator_f ) +
			     m_params.m_principalPointControlY_f ) );
  

  return true;
}

/* *************************** METHOD ************************************** */
/* print
 *
 * \brief      Prints out the required parameters (stereo cam intinsic and
 *             extrinsic parameters; relative position of the control cam wrt
 *             the stereo came and the internal params of the control cam)
 *             used to generated the virtual image.
 *
 * \author     Sandino Morales.
 * \date       29.10.2010
 *
 * \param[in]  -
 * \param[out] -
 *
 * \return     -
 *************************************************************************** */
void CThirdEye::print()
{
  cout << "*************** Stereo Camera *********************\n" 
       << "Intrinsic params:\n"
       << "Principal Point X = " << m_params.m_principalPointBaseX_f << endl
       << "Principal Point Y = " << m_params.m_principalPointBaseY_f << endl
       << "Focal Lenght X    = " << m_params.m_focalLengthBaseX_f    << endl
       << "Focal Lenght Y    = " << m_params.m_focalLengthBaseY_f    << endl
       << "Extrinsic params:\n"
       << "Base line = " << m_params.m_baseLine_f << endl
       << "*************** Control camera *********************\n"
       << "Intrinsic params:\n"
       << "Principal Point X = " << m_params.m_principalPointControlX_f << endl
       << "Principal Point Y = " << m_params.m_principalPointControlY_f << endl
       << "Focal Lenght X    = " << m_params.m_focalLengthControlX_f    << endl
       << "Focal Lenght Y    = " << m_params.m_focalLengthControlY_f    << endl
       << "Extrinsic params:\n"
       << "Rotation Matrix:\n"
       <<  m_params.m_m11_f << "  " <<  m_params.m_m12_f << "  " << m_params.m_m13_f << endl
       <<  m_params.m_m21_f << "  " <<  m_params.m_m22_f << "  " << m_params.m_m23_f << endl
       <<  m_params.m_m31_f << "  " <<  m_params.m_m32_f << "  " << m_params.m_m33_f << endl
       << "Translation Vector:\n"
       << m_params.m_translationX_f << "  " << m_params.m_translationY_f
       << "  " <<  m_params.m_translationZ_f << endl
       << "****************************************************\n";
 }
