#include "stdafx.h"
#include "CConfiguration.h"
#include "PlatformConfig.h"

CConfingation* CConfingation::m_pInstance = NULL;

CConfingation::CConfingation()
{
    m_maxTextureSize = 0;
    m_maxModeviewStackDepth = 0;
    m_supportsPVRTC = false;
    m_supportsETC1 = false;
    m_supportsS3TC = false;
    m_supportsATITC = false;
    m_supportsNPOT = false;
    m_supportsBGRA8888 = false;
    m_supportsDiscardFramebuffer = false;
    m_supportsShareableVAO = false;
    m_maxSamplesAllowed = 0;
    m_maxTextureUnits = 0;
    m_glExtensions = NULL;
}

CConfingation::~CConfingation()
{
    m_valueDict.clear();
}

int CConfingation::GetMaxTextureSize() const
{
    return m_maxTextureSize;
}

int CConfingation::GetMaxModelviewStackDepth() const
{
    return m_maxModeviewStackDepth;
}

int CConfingation::GetMaxTextureUnits() const
{
    return m_maxTextureUnits;
}

bool CConfingation::SupportsNPOT() const
{
    return m_supportsNPOT;
}

bool CConfingation::SupportsPVRTC() const
{
    return m_supportsPVRTC;
}

bool CConfingation::SupportsETC() const
{
    //GL_ETC1_RGB8_OES is not defined in old opengl version
#ifdef GL_ETC1_RGB8_OES
    return _supportsETC1;
#else
    return false;
#endif
}

bool CConfingation::SupportsS3TC() const
{
    return m_supportsS3TC;
}

bool CConfingation::SupportsATITC() const
{
    return m_supportsATITC;
}

bool CConfingation::SupportsBGRA8888() const
{
    return m_supportsBGRA8888;
}

bool CConfingation::SupportsDiscardFramebuffer() const
{
    return m_supportsDiscardFramebuffer;
}

bool CConfingation::SupportsShareableVAO() const
{
    return m_supportsShareableVAO;
}

bool CConfingation::CheckForGLExtension( const TString& searchName ) const
{
    bool bReturn = false;
    char name[ MAX_PATH ];
    CStringHelper::GetInstance()->ConvertToCHAR( searchName.c_str(), name, searchName.length() );
    bReturn = (m_glExtensions && strstr(m_glExtensions, name) ) ? true : false;
    return bReturn;
}

bool CConfingation::Init()
{
    return true;
}

const CValue& CConfingation::GetValue( const TString& key, const CValue& defaultValue /*= CValue::Null*/ ) const
{
    auto iter = m_valueDict.find(key);
    if (iter != m_valueDict.cend())
        return m_valueDict.at(key);
    return defaultValue;
}

void CConfingation::SetValue( const TString& key, const CValue& value )
{
    m_valueDict[key] = value;
}

const TString& CConfingation::GetInfo()
{
    // Dump
    CValue forDump = CValue(m_valueDict);
    m_strInfo = forDump.GetDescription();
    return m_strInfo;
}

void CConfingation::GatherGPUInfo()
{
    m_valueDict[_T("gl.vendor")] = CValue((const char*)glGetString(GL_VENDOR));
    m_valueDict[_T("gl.renderer")] = CValue((const char*)glGetString(GL_RENDERER));
    m_valueDict[_T("gl.version")] = CValue((const char*)glGetString(GL_VERSION));

    m_glExtensions = (char*)glGetString(GL_EXTENSIONS);
    
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxTextureSize);
    m_valueDict[_T("gl.max_texture_size")] = CValue((int)m_maxTextureSize);

    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_maxTextureUnits);
    m_valueDict[_T("gl.max_texture_units")] = CValue((int)m_maxTextureUnits);

#if (FC_PLATFOEM == PLATFORM_IOS)
    glGetIntegerv(GL_MAX_SAMPLES_APPLE, &m_maxSamplesAllowed);
    m_valueDict[_T("gl.max_samples_allowed")] = CValue((int)m_maxSamplesAllowed);
#endif

    m_supportsETC1 = CheckForGLExtension(_T("GL_OES_compressed_ETC1_RGB8_texture"));
    m_valueDict[_T("gl.supports_ETC1")] = CValue(m_supportsETC1);

    m_supportsS3TC = CheckForGLExtension(_T("GL_EXT_texture_compression_s3tc"));
    m_valueDict[_T("gl.supports_S3TC")] = CValue(m_supportsS3TC);

    m_supportsATITC = CheckForGLExtension(_T("GL_AMD_compressed_ATC_texture"));
    m_valueDict[_T("gl.supports_ATITC")] = CValue(m_supportsATITC);

    m_supportsPVRTC = CheckForGLExtension(_T("GL_IMG_texture_compression_pvrtc"));
    m_valueDict[_T("gl.supports_PVRTC")] = CValue(m_supportsPVRTC);

    m_supportsNPOT = true;
    m_valueDict[_T("gl.supports_NPOT")] = CValue(m_supportsNPOT);

    m_supportsBGRA8888 = CheckForGLExtension(_T("GL_IMG_texture_format_BGRA888"));
    m_valueDict[_T("gl.supports_BGRA8888")] = CValue(m_supportsBGRA8888);

    m_supportsDiscardFramebuffer = CheckForGLExtension(_T("GL_EXT_discard_framebuffer"));
    m_valueDict[_T("gl.supports_discard_framebuffer")] = CValue(m_supportsDiscardFramebuffer);

    m_supportsShareableVAO = CheckForGLExtension(_T("vertex_array_object"));
    m_valueDict[_T("gl.supports_vertex_array_object")] = CValue(m_supportsShareableVAO);

    FC_CHECK_GL_ERROR_DEBUG();
}
