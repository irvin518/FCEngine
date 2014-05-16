#ifndef NODE2D_H_INCLUDE
#define NODE2D_H_INCLUDE

#include "CNode.h"
#include "Render/Sprite.h"

class CNode2D : public CNode
{
public:
    CNode2D( const TString& name );
    virtual ~CNode2D();

    void SetPosition( const kmVec2& position );
    const kmVec2& GetPosition() const;

    void SetWorldPosition( const kmVec2& position );
    const kmVec2& GetWorldPosition() const;

    void SetDirection( const kmVec2& direction );
    const kmVec2& GetDirection() const;

    void LookAtNode( CNode2D* pNode );
    void Roll( kmScalar degree );

    void Move( const kmVec2& direction, kmScalar velocity );

    void SetSize( const kmVec2& size );
    const kmVec2& GetSize() const;

private:

    kmVec2 m_ve2Position;
    kmVec2 m_ve2Direction;
    kmVec2 m_ve2WorldPosition;
    kmVec2 m_ve2Size;
};



#endif

