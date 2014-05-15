#ifndef RENDER_QUAD_BATCH_RENDER_GROUP_H__INCLUDE
#define RENDER_QUAD_BATCH_RENDER_GROUP_H__INCLUDE

#include "RenderGroup.h"

class CRenderBatch;
class CMaterial;

class CBatchRenderGroup : public CRenderGroup
{
public:
    CBatchRenderGroup(int ID);
    virtual ~CBatchRenderGroup();

    virtual void AddRenderObject( CRenderObject *object );

    virtual void Clear();

    CRenderBatch *GetRenderBatch(SharePtr<CMaterial> material, GLenum primitiveType, bool bIndexed);

};

#endif // !RENDER_QUAD_BATCH_RENDER_GROUP_H__INCLUDE
