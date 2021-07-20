/**********************************************************************************
* MIT License																	  *
*																				  *
* Copyright(c) 2021 Mrx42Code                                                     *
* https://github.com/Mrx42Code							 				          *
*																				  *
* Permission is hereby granted, free of charge, to any person obtaining a copy    *
* of this softwareand associated documentation files(the "Software"), to deal	  *
* in the Software without restriction, including without limitation the rights	  *
* to use, copy, modify, merge, publish, distribute, sublicense, and /or sell	  *
* copies of the Software, and to permit persons to whom the Software is			  *
* furnished to do so, subject to the following conditions :						  *
*																				  *
* The above copyright noticeand this permission notice shall be included in all   *
* copies or substantial portions of the Software.								  *
*																				  *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR	  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,		  *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE	  *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER		  *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE	  *
* SOFTWARE.																		  *
 **********************************************************************************/

//-----------------------------------------------------------------------------
// File: GL_TextureLoader.cpp: implementation of the GL_TextureLoader class.
//-----------------------------------------------------------------------------
#include "ExternNamespace.h"

using namespace std;

namespace NanoVNA {
	//*****************************************************************************  
	// Public Code
	//*****************************************************************************

	//-----------------------------------------------------------------------------
	// IMPLEMENT_DYNCREATE
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	// message handlers
	//-----------------------------------------------------------------------------

	//-Public----------------------------------------------------------------------
	// Name: MC_OpenGLTextureLoader(void)
	// Desc: MC_OpenGLTextureLoader class
	//-----------------------------------------------------------------------------
	MC_OpenGLTextureLoader::MC_OpenGLTextureLoader(void)
	{
		m_fAlphaConversion = false;
		m_fHighQualityTextures = false;
		m_fMipMapping = false;
		m_RedAlphaMatch = 0;
		m_GreenAlphaMatch = 0;
		m_BlueAlphaMatch = 0;
		ZeroMemory(&m_TextureFilterType, sizeof(m_TextureFilterType));
		ZeroMemory(&m_Texture, sizeof(m_Texture));
		m_pglTexture = &m_Texture;
		// default to alpha matching BLACK
		SetAlphaMatch(true, 0, 0, 0);													// Set the Alpha Matching State
		// default to full sized textures
		SetHighQualityTextures(true);
		// no mipmap textures
		SetMipMapping(false);
		//	SetMipMapping(true);
			// no texture filtering
		//	SetTextureFilter(eglTexFilterType::txNoFilter);
		SetTextureFilter(eglTexFilterType::txBilinear);
	}
	//-Public----------------------------------------------------------------------
	// Name: ~MC_OpenGLTextureLoader(void)
	//-----------------------------------------------------------------------------
	MC_OpenGLTextureLoader::~MC_OpenGLTextureLoader(void)
	{
		m_pglTexture = NULL;
	}
	//-Public----------------------------------------------------------------------
	// Name: SetAlphaMatch(GLboolean fEnabled,GLubyte RedAlphaMatch,GLubyte GreenAlphaMatch,GLubyte BlueAlphaMatch)
	//-----------------------------------------------------------------------------
	void MC_OpenGLTextureLoader::SetAlphaMatch(GLboolean fEnabled, GLubyte RedAlphaMatch, GLubyte GreenAlphaMatch, GLubyte BlueAlphaMatch)
	{
		m_fAlphaConversion = fEnabled;

		if (fEnabled == TRUE) {														// only set the colour match if the conversion is enabled
			m_RedAlphaMatch = RedAlphaMatch;
			m_GreenAlphaMatch = GreenAlphaMatch;
			m_BlueAlphaMatch = BlueAlphaMatch;
		}
	}
	//-Public----------------------------------------------------------------------
	// Name: SetHighQualityTextures(GLboolean fEnabled)
	//-----------------------------------------------------------------------------
	void MC_OpenGLTextureLoader::SetHighQualityTextures(GLboolean fEnabled)
	{
		m_fHighQualityTextures = fEnabled;
	}
	//-Public----------------------------------------------------------------------
	// Name: SetMipMapping(GLboolean fEnabled)
	//-----------------------------------------------------------------------------
	void MC_OpenGLTextureLoader::SetMipMapping(GLboolean fEnabled)
	{
		m_fMipMapping = fEnabled;
	}
	//-Public----------------------------------------------------------------------
	// Name: SetTextureFilter(eglTexFilterType type)
	//-----------------------------------------------------------------------------
	void MC_OpenGLTextureLoader::SetTextureFilter(eglTexFilterType type)
	{
		m_TextureFilterType = type;
	}
	//-Public----------------------------------------------------------------------
	// Name: LoadTextureFromDisk(std::string _szFileNamee)
	//-----------------------------------------------------------------------------
	int	MC_OpenGLTextureLoader::LoadTextureFromDisk(std::string _szFileName)
	{
		char* szFileName = (char*)_szFileName.c_str();

		char szFullPath[MAX_PATH + 1];												// Full Path To Picture
		char szExtension[16];														// Extenstion of Picture

		strcpy_s(m_pglTexture->TextureFileName, szFileName);
		if (strstr(szFileName, "http://")) {										// If PathName Contains http:// Then...
			strcpy_s(szFullPath, szFileName);										// Append The PathName To FullPath
		} else {																	// Otherwise... We Are Loading From A File
			GetCurrentDirectoryA(MAX_PATH, szFullPath);								// Get Our Working Directory
			strcat_s(szFullPath, "\\");												// Append "\" After The Working Directory
			strcat_s(szFullPath, szFileName);										// Append The PathName
		}
		ExtensionFromFilename(szFileName, szExtension);
		// if the file is a TGA then use the TGA file loader
		if (lstrcmpiA(szExtension, "tga") == 0) {
			return(LoadTGAFromDisk(szFullPath, m_pglTexture));						// Load TGA (Compressed/Uncompressed)
		} else {
			// else load BMP, JPG, GIF
			m_pglTexture->TexType = eglTexType::txUnknown;
			if (lstrcmpiA(szExtension, "bmp") == 0) {
				m_pglTexture->TexType = eglTexType::txBmp;
			} else if ((lstrcmpiA(szExtension, "jpg") == 0) || (lstrcmpiA(szExtension, "jpeg") == 0)) {
				m_pglTexture->TexType = eglTexType::txJpg;
			} else if (lstrcmpiA(szExtension, "gif") == 0) {
				m_pglTexture->TexType = eglTexType::txGif;
			} else if (lstrcmpiA(szExtension, "ico") == 0) {
				m_pglTexture->TexType = eglTexType::txIco;
			} else if (lstrcmpiA(szExtension, "emf") == 0) {
				m_pglTexture->TexType = eglTexType::txEmf;
			} else if (lstrcmpiA(szExtension, "wmf") == 0) {
				m_pglTexture->TexType = eglTexType::txWmf;
			}
			return(BuildTexture(szFullPath, m_pglTexture));							// Load BMP, JPG, GIF etc..
		}
	}
	//-Public----------------------------------------------------------------------
	// Name: LoadTextureFromRam(BYTE* pData,int Length,eglTexType TexType)
	//-----------------------------------------------------------------------------
	int	MC_OpenGLTextureLoader::LoadTextureFromRam(BYTE* pData, int Length, eglTexType TexType)
	{
		char szTempFileName[MAX_PATH + 1];

		GetCurrentDirectoryA(MAX_PATH, szTempFileName);								// Get Our Working Directory
		lstrcatA(szTempFileName, "glTEMP$$.tmp");
		HANDLE hFile = CreateFileA(szTempFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
		if (hFile != INVALID_HANDLE_VALUE) {
			int	  rc;
			DWORD BytesWritten;

			WriteFile(hFile, pData, Length, &BytesWritten, NULL);					// Write The Data To disk (To A Temp File)
			CloseHandle(hFile);														// Close The File
			// set the texture type
			m_pglTexture->TexType = TexType;
			// if the file is a TGA then use the TGA file loader
			if (TexType == eglTexType::txTga) {
				rc = LoadTGAFromDisk(szTempFileName, m_pglTexture);
			} else {
				// else load BMP, JPG, GIF
				rc = BuildTexture(szTempFileName, m_pglTexture);
			}
			DeleteFileA(szTempFileName);											// Delete The Temp File
			return(rc);																// Teturn State Of Texture Load
		} else {
			return false;															// Return Failed
		}
	}
	//-Public----------------------------------------------------------------------
	// Name: LoadTextureFromResource(UINT ResourceName,CHAR* pResourceType,eglTexType TexType)
	//-----------------------------------------------------------------------------
	int	MC_OpenGLTextureLoader::LoadTextureFromResource(UINT ResourceName, char* pResourceType, eglTexType TexType)
	{
		int	rc = false;

		if ((TexType == eglTexType::txJpg) || (TexType == eglTexType::txGif)) {
			m_pglTexture->TexType = TexType;										// set the texture type
			rc = LoadJPG_GIFResource(MAKEINTRESOURCEA(ResourceName), pResourceType, m_pglTexture);
		}
		if (TexType == eglTexType::txBmp) {
			m_pglTexture->TexType = TexType;										// set the texture type
			// some bug with the bitmap resource loader..
			//rc = LoadJPG_GIFResource(MAKEINTRESOURCE(ResourceName),pResourceType,pglTexture);
			//rc = LoadBMPResource(pResourceName,pResourceType,pglTexture);
		}
		if (TexType == eglTexType::txTga) {
			m_pglTexture->TexType = TexType;										// set the texture type
			rc = LoadTGAResource(MAKEINTRESOURCEA(ResourceName), pResourceType, m_pglTexture);
		}
		return (rc);
	}
	//-Public----------------------------------------------------------------------
	// Name: FreeTexture(void)
	//-----------------------------------------------------------------------------
	void MC_OpenGLTextureLoader::FreeTexture(void)
	{
		glDeleteTextures(1, &m_pglTexture->TextureID);
	}
	//-Public----------------------------------------------------------------------
	// Name: BuildColorTexture(BYTE r, BYTE g, BYTE b)
	//-----------------------------------------------------------------------------
	void MC_OpenGLTextureLoader::BuildColorTexture(BYTE r, BYTE g, BYTE b)
	{
		BYTE data[12];																// a 2x2 texture at 24 bits

		for (int i = 0; i < 12; i += 3) {											// Store the data
			data[i] = r;
			data[i + 1] = g;
			data[i + 2] = b;
		}
		glGenTextures(1, &m_Texture.TextureID);										// Generate the OpenGL texture id
		glBindTexture(GL_TEXTURE_2D, m_Texture.TextureID);							// Bind this texture to its id
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);	// Use mipmapping filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2, GL_RGB, GL_UNSIGNED_BYTE, data);	// Generate the texture
	}










	//*****************************************************************************  
	// Protected Code
	//*****************************************************************************

	//-Protected-------------------------------------------------------------------
	// Name: BuildTexture(CHAR* szPathName,glTexture* pglTexture)
	//-----------------------------------------------------------------------------
	int MC_OpenGLTextureLoader::BuildTexture(char* szPathName, glTexture* pglTexture)
	{																				// Load BMP,GIF and JPG and Convert To A Texture
		HDC			hdcTemp;														// The DC To Hold Our Bitmap
		HBITMAP		hbmpTemp;														// Holds The Bitmap Temporarily
		IPicture* pPicture;														// IPicture Interface
		OLECHAR		wszPath[MAX_PATH + 1];											// Full Path To Picture (WCHAR)
		long		lWidth;															// Width In Logical Units
		long		lHeight;														// Height In Logical Units
		long		lWidthPixels;													// Width In Pixels
		long		lHeightPixels;													// Height In Pixels
		GLint		glMaxTexDim;													// Holds Maximum Texture Size
		RECT		PictureRect;

		ZeroMemory(&PictureRect, sizeof(PictureRect));
		MultiByteToWideChar(CP_ACP, 0, szPathName, -1, wszPath, MAX_PATH);			// Convert From ASCII To Unicode
		HRESULT hr = OleLoadPicturePath(wszPath,									// Path
			NULL,																	// punkCaller
			0,																		// Reserved
			0,																		// Reserved Transparent Colour
			IID_IPicture,															// riid of Inertface to return
			(void**)&pPicture);														// pointer to returned interface

		if (FAILED(hr)) {															// If Loading Failed
			return false;															// Return False
		}
		hdcTemp = CreateCompatibleDC(GetDC(0));										// Create The Windows Compatible Device Context
		if (!hdcTemp) {																// Did Creation Fail?
			pPicture->Release();													// Decrements IPicture Reference Count
			return false;															// Return False (Failure)
		}
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);							// Get Maximum Texture Size Supported
		pPicture->get_Width(&lWidth);												// Get IPicture Width (Convert To Pixels)
		lWidthPixels = MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
		pPicture->get_Height(&lHeight);												// Get IPicture Height (Convert To Pixels)
		lHeightPixels = MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);		// Resize Image To Closest Power Of Two
		if (lWidthPixels <= glMaxTexDim) {											// Is Image Width Less Than Or Equal To Cards Limit
			lWidthPixels = 1 << (int)floor((log((double)lWidthPixels) / log(2.0f)) + 0.5f);
		} else {																	// Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
			lWidthPixels = glMaxTexDim;
		}
		if (lHeightPixels <= glMaxTexDim) {											// Is Image Height Greater Than Cards Limit
			lHeightPixels = 1 << (int)floor((log((double)lHeightPixels) / log(2.0f)) + 0.5f);
		} else {																	// Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
			lHeightPixels = glMaxTexDim;
		}
		if ((m_fHighQualityTextures == false) && (lWidthPixels > 64)) {				// if low quality textures then make them halve the size which saved 4 times the texture space
			lWidthPixels /= 2;
			lHeightPixels /= 2;
		}																			//	Create A Temporary Bitmap
		BITMAPINFO	bi = { 0 };														// The Type Of Bitmap We Request
		DWORD* pBits = 0;														// Pointer To The Bitmap Bits
		bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);						// Set Structure Size
		bi.bmiHeader.biBitCount = 32;											// 32 Bit
		bi.bmiHeader.biWidth = lWidthPixels;									// Power Of Two Width
		bi.bmiHeader.biHeight = lHeightPixels;								// Make Image Top Up (Positive Y-Axis)
		bi.bmiHeader.biCompression = BI_RGB;										// RGB Encoding
		bi.bmiHeader.biPlanes = 1;											// 1 Bitplane
																					//	Creating A Bitmap This Way Allows Us To Specify Color Depth And Gives Us Imediate Access To The Bits
		hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);
		if (!hbmpTemp) {																// Did Creation Fail?
			DeleteDC(hdcTemp);														// Delete The Device Context
			pPicture->Release();													// Decrements IPicture Reference Count
			return false;															// Return False (Failure)
		}
		SelectObject(hdcTemp, hbmpTemp);											// Select Handle To Our Temp DC And Our Temp Bitmap Object
																					// Render The IPicture On To The Bitmap
		pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, &PictureRect);	// Convert From BGR To RGB Format And Add An Alpha Value Of 255
		if (m_fAlphaConversion == TRUE) {										// it is important to remember that the colour order is different as we are reading in a DWORD so the intel processor will read it in as little endian so the order is reversed (ABGR)
			DWORD AlphaMatch = (((m_RedAlphaMatch) | (m_GreenAlphaMatch << 8)) | (m_BlueAlphaMatch << 16));
			DWORD* pRGBA = (DWORD*)pBits;
			LONG NumPixels = lWidthPixels * lHeightPixels;
			while (NumPixels--) {													// Loop Through All Of The Pixels
				DWORD	tempRGB;

				tempRGB = *pRGBA;													// swap red and blue over
				tempRGB = ((tempRGB & 0x00FF0000) >> 16) | (tempRGB & 0x0000FF00) | ((tempRGB & 0x000000FF) << 16);
				if (AlphaMatch != tempRGB) {
					tempRGB |= 0xFF000000;
				}
				*pRGBA++ = tempRGB;
			}
		} else {
			byte* pPixel = (BYTE*)pBits;											// Grab The Current Pixel
			long NumPixels = lWidthPixels * lHeightPixels;
			while (NumPixels--) {
				byte  temp = pPixel[0];												// Store 1st Color In Temp Variable (Blue)
				pPixel[0] = pPixel[2];												// Move Red Value To Correct Position (1st)
				pPixel[2] = temp;													// Move Temp Value To Correct Blue Position (3rd)
				pPixel[3] = 255;													// Set The Alpha Value To 255
				pPixel += 4;
			}
		}
		pglTexture->Width = bi.bmiHeader.biWidth;
		pglTexture->Height = bi.bmiHeader.biHeight;
		pglTexture->Bpp = bi.bmiHeader.biBitCount;									// Image Color Depth In Bits Per Pixel
		pglTexture->Type = GL_RGBA;
		glGenTextures(1, &pglTexture->TextureID);									// Create The Texture
		(void)GenerateTexture(pglTexture, (byte*)pBits);							// generate the texture using the filtering model selected
		DeleteObject(hbmpTemp);														// Delete The Object
		DeleteDC(hdcTemp);															// Delete The Device Context
		pPicture->Release();														// Decrements IPicture Reference Count
		return true;																// Return True (All Good)
	}
	//-Protected-------------------------------------------------------------------
	// Name: LoadTGAFromDisk(CHAR* pszFileName,glTexture* pglTexture)
	//-----------------------------------------------------------------------------
	int MC_OpenGLTextureLoader::LoadTGAFromDisk(char* pszFileName, glTexture* pglTexture)
	{
		FILE* fTGA;																	// File pointer to texture file
		_TGAHeader	header;
		GLubyte* pImgData;
		GLint		glMaxTexDim;													// Holds Maximum Texture Size
		size_t		MemSize = 0;

		fopen_s(&fTGA, pszFileName, "rb");											// Open file for reading
		if (fTGA == NULL) {															// If it didn't open....
			return false;															// Exit function
		}
		if (fread(&header, sizeof(_TGAHeader), 1, fTGA) == 0) {						// Attempt to read 12 byte header from file
			if (fTGA != NULL) {														// Check to seeiffile is still open
				fclose(fTGA);														// If it is, close it
			}
			return false;															// Exit function
		}
		const UINT imageType = header.ImgType;										// Precalc some values from the header
		const UINT imageWidth = header.WidthLo + header.WidthHi * 256;
		const UINT imageHeight = header.HeightLo + header.HeightHi * 256;
		const UINT imageBytesPerPel = header.Bpp / 8;
		const UINT imageSize = imageWidth * imageHeight * imageBytesPerPel;
		pglTexture->Width = imageWidth;												// load up our texture information
		pglTexture->Height = imageHeight;
		pglTexture->Bpp = header.Bpp;
		if (pglTexture->Bpp == 24) {												// If the BPP of the image is 24...
			pglTexture->Type = GL_RGB;												// Set Image type to GL_RGB
		} else {																	// Else if its 32 BPP
			pglTexture->Type = GL_RGBA;												// Set image type to GL_RGBA
		}																			// Validate header info
		if ((imageType != 2 && imageType != 10) || (imageWidth == 0) || (imageHeight == 0) || (imageBytesPerPel != 3 && imageBytesPerPel != 4)) {
			// invalid header, bomb out
			fclose(fTGA);
			return(false);
		}
		pImgData = (GLubyte*)malloc(imageSize);										// Allocate the memory for the image size
		if (pImgData == NULL) {														// If no space was allocated
			fclose(fTGA);															// Close the file
			return false;															// Return failed
		}
		if (header.ImgIdent > 0) {													// Skip image ident field
			fseek(fTGA, header.ImgIdent, SEEK_CUR);
		}
		if (imageType == 2) {														// un-compresses image ?
			if (fread(pImgData, 1, imageSize, fTGA) != imageSize) {						// Attempt to read image data
				if (pImgData != NULL) {												// If imagedata has data in it
					free(pImgData);													// Delete data from memory
				}
				fclose(fTGA);														// Close file
				return false;														// Return failed
			}
			for (GLuint cswap = 0; cswap < imageSize; cswap += imageBytesPerPel) {	// Byte Swapping Optimized By Steve Thomas
				pImgData[cswap] ^= pImgData[cswap + 2] ^=
					pImgData[cswap] ^= pImgData[cswap + 2];
			}
		} else {																	// compressed image
			GLuint pixelcount = imageHeight * imageWidth;							// Nuber of pixels in the image
			GLuint currentpixel = 0;												// Current pixel being read
			GLuint currentbyte = 0;													// Current byte
			GLubyte* colorbuffer = (GLubyte*)malloc(imageBytesPerPel);				// Storage for 1 pixel
			do {
				GLubyte chunkheader = 0;											// Storage for "chunk" header
				if (fread(&chunkheader, sizeof(GLubyte), 1, fTGA) == 0) {				// Read in the 1 byte header
					if (fTGA != NULL) {												// If file is open
						fclose(fTGA);												// Close file
					}
					if (pImgData != NULL) {											// If there is stored image data
						free(pImgData);												// Delete image data
					}
					return false;													// Return failed
				}
				if (chunkheader < 128) {											// If the ehader is < 128, it means the that is the number of RAW color packets minus 1 that follow the header
					chunkheader++;													// add 1 to get number of following color values
					for (SHORT counter = 0; counter < chunkheader; counter++) {		// Read RAW color values
						if (fread(colorbuffer, 1, imageBytesPerPel, fTGA) != imageBytesPerPel) { // Try to read 1 pixel
							if (fTGA != NULL) {										// See if file is open
								fclose(fTGA);										// If so, close file
							}
							if (colorbuffer != NULL) {								// See if colorbuffer has data in it
								free(colorbuffer);									// If so, delete it
							}
							if (pImgData != NULL) {									// See if there is stored Image data
								free(pImgData);										// If so, delete it too
							}
							return false;											// Return failed
						}
						pImgData[currentbyte] = colorbuffer[2];						// Flip R and B vcolor values around in the process
						pImgData[currentbyte + 1] = colorbuffer[1];
						pImgData[currentbyte + 2] = colorbuffer[0];
						if (imageBytesPerPel == 4) {								// if its a 32 bpp image
							pImgData[currentbyte + 3] = colorbuffer[3];				// copy the 4th byte
						}
						currentbyte += imageBytesPerPel;							// Increase thecurrent byte by the number of bytes per pixel
						currentpixel++;												// Increase current pixel by 1
						if (currentpixel > pixelcount) {								// Make sure we havent read too many pixels
							if (fTGA != NULL) {										// If there is a file open
								fclose(fTGA);										// Close file
							}
							if (colorbuffer != NULL) {								// If there is data in colorbuffer
								free(colorbuffer);									// Delete it
							}
							if (pImgData != NULL) {									// If there is Image data
								free(pImgData);										// delete it
							}
							return false;											// Return failed
						}
					}
				} else {															// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
					chunkheader -= 127;												// Subtract 127 to get rid of the ID bit
					if (fread(colorbuffer, 1, imageBytesPerPel, fTGA) != imageBytesPerPel) {	// Attempt to read following color values
						if (fTGA != NULL) {											// If thereis a file open
							fclose(fTGA);											// Close it
						}
						if (colorbuffer != NULL) {									// If there is data in the colorbuffer
							free(colorbuffer);										// delete it
						}
						if (pImgData != NULL) {										// If there is image data
							free(pImgData);											// delete it
						}
						return false;												// return failed
					}
					for (short counter = 0; counter < chunkheader; counter++) {		// copy the color into the image data as many times as dictated
						pImgData[currentbyte] = colorbuffer[2];						// switch R and B bytes areound while copying
						pImgData[currentbyte + 1] = colorbuffer[1];
						pImgData[currentbyte + 2] = colorbuffer[0];
						if (imageBytesPerPel == 4) {								// If TGA images is 32 bpp
							pImgData[currentbyte + 3] = colorbuffer[3];				// Copy 4th byte
						}
						currentbyte += imageBytesPerPel;							// Increase current byte by the number of bytes per pixel
						currentpixel++;												// Increase pixel count by 1
						if (currentpixel > pixelcount) {								// Make sure we havent written too many pixels
							if (fTGA != NULL) {										// If there is a file open
								fclose(fTGA);										// Close file
							}
							if (colorbuffer != NULL) {								// If there is data in colorbuffer
								free(colorbuffer);									// Delete it
							}
							if (pImgData != NULL) {									// If there is Image data
								free(pImgData);										// delete it
							}
							return false;											// Return failed
						}
					}																// for(counter)
				}																	// if(chunkheader)
			} while (currentpixel < pixelcount);										// Loop while there are still pixels left
		}																			// if (imageType == 2)
		fclose(fTGA);																// Close the TGA file
																					// Scale Image to be a power of 2
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);							// Get Maximum Texture Size Supported
		int lWidthPixels = imageWidth;
		int lHeightPixels = imageHeight;
		// Resize Image To Closest Power Of Two
		if (lWidthPixels <= glMaxTexDim)											// Is Image Width Less Than Or Equal To Cards Limit
			lWidthPixels = 1 << (int)floor((log((double)lWidthPixels) / log(2.0f)) + 0.5f);
		else																		// Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
			lWidthPixels = glMaxTexDim;

		if (lHeightPixels <= glMaxTexDim)											// Is Image Height Greater Than Cards Limit
			lHeightPixels = 1 << (int)floor((log((double)lHeightPixels) / log(2.0f)) + 0.5f);
		else																		// Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
			lHeightPixels = glMaxTexDim;

		if ((m_fHighQualityTextures == false) && (lWidthPixels > 64)) {				// if low quality textures then make them halve the size which saved 4 times the texture space
			lWidthPixels /= 2;
			lHeightPixels /= 2;
		}
		if ((lWidthPixels != (int)imageWidth) && (lHeightPixels != (int)imageHeight)) {	// if the size needs to change, the rescale the raw image data
			MemSize = (size_t)((__int64)lWidthPixels * (__int64)lHeightPixels * (__int64)imageBytesPerPel);
			GLubyte* pNewImgData = (GLubyte*)malloc(MemSize);							// allocated the some memory for the new texture
			GLenum format;
			if (imageBytesPerPel == 4) {
				format = GL_RGBA;
			} else {
				format = GL_RGB;
			}
			gluScaleImage(format, imageWidth, imageHeight, GL_UNSIGNED_BYTE, pImgData, lWidthPixels, lHeightPixels, GL_UNSIGNED_BYTE, pNewImgData);
			free(pImgData);															// free the original image data
			pImgData = pNewImgData;													// old becomes new..
			pglTexture->Width = lWidthPixels;										// update our texture structure
			pglTexture->Height = lHeightPixels;
		}
		// Typical Texture Generation Using Data From The TGA loader
		glGenTextures(1, &pglTexture->TextureID);									// Create The Texture
		(void)GenerateTexture(pglTexture, (byte*)pImgData);							// generate the texture using the filtering model selected
		free(pImgData);																// free the memory allocated
		return true;																// All went well, continue on
	}
	//-Protected-------------------------------------------------------------------
	// Name: LoadJPG_GIFResource(CHAR* pResourceName,CHAR* pResourceType,glTexture* pglTexture)
	//-----------------------------------------------------------------------------
	int MC_OpenGLTextureLoader::LoadJPG_GIFResource(char* pResourceName, char* pResourceType, glTexture* pglTexture)
	{
		HDC			hdcTemp;														// The DC To Hold Our Bitmap
		HBITMAP		hbmpTemp;														// Holds The Bitmap Temporarily
		IPicture* pPicture = NULL;													// IPicture Interface
		long		lWidth;															// Width In Logical Units
		long		lHeight;														// Height In Logical Units
		long		lWidthPixels;													// Width In Pixels
		long		lHeightPixels;													// Height In Pixels
		GLint		glMaxTexDim;													// Holds Maximum Texture Size
		void* pData2 = NULL;
		HRESULT		hr = E_FAIL;
		RECT		PictureRect;

		HRSRC	hRes;																// from resources
		hRes = FindResourceA(NULL, pResourceName, pResourceType);
		DWORD	dwDataSize = SizeofResource(NULL, hRes);
		HGLOBAL	hGlob = LoadResource(NULL, hRes);
		LPVOID	pData = LockResource(hGlob);
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwDataSize);
		ZeroMemory(&PictureRect, sizeof(PictureRect));

		if (hGlobal) {
			pData2 = GlobalLock(hGlobal);
			if (pData2) {
				memcpy(pData2, pData, dwDataSize);
				GlobalUnlock(hGlobal);
				UnlockResource(hGlob);
				FreeResource(hGlob);
				IStream* pStream = NULL;

				if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK) {
					hr = OleLoadPicture(pStream, 0, 0, IID_IPicture, (void**)&pPicture);
					pStream->Release();
				}
			}
		}
		if (FAILED(hr))																// If Loading Failed
			return false;															// Return False

		if (pPicture == NULL)														// If Loading Failed
			return false;															// Return False

		hdcTemp = CreateCompatibleDC(GetDC(0));										// Create The Windows Compatible Device Context
		if (!hdcTemp) {																// Did Creation Fail?
			pPicture->Release();													// Decrements IPicture Reference Count
			return false;															// Return False (Failure)
		}
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);							// Get Maximum Texture Size Supported

		pPicture->get_Width(&lWidth);												// Get IPicture Width (Convert To Pixels)
		lWidthPixels = MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
		pPicture->get_Height(&lHeight);												// Get IPicture Height (Convert To Pixels)
		lHeightPixels = MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);
		// Resize Image To Closest Power Of Two
		if (lWidthPixels <= glMaxTexDim)											// Is Image Width Less Than Or Equal To Cards Limit
			lWidthPixels = 1 << (INT)floor((log((double)lWidthPixels) / log(2.0f)) + 0.5f);
		else																		// Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
			lWidthPixels = glMaxTexDim;

		if (lHeightPixels <= glMaxTexDim)											// Is Image Height Greater Than Cards Limit
			lHeightPixels = 1 << (INT)floor((log((double)lHeightPixels) / log(2.0f)) + 0.5f);
		else																		// Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
			lHeightPixels = glMaxTexDim;

		if ((m_fHighQualityTextures == false) && (lWidthPixels > 64)) {				// if low quality textures then make them halve the size which saved 4 times the texture space
			lWidthPixels /= 2;
			lHeightPixels /= 2;
		}
		// Create A Temporary Bitmap
		BITMAPINFO	bi = { 0 };														// The Type Of Bitmap We Request
		DWORD* pBits = 0;															// Pointer To The Bitmap Bits
		bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);								// Set Structure Size
		bi.bmiHeader.biBitCount = 32;												// 32 Bit
		bi.bmiHeader.biWidth = lWidthPixels;										// Power Of Two Width
		bi.bmiHeader.biHeight = lHeightPixels;										// Make Image Top Up (Positive Y-Axis)
		bi.bmiHeader.biCompression = BI_RGB;										// RGB Encoding
		bi.bmiHeader.biPlanes = 1;													// 1 Bitplane
		hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0); // Creating A Bitmap This Way Allows Us To Specify Color Depth And Gives Us Imediate Access To The Bits
		if (!hbmpTemp) {															// Did Creation Fail?
			DeleteDC(hdcTemp);														// Delete The Device Context
			pPicture->Release();													// Decrements IPicture Reference Count
			return false;															// Return False (Failure)
		}
		SelectObject(hdcTemp, hbmpTemp);											// Select Handle To Our Temp DC And Our Temp Bitmap Object
		pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, &PictureRect); // Render The IPicture On To The Bitmap
		if ((m_fAlphaConversion == TRUE) && (pglTexture->TexType != eglTexType::txJpg)) {	// Convert From BGR To RGB Format And Add An Alpha Value Of 255
			// it is important to remember that the colour order is different as we are reading in a DWORD so the intel processor will read it in as little endian so the order is reversed (ABGR)
			DWORD AlphaMatch = (((m_RedAlphaMatch) | (m_GreenAlphaMatch << 8)) | (m_BlueAlphaMatch << 16));
			DWORD* pRGBA = (DWORD*)pBits;
			long NumPixels = lWidthPixels * lHeightPixels;
			while (NumPixels--) {													// Loop Through All Of The Pixels
				DWORD	tempRGB;
				tempRGB = *pRGBA;
				tempRGB = ((tempRGB & 0x00FF0000) >> 16) | (tempRGB & 0x0000FF00) | ((tempRGB & 0x000000FF) << 16);	// swap red and blue over
				if (AlphaMatch != tempRGB) {
					tempRGB |= 0xFF000000;
				}
				*pRGBA++ = tempRGB;
			}
		} else {
			byte* pPixel = (BYTE*)pBits;											// Grab The Current Pixel
			long NumPixels = lWidthPixels * lHeightPixels;
			while (NumPixels--) {
				BYTE  temp = pPixel[0];												// Store 1st Color In Temp Variable (Blue)
				pPixel[0] = pPixel[2];												// Move Red Value To Correct Position (1st)
				pPixel[2] = temp;													// Move Temp Value To Correct Blue Position (3rd)
				pPixel[3] = 255;													// Set The Alpha Value To 255
				pPixel += 4;
			}
		}
		pglTexture->Width = bi.bmiHeader.biWidth;
		pglTexture->Height = bi.bmiHeader.biHeight;
		pglTexture->Bpp = bi.bmiHeader.biBitCount;									// Image Color Depth In Bits Per Pixel
		pglTexture->Type = GL_RGBA;
		glGenTextures(1, &pglTexture->TextureID);									// Create The Texture
		(void)GenerateTexture(pglTexture, (BYTE*)pBits);							// generate the texture using the filtering model selected
		DeleteObject(hbmpTemp);														// Delete The Object
		DeleteDC(hdcTemp);															// Delete The Device Context
		pPicture->Release();														// Decrements IPicture Reference Count
		return true;																// Return True (All Good)
	}
	//-Protected-------------------------------------------------------------------
	// Name: LoadTGAResource(char* pResourceName,char* pResourceType,glTexture* pglTexture)
	//-----------------------------------------------------------------------------
	int	MC_OpenGLTextureLoader::LoadTGAResource(char* pResourceName, char* pResourceType, glTexture* pglTexture)
	{
		HRSRC	hRes;
		DWORD	dwDataSize;
		HGLOBAL	hGlob;
		LPVOID	pData;
		HGLOBAL hGlobal;
		void* pData2 = NULL;
		int		rc = 0;

		hRes = FindResourceA(NULL, pResourceName, pResourceType);					// from resources
		if (hRes) {
			dwDataSize = SizeofResource(NULL, hRes);
			hGlob = LoadResource(NULL, hRes);
			if (hGlob) {
				pData = LockResource(hGlob);
				hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwDataSize);					// allocate some global memory and copy the resource data into it..
				if (hGlobal) {
					pData2 = GlobalLock(hGlobal);
					if (pData2 != NULL) {
						memcpy(pData2, pData, dwDataSize);
						GlobalUnlock(hGlobal);										// unlock and free the resource
						UnlockResource(hGlob);
						FreeResource(hGlob);
						rc = LoadTextureFromRam((BYTE*)pData2, dwDataSize, eglTexType::txTga);	// load it
						GlobalFree(hGlobal);										// free our global memory (16 bit os's only)
					}
				}
			}
		}
		return rc;
	}
	//-Protected-------------------------------------------------------------------
	// Name: GenerateTexture(glTexture* pglTexture,GLubyte* pImgData)
	//-----------------------------------------------------------------------------
	int MC_OpenGLTextureLoader::GenerateTexture(glTexture* pglTexture, GLubyte* pImgData)
	{
		int result = 0;
		int	components = 0;

		if (pglTexture->Type == GL_RGBA) {											// set the bytes per pixel
			components = 4;
		} else {
			components = 3;
		}
		glBindTexture(GL_TEXTURE_2D, pglTexture->TextureID);						// Bind To The Texture ID Typical Texture Generation Using Data From The Bitmap
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		BOOL	Mipping;
		switch (m_TextureFilterType) {
			default:
			case eglTexFilterType::txNoFilter:
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				Mipping = false;
				break;
			case eglTexFilterType::txBilinear:
				if (m_fMipMapping == FALSE) {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					Mipping = false;												// set the build type flag
				}
				else {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
					Mipping = true;
				}
				break;
			case eglTexFilterType::txTrilinear:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				Mipping = true;														// always mip mapping for trilinear
				break;
		}
		// crank out the texture
		if (Mipping == false) {
			glTexImage2D(GL_TEXTURE_2D, 0, components, pglTexture->Width, pglTexture->Height, 0, pglTexture->Type, GL_UNSIGNED_BYTE, pImgData);
		} else {
			// Build Mipmaps (builds different versions of the picture for distances - looks better)
			result = gluBuild2DMipmaps(GL_TEXTURE_2D, components, pglTexture->Width, pglTexture->Height, pglTexture->Type, GL_UNSIGNED_BYTE, pImgData);
		}
		return 0;
	}
	//-Protected-------------------------------------------------------------------
	// Name: ExtensionFromFilename(char* szFileName,char* szExtension)
	//-----------------------------------------------------------------------------
	void MC_OpenGLTextureLoader::ExtensionFromFilename(char* szFileName, char* szExtension)
	{
		int len = lstrlenA(szFileName);
		int begin = 0;

		for (begin = len; begin >= 0; begin--) {
			if (szFileName[begin] == '.') {
				begin++;
				break;
			}
		}
		if (begin <= 0) {
			szExtension[0] = '\0';
		} else {
			lstrcpyA(szExtension, &szFileName[begin]);
		}
	}
};