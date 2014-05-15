#include "stdafx.h"
#include "BatchRenderGroup.h"
#include "RenderBatch.h"
#include "Material.h"

CBatchRenderGroup::CBatchRenderGroup(int ID)
    : CRenderGroup(ID)
{

}

CBatchRenderGroup::~CBatchRenderGroup()
{
    Clear();
}

void CBatchRenderGroup::AddRenderObject(CRenderObject *object)
{
    BEATS_ASSERT(dynamic_cast<CRenderBatch *>(object));
    CRenderGroup::AddRenderObject(object);
}

CRenderBatch *CBatchRenderGroup::GetRenderBatch(SharePtr<CMaterial> material, 
                                                GLenum primitiveType, bool bIndexed)
{
    CRenderBatch *batch = nullptr;
    if(m_objects.empty())
    {
        batch = new CRenderBatch(material, primitiveType, bIndexed);
        AddRenderObject(batch);
    }
    else
    {
        batch = static_cast<CRenderBatch *>(m_objects.back());
        if( *material != *batch->GetMaterial() ||
            batch->GetPrimitiveType() != primitiveType ||
            batch->IsIndexed() != bIndexed ||
            batch->IsCompleted())
        {
            batch = new CRenderBatch(material, primitiveType, bIndexed);
            AddRenderObject(batch);
        }
    }
    return batch;
}

void CBatchRenderGroup::Clear()
{
    for(auto object : m_objects)
    {
        BEATS_SAFE_DELETE(object);
    }
    CRenderGroup::Clear();
}
