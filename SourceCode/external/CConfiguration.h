#ifndef CCONFIGURATION_H_INCLUDE
#define CCONFIGURATION_H_INCLUDE

#include "external/CValue.h"

class CConfingation
{
    BEATS_DECLARE_SINGLETON(CConfingation);

public:

    int GetMaxTextureSize() const;

    int GetMaxModelviewStackDepth() const;

    int GetMaxTextureUnits() const;

    bool SupportsNPOT() const;

    bool SupportsPVRTC() const;

    bool SupportsETC() const;

    bool SupportsS3TC() const;

    bool SupportsATITC() const;

    bool SupportsBGRA8888() const;

    bool SupportsDiscardFramebuffer() const;

    bool SupportsShareableVAO() const;

    bool CheckForGLExtension( const TString& searchName ) const;

    bool Init();

    /** returns the value of a given key as a double */
    const CValue& GetValue(const TString& key, const CValue& defaultValue = CValue::Null) const;

    /** sets a new key/value pair  in the configuration dictionary */
    void SetValue(const TString& key, const CValue& value);

    const TString& GetInfo();

    void GatherGPUInfo();


protected:
    GLint   m_maxTextureSize;
    GLint   m_maxModeviewStackDepth;
    bool    m_supportsPVRTC;
    bool    m_supportsETC1;
    bool    m_supportsS3TC;
    bool    m_supportsATITC;
    bool    m_supportsNPOT;
    bool    m_supportsBGRA8888;
    bool    m_supportsDiscardFramebuffer;
    bool    m_supportsShareableVAO;
    GLint   m_maxSamplesAllowed;
    GLint   m_maxTextureUnits;
    char*   m_glExtensions;

    TValueMap m_valueDict;
    TString m_strInfo;
};

#endif//CCONFIGURATION_H_INCLUDE