#include "stdafx.h"
#include "CData.h"

const CData CData::Null;

CData::CData() :
    m_bytes( NULL ),
    m_size( 0 )
{

}

CData::CData( const CData& other ) :
    m_bytes( NULL ),
    m_size( 0 )
{
    Copy( other.m_bytes, other.m_size );
}

CData::CData( CData&& other ) :
    m_bytes( NULL ),
    m_size( 0 )
{
    Move( other );
}

CData::~CData()
{
    Clear();
}

CData& CData::operator=( const CData& other )
{
    Copy( other.m_bytes, other.m_size );
    return *this;
}

CData& CData::operator=( CData&& other )
{
    Move( other );
    return *this;
}

unsigned char* CData::GetBytes() const
{
    return m_bytes;
}

ssize_t CData::GetSize() const
{
    return m_size;
}

void CData::Copy( unsigned char* bytes, const ssize_t size )
{
    Clear();

    if ( size > 0 )
    {
        m_size = size;
        m_bytes = new unsigned char[ m_size ];
        memcpy( m_bytes, bytes, m_size );
    }
}

void CData::FastSet( unsigned char* bytes, const ssize_t size )
{
    m_bytes = bytes;
    m_size = size;
}

void CData::Clear()
{
    BEATS_SAFE_DELETE_ARRAY( m_bytes );
    m_size = 0;
}

bool CData::IsNull() const
{
    return ( NULL == m_bytes || 0 == m_size);
}

void CData::Move( CData& other )
{
    m_bytes = other.m_bytes;
    m_size = other.m_size;

    other.m_bytes = NULL;
    other.m_size = 0;
}
