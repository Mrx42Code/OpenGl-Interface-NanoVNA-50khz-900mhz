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
// File: MC_OpenGLTextureLoader.h: interface for the MC_OpenGLTextureLoader class.
// Desc: Application interface for the MC_OpenGLTextureLoader class.
//-----------------------------------------------------------------------------
#ifndef MC_OpenGLTextureLoader_H
	#define MC_OpenGLTextureLoader_H

#pragma once

#include <gl\gl.h>
#include <gl\glu.h>

namespace NanoVNA {
	//-----------------------------------------------------------------------------
	// Const
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	// Struct
	//-----------------------------------------------------------------------------
	typedef enum class MyeglTexType : unsigned char {
		txUnknown			= 0,													// images
		txBmp				= 1,
		txJpg				= 2,
		txPng				= 3,
		txTga				= 4,
		txGif				= 5,
		txIco				= 6,
		txEmf				= 7,
		txWmf				= 8,
	}eglTexType;

	typedef enum class MyeglTexFilterType : unsigned char {
		txNoFilter			= 0,
		txBilinear			= 1,
		txTrilinear			= 2,
	}eglTexFilterType;

	typedef	struct
	{
		GLuint				TextureID;												// Texture ID Used To Select A Texture
		char				TextureFileName[MAX_PATH];								// Texture FileName
		eglTexType			TexType;												// Texture Format
		GLuint				Width;													// Image Width
		GLuint				Height;													// Image Height
		GLuint				Type;													// Image Type (GL_RGB, GL_RGBA)
		GLuint				Bpp;													// Image Color Depth In Bits Per Pixel
	}glTexture;

	typedef struct {
		float				s;
		float				t;
	}_glTexturCord;

	typedef struct {
		_glTexturCord		TopRight;
		_glTexturCord		TopLeft;
		_glTexturCord		BottomLeft;
		_glTexturCord		BottomRight;
	}glTexturCordTable;

	typedef struct {
		BYTE				ImgIdent;
		BYTE				ignored[1];
		BYTE				ImgType;
		BYTE				ignored2[9];
		BYTE				WidthLo;
		BYTE				WidthHi;
		BYTE				HeightLo;
		BYTE				HeightHi;
		BYTE				Bpp;
		BYTE				ignored3[1];
	}_TGAHeader;

	//-----------------------------------------------------------------------------
	// Name: class MC_OpenGLTextureLoader
	// Desc: Application class
	//-----------------------------------------------------------------------------
	class MC_OpenGLTextureLoader
	{

		public:
			glTexture			m_Texture;

		protected:
			GLboolean			m_fAlphaConversion;
			GLboolean			m_fHighQualityTextures;
			GLboolean			m_fMipMapping;
			eglTexFilterType	m_TextureFilterType;
			GLubyte				m_RedAlphaMatch;
			GLubyte				m_GreenAlphaMatch;
			GLubyte 			m_BlueAlphaMatch;
			glTexture*			m_pglTexture;

		private:

			//-----------------------------------------------------------------------------

		public:
								MC_OpenGLTextureLoader(void);
			virtual				~MC_OpenGLTextureLoader(void);
			void				SetAlphaMatch(GLboolean fEnabled, GLubyte RedAlphaMatch, GLubyte GreenAlphaMatch, GLubyte BlueAlphaMatch);
			void				SetHighQualityTextures(GLboolean fEnabled);
			void				SetMipMapping(GLboolean fEnabled);
			void				SetTextureFilter(eglTexFilterType type);
			int					LoadTextureFromDisk(std::string _szFileName);
			int					LoadTextureFromRam(BYTE* pData, int Length, eglTexType TexType);
			int					LoadTextureFromResource(UINT ResourceName, char* pResourceType, eglTexType TexType);
			void				FreeTexture(void);
			void				BuildColorTexture(BYTE r, BYTE g, BYTE b);

		private:
			int					BuildTexture(char* szPathName, glTexture* pglTexture);
			int					LoadTGAFromDisk(char* pszFileName, glTexture* pglTexture);
			int					LoadJPG_GIFResource(char* pResourceName, char* pResourceType, glTexture* pglTexture);
			int					LoadTGAResource(char* pResourceName, char* pResourceType, glTexture* pglTexture);
			int					GenerateTexture(glTexture* pglTexture, GLubyte* pImgData);
			void				ExtensionFromFilename(char* szFileName, char* szExtension);

		private:

	};
};
//-----------------------------------------------------------------------------
#endif // MC_OpenGLTextureLoader_H
