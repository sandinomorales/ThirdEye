// Regular includes
#include <iostream>

/// OpenCv icnludes
#include <opencv2/core/core.hpp>

// Project includes
#include "../h/thirdeyeEval.h"
#include "../h/loader.h"

int main( void )
{
  // Create an evaluation object
  CThirdEyeEvaluation eval;

  // Set a RoI to discard the obvious occlusions in the border of the images.
  // Depends on the calibration (has to be set manually by analizing the control, 
  // base and match images)
  eval.setEvaluationRoi( 50, 20, 600, 420);

  /// Set the parameters that define the transformation 
  // between the reference and control camera.
  // Depends on the calibration
  SThirdEyeParams paramsSample( 0.299663f,              // Base line StereoCam
				-0.505707f, 0.0f, 0.0f, // Translation base-control
				1.f, 0.f, 0.f,	        // Rotation matrix base-control
				0.f, 1.f, 0.f,
				0.f, 0.f, 1.f,
				302.454f, 285.46f,	// PP base camera
				1031.02f, 1031.02f,	// Focal lenght base camera
				302.454f, 285.46f,	// PP control camera
				1031.02f, 1031.02f,	// Focal lenght control camera
				1.f, 0.998045f	        // Pixel size
				);

  // Set the params
  eval.setParams( paramsSample );

  /// Set the mask params
  eval.setMaskParams( 10.f, 5.f );

  /// Load images. Input images are expected to be float images scaled to the range [0,255]
  cv::Mat base     = loadImageFile( "../images/img_000001_c0.pgm", 16 );
  showImage( base, "Base" );
  cv::Mat control = loadImageFile( "../images/img_000001_c1.pgm", 16 );
  showImage( control, "Control" );

  // The disparity image is expected to be 32-float image. 
  cv::Mat disp    = loadRawImage( "../images/disp_bp.raw" );
  showImage( disp, "Disparity Map" );

  eval.print();

  /// Compute the error
  float fullIndex_f = -1.f;
  float maskIndex_f = -1.f;
  eval.computeEvaluationIndices( disp,        // Disparity map
				 base,        // Base image of the stereo pair
				 control,     // Control image of the trinocular set
				 fullIndex_f, // NCC index of the full approach
				 maskIndex_f  // NCC index of the masked approach
				 );
  
  std::cout << "Ncc      = " << fullIndex_f << std::endl
	    << "Ncc mask = " << maskIndex_f << std::endl;

  

  /// Get and show the virtual image
  showImage( eval.getVirtualImage( disp, base ), "Virtual Image" );
  // Get and show the masked control image
  showImage( eval.getMask( control ), "Control Image" );

  return 0;

}
