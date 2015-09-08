// Common includes
#include <string>
#include <typeinfo>

// Corresponding header
#include "../h/rawImageIO.h"

using std::cout;
using std::endl;
using std::string;

/*******************************************************************************/
/***********************  Class CImageSize *************************************/
// Constructors
CImageSize::CImageSize( )
	: m_width_ui(	   640 ),
	  m_height_ui(     480 ),
	  m_dataSize_ui(    10 ),
	  m_pixelDepth_ui(   1 ),
	  m_nChannels_ui(    1 ),
	  m_maxValue_ui(  1023 )
{ 
	
}

CImageSize::CImageSize( const int f_width_i, const int f_height_i, 
			const int f_dataType_i, const int f_nChannels_i )
{ 
  setImage( f_width_i, f_height_i, f_dataType_i, f_nChannels_i); 
}
	
// Destructor
CImageSize::~CImageSize()
{

}



bool CImageSize::setImage( const int f_width_i, const int f_height_i, 
			   const int f_dataType_i, const int f_nChannels_i )
{
  // Error checking
  if ( f_width_i < 0 || f_height_i < 0 || f_dataType_i < 0 || f_nChannels_i < 0 )
  {
    cout << "ERROR CImageSize::setImage: One of your input parameters is negative!!\n";
    return false;
  }

  // Set member variables
  m_width_ui      = f_width_i;
  m_height_ui     = f_height_i;
  m_pixelDepth_ui = f_dataType_i;
  m_nChannels_ui  = f_nChannels_i;
  setDataSize();

  return true;
}

void CImageSize::setDataSize()
{
  switch ( m_pixelDepth_ui )
  {
    case IO_DATATYPE_32F:
      m_dataSize_ui = sizeof( float );
      break;
    case IO_DATATYPE_64F:
      m_dataSize_ui = sizeof( double );
      break;
    default:
      m_dataSize_ui = 0;
  }
}

unsigned CImageSize::getNumberElements() const
{		 
  return m_width_ui * m_height_ui * m_nChannels_ui;	
}

// Return the number of bytes in the image
unsigned CImageSize::getNumberBytes() const
{		
  return m_width_ui * m_height_ui * m_dataSize_ui * m_nChannels_ui;	
}

void CImageSize::print()
{
  printf( "***************** Image Specs *****************\n" );
  printf( "Image dimensions: %ix%i pixels\n", m_width_ui, m_height_ui );
  printf( "Depth per pixel: %i bits\n", m_pixelDepth_ui );
  printf( "Max gray level value: %i\n", m_maxValue_ui );
  printf( "No. of channels: %i\n", m_nChannels_ui );
  printf( "Size of image: %i bits\n", m_width_ui * m_height_ui * m_dataSize_ui * m_nChannels_ui );
  printf( "***********************************************\n" );
}

/********************************  Class CImageSize ************************************************/
/***************************************************************************************************/
CRawImageIO::CRawImageIO()
{
  /* Empty body */
}

CRawImageIO::~CRawImageIO()
{
  /* Empty body */
}

// Load Raw Data Methods
char* CRawImageIO::loadRawDataImage( const std::string &f_fileInName_s, CImageSize& f_imageSize )
{
  // Set up input file
  std::ifstream fileIn;
  fileIn.open ( f_fileInName_s.c_str(), std::ios::in | std::ios::binary );
	
  // Error checking
  if ( !fileIn )
  {
    cout << "ERROR CRawImageIO::loadRawDataImage: Cannot open the input file: " 
	 << f_fileInName_s.c_str() << endl;
    return nullptr;
  }
	
  // Load data from header
  if ( !loadRawDataFileHeader( fileIn, f_imageSize ) )
  {
    fileIn.close();
    return nullptr;
  }

  // Create output pointer
  char* outImage_p = new char[f_imageSize.getNumberBytes()];

  // Read image data
  if ( !readData( fileIn, outImage_p, f_imageSize ) )
  {
    cout << "ERROR CRawImageIO::loadRawDataImage: input file's image data is incorrect!\n";
    delete[] outImage_p;
    fileIn.close();
    return nullptr;
  }

  fileIn.close();
  return outImage_p;
}

bool CRawImageIO::loadRawDataImage( const std::string &f_fileInName_s, 
				    CImageSize & f_imageSize, char* f_imageData_p )
{
  // Set up a read only binary file
  std::ifstream fileIn;
  fileIn.open ( f_fileInName_s.c_str(), std::ios::in | std::ios::binary );
  // Error checking
  if ( !fileIn )
    {
      cout << "ERROR CRawImageIO::loadRawDataImage: Cannot open the input file: " 
	   << f_fileInName_s.c_str() << endl;
      return false;
    }

  // Load data from header
  CImageSize tempImageSize;
  if ( !loadRawDataFileHeader( fileIn, tempImageSize ) )
    {
      fileIn.close();
      return false;
    }

  // Load the image if headers match
  if ( tempImageSize == f_imageSize )
    {
      // Read the ground truth data
      if ( !readData( fileIn, f_imageData_p, f_imageSize ) )
	{
	  cout << "ERROR CRawImageIO::loadRawDataImage: input file's image data not correct!\n";
	  fileIn.close();
	  return false;
	}
      else
	{
	  fileIn.close();
	  return true;
	}
    }
  else
    {
      cout << "ERROR CRawImageIO::loadRawDataImage: input image does not match input file!" << endl;
      fileIn.close();
      return false;
    }
}

// Write Raw Data Methods
bool CRawImageIO::writeRawDataImage( const std::string &f_fileOutName_s,
				     CImageSize &f_imageSize, char* f_imageData_p, 
				     const std::string &f_comments_p )
{
 std::ofstream fileOut;
  fileOut.open ( f_fileOutName_s.c_str(), std::fstream::binary | std::fstream::out );

  if ( !fileOut.is_open() )
  {
    cout << "ERROR CRawImageIO::writeRawData: Cannot open the file: " 
	      << f_fileOutName_s.c_str() << endl;
    return false;
  }

  // Write header
  writeRawDataHeader( fileOut, f_imageSize, f_comments_p );

  // Write data ito file
  if ( !writeData( fileOut, f_imageSize, f_imageData_p) )
  {
    cout << "ERROR CRawImageIO::writeRawData: Output file failed to write!\n";
    fileOut.close();
    return false;
  }

  fileOut.close();
  return true;
}


// Internal Raw Data Methods
void CRawImageIO::writeRawDataHeader( std::ofstream& f_fileOut, CImageSize & f_imageSize,
				      const std::string &f_comments_s )
{
  if ( !f_comments_s.empty() )
  {
    writeComments( f_fileOut, f_comments_s );
  }

  f_fileOut << "# width height DataType(";
  f_fileOut << writeDataType( f_imageSize ) << ")\n";
  f_fileOut << f_imageSize.m_width_ui << " " 
	    << f_imageSize.m_height_ui << " " 
	    << f_imageSize.m_pixelDepth_ui << "\n";
}

bool CRawImageIO::loadRawDataFileHeader( std::ifstream& f_fileIn, CImageSize& f_imageSize )
{
  int width_i, height_i, dataType_i;

  if( !readRawDataHeader( f_fileIn, width_i, height_i, dataType_i ) )
    {
      cout << "ERROR CRawImageIO::loadRawData: Cannot read header of input file!\n";
      return false;
    }

  // Debug
  //printf( "widht=%i  height=%i type=%i\n", width_i, height_i, dataType_i );


  // Check datatype max depth_i
  const char* temp_p = writeDataType( dataType_i );
  if ( temp_p == nullptr )
    {
      cout << "ERROR CRawImageIO::loadRawData: datatype not supported!\n";
      delete[] temp_p;
      return false;
    }

  f_imageSize.setImage( width_i, height_i, dataType_i );
	
  return true;
}

// Read the Raw Data Header
bool CRawImageIO::readRawDataHeader( std::ifstream& f_fileIn, int& f_xsize_i, int& f_ysize_i, 
				     int& f_dataType_i  )
{
  // Set up reading parameters
  int count_i;
  char line_p[INTERNAL_STRINGSIZE];
  char* next_p;
  int step_i = 1;
  int width_i;
  //char word_p[INTERNAL_STRINGSIZE];

  while ( true )
  {
    f_fileIn.getline ( line_p, sizeof ( line_p ) );

    if ( f_fileIn.eof() )
      {
	return false;
      }

    next_p = line_p;

    // Ignore comments
    if ( line_p[0] == '#' )
      {
	continue;
      }

    // Get width of image
    if ( step_i == 1 )
      {
	count_i = sscanf ( next_p, "%d%n", &f_xsize_i, &width_i );
	next_p = next_p + width_i;
	if ( count_i == EOF )
	  {
	    continue;
	  }
	step_i = 2;
      }

    // Get height of image
    if ( step_i == 2 )
      {
	count_i = sscanf ( next_p, "%d%n", &f_ysize_i, &width_i );
	next_p = next_p + width_i;
	if ( count_i == EOF )
	  {
	    continue;
	  }
	step_i = 3;
      }

    // Get size of data
    if ( step_i == 3 )
      {
	count_i = sscanf ( next_p, "%d%n", &f_dataType_i, &width_i );
	next_p = next_p + width_i;
	if ( count_i == EOF )
	  {
	    continue;
	  }
	break;
      }
  }

  return true;

}

// Read 2D data from file
bool CRawImageIO::readData( std::ifstream& f_fileIn, char* src_p, CImageSize& f_imageSize )
{
  // Read data
  f_fileIn.read( src_p, f_imageSize.getNumberBytes() );
  // Check for errors
  if( f_fileIn.eof() )
  {
    cout << "Error CRawImageIO::readData: Something is wrong with the input file!\n";
    return false;
  }
  return true;
}


// Write 2D data to file
bool CRawImageIO::writeData( std::ofstream& f_fileOut, const CImageSize& f_imageSize,
			     char* f_src_p )
{
  //printf("num of bytes=%i\n", f_imageSize.getNumberBytes()  ); 
  // Write data
  f_fileOut.write( f_src_p, f_imageSize.getNumberBytes() );

  // Check for errors
  if( f_fileOut.eof() )
  {
    cout << "ERROR CRawImageIO::writeData: Error writing data!\n";
    return false;
  }

  return true;
}

const char * CRawImageIO::writeDataType( const int f_dataType_i )
{
  // the break statements are redundant
  switch( f_dataType_i )
  {
  case IO_DATATYPE_32F:
    return typeid(float).name();
    break;
  case IO_DATATYPE_64F:
    return typeid(double).name();
    break;
  default:
    return nullptr;
  }
}

