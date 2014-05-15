#include "stdafx.h"
#include "Texture.h"
#include "Utility/BeatsUtility/StringHelper.h"
#include "external/CConfiguration.h"
#include "TextureFormatConverter.h"
#include "Renderer.h"

static PixelFormat g_defaultAlphaPixelFormat = PixelFormat::DEFAULT;

CTexture::CTexture()
: m_bPremultipliedAlpha(false)
, m_iWidth(0)
, m_iHeight(0)
, m_uMipmapCount(0)
, m_uId(0)
, m_pixelFormat(PixelFormat::NONE)
{
}

CTexture::~CTexture()
{
    m_uId = 0;
}

GLuint CTexture::ID() const
{
    return m_uId;
}

bool CTexture::Load()
{
    bool bRet = false;

    GLint currTex;
    CRenderer::GetInstance()->GetIntegerV(GL_TEXTURE_2D, &currTex);
    BEATS_ASSERT(!IsLoaded(), _T("Can't Load a texture which is already loaded!"));
    CImage* pImage = new CImage;
    bRet = pImage->InitWithImageFile(GetFilePath());
    BEATS_ASSERT(bRet, _T("Load file %s failed!"), GetFilePath().c_str());

    if (bRet)
    {
        int imageWidth = pImage->GetWidth();
        int imageHeight = pImage->GetHeight();

#ifdef _DEBUG
        int maxTextureSize = CConfingation::GetInstance()->GetMaxTextureSize();
        BEATS_ASSERT(imageWidth <= maxTextureSize &&  imageHeight <= maxTextureSize, 
            _T("WARNING: Image (%u x %u) is bigger than the supported %u x %u"), 
            imageWidth, imageHeight, maxTextureSize, maxTextureSize);
#endif

        const unsigned char*   tempData = pImage->GetData();
        PixelFormat pixelFormat = PixelFormat::NONE;
        PixelFormat renderFormat = (PixelFormat)pImage->GetRenderFormat();
        size_t tempDataLen = pImage->GetDataLength();
        PixelFormat format = PixelFormat::NONE;

        if (pImage->GetNumberOfMipmaps() > 1)
        {
            BEATS_WARNING(format == PixelFormat::NONE, _T("WARNING: This image has more than 1 mipmaps and we will not convert the data format"));
            InitWithMipmaps(pImage->GetMipmaps(), pImage->GetNumberOfMipmaps(), (PixelFormat)pImage->GetRenderFormat(), imageWidth, imageHeight);
            bRet = true;
        }
        else if (pImage->IsCompressed())
        {
            BEATS_WARNING(format == PixelFormat::NONE, _T("WARNING: This image is compressed and we cann't convert it for now"));
            InitWithData(tempData, tempDataLen, (PixelFormat)pImage->GetRenderFormat(), imageWidth, imageHeight);
            bRet = true;
        }
        else
        {
            pixelFormat = format != PixelFormat::NONE ? pixelFormat : g_defaultAlphaPixelFormat;
            unsigned char* outTempData = nullptr;
            ssize_t outTempDataLen = 0;
            pixelFormat = ConvertDataToFormat(tempData, tempDataLen, renderFormat, pixelFormat, &outTempData, &outTempDataLen);
            InitWithData(outTempData, outTempDataLen, pixelFormat, imageWidth, imageHeight);
            if (outTempData != nullptr && outTempData != tempData)
            {
                BEATS_SAFE_DELETE_ARRAY(outTempData);
            }

            // set the premultiplied tag
            if (!pImage->HasPremultipliedAlpha())
            {
                m_bPremultipliedAlpha = pImage->GetFileType() != CImage::EFormat::eF_PVR;
                BEATS_WARNING( !m_bPremultipliedAlpha, _T("wanning: We cann't find the data is premultiplied or not, we will assume it's false."));
            }
            else
            {
                m_bPremultipliedAlpha = pImage->IsPremultipliedAlpha();
            }
            BEATS_SAFE_DELETE(pImage);
            bRet = true;
        }
    }
    CRenderer::GetInstance()->BindTexture(GL_TEXTURE_2D, currTex);
    SetLoadedFlag(bRet);
    return bRet;
}

bool CTexture::Unload()
{
    bool bRet = IsLoaded();
    if (bRet)
    {
        CRenderer::GetInstance()->DelTexture(1, &m_uId);
    }
    SetLoadedFlag(false);
    return bRet;
}

bool CTexture::InitWithMipmaps( const SMipmapInfo* mipmaps, int mipmapsNum, PixelFormat pixelFormat, int pixelsWide, int pixelsHigh )
{
    bool bRet = false;

    //the pixelFormat must be a certain value 
    BEATS_ASSERT(pixelFormat != PixelFormat::NONE && pixelFormat != PixelFormat::AUTO, _T("the \"pixelFormat\" param must be a certain value!"));
    BEATS_ASSERT(pixelsWide>0 && pixelsHigh>0, _T("Invalid size"));
    BEATS_ASSERT(mipmapsNum > 0, _T("mipmap number is less than 1"));

    if(CImage::GetPixelFormatInfoMap().find(pixelFormat) == CImage::GetPixelFormatInfoMap().end())
    {
        BEATS_WARNING( false, _T("WARNING: unsupported pixelformat: %lx"), (unsigned long)pixelFormat );
    }
    else
    {
        const PixelFormatInfo& info = CImage::GetPixelFormatInfoMap().at(pixelFormat);

        if (info.compressed && !CConfingation::GetInstance()->SupportsPVRTC()
            && !CConfingation::GetInstance()->SupportsETC()
            && !CConfingation::GetInstance()->SupportsS3TC()
            && !CConfingation::GetInstance()->SupportsATITC())
        {
            BEATS_WARNING( false, _T("WARNING: PVRTC/ETC images are not supported"));
        }
        else
        {
            //Set the row align only when mipmapsNum == 1 and the data is uncompressed
            CRenderer* pRenderer = CRenderer::GetInstance();
            unsigned int packValue = 1;
            if (mipmapsNum == 1 && !info.compressed)
            {
                unsigned int bytesPerRow = pixelsWide * info.bpp / 8;
                if(bytesPerRow % 8 == 0)
                {
                    packValue = 8;
                }
                else if(bytesPerRow % 4 == 0)
                {
                    packValue = 4;
                }
                else if(bytesPerRow % 2 == 0)
                {
                    packValue = 2;
                }
            }
            pRenderer->PixelStorei(GL_UNPACK_ALIGNMENT, packValue);

            pRenderer->GenTextures(1, &m_uId);
            pRenderer->BindTexture(GL_TEXTURE_2D, m_uId);

            if (mipmapsNum == 1)
            {
                pRenderer->TexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            }
            else
            {
                pRenderer->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            }

            pRenderer->TexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            pRenderer->TexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
            pRenderer->TexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

            FC_CHECK_GL_ERROR_DEBUG(); // clean possible GL error

            // Specify OpenGL texture image
            int width = pixelsWide;
            int height = pixelsHigh;

            for (int i = 0; i < mipmapsNum; ++i)
            {
                unsigned char *data = mipmaps[i].address;
                GLsizei datalen = mipmaps[i].len;

                if (info.compressed)
                {
                    pRenderer->CompressedTexImage2D(GL_TEXTURE_2D, i, info.internalFormat, (GLsizei)width, (GLsizei)height, 0, datalen, data);
                }
                else
                {
                    pRenderer->TextureImage2D(GL_TEXTURE_2D, i, info.internalFormat, (GLsizei)width, (GLsizei)height, 0, info.format, info.type, data);
                }

                BEATS_ASSERT(!(i > 0 && (width != height || CImage::CcNextPOT(width) != width )), 
                                    _T("CTexture. WARNING. Mipmap level %u is not squared. Texture won't render correctly. width=%d != height=%d"), i, width, height);
                FC_CHECK_GL_ERROR_DEBUG();

                width = MAX(width >> 1, 1);
                height = MAX(height >> 1, 1);
            }

            m_iWidth = pixelsWide;
            m_iHeight = pixelsHigh;
            m_pixelFormat = pixelFormat;

            m_bPremultipliedAlpha = false;
            m_uMipmapCount = mipmapsNum;
        }
    }
    return bRet;
}

bool CTexture::InitWithData(const void *data, ssize_t dataLen, PixelFormat pixelFormat, int pixelsWide, int pixelsHigh)
{
    BEATS_ASSERT(pixelsWide > 0 && pixelsHigh > 0, _T("Invalid size"));

    //if data has no mipmaps, we will consider it has only one mipmap
    SMipmapInfo mipmap;
    mipmap.address = (unsigned char*)data;
    mipmap.len = static_cast<int>(dataLen);
    return InitWithMipmaps(&mipmap, 1, pixelFormat, pixelsWide, pixelsHigh);
}

bool CTexture::UpdateSubImage( GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, const GLvoid *data )
{
    const PixelFormatInfo& info = CImage::GetPixelFormatInfoMap().at(m_pixelFormat);
    CRenderer* pRenderer = CRenderer::GetInstance();
    pRenderer->BindTexture(GL_TEXTURE_2D, m_uId);
    pRenderer->TextureSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, info.format, info.type, data);
    pRenderer->BindTexture(GL_TEXTURE_2D, 0);
    return true;
}

PixelFormat CTexture::ConvertDataToFormat(const unsigned char* data, ssize_t dataLen, PixelFormat originFormat, PixelFormat format, unsigned char** outData, ssize_t* outDataLen)
{
    CTextureFormatConverter* pFormatConverter = CTextureFormatConverter::GetInstance();
    switch (originFormat)
    {
    case PixelFormat::I8:
        return pFormatConverter->convertI8ToFormat(data, dataLen, format, outData, outDataLen);
    case PixelFormat::AI88:
        return pFormatConverter->convertAI88ToFormat(data, dataLen, format, outData, outDataLen);
    case PixelFormat::RGB888:
        return pFormatConverter->convertRGB888ToFormat(data, dataLen, format, outData, outDataLen);
    case PixelFormat::RGBA8888:
        return pFormatConverter->convertRGBA8888ToFormat(data, dataLen, format, outData, outDataLen);
    default:
        BEATS_WARNING(false, _T("unsupport convert for format %d to format %d"), originFormat, format);
        *outData = (unsigned char*)data;
        *outDataLen = dataLen;
        return originFormat;
    }
}

int CTexture::Width() const
{
    return m_iWidth;
}

int CTexture::Height() const
{
    return m_iHeight;
}

bool CTexture::operator==( const CTexture& other )
{
    return m_uId == other.m_uId;
}

bool CTexture::operator!=( const CTexture& other )
{
    return m_uId != other.m_uId;
}
