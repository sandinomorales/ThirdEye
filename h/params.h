/* ******************************** FILE *********************************** */
/** \file    params.h
 *
 *  \brief   This file contains the definition and declaration of the struct
 *           SThirdEyeParams. In this struct are contained the params required
 *           for generating the virtual image used in the trinocular analysis.
 *
 *  \author  Sandino Morales
 *  \date    10.02.2010
 *  \note    .enpeda.. group. The University of Auckland
 *
 *************************************************************************** */
#ifndef FILE_PARAMS_H
#define FILE_PARAMS_H


struct SThirdEyeParams
{
  float m_baseLine_f;
  float m_translationX_f,   m_translationY_f,   m_translationZ_f;  //[cm]
  float m_m11_f,   m_m21_f,   m_m31_f,
        m_m12_f,   m_m22_f,   m_m32_f,
        m_m13_f,   m_m23_f,   m_m33_f;
  float m_principalPointBaseX_f, m_principalPointBaseY_f;          //[px]	
  float	m_focalLengthBaseX_f,    m_focalLengthBaseY_f;             //[px]
  float	m_principalPointControlX_f,   m_principalPointControlY_f;  //[px]
  float m_focalLengthControlX_f,      m_focalLengthControlY_f;     //[px]
  float	m_pixelSizeX_f,   m_pixelSizeY_f;			   //[micrometer]

  // Default constructor
  SThirdEyeParams()
  {
    setToThis(  0.f, 	 	// base line
		0.f, 0.f, 0.f,	// translation vector
		1.f, 0.f, 0.f,	// rotation matrix
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,	
		0.f, 0.f,	// principal point reference
		1.f, 1.f,	// focal lenght reference
		0.f, 0.f,	// principal point control
		1.f, 1.f,	// focal lenght control
		1.f, 1.f        // pixel size
		);
					
  };

  // Overloaded constructor. The arguments are the needed params
  SThirdEyeParams( const float f_baseLine_f,
		   const float f_translationX_f, const float f_translationY_f,
		   const float f_translationZ_f,
		   const float f_m11_f, const float f_m12_f, const float f_m13_f, 
		   const float f_m21_f, const float f_m22_f, const float f_m23_f,
		   const float f_m31_f, const float f_m32_f, const float f_m33_f,
		   const float f_principalPointBaseX_f, 
		   const float f_principalPointBaseY_f,
		   const float f_focalLengthBaseX_f,    
		   const float f_focalLengthBaseY_f,
		   const float f_principalPointControlX_f, 
		   const float f_principalPointControlY_f,
		   const float f_focalLengthControlX_f,      
		   const float f_focalLengthControlY_f,
		   const float f_pixelSizeX_f, const float f_pixelSizeY_f  )	
  {
    setToThis(  f_baseLine_f,
		f_translationX_f, f_translationY_f, f_translationZ_f,
		f_m11_f,  f_m12_f,  f_m13_f, 
		f_m21_f,  f_m22_f,  f_m23_f,
		f_m31_f,  f_m32_f,  f_m33_f,
		f_principalPointBaseX_f,    f_principalPointBaseY_f,
		f_focalLengthBaseX_f,       f_focalLengthBaseY_f,
		f_principalPointControlX_f, f_principalPointControlY_f,
		f_focalLengthControlX_f,    f_focalLengthControlY_f,
		f_pixelSizeX_f,             f_pixelSizeY_f );
  }
    
  // Default destructor
  ~SThirdEyeParams()
  {};

  // Overload operators
  SThirdEyeParams& operator=( const SThirdEyeParams &f_params )
  {
    setToThis(  f_params.m_baseLine_f, 
		f_params.m_translationX_f, f_params.m_translationY_f, 
		f_params.m_translationZ_f,
		f_params.m_m11_f, f_params.m_m12_f, f_params.m_m13_f,
		f_params.m_m21_f, f_params.m_m22_f, f_params.m_m23_f,
		f_params.m_m31_f, f_params.m_m32_f, f_params.m_m33_f,
		f_params.m_principalPointBaseX_f, 
		f_params.m_principalPointBaseY_f,
		f_params.m_focalLengthBaseX_f,    
		f_params.m_focalLengthBaseY_f,
		f_params.m_principalPointControlX_f,   
		f_params.m_principalPointControlY_f,
		f_params.m_focalLengthControlX_f,      
		f_params.m_focalLengthControlY_f,
		f_params.m_pixelSizeX_f, f_params.m_pixelSizeY_f );
		
    return *this;
  };

  // Methods
  void setToThis( const float f_baseLine_f,
		  const float f_translationX_f, const float f_translationY_f, 
		  const float f_translationZ_f,
		  const float f_m11_f, const float f_m12_f, const float f_m13_f, 
		  const float f_m21_f, const float f_m22_f, const float f_m23_f,
		  const float f_m31_f, const float f_m32_f, const float f_m33_f,
		  const float f_principalPointBaseX_f, 
		  const float f_principalPointBaseY_f,
		  const float f_focalLengthBaseX_f, 
		  const float f_focalLengthBaseY_f,
		  const float f_principalPointControlX_f, 
		  const float f_principalPointControlY_f,
		  const float f_focalLengthControlX_f, 
		  const float f_focalLengthControlY_f,
		  const float f_pixelSizeX_f, const float f_pixelSizeY_f )
  {
    // Base line of the stereo camera
    m_baseLine_f = f_baseLine_f;

    // Translation vector between the reference and control camera
    m_translationX_f = f_translationX_f;
    m_translationY_f = f_translationY_f;
    m_translationZ_f = f_translationZ_f;

    // Rotation matrix betwee the reference and control camera
    m_m11_f = f_m11_f; m_m12_f = f_m12_f; m_m13_f = f_m13_f;
    m_m21_f = f_m21_f; m_m22_f = f_m22_f; m_m23_f = f_m23_f;
    m_m31_f = f_m31_f; m_m32_f = f_m32_f; m_m33_f = f_m33_f;
	    
    // Internal params of the reference and control cameras
    m_principalPointBaseX_f    = f_principalPointBaseX_f;
    m_principalPointBaseY_f    = f_principalPointBaseY_f;
    m_focalLengthBaseX_f       = f_focalLengthBaseX_f; 
    m_focalLengthBaseY_f       = f_focalLengthBaseY_f;
    m_principalPointControlX_f = f_principalPointControlX_f;
    m_principalPointControlY_f = f_principalPointControlY_f;
    m_focalLengthControlX_f    = f_focalLengthControlX_f;
    m_focalLengthControlY_f    = f_focalLengthControlY_f;
    m_pixelSizeX_f             = f_pixelSizeX_f;
    m_pixelSizeY_f             = f_pixelSizeY_f;
  }
};

#endif /* FILE_PARAMS_H */
