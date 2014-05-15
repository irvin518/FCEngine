#ifndef CODEANALYSIS_H
#define CODEANALYSIS_H

#include "PublicDef.h"
#include <windows.h>

class CSerializer;

enum EDATATYPE{
	EDT_CLASSTYPE,
	EDT_PROPERTY,
	EDT_FUNCTION,
	EDT_ENUMTYPE
};

struct SBaseData
{
    TString m_str;
	TString m_strName;
	TString m_strNewName;
    SBaseData()
		: m_eType(EDT_FUNCTION)
    {

    }
	EDATATYPE m_eType;
	std::vector<TString> m_vParamValue;
};


struct SClassData
{
	SClassData()
		: m_eType(EDT_CLASSTYPE)
		, m_nCount(0)
	{

	}
	~SClassData()
	{
		BEATS_SAFE_DELETE_VECTOR(m_vMember);
	}
	bool find(SBaseData* pData);
	EDATATYPE m_eType;
	int m_nCount;
	TString m_strName;
	TString m_str;
	std::vector<SBaseData*> m_vMember;
};

struct SFileData
{
	SFileData()
	{

	}
	~SFileData()
	{
		BEATS_SAFE_DELETE_VECTOR(m_vClass);
		BEATS_SAFE_DELETE_VECTOR(m_vGlobal);
	}
	bool find(SBaseData* pData);
	TString m_strFilePath;
	std::vector<SClassData*> m_vClass;
	std::vector<SBaseData*> m_vGlobal;
};

class CCodeAnalysis
{
	BEATS_DECLARE_SINGLETON(CCodeAnalysis);
public:
	void Init(const std::vector<TString>& scanPathList);
    bool SaveToPkg(const TCHAR* pFileName);
    bool SaveToLua(const TCHAR* pFileName);

private:
    bool ScanKeyInFile(const TCHAR* pFileName);
    bool ScanKeyInDirectory(const TCHAR* pDirectory);
    // This method allows you scan a C++ file, which means it will handle "//" "/*" to filter comments content.
    int ScanKeyWordInCPlusPlusFile(const std::vector<const char*>& vKeyWords, CSerializer* fileSerializer);
    bool FilterCPlusPlusFileComments(const char* text, char* outBuffer, size_t& bufferLenth);
    bool ScanKey(const WIN32_FIND_DATA& fileData, const TString& fullDirectoryPath);
	bool ReadFunction(SBaseData*& pResult, CSerializer* fileSerializer);
	bool ReadVariable(SBaseData*& pResult, CSerializer* fileSerializer);
    bool ReadClass(SClassData*& pResult, CSerializer* fileSerializer, SFileData*& pFileData);
	bool ReadClassMember(SClassData*& pResult, CSerializer* fileSerializer, size_t startPos, SFileData*& pFileData);
	bool IsKeyword( CSerializer* pSerailizer, size_t length );
	std::vector<TString> Split(TString str,TString pattern);
	TString ReplaceString(TString str,TString pattern, TString goal);
	int GetReflectType(CSerializer* fileSerializer, size_t startPos, size_t endPos);
	
private:
    bool m_bInitFlag;
    std::map<TString, SFileData*> m_mStrPool;
	TString m_strClass;
};

#endif