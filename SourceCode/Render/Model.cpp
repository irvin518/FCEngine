#include "stdafx.h"
#include "Model.h"
#include "Skin.h"
#include "Skeleton.h"
#include "SkeletonBone.h"
#include "Texture.h"
#include "AnimationController.h"
#include "ShaderProgram.h"
#include "RenderManager.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "Resource/ResourceManager.h"
#include "Renderer.h"
#include "Material.h"
#include "RenderState.h"
#include "Shader.h"
#include "RenderObjectManager.h"

#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
                ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))

CModel::CModel()
    : m_pAnimationController(NULL)
    , m_bRenderSkeleton(true)
    , m_bPlayAnimation(false)
    , m_bInit(false)
{
    m_strSkeleton.m_value = _T("org.ske");
    m_strSkin.m_value = _T("org.skin");
    m_strSelAnimtaionName = _T("org.ani");
    SReflectFilePath filePath;
    filePath.m_value = _T("org.ani");
    m_animationNamePathList.push_back(filePath);

    for (auto &item : m_animationNamePathList)
    {
        m_animationNameList.push_back(item.m_value);
    }
}

CModel::~CModel()
{
    CRenderObjectManager::GetInstance()->DeleteModel(this);
    CAnimationManager::GetInstance()->DeleteController(m_pAnimationController);
}

void CModel::ReflectData(CSerializer& serializer)
{
    super::ReflectData(serializer);
    DECLARE_PROPERTY(serializer, m_pMaterial, true, 0xFFFFFFFF, _T("材质"), NULL, NULL, NULL);

    DECLARE_PROPERTY(serializer, m_animationNamePathList, true, 0xFFFFFFFF, _T("动画文件列表"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strSkeleton, true, 0xFFFFFFFF, _T("骨骼资源名称"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strSkin, true, 0xFFFFFFFF, _T("蒙皮资源名称"), NULL, NULL, NULL);
    DECLARE_PROPERTY(serializer, m_strSelAnimtaionName, true, 0xFFFFFFFF, _T("动画名称"), NULL, NULL, GEN_ENUM_PARAM(m_animationNameList, NULL));
    DECLARE_PROPERTY(serializer, m_bPlayAnimation, true, 0xFFFFFFFF, _T("是否显示并播放动画"), NULL, NULL, NULL);

}

bool CModel::OnPropertyChange(void* pVariableAddr, void* pNewValueToBeSet)
{
    bool bRet = super::OnPropertyChange(pVariableAddr, pNewValueToBeSet);
    if (!bRet)
    {
        if(pVariableAddr == &m_bPlayAnimation)
        {
            DeserializeVarialble(m_bPlayAnimation, (CSerializer*) pNewValueToBeSet);
            if(m_bPlayAnimation)
            {
                if(!m_bInit)
                {
                    Init();
                }
                m_pSkin =  CResourceManager::GetInstance()->GetResource<CSkin>(m_strSkin.m_value, false);
                SharePtr<CTexture> pTestTexture = CResourceManager::GetInstance()->GetResource<CTexture>(_T("TestTexture.tga"), false);
                AddTexture(pTestTexture);
                m_pSkeleton =  CResourceManager::GetInstance()->GetResource<CSkeleton>(m_strSkeleton.m_value, false);
                CRenderObjectManager::GetInstance()->AddModel(this);
                m_pAnimationController->SetSkeleton(m_pSkeleton);
                SharePtr<CAnimation> pAnimation = CResourceManager::GetInstance()->GetResource<CAnimation>(m_strSelAnimtaionName, false);
                m_pAnimationController->PlayAnimation(pAnimation, 0, true);
            }
            else
            {
                if(m_bInit && m_pAnimationController->IsPlaying())
                    m_pAnimationController->Stop();
            }
        }
        else if(pVariableAddr == &m_strSkin)
        {
            DeserializeVarialble(m_strSkin, (CSerializer*) pNewValueToBeSet);
            m_pSkin =  CResourceManager::GetInstance()->GetResource<CSkin>(m_strSkin.m_value, false);
        }
        else if(pVariableAddr == &m_strSkeleton)
        {
            DeserializeVarialble(m_strSkeleton, (CSerializer*) pNewValueToBeSet);
            m_pSkeleton =  CResourceManager::GetInstance()->GetResource<CSkeleton>(m_strSkeleton.m_value, false);
            m_pAnimationController->SetSkeleton(m_pSkeleton);
        }
        else if(pVariableAddr == &m_strSelAnimtaionName)
        {
            DeserializeVarialble(m_strSelAnimtaionName,(CSerializer*) pNewValueToBeSet);
            if(m_bPlayAnimation)
            {
                SharePtr<CAnimation> pAnimation = CResourceManager::GetInstance()->GetResource<CAnimation>(m_strSelAnimtaionName, false);
                m_pAnimationController->PlayAnimation(pAnimation, 0, true);
            }
        }
        else if(pVariableAddr == &m_animationNamePathList)
        {
            DeserializeVarialble(m_animationNamePathList, (CSerializer*) pNewValueToBeSet);
            if(!m_animationNamePathList.empty())
            {
                CComponentEditorProxy* pProxy = static_cast<CComponentEditorProxy*>(CComponentProxyManager::GetInstance()->GetComponentInstance(this->GetId(), this->GetGuid()));
                BEATS_ASSERT(pProxy != NULL, _T("Can't get proxy with id: %d guid:0x%x"), GetId(), GetGuid());
                CPropertyDescriptionBase* pProperty = pProxy->GetPropertyDescription(GET_VAR_NAME(m_strSelAnimtaionName));
                BEATS_ASSERT(pProperty != NULL, _T("Get property %s failed!"), GET_VAR_NAME(m_strSelAnimtaionName));

                m_animationNameList.clear();
                for (auto &item : m_animationNamePathList)
                {
                    if(!item.m_value.empty())
                        m_animationNameList.push_back(item.m_value);
                }
                if(!m_animationNameList.empty())
                    pProperty->SetValueList(m_animationNameList);
            }
        }

        bRet = true;
    }
    return bRet;
}

bool CModel::Init()
{
    CreateController();
#ifdef SW_SKEL_ANIM
    SharePtr<CShader> pVS = CResourceManager::GetInstance()->GetResource<CShader>(_T("PointTexShader.vs"), false);
    SharePtr<CShader> pPS = CResourceManager::GetInstance()->GetResource<CShader>(_T("PointTexShader.ps"), false);
#else
    SharePtr<CShader> pVS = CResourceManager::GetInstance()->GetResource<CShader>(_T("SkinShader.vs"), false);
    SharePtr<CShader> pPS = CResourceManager::GetInstance()->GetResource<CShader>(_T("SkinShader.ps"), false);
#endif

    m_pSkinProgram = CRenderManager::GetInstance()->GetShaderProgram(pVS->ID(), pPS->ID());

    m_bInit = true;
    return true;
}

void CModel::PlayAnimationById( long id, float fBlendTime, bool bLoop )
{
    auto itr = m_animations.find(id);
    if(itr != m_animations.end())
    {
        m_pAnimationController->PlayAnimation(itr->second, fBlendTime, bLoop);
    }
}

void CModel::PlayAnimationByName( const char *name, float fBlendTime, bool bLoop )
{
    long id = MAKEFOURCC(name[0], name[1], name[2], name[3]);
    PlayAnimationById(id, fBlendTime, bLoop);
}

void CModel::AddTexture(SharePtr<CTexture> pTexture)
{
    m_textures.push_back(pTexture);
}

CAnimationController* CModel::GetAnimationController()
{
    return m_pAnimationController;
}

void CModel::PreRender()
{
    CRenderManager* pRenderMgr = CRenderManager::GetInstance();
    CRenderer* pRenderer = CRenderer::GetInstance();
    pRenderer->UseProgram(m_pSkinProgram->ID());
    FC_CHECK_GL_ERROR_DEBUG();

    if (m_pSkin && m_pSkin->IsLoaded())
    {
        for(size_t i = 0; i < m_textures.size(); ++i)
        {
            pRenderMgr->ApplyTexture(i, m_textures[i]->ID());
        }
        FC_CHECK_GL_ERROR_DEBUG();

        if(m_pAnimationController)
        {
            GLuint currShaderProgram = CRenderer::GetInstance()->GetCurrentState()->GetShaderProgram();
            if (currShaderProgram != 0)
            {
                auto &matrices = m_pAnimationController->GetDeltaMatrices();
#ifdef SW_SKEL_ANIM
                m_pSkin->CalcSkelAnim(matrices);
#else
                GLuint curProgram = currShaderProgram;
                for(auto &item : matrices)
                {
                    ESkeletonBoneType boneType = item.first;
                    char uniformName[32];
                    sprintf_s(uniformName, "%s[%d]", COMMON_UNIFORM_NAMES[UNIFORM_BONE_MATRICES], boneType);
                    GLuint location = pRenderer->GetUniformLocation(curProgram, uniformName);
                    pRenderer->SetUniformMatrix4fv(location, (const GLfloat *)item.second.mat, 1);
                }
#endif
            }
            FC_CHECK_GL_ERROR_DEBUG();
        }
    }
}

void CModel::Render()
{
    if (m_pSkin && m_pSkin->IsLoaded())
    {
        CRenderer* pRenderer = CRenderer::GetInstance();

        pRenderer->BindVertexArray(m_pSkin->GetVAO());
        int nBufferSize = 0;
        pRenderer->GetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &nBufferSize);
        int originalVertexArraySize = ( nBufferSize / sizeof(short) );
        pRenderer->DrawElements(GL_TRIANGLES, originalVertexArraySize, GL_UNSIGNED_SHORT, 0);
        FC_CHECK_GL_ERROR_DEBUG();
        pRenderer->BindVertexArray(0);

        if (m_pSkeleton != NULL && m_pAnimationController != NULL)
        {
            RenderSkeleton();
        }
    }
}

void CModel::PostRender()
{
    if (m_pSkin && m_pSkin->IsLoaded())
    {
        CRenderer::GetInstance()->BindVertexArray(0);
    }
}

void CModel::SetAnimaton( SharePtr<CAnimation> pAnimation )
{
    m_animations[0] = pAnimation;
}

void CModel::SetSkeleton( SharePtr<CSkeleton> pSkeleton )
{
    m_pSkeleton = pSkeleton;
    m_pAnimationController->SetSkeleton(pSkeleton);
}

SharePtr<CSkeleton> CModel::GetSkeleton() const
{
    return m_pSkeleton;
}

void CModel::SetSkin( SharePtr<CSkin> pSkin )
{
    m_pSkin = pSkin;
}

void CModel::CreateController()
{
    BEATS_ASSERT(m_pAnimationController == NULL);
    m_pAnimationController = CAnimationManager::GetInstance()->CreateSkelAnimationController();
}

void CModel::RenderSkeleton()
{
    SharePtr<CAnimation> pCurrentAnimation = m_pAnimationController->GetCurrentAnimation();
    BEATS_ASSERT(pCurrentAnimation);

    //Construct render info
    std::vector<float> bonesRenderData;
    CVertexPC startVertex;
    CVertexPC endVertex;
    const std::vector<ESkeletonBoneType>& bones = pCurrentAnimation->GetBones();
    for (size_t i = 0; i < bones.size(); ++i)
    {
        SharePtr<CSkeletonBone> pSkeletonBone = m_pSkeleton->GetSkeletonBone(bones[i]);
        
        kmMat4 matrix;
        m_pAnimationController->GetBoneCurWorldTM(matrix, pSkeletonBone->GetBoneType());
        if (pSkeletonBone->GetVisible())
        {
            SharePtr<CSkeletonBone> pParentBone = pSkeletonBone->GetParent();
            if (pParentBone)
            {
                kmMat4 parentMatrix;
                m_pAnimationController->GetBoneCurWorldTM(parentMatrix,  pParentBone->GetBoneType());

                startVertex.position.x = matrix.mat[12];
                startVertex.position.y = matrix.mat[13];
                startVertex.position.z = matrix.mat[14];
                startVertex.color = 0xFF0000FF;

                endVertex.position.x = parentMatrix.mat[12];
                endVertex.position.y = parentMatrix.mat[13];
                endVertex.position.z = parentMatrix.mat[14];
                endVertex.color = 0xFFFF00FF;
                CRenderManager::GetInstance()->RenderLine(startVertex, endVertex);
            }
        }
        if (pSkeletonBone->GetCoordinateVisible())
        {
            CRenderManager::GetInstance()->RenderCoordinate(&matrix);
        }
    }
}
