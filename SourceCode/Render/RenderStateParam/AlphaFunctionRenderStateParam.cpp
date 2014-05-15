#include "stdafx.h"
#include "AlphaFunctionRenderStateParam.h"
#include "render/Renderer.h"

CAlphaFunctionRenderParam::CAlphaFunctionRenderParam()
{
    SetType(eFSP_AlphaFunction);
}

CAlphaFunctionRenderParam::~CAlphaFunctionRenderParam()
{

}

void CAlphaFunctionRenderParam::Apply()
{
    BEATS_ASSERT( GetType() == eFSP_AlphaFunction );
    CRenderer::GetInstance()->AlphaFunc( m_func, m_ref );
}

void CAlphaFunctionRenderParam::ReflectData( CSerializer& serializer )
{
    DECLARE_PROPERTY(serializer, m_func, true, 0xFFFFFFFF, _T("alpha比较函数"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_ref, true, 0xFFFFFFFF, _T("alpha参考值"), NULL, NULL, NULL);
}

void CAlphaFunctionRenderParam::SetFunc( EFunctionType func )
{
    m_func = func;
}

void CAlphaFunctionRenderParam::SetRef( float ref )
{
    m_ref = ref;
}

float CAlphaFunctionRenderParam::GetRef() const
{
    return m_ref;
}

CFunctionRenderStateParam::EFunctionType CAlphaFunctionRenderParam::GetFunc() const
{
     return m_func;
}
