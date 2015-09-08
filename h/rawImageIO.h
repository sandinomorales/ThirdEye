/* ******************************** FILE *********************************** */
/** \file   rawImageIO.h
 *
 *	\breif	This file declares the classes CImageSize and CRawImageIO
 *			used to load pgm, ppm and raw image files.
 *
 *  \author  Sandino Morales
 *  \date    05.05.2012
 *  \note    .enpeda.. Project, The University of Auckland
 *
 *************************************************************************** */
#ifndef FILE_CRawImageIO_h
#define FILE_CRawImageIO_h

// Common includes
#include <fstream>
#include <iostream>

#define IO_DATATYPE_32F	     32	// 32-bit float (float)
#define IO_DATATYPE_64F	     64	// 64-bit float (double)
#define INTERNAL_STRINGSIZE 255


// Class for defining size of an image
class CImageSize
{
public:
  // Constructors
  CImageSize( );

  CImageSize( const int f_width_i, const int f_height_i, const int f_dataType_i, 
	      const int f_nChannels_i = 1 );
	
  // Destructor
  ~CImageSize();

  // Equal to operator
  bool operator == (const CImageSize& f_imageSize) const
  {
    return ( f_imageSize.m_width_ui	 == m_width_ui      && 
	     f_imageSize.m_height_ui     == m_height_ui     &&
	     f_imageSize.m_pixelDepth_ui == m_pixelDepth_ui &&
	     f_imageSize.m_dataSize_ui   == m_dataSize_ui   &&
	     f_imageSize.m_maxValue_ui   == m_maxValue_ui       );
  };

  CImageSize operator= (const CImageSize& f_imageSize) const
  {
    CImageSize outSize;
    outSize.m_width_ui      = f_imageSize.m_width_ui;
    outSize.m_height_ui     = f_imageSize.m_height_ui;
    outSize.m_dataSize_ui   = f_imageSize.m_dataSize_ui;
    outSize.m_pixelDepth_ui = f_imageSize.m_pixelDepth_ui;
    outSize.m_maxValue_ui   = f_imageSize.m_maxValue_ui;
    return outSize;
  };	
	
  // Return number of elements / pixels in an image
  unsigned	getNumberElements() const;
	

  // Return the number of bytes in the image
  unsigned	getNumberBytes() const;
	

  // Displays in console the properties if the image
  void		print();
	

  //private:

  // Resets the image data
  bool		setImage( const int f_width_i, const int f_height_i, 
			  const int f_dataType_i, 
			  const int f_nChannels_i = 1 );	

  // Set the data size of individual values (Num of bits per pixel)
  void		setDataSize();

  // Member variables
  unsigned	m_width_ui;
  unsigned	m_height_ui;
  unsigned	m_dataSize_ui;
  unsigned	m_pixelDepth_ui;
  unsigned	m_nChannels_ui;
  unsigned	m_maxValue_ui;
};


// Image reading/writing class
class CRawImageIO
{
public:
  // Constructor
  CRawImageIO();
  // Desctructor
  ~CRawImageIO();


  // Raw image format functions
  // Used for reading/writing float, double, char, unsigned char, short, and unsigned short 2D data to file
  char* loadRawDataImage(  const std::string &f_file_in_name, 
			   CImageSize& f_imageSize );
	
  bool  loadRawDataImage(  const std::string &f_file_in_name, 
			   CImageSize& f_imageSize, char* f_imageData );
	
  bool  writeRawDataImage( const std::string &f_fileOutName_s, 
			   CImageSize &f_imageSize, 
			   char* f_imageData_p, 
			   const std::string &f_comments_p = nullptr );
  // Raw Data file functions
  bool readRawDataHeader( std::ifstream & f_file_in, 
			  int& f_xsize_i, int& f_ysize_i, 
			  int& f_dataType_i );

  void writeRawDataHeader( std::ofstream & f_file_out, 
			   CImageSize& f_imageSize, 
			   const std::string &f_comments = nullptr );
	
  bool loadRawDataFileHeader( std::ifstream & f_file_in, 
			      CImageSize& f_imageSize );

  // Generic data functions
  bool readData( std::ifstream & f_file_in, char* f_src_p, 
		 CImageSize& f_imageSize );

  bool writeData ( std::ofstream & f_file_out, const CImageSize& f_imageSize, 
		   char* f_src );
	
  inline void writeComments( std::ofstream &f_fileOut, 
			     const std::string &f_comments_s )
  {
    f_fileOut << "# " << f_comments_s.c_str() << "\n";
  };
	
  const char* writeDataType( const int f_dataType_i );
	
  inline const char* writeDataType( CImageSize& f_imageSize )
  { 
    return writeDataType( f_imageSize.m_pixelDepth_ui );		
  };
};



#endif // FILE_CRawImageIO_h
