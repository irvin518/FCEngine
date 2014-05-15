#ifndef TEST_COMPONENT_TESECOMPONENT_H__INCLUDE
#define TEST_COMPONENT_TESECOMPONENT_H__INCLUDE

#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"
#include "ParticlesSystem/Emitter.h"

class CTestAbstractClass : public CComponentBase
{
    DECLARE_REFLECT_GUID_ABSTRACT(CTestAbstractClass, 0x1A7BB177, CComponentBase)
public:
    CTestAbstractClass(){};
    ~CTestAbstractClass(){};

    virtual void ReflectData(CSerializer& serializer) override
    {DECLARE_PROPERTY(serializer, a, true, 0xFFFFFFFF, _T("int"), NULL, NULL, NULL);};
protected:
    virtual void fun() = 0;
private:
    int a;
};

class CTestComponent : public CComponentBase
{
    DECLARE_REFLECT_GUID(CTestComponent, 0x7B1A17B7, CComponentBase)
public:
    enum ETestEnum
    {
        eTE_ENUM_1,
        eTE_ENUM_2,
        eTE_ENUM_3,
        eTE_ENUM_4
    };

public:
    CTestComponent();
    ~CTestComponent();
    virtual void ReflectData(CSerializer& serializer) override;

private:
    ETestEnum   m_enum;
    bool        m_bool;
    TString     m_str;
    CColor      m_colour;
    double      m_double;
    int         m_int;
    UINT        m_uint;
    float       m_float;
    kmVec2      m_vec2;
    kmVec3      m_vec3;
    kmVec4      m_vec4;
    std::map<int, int>     m_map;
    std::vector<int>          m_vector;
    FCEngine::CParticleEmitter* m_ptr;
    CTestAbstractClass*      m_abstractptr;
};

#endif