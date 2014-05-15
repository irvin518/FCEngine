
#include "stdafx.h"
#include "CValue.h"

#include <sstream>
#include <iomanip>

const CValue CValue::Null;

TString CValue::m_retStringStatic = _T("");

CValue::CValue()
    : m_vectorData( NULL )
    , m_mapData( NULL )
    , m_intKeyMapData( NULL )
    , m_type(EType::eT_NONE)
{

}

CValue::CValue(unsigned char v)
    : m_vectorData(NULL)
    , m_mapData(NULL)
    , m_intKeyMapData(NULL)
    , m_type(EType::eT_BYTE)
{
    m_baseData.byteVal = v;
}

CValue::CValue(int v)
    : m_vectorData(NULL)
    , m_mapData(NULL)
    , m_intKeyMapData(NULL)
    , m_type(EType::eT_INTEGER)
{
    m_baseData.intVal = v;
}

CValue::CValue(float v)
    : m_vectorData(NULL)
    , m_mapData(NULL)
    , m_intKeyMapData(NULL)
    , m_type(EType::eT_FLOAT)
{
    m_baseData.floatVal = v;
}

CValue::CValue(double v)
    : m_vectorData(NULL)
    , m_mapData(NULL)
    , m_intKeyMapData(NULL)
    , m_type(EType::eT_DOUBLE)
{
    m_baseData.doubleVal = v;
}

CValue::CValue(bool v)
    : m_vectorData(NULL)
    , m_mapData(NULL)
    , m_intKeyMapData(NULL)
    , m_type(EType::eT_BOOLEAN)
{
    m_baseData.boolVal = v;
}

CValue::CValue(const char* v)
    : m_vectorData(NULL)
    , m_mapData(NULL)
    , m_intKeyMapData(NULL)
    , m_type(EType::eT_STRING)
{
    TCHAR val[MAX_PATH];
    CStringHelper::GetInstance()->ConvertToTCHAR( v, val, MAX_PATH);
    m_strData = val;
}

CValue::CValue(const TString& v)
    : m_vectorData(NULL)
    , m_mapData(NULL)
    , m_intKeyMapData(NULL)
    , m_type(EType::eT_STRING)
{
    m_strData = v;
}

CValue::CValue(const TValueVector& v)
    : m_vectorData(new TValueVector())
    , m_mapData(NULL)
    , m_intKeyMapData(NULL)
    , m_type(EType::eT_VECTOR)
{
    *m_vectorData = v;
}

CValue::CValue(TValueVector&& v)
    : m_vectorData(new TValueVector())
    , m_mapData(NULL)
    , m_intKeyMapData(NULL)
    , m_type(EType::eT_VECTOR)
{
    *m_vectorData = std::move(v);
}

CValue::CValue(const TValueMap& v)
    : m_vectorData(NULL)
    , m_mapData(new TValueMap())
    , m_intKeyMapData(NULL)
    , m_type(EType::eT_MAP)
{
    *m_mapData = v;
}

CValue::CValue(TValueMap&& v)
    : m_vectorData(NULL)
    , m_mapData(new TValueMap())
    , m_intKeyMapData(NULL)
    , m_type(EType::eT_MAP)
{
    *m_mapData = std::move(v);
}

CValue::CValue(const TValueMapIntKey& v)
    : m_vectorData(NULL)
    , m_mapData(NULL)
    , m_intKeyMapData(new TValueMapIntKey())
    , m_type(EType::eT_INT_KEY_MAP)
{
    *m_intKeyMapData = v;
}

CValue::CValue(TValueMapIntKey&& v)
    : m_vectorData(NULL)
    , m_mapData(NULL)
    , m_intKeyMapData(new TValueMapIntKey())
    , m_type(EType::eT_INT_KEY_MAP)
{
    *m_intKeyMapData = std::move(v);
}

CValue::CValue(const CValue& other)
    : m_vectorData(NULL)
    , m_mapData(NULL)
    , m_intKeyMapData(NULL)
{
    *this = other;
}

CValue::CValue(CValue&& other)
    : m_vectorData(NULL)
    , m_mapData(NULL)
    , m_intKeyMapData(NULL)
{
    *this = std::move(other);
}

CValue::~CValue()
{
    Clear();
}

CValue& CValue::operator= (const CValue& other)
{
    if (this != &other)
    {
        switch (other.m_type)
        {
        case EType::eT_BYTE:
            m_baseData.byteVal = other.m_baseData.byteVal;
            break;
        case EType::eT_INTEGER:
            m_baseData.intVal = other.m_baseData.intVal;
            break;
        case EType::eT_FLOAT:
            m_baseData.floatVal = other.m_baseData.floatVal;
            break;
        case EType::eT_DOUBLE:
            m_baseData.doubleVal = other.m_baseData.doubleVal;
            break;
        case EType::eT_BOOLEAN:
            m_baseData.boolVal = other.m_baseData.boolVal;
            break;
        case EType::eT_STRING:
            m_strData = other.m_strData;
            break;
        case EType::eT_VECTOR:
            if (m_vectorData == NULL)
                m_vectorData = new TValueVector();
            *m_vectorData = *other.m_vectorData;
            break;
        case EType::eT_MAP:
            if (m_mapData == NULL)
                m_mapData = new TValueMap();
            *m_mapData = *other.m_mapData;
            break;
        case EType::eT_INT_KEY_MAP:
            if (m_intKeyMapData == NULL)
                m_intKeyMapData = new TValueMapIntKey();
            *m_intKeyMapData = *other.m_intKeyMapData;
            break;
        default:
            break;
        }
        m_type = other.m_type;
    }
    return *this;
}

CValue& CValue::operator= (CValue&& other)
{
    if (this != &other)
    {
        switch (other.m_type)
        {
        case EType::eT_BYTE:
            m_baseData.byteVal = other.m_baseData.byteVal;
            break;
        case EType::eT_INTEGER:
            m_baseData.intVal = other.m_baseData.intVal;
            break;
        case EType::eT_FLOAT:
            m_baseData.floatVal = other.m_baseData.floatVal;
            break;
        case EType::eT_DOUBLE:
            m_baseData.doubleVal = other.m_baseData.doubleVal;
            break;
        case EType::eT_BOOLEAN:
            m_baseData.boolVal = other.m_baseData.boolVal;
            break;
        case EType::eT_STRING:
            m_strData = other.m_strData;
            break;
        case EType::eT_VECTOR:
            {
                BEATS_SAFE_DELETE(m_vectorData);
                m_vectorData = other.m_vectorData;
                break;
            }
        case EType::eT_MAP:
            {
                BEATS_SAFE_DELETE(m_mapData);
                m_mapData = other.m_mapData;
                break;
            }
        case EType::eT_INT_KEY_MAP:
            {
                BEATS_SAFE_DELETE(m_intKeyMapData);
                m_intKeyMapData = other.m_intKeyMapData;
                break;
            }
        default:
            break;
        }
        m_type = other.m_type;
        other.m_type = EType::eT_NONE;

        other.m_vectorData = NULL;
        other.m_mapData = NULL;
        other.m_intKeyMapData = NULL;
    }

    return *this;
}

CValue& CValue::operator= (unsigned char v)
{
    Clear();
    m_type = EType::eT_BYTE;
    m_baseData.byteVal = v;
    return *this;
}

CValue& CValue::operator= (int v)
{
    Clear();
    m_type = EType::eT_INTEGER;
    m_baseData.intVal = v;
    return *this;
}

CValue& CValue::operator= (float v)
{
    Clear();
    m_type = EType::eT_FLOAT;
    m_baseData.floatVal = v;
    return *this;
}

CValue& CValue::operator= (double v)
{
    Clear();
    m_type = EType::eT_DOUBLE;
    m_baseData.doubleVal = v;
    return *this;
}

CValue& CValue::operator= (bool v)
{
    Clear();
    m_type = EType::eT_BOOLEAN;
    m_baseData.boolVal = v;
    return *this;
}

CValue& CValue::operator= (const char* v)
{
    Clear();
    m_type = EType::eT_STRING;
    TCHAR val[MAX_PATH];
    CStringHelper::GetInstance()->ConvertToTCHAR( v, val, MAX_PATH);
    m_strData = val ? val : _T("");
    return *this;
}

CValue& CValue::operator= (const TString& v)
{
    Clear();
    m_type = EType::eT_STRING;
    m_strData = v;
    return *this;
}

CValue& CValue::operator= (const TValueVector& v)
{
    Clear();
    m_type = EType::eT_VECTOR;
    m_vectorData = new TValueVector();
    *m_vectorData = v;
    return *this;
}

CValue& CValue::operator= (TValueVector&& v)
{
    Clear();
    m_type = EType::eT_VECTOR;
    m_vectorData = new TValueVector();
    *m_vectorData = std::move(v);
    return *this;
}

CValue& CValue::operator= (const TValueMap& v)
{
    Clear();
    m_type = EType::eT_MAP;
    m_mapData = new TValueMap();
    *m_mapData = v;
    return *this;
}

CValue& CValue::operator= (TValueMap&& v)
{
    Clear();
    m_type = EType::eT_MAP;
    m_mapData = new TValueMap();
    *m_mapData = std::move(v);
    return *this;
}

CValue& CValue::operator= (const TValueMapIntKey& v)
{
    Clear();
    m_type = EType::eT_INT_KEY_MAP;
    m_intKeyMapData = new TValueMapIntKey();
    *m_intKeyMapData = v;
    return *this;
}

CValue& CValue::operator= (TValueMapIntKey&& v)
{
    Clear();
    m_type = EType::eT_INT_KEY_MAP;
    m_intKeyMapData = new TValueMapIntKey();
    *m_intKeyMapData = std::move(v);
    return *this;
}

///
unsigned char CValue::AsByte() const
{
    BEATS_ASSERT(m_type != EType::eT_VECTOR && m_type != EType::eT_MAP, _T(""));

    unsigned char returnChar = 0;
    switch ( m_type )
    {
    case EType::eT_BYTE:
        returnChar = m_baseData.byteVal;
        break;
    case EType::eT_INTEGER:
        returnChar = static_cast<unsigned char>(m_baseData.intVal);
        break;
    case EType::eT_STRING:
        {
            char intString[ MAX_PATH ];
            CStringHelper::GetInstance()->ConvertToCHAR( m_strData.c_str(), intString, m_strData.length() );
            returnChar = static_cast<unsigned char>(atoi(intString));
            break;
        }
    case EType::eT_FLOAT:
        returnChar = static_cast<unsigned char>(m_baseData.floatVal);
        break;
    case EType::eT_DOUBLE:
        returnChar = static_cast<unsigned char>(m_baseData.doubleVal);
        break;
    case EType::eT_BOOLEAN:
        returnChar = m_baseData.boolVal ? 1 : 0;
        break;

    default:
        break;
    }
    return returnChar;
}

int CValue::AsInt() const
{
    BEATS_ASSERT(m_type != EType::eT_VECTOR && m_type != EType::eT_MAP, _T(""));
    int renturnInt = 0;
    switch ( m_type )
    {
    case EType::eT_INTEGER:
        renturnInt = m_baseData.intVal;
        break;
    case EType::eT_BYTE:
        renturnInt = m_baseData.byteVal;
        break;
    case EType::eT_STRING:
        {
            char intString[ MAX_PATH ];
            CStringHelper::GetInstance()->ConvertToCHAR( m_strData.c_str(), intString, m_strData.length() );
            renturnInt = atoi(intString);
            break;
        }
    case EType::eT_FLOAT:
        renturnInt = static_cast<int>(m_baseData.floatVal);
        break;
    case EType::eT_DOUBLE:
        renturnInt = static_cast<int>(m_baseData.doubleVal);
        break;
    case EType::eT_BOOLEAN:
        renturnInt = m_baseData.boolVal ? 1 : 0;
        break;

    default:
        break;
    }
    return renturnInt;
}

float CValue::AsFloat() const
{
    BEATS_ASSERT(m_type != EType::eT_VECTOR && m_type != EType::eT_MAP, _T(""));

    float returnFloat = 0.0f;
    switch ( m_type )
    {
    case EType::eT_INTEGER:
        returnFloat = static_cast<float>(m_baseData.intVal);
        break;
    case EType::eT_BYTE:
        returnFloat = static_cast<float>(m_baseData.byteVal);
        break;
    case EType::eT_STRING:
        {
            char floatString[ MAX_PATH ];
            CStringHelper::GetInstance()->ConvertToCHAR( m_strData.c_str(), floatString, m_strData.length() );
            returnFloat = static_cast<float>(atof(floatString));
            break;
        }
    case EType::eT_FLOAT:
        returnFloat = m_baseData.floatVal;
        break;
    case EType::eT_DOUBLE:
        returnFloat = static_cast<float>(m_baseData.doubleVal);
        break;
    case EType::eT_BOOLEAN:
        returnFloat = m_baseData.boolVal ? 1.0f : 0.0f;
        break;

    default:
        break;
    }
    return returnFloat;
}

double CValue::AsDouble() const
{
    BEATS_ASSERT(m_type != EType::eT_VECTOR && m_type != EType::eT_MAP, _T(""));

    double returnDouble = 0.0;
    switch ( m_type )
    {
    case EType::eT_INTEGER:
        returnDouble = static_cast<double>(m_baseData.intVal);
        break;
    case EType::eT_BYTE:
        returnDouble = static_cast<double>(m_baseData.byteVal);
        break;
    case EType::eT_STRING:
        {
            char doubleString[ MAX_PATH ];
            CStringHelper::GetInstance()->ConvertToCHAR( m_strData.c_str(), doubleString, m_strData.length() );
            returnDouble = static_cast<double>(atof(doubleString));
            break;
        }
    case EType::eT_FLOAT:
        returnDouble = static_cast<double>(m_baseData.floatVal);
        break;
    case EType::eT_DOUBLE:
        returnDouble = m_baseData.doubleVal;
        break;
    case EType::eT_BOOLEAN:
        returnDouble = m_baseData.boolVal ? 1.0 : 0.0;
        break;

    default:
        break;
    }

    return returnDouble;
}

bool CValue::AsBool() const
{
    BEATS_ASSERT(m_type != EType::eT_VECTOR && m_type != EType::eT_MAP, _T(""));

    bool returnBool = true;
    switch ( m_type )
    {
    case EType::eT_INTEGER:
        returnBool = (m_baseData.intVal == 0);
        break;
    case EType::eT_BYTE:
        returnBool = (m_baseData.byteVal == 0);
        break;
    case EType::eT_STRING:
        returnBool = (m_strData == _T("0") || m_strData == _T("false")) ? false : true;
        break;
    case EType::eT_FLOAT:
        returnBool = (m_baseData.floatVal == 0.0f);
        break;
    case EType::eT_DOUBLE:
        returnBool = (m_baseData.doubleVal == 0.0);
        break;
    case EType::eT_BOOLEAN:
        returnBool = m_baseData.boolVal;
        break;

    default:
        break;
    }
    return returnBool;
}

TString CValue::AsString() const
{
    BEATS_ASSERT(m_type != EType::eT_VECTOR && m_type != EType::eT_MAP, _T(""));

    TString retString = _T("");
    if (m_type == EType::eT_STRING)
    {
        retString = m_strData;
    }
    else
    {
        TStringstream ret;

        switch (m_type) {
        case EType::eT_BYTE:
            ret << m_baseData.byteVal;
            break;
        case EType::eT_INTEGER:
            ret << m_baseData.intVal;
            break;
        case EType::eT_FLOAT:
            ret << std::fixed << std::setprecision( 7 )<< m_baseData.floatVal;
            break;
        case EType::eT_DOUBLE:
            ret << std::fixed << std::setprecision( 16 ) << m_baseData.doubleVal;
            break;
        case EType::eT_BOOLEAN:
            ret << (m_baseData.boolVal ? _T("true") : _T("false"));
            break;
        default:
            break;
        }
        retString = ret.str();
    }
    return retString;
}

TValueVector& CValue::AsValueVector()
{
    if (NULL == m_vectorData)
        m_vectorData = new TValueVector();
    return *m_vectorData;
}

const TValueVector& CValue::AsValueVector() const
{
    static const TValueVector EMPTY_VALUEVECTOR;
    if (NULL == m_vectorData)
        return EMPTY_VALUEVECTOR;
    return *m_vectorData; 
}

TValueMap& CValue::AsValueMap()
{
    if (NULL == m_mapData)
        m_mapData = new TValueMap();
    return *m_mapData;
}

const TValueMap& CValue::AsValueMap() const
{
    static const TValueMap EMPTY_VALUEMAP;
    if (NULL == m_mapData)
        return EMPTY_VALUEMAP;
    return *m_mapData;
}

TValueMapIntKey& CValue::AsIntKeyMap()
{
    if (NULL == m_intKeyMapData)
        m_intKeyMapData = new TValueMapIntKey();
    return *m_intKeyMapData;
}

const TValueMapIntKey& CValue::AsIntKeyMap() const
{
    static const TValueMapIntKey EMPTY_VALUEMAP_INT_KEY;
    if (NULL == m_intKeyMapData)
        return EMPTY_VALUEMAP_INT_KEY;
    return *m_intKeyMapData;
}

const TString& CValue::GetDescription()
{
    m_retString = _T("\n");
    m_retString += Visit(*this, 0);
    return m_retString;
}

void CValue::Clear()
{
    m_type = EType::eT_NONE;
    m_baseData.doubleVal = 0.0;
    m_strData.clear();
    BEATS_SAFE_DELETE(m_vectorData);
    BEATS_SAFE_DELETE(m_mapData);
    BEATS_SAFE_DELETE(m_intKeyMapData);
}

const TString& CValue::Visit( const CValue& v, int depth )
{
    m_retStringStatic = _T("");
    TStringstream ret;

    switch (v.GetType())
    {
    case CValue::EType::eT_NONE:
    case CValue::EType::eT_BYTE:
    case CValue::EType::eT_INTEGER:
    case CValue::EType::eT_FLOAT:
    case CValue::EType::eT_DOUBLE:
    case CValue::EType::eT_BOOLEAN:
    case CValue::EType::eT_STRING:
        ret << v.AsString() << _T("\n");
        break;
    case CValue::EType::eT_VECTOR:
        ret << VisitVector(v.AsValueVector(), depth);
        break;
    case CValue::EType::eT_MAP:
        ret << VisitMap(v.AsValueMap(), depth);
        break;
    case CValue::EType::eT_INT_KEY_MAP:
        ret << VisitMap(v.AsIntKeyMap(), depth);
        break;
    default:
        BEATS_ASSERT(false, _T("Invalid type!"));
        break;
    }

    m_retStringStatic = ret.str();
    return m_retStringStatic;
}

const TString& CValue::VisitVector( const TValueVector& v, int depth )
{
    m_retStringStatic = _T("");
    TStringstream ret;

    if (depth > 0)
        ret << _T("\n");

    ret << GetTabs(depth) << _T("[\n");

    int i = 0;
    for (const auto& child : v)
    {
        ret << GetTabs(depth+1) << i << _T(": ") << Visit(child, depth + 1);
        ++i;
    }

    ret << GetTabs(depth) << _T("]\n");

    m_retStringStatic = ret.str();
    return m_retStringStatic;
}

const TString& CValue::GetTabs( int depth )
{
    m_retStringStatic = _T("");
    for (int i = 0; i < depth; ++i)
    {
        m_retStringStatic += _T("\t");
    }
    return m_retStringStatic;
}

bool CValue::IsNull() const
{
    return m_type == EType::eT_NONE;
}

CValue::EType CValue::GetType() const
{
    return m_type;
}

template <class T>
const TString& CValue::VisitMap( const T& v, int depth )
{
    m_retStringStatic = _T("");
    TStringstream ret;

    if (depth > 0)
        ret << _T("\n");

    ret << GetTabs(depth) << _T("{\n");

    for (auto iter = v.begin(); iter != v.end(); ++iter)
    {
        ret << GetTabs(depth + 1) << iter->first << _T(": ");
        ret << Visit(iter->second, depth + 1);
    }

    ret << GetTabs(depth) << _T("}\n");

    m_retStringStatic = ret.str();
    return m_retStringStatic;
}
