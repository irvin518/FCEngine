#ifndef CVALUE_H_INCLUDE
#define CVALUE_H_INCLUDE

#include <unordered_map>

class CValue;

typedef std::vector<CValue> TValueVector;
typedef std::unordered_map<TString, CValue> TValueMap;
typedef std::unordered_map<int, CValue> TValueMapIntKey;

class CValue
{
public:
    static const CValue Null;

    CValue();
    explicit CValue(unsigned char v);
    explicit CValue(int v);
    explicit CValue(float v);
    explicit CValue(double v);
    explicit CValue(bool v);
    explicit CValue(const char* v);
    explicit CValue(const TString& v);

    explicit CValue(const TValueVector& v);
    explicit CValue(TValueVector&& v);

    explicit CValue(const TValueMap& v);
    explicit CValue(TValueMap&& v);

    explicit CValue(const TValueMapIntKey& v);
    explicit CValue(TValueMapIntKey&& v);

    CValue(const CValue& other);
    CValue(CValue&& other);
    ~CValue();

    // assignment operator
    CValue& operator= (const CValue& other);
    CValue& operator= (CValue&& other);

    CValue& operator= (unsigned char v);
    CValue& operator= (int v);
    CValue& operator= (float v);
    CValue& operator= (double v);
    CValue& operator= (bool v);
    CValue& operator= (const char* v);
    CValue& operator= (const TString& v);

    CValue& operator= (const TValueVector& v);
    CValue& operator= (TValueVector&& v);

    CValue& operator= (const TValueMap& v);
    CValue& operator= (TValueMap&& v);

    CValue& operator= (const TValueMapIntKey& v);
    CValue& operator= (TValueMapIntKey&& v);

    unsigned char AsByte() const;
    int AsInt() const;
    float AsFloat() const;
    double AsDouble() const;
    bool AsBool() const;
    TString AsString() const;

    TValueVector& AsValueVector();
    const TValueVector& AsValueVector() const;

    TValueMap& AsValueMap();
    const TValueMap& AsValueMap() const;

    TValueMapIntKey& AsIntKeyMap();
    const TValueMapIntKey& AsIntKeyMap() const;

    bool IsNull() const;

    enum class EType
    {
        eT_NONE,
        eT_BYTE,
        eT_INTEGER,
        eT_FLOAT,
        eT_DOUBLE,
        eT_BOOLEAN,
        eT_STRING,
        eT_VECTOR,
        eT_MAP,
        eT_INT_KEY_MAP
    };

    inline EType GetType() const;

    const TString& GetDescription();

private:

    static const TString& Visit(const CValue& v, int depth);
    static const TString& VisitVector(const TValueVector& v, int depth);
    static const TString& GetTabs(int depth);
    template <class T>
    static const TString& VisitMap(const T& v, int depth);

private:
    void Clear();

    union
    {
        unsigned char byteVal;
        int intVal;
        float floatVal;
        double doubleVal;
        bool boolVal;
    }m_baseData;

    TString m_strData;
    TValueVector* m_vectorData;
    TValueMap* m_mapData;
    TValueMapIntKey* m_intKeyMapData;

    EType m_type;
    TString m_retString;
    static TString m_retStringStatic;
};


#endif//CVALUE_H_INCLUDE