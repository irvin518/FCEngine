#ifndef CDATA_H_INCLUDE
#define CDATA_H_INCLUDE

class CData
{
public:
    CData();
    CData( const CData& other );
    CData( CData&& other );
    ~CData();

    CData& operator=( const CData& other );
    CData& operator=( CData&& other );

    unsigned char* GetBytes() const;

    ssize_t GetSize() const;

    void Copy( unsigned char* bytes, const ssize_t size );

    void FastSet( unsigned char* bytes, const ssize_t size );

    void Clear();

    bool IsNull() const;

private:
    void Move( CData& other );

private:
    unsigned char* m_bytes;
    ssize_t m_size;

public:
    static const CData Null;

};

#endif//CDATA_H_INCLUDE