#include "StdAfx.h"
#include ".\ptavifile.h"


#define EXTRA_TEMP_SPACE 10000

CPTAviFile::CPTAviFile(void)
{
	m_pavifile		= NULL;
	m_pstream		= NULL;
	m_pstreamcompressed	= NULL;
	m_pTempBMPBuffer	= NULL;
	m_pTempBuffer	= NULL;
	m_pBitmapInfo        = NULL;

	m_iTimeIndex	     = 0;

	m_iCols     = 0;
	m_iRows     = 0;
	m_iBPP      = 0;
	m_iSize     = 0;
	m_iRowInc   = 0;
	m_frameRate = 0.0;

	m_bSizeLimited = false;
	m_iSplitFile = 0;   

	// Default compression options.
	m_compvars.fccHandler = mmioFOURCC( 'D', 'I', 'B', ' ' );
	m_compvars.dwFlags    = 0x0;
	m_compvars.lQ         = ICQUALITY_DEFAULT;
	      
	m_compvars.lpState = NULL;

	m_liBytesWritten = 0;
}

CPTAviFile::~CPTAviFile(void)
{
	close();

	if( m_pBitmapInfo != NULL )
	{
		free( m_pBitmapInfo );
		m_pBitmapInfo = NULL;
	}

	if( m_pTempBuffer != NULL )
	{
		delete [] m_pTempBuffer;
		m_pTempBuffer = NULL;
	}
	   
	if( m_pTempBMPBuffer != NULL )
	{
		delete [] m_pTempBMPBuffer;
		m_pTempBMPBuffer = NULL;
	}

	if( m_compvars.lpState != NULL )
	{
		delete [] m_compvars.lpState;
		m_compvars.lpState = NULL;
	}
}



bool CPTAviFile::checkVFWVersion()
{
   DWORD dwVer = ::VideoForWindowsVersion();

   if( dwVer < 0x010a )
   {
      ASSERT( false );  // vfw version too old.
      return false;
   }

   return true;
}

/**
    * Open an AVI for writing.  Deprecated.
    *
    * @param   iCols       Width, in pixels, of each frame.
    * @param   iRows       Hight, in pixels, of each frame.
    * @param   ibpp	   Bits per pixel -- 24 (BGR), 32 (BGRU). or 8 bit greyscale.
    * @param   iFramerate  Framerate that the .avi will play back in.
    */
bool CPTAviFile::open( const char* pszFilename, int iCols, int iRows, int ibpp, double dFramerate)
{
	//
   // Initialize the library.
   //
   ::AVIFileInit();

   if( !checkVFWVersion() )
   {
	   ::AfxMessageBox("VFW version is not match in CPTAviFile");
	
      return false;
   } 

   if( pszFilename == NULL || iRows <= 0 || iCols <= 0 || dFramerate <= 0.0 )
   {
      ASSERT( false );
      return false;
   }

   if( ibpp != 24 && ibpp != 32 && ibpp != 8 && ibpp != 16)
   {
      ASSERT( false );
      return false;
   }

   m_frameRate = dFramerate;
   m_iTimeIndex	= 0;
   m_liBytesWritten = 0;
   //
   // If these parameters change, set them and allocate the buffers
   //
   if ( m_iCols != iCols || m_iRows != iRows || m_iBPP  != ibpp ) 
   {
      //
      // Initialize members.
      //
      m_iCols     = iCols;
      m_iRows     = iRows;
      m_iBPP      = ibpp;
      m_iRowInc   = m_iCols * ( m_iBPP / 8 );
      m_iSize     = m_iRows * m_iRowInc;
      //
      // Allocate temp image buffers.
      //
      if( m_pTempBMPBuffer != NULL )
      {
         delete [] m_pTempBMPBuffer;
         m_pTempBMPBuffer = NULL;
      }
      m_pTempBMPBuffer  = new unsigned char[ m_iSize + EXTRA_TEMP_SPACE ];

      if( m_pTempBuffer != NULL )
      {
         delete [] m_pTempBuffer;
         m_pTempBuffer = NULL;
      }
      m_pTempBuffer = new unsigned char[ m_iSize ];
   }

   if ( m_pTempBMPBuffer == NULL || m_pTempBuffer == NULL ) 
      return false;

   
   
   //
   // Open the file.
   //
   HRESULT hr = ::AVIFileOpen( 
      &m_pavifile, pszFilename, OF_WRITE | OF_CREATE, NULL );
   
   if( hr != AVIERR_OK )
   {
      ASSERT( false );
      return false;
   }   

   //
   // Create the bitmap info structure for our avi.
   //
   if ( m_iBPP == 8 )
   {
      // we'll need room for the colour palette.

      m_pBitmapInfo = 
	 (BITMAPINFO*)malloc( sizeof( BITMAPINFO ) + sizeof( RGBQUAD ) * 256 );

      //
      // assume that it's greyscale for now.
      //
      for( int i = 0; i < 256; i++ )
      {
      	 m_pBitmapInfo->bmiColors[i].rgbBlue     = (unsigned char)i;
      	 m_pBitmapInfo->bmiColors[i].rgbGreen    = (unsigned char)i;
      	 m_pBitmapInfo->bmiColors[i].rgbRed      = (unsigned char)i;
      	 m_pBitmapInfo->bmiColors[i].rgbReserved = (unsigned char)0;
      }

   }
   else
   {
      m_pBitmapInfo = (BITMAPINFO*)malloc( sizeof( BITMAPINFO ) );
   }

   ::memset( m_pBitmapInfo, 0x0, sizeof( BITMAPINFOHEADER ) );

   BITMAPINFOHEADER* pbmiHeader = &m_pBitmapInfo->bmiHeader;

   pbmiHeader->biBitCount        = (unsigned short)m_iBPP;
   pbmiHeader->biHeight	         = m_iRows,
   pbmiHeader->biWidth	         = m_iCols,
   pbmiHeader->biSizeImage       = m_iSize;
   pbmiHeader->biCompression     = BI_RGB;
   pbmiHeader->biXPelsPerMeter   = 100;
   pbmiHeader->biYPelsPerMeter   = 100;
   pbmiHeader->biPlanes	         = 1;
   pbmiHeader->biClrUsed	 = 0;
   pbmiHeader->biClrImportant    = 0;
   pbmiHeader->biSize            = sizeof( BITMAPINFOHEADER );   

   AVISTREAMINFO  streaminfo;
   ::memset( &streaminfo, 0x0, sizeof( AVISTREAMINFO ) );

   streaminfo.fccType		    = streamtypeVIDEO;
   streaminfo.fccHandler	    = m_compvars.fccHandler;
   streaminfo.dwScale		    = 100;
   streaminfo.dwRate		    = (int)( dFramerate * 100.0 );
   streaminfo.dwSuggestedBufferSize = m_pBitmapInfo->bmiHeader.biSizeImage;
   ::SetRect( &streaminfo.rcFrame, 0, 0, m_iCols, m_iRows );   
   streaminfo.dwQuality             = m_compvars.lQ;

   hr = ::AVIFileCreateStream( m_pavifile, &m_pstream, &streaminfo );
   if( hr != AVIERR_OK )
   {
      ASSERT( false );
      return false;
   }

   //
   // Setup the compression options.
   //
   AVICOMPRESSOPTIONS compopts;

   if ( m_iBPP == 8 )
   {
	   compopts.fccHandler = mmioFOURCC( 'M', 'R', 'L', 'E' );
	   compopts.fccType    = streamtypeVIDEO;
	   compopts.dwQuality  = 10000; 
	   compopts.cbParms    = m_compvars.cbState;
	   compopts.lpParms    = m_compvars.lpState;
   } else {
	   compopts.fccHandler = m_compvars.fccHandler;
	   compopts.fccType    = streamtypeVIDEO;
	   compopts.dwQuality  = m_compvars.lQ; 
	   compopts.cbParms    = m_compvars.cbState;
	   compopts.lpParms    = m_compvars.lpState;
   }

   hr = ::AVIMakeCompressedStream( &m_pstreamcompressed, m_pstream, &compopts, NULL );
   if( hr != AVIERR_OK )
   {
      ASSERT( false );
      return false;
   }   

   hr = ::AVIStreamSetFormat(      
      m_pstreamcompressed, 0,  m_pBitmapInfo, 
      m_pBitmapInfo->bmiHeader.biSize );

   if( hr != AVIERR_OK )
   {
      ASSERT( false );      
      return false;
   }
   
   return true;

	
}

/**
    * Add a frame (in the specified format) to the open .avi
    */
bool CPTAviFile::appendFrame( const unsigned char* pBuffer, bool bInvert)
{
	if( m_pavifile == NULL || m_pstream == NULL )
	{
		ASSERT( false );
		return false;
	}

	const unsigned char* pWriteBuffer = pBuffer;

	if( bInvert )
	{
		unsigned char* pDest = m_pTempBuffer;
		for( int iRow = m_iRows - 1; iRow >= 0; iRow-- )
		{
			::memcpy( pDest, &pBuffer[ iRow * m_iRowInc ], m_iRowInc );
			pDest += m_iRowInc;
		}

		pWriteBuffer = m_pTempBuffer;
	}

	LONG lSamplesWritten;
	LONG lBytesWritten;

	   
	HRESULT hr = ::AVIStreamWrite(
		m_pstreamcompressed,
		m_iTimeIndex,
		1,
		(LPVOID)pWriteBuffer,
		m_iSize,
		AVIIF_KEYFRAME,
		&lSamplesWritten,
		&lBytesWritten  );

	m_liBytesWritten += lBytesWritten;

	m_iTimeIndex++;

	if( hr != AVIERR_OK )
	{
		::AfxMessageBox("error in appendFrame() func");
		close();
		return false;
	}

	return true;
}

/** Close the .avi file.  This is also done by the destructor. */
bool CPTAviFile::close()
{
	if( m_pstream != NULL )
   {
      ::AVIStreamClose( m_pstream );
      m_pstream = NULL;
   }

   if( m_pstreamcompressed != NULL )
   {
      ::AVIStreamClose( m_pstreamcompressed );
      m_pstreamcompressed = NULL;
   }

   if( m_pavifile != NULL )
   {
      ::AVIFileRelease( m_pavifile );
      m_pavifile = NULL;
   }
  
   //
   // Release the library.
   //
   ::AVIFileExit();   

   return true;
}

/***
    * Enumerates all available codecs using iRows, iCols, and iBPP.
    *
    * Call this function twice:
    * 1) Set picinfo as NULL and use the return value to setup
    *    an array of ICINFO structures.
    * 2) Pass in the array and it will be filled accordingly.
    *
    * @param   iCols       Width, in pixels, of each frame.
    * @param   iRows       Hight, in pixels, of each frame.
    * @param   iBPP	   Bits per pixel -- 24 (BGR), 32 (BGRU). or 8 bit greyscale.
    * @param   picinfo     Array of compressor info structures.
    * @param   iNumICInfo  Number of compressor info structures passed in.
    *
    * @return  The number of compressors available.
    */
int CPTAviFile::enumerateCompressors( int iRows, int iCols, int iBPP, ICINFO* picinfo, int iNumICInfo )
{

	// If picinfo is NULL, then we are retreiving the number of
   // usable compressors at the current settings so we can allocate
   // enough memory.
   bool bpicinfoNull = picinfo == NULL;

   ICINFO* picinfoTemp = new ICINFO[ 50 ];

   HIC hic;
   
   BITMAPINFOHEADER bih; 
   
   // Initialize the bitmap structure. 
   bih.biSize          = sizeof( BITMAPINFOHEADER );
   bih.biPlanes        = 1;
   bih.biCompression   = BI_RGB;
   bih.biXPelsPerMeter = 100;
   bih.biYPelsPerMeter = 100;
   bih.biClrUsed       = 0;
   bih.biClrImportant  = 0;
   bih.biWidth         = iCols;
   bih.biHeight        = iRows;
   bih.biBitCount      = (unsigned short)iBPP;
   
   bih.biSizeImage = 
      bih.biWidth * bih.biHeight * ( bih.biBitCount / 8 );

   int  iNumCompressors = 0;
   bool bICInfoFull     = false;
   
   for( int i = 0; ICInfo( 0, i, &picinfoTemp[ i ] ) && !bICInfoFull; i++ ) 
   { 
      // Open the compressor so we can query it.
      hic = ICOpen( picinfoTemp[ i ].fccType, 
         picinfoTemp[ i ].fccHandler, ICMODE_QUERY ); 
      
	  char fcc[4];
	  
	  fcc[3] = (picinfoTemp[ i ].fccHandler & (0xff<<24))>>24;
	  fcc[2] = (picinfoTemp[ i ].fccHandler & (0xff<<16))>>16;
	  fcc[1] = (picinfoTemp[ i ].fccHandler & (0xff<<8))>>8;
	  fcc[0] = picinfoTemp[ i ].fccHandler & 0xff;
	  

      if( hic ) 
      { 
         // Skip this compressor if it can't handle the format. 
         if( ICCompressQuery( hic, &bih, 0 ) != ICERR_OK ) 
         { 
            ICClose( hic ); 
            continue; 
         } 
         
         // Find out the compressor info. 
         if( !bpicinfoNull )
         {
            ICGetInfo( hic, &picinfo[ iNumCompressors ], sizeof( ICINFO ) );
         }

         iNumCompressors++;

         if( !bpicinfoNull && (iNumCompressors == iNumICInfo) )
         {
            bICInfoFull = true;
         }
         
         // Close the compressor.
         ICClose( hic ); 
      } 
   } 

   delete [] picinfoTemp;

   return iNumCompressors;
}

/*
    * Sets the compressor type.  Call this before opening an AVI.
    *
    * @param   compvars  Compressor options.
    */
void CPTAviFile::setCompressor( COMPVARS* pcompvars )
{
	if( pcompvars != NULL )
   {
      m_compvars = *pcompvars;
      m_compvars.lpState = new char[ pcompvars->cbState ];
      memcpy( m_compvars.lpState, pcompvars->lpState, pcompvars->cbState );
   }
}