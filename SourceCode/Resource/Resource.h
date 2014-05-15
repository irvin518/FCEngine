#ifndef RESOURCE_RESOURCE_H__INCLUDE
#define RESOURCE_RESOURCE_H__INCLUDE

#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"
#include "Utility/BeatsUtility/ComponentSystem/ComponentPublic.h"

#define DECLARE_RESOURCE_TYPE(type)\
    public:\
    virtual EResourceType GetType(){return type;}\
    static const EResourceType RESOURCE_TYPE = type;

class CResource : public CComponentBase
{
    typedef CComponentBase super;
    DECLARE_REFLECT_GUID_ABSTRACT(CResource, 0x480AD1CB, CComponentBase)
public:
    CResource();
    virtual ~CResource();

    virtual void ReflectData(CSerializer& serializer) override;
    const TString& GetFilePath() const;
    void SetFilePath(const TString& str);
    
    bool IsLoaded() const;
    void SetLoadedFlag(bool bFlag);

    virtual EResourceType GetType() = 0;
    virtual bool Load() = 0;
    virtual bool Unload() = 0;
     
private:
    bool m_bIsLoaded;
    SReflectFilePath m_strPath;
};

#endif