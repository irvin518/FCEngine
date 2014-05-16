#ifndef RENDER_QUAD_BATCH_H__INCLUDE
#define RENDER_QUAD_BATCH_H__INCLUDE

#include "RenderObject.h"
#include "CommonTypes.h"

class CMaterial;
class CRenderBatch : public CRenderObject
{
public:
    CRenderBatch(SharePtr<CMaterial> material, GLenum primitiveType, bool bIndexed);
    virtual ~CRenderBatch();

    virtual void PreRender() override;

    virtual void Render() override;

    virtual void PostRender() override;

    void AddQuad(const CQuadP &quadP, const CQuadT &quadT, const kmMat4 &transform);

    void AddQuad(const CQuadPTC &quad, const kmMat4 *pTransform = nullptr);

    void AddVertices(const CVertexPTC *pVertices, size_t count, const kmMat4 *pTransform = nullptr);

    void AddIndexedVertices(const CVertexPTC *pVertices, size_t vertexCount, 
        const unsigned short *pIndices, size_t indexCount, const kmMat4 *pTransform = nullptr);

    SharePtr<CMaterial> GetMaterial() const;

    GLenum GetPrimitiveType() const;

    bool IsIndexed() const;

    void Clear();

    void Complete();

    bool IsCompleted() const;

private:
    void setupVAO();
    void updateVBO();

private:
    SharePtr<CMaterial> m_pMaterial;
    GLenum m_primitiveType;
    bool m_bIndexed;

    std::vector<CVertexPTC> m_vertices;
    std::vector<unsigned short> m_indices;
    bool m_bCompleted;

    GLuint m_VAO, m_VBO[2];
};

#endif