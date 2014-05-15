#include "stdafx.h"
#include "RenderBatch.h"
#include "RenderManager.h"
#include "Texture.h"
#include "Renderer.h"
#include "Material.h"

CRenderBatch::CRenderBatch(SharePtr<CMaterial> material, GLenum primitiveType, bool bIndexed)
    : m_pMaterial(material)
    , m_primitiveType(primitiveType)
    , m_bIndexed(bIndexed)
    , m_bCompleted(false)
    , m_VAO(0)
{
    FC_PERFORMDETECT_START(ePNT_CreateRenderBatch);
    memset(m_VBO, 0, sizeof(m_VBO));
    CRenderer::GetInstance()->GenVertexArrays(1, &m_VAO);
    CRenderer::GetInstance()->GenBuffers(m_bIndexed ? 2 : 1, m_VBO);
    setupVAO();
    FC_PERFORMDETECT_STOP(ePNT_CreateRenderBatch);
}

CRenderBatch::~CRenderBatch()
{
    CRenderer::GetInstance()->DeleteVertexArrays(1, &m_VAO);
    CRenderer::GetInstance()->DeleteBuffers(m_bIndexed ? 2 : 1, m_VBO);
}

void CRenderBatch::PreRender()
{
    FC_PERFORMDETECT_START(ePNT_RenderBatchPreRender);
    if(!m_vertices.empty())
    {
        updateVBO();
        CRenderer::GetInstance()->BindVertexArray(m_VAO);
        m_pMaterial->Use();
    }
    FC_PERFORMDETECT_STOP(ePNT_RenderBatchPreRender);
}

void CRenderBatch::Render()
{
    FC_PERFORMDETECT_START(ePNT_RenderBatchRender);
    if(!m_vertices.empty())
    {
        if(m_bIndexed)
        {
            BEATS_ASSERT(!m_indices.empty());
            CRenderer::GetInstance()->DrawElements(
                m_primitiveType, m_indices.size(), GL_UNSIGNED_SHORT, 0);
        }
        else
        {
            CRenderer::GetInstance()->DrawArrays(m_primitiveType, 0, m_vertices.size());
        }
    }
    FC_PERFORMDETECT_STOP(ePNT_RenderBatchRender);
}

void CRenderBatch::PostRender()
{
    FC_PERFORMDETECT_START(ePNT_RenderBatchPostRender);
    if(!m_vertices.empty())
    {
        CRenderer::GetInstance()->BindVertexArray(0);
    }
    FC_PERFORMDETECT_STOP(ePNT_RenderBatchPostRender);
}

void CRenderBatch::AddQuad(const CQuadP &quadP, const CQuadT &quadT, const kmMat4 &transform)
{
    CQuadPTC quadPTC;
    quadPTC.tl.position = quadP.tl;
    quadPTC.tr.position = quadP.tr;
    quadPTC.bl.position = quadP.bl;
    quadPTC.br.position = quadP.br;
    quadPTC.tl.tex = quadT.tl;
    quadPTC.tr.tex = quadT.tr;
    quadPTC.bl.tex = quadT.bl;
    quadPTC.br.tex = quadT.br;
    AddQuad(quadPTC, &transform);
}

void CRenderBatch::AddQuad( const CQuadPTC &quad, const kmMat4 *pTransform)
{
    unsigned short indices[6] = {0, 1, 2, 3, 2, 1};
    AddIndexedVertices((const CVertexPTC *)&quad, 4, indices, 6, pTransform);
}

void CRenderBatch::AddVertices(const CVertexPTC *pVertices, size_t count, const kmMat4 *pTransform)
{
    for(size_t i = 0; i < count; ++i)
    {
        const CVertexPTC &vertex = pVertices[i];
        m_vertices.push_back(vertex);
        if(pTransform)
        {
            kmVec3Transform(&m_vertices.back().position, &vertex.position, pTransform);
        }
    }
}

void CRenderBatch::AddIndexedVertices(const CVertexPTC *pVertices, size_t vertexCount, 
                                    const unsigned short *pIndices, size_t indexCount, 
                                    const kmMat4 *pTransform /* = nullptr */)
{
    BEATS_ASSERT(m_bIndexed);
    unsigned short currVertexCount = static_cast<unsigned short>(m_vertices.size());
    AddVertices(pVertices, vertexCount, pTransform);
    for(size_t i = 0; i < indexCount; ++i)
    {
        BEATS_ASSERT(pIndices[i] < vertexCount);
        m_indices.push_back(currVertexCount + pIndices[i]);
    }
}

void CRenderBatch::setupVAO()
{
    CRenderer *pRenderer = CRenderer::GetInstance();
    pRenderer->BindVertexArray(m_VAO);

    pRenderer->BindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);

    pRenderer->EnableVertexAttribArray(ATTRIB_INDEX_POSITION);
    pRenderer->EnableVertexAttribArray(ATTRIB_INDEX_TEXCOORD0);
    pRenderer->EnableVertexAttribArray(ATTRIB_INDEX_COLOR);

    pRenderer->VertexAttribPointer(ATTRIB_INDEX_POSITION, 
        3, GL_FLOAT, GL_FALSE, sizeof(CVertexPTC), (const GLvoid *)offsetof(CVertexPTC, position));
    pRenderer->VertexAttribPointer(ATTRIB_INDEX_TEXCOORD0,
        2, GL_FLOAT, GL_FALSE, sizeof(CVertexPTC), (const GLvoid *)offsetof(CVertexPTC, tex));
    pRenderer->VertexAttribPointer(ATTRIB_INDEX_COLOR,
        4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(CVertexPTC), (const GLvoid *)offsetof(CVertexPTC, color));

    pRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bIndexed ? m_VBO[1] : 0);

    pRenderer->BindVertexArray(0);
    pRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    pRenderer->BindBuffer(GL_ARRAY_BUFFER, 0);
}

void CRenderBatch::updateVBO()
{
    FC_PERFORMDETECT_START(ePNT_RenderBatchUpdateVBO);
    if(!m_vertices.empty())
    {
        CRenderer *pRenderer = CRenderer::GetInstance();

        FC_PERFORMDETECT_START(ePNT_RenderBatchUpdateVBO0);
        pRenderer->BindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
        pRenderer->BufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(CVertexPTC), 
            &m_vertices[0], GL_DYNAMIC_DRAW);
        FC_PERFORMDETECT_STOP(ePNT_RenderBatchUpdateVBO0);

        if(m_bIndexed)
        {
            FC_PERFORMDETECT_START(ePNT_RenderBatchUpdateVBO1);
            BEATS_ASSERT(!m_indices.empty());
            pRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[1]);
            pRenderer->BufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned short),
                &m_indices[0], GL_DYNAMIC_DRAW);
            FC_PERFORMDETECT_STOP(ePNT_RenderBatchUpdateVBO1);
        }
    }
    FC_PERFORMDETECT_STOP(ePNT_RenderBatchUpdateVBO);
}

void CRenderBatch::Clear()
{
    m_vertices.clear();
    m_indices.clear();
    m_bCompleted = false;
}

SharePtr<CMaterial> CRenderBatch::GetMaterial() const
{
    return m_pMaterial;
}

GLenum CRenderBatch::GetPrimitiveType() const
{
    return m_primitiveType;
}

bool CRenderBatch::IsIndexed() const
{
    return m_bIndexed;
}

void CRenderBatch::Complete()
{
    m_bCompleted = true;
}

bool CRenderBatch::IsCompleted() const
{
    return m_bCompleted;
}
