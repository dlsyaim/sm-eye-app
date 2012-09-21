#pragma once
#include "vfw.h"

class CPTAviFile
{
public:
	CPTAviFile(void);
	~CPTAviFile(void);

	/** Read and verify the Video For Windows version. */
	bool checkVFWVersion();

	//open avi for writing
	bool open( const char* pszFilename, int height, int width, int ibpp, double dFramerate);

	bool appendFrame( const unsigned char* pBuffer, bool bInvert = true );

	bool close();

	int enumerateCompressors( int height, int width, int iBPP, ICINFO* picinfo, int iNumICInfo );

	void setCompressor( COMPVARS* pcompvars );

protected:
	PAVIFILE    m_pavifile;
	PAVISTREAM  m_pstream;
	PAVISTREAM  m_pstreamcompressed;

	/** Height, in pixels, of each frame in the .avi. */
	int	 m_iRows;

	/** Width, in pixels, of each frame in the .avi. */
	int	 m_iCols;

	/** Bits per pixel of the .avi. */
	int	 m_iBPP;

	/** Row increment, in bytes. */
	int	 m_iRowInc;

	/** Image size in bytes. */
	int	 m_iSize;

	/** Frame rate */
	double m_frameRate;

	/** Time index for current frame. */
	int	 m_iTimeIndex;

	/** Temporary image buffer. */
	unsigned char* m_pTempBuffer;

	/** Temporary buffer for saving .bmps. */
	unsigned char* m_pTempBMPBuffer;

	/** Our bitmapinfo structure */
	BITMAPINFO* m_pBitmapInfo;

	/** Avi file counter. */
	int m_iSplitFile;

	/** Flag indicating if the size of the avi file is limited to AVI_FILE_SPLIT_SIZE bytes */
	bool m_bSizeLimited;

	/** Total bytes written. */
	long int m_liBytesWritten;

	/** avi file name */
	char m_szAVIDestFile[ _MAX_PATH ];

	/** Compressor options. */
	COMPVARS m_compvars;
};
