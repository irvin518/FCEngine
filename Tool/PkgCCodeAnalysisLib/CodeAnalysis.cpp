
#include "stdafx.h"
#include "CodeAnalysis.h"
#include "../../SourceCode/Utility/BeatsUtility/Serializer.h"
#include "../../SourceCode/Utility/BeatsUtility/StringHelper.h"
#include <Shlwapi.h>

static const char* KEYWORD_STR = "REFLECT";
static const char* KEYWORD_START_STR = "{";
static const char* KEYWORD_END_STR = "}";
static const TCHAR* SCAN_DATA_NAME = _T("ScanData.bin");


static int CLASS_TYPE = 3;
static int STRUCT_TYPE = 4;
static int ENUM_TYPE = 5;

static int REFLECT_TYPE = 3;
static int REFLECT_FUNCTION = 4;
static int REFLECT_VARIABLE = 5;
static int REFLECT_START = 4;
static int REFLECT_END = 5;
static int EXEGESIS0 = 0;
static int EXEGESIS1 = 1;
static int EXEGESIS2 = 2;

static int functionCount = 0;
static int classCount = 0;
bool SClassData::find(SBaseData* pData)
{
	bool bRet = false;
	functionCount = 0;
	for( size_t i = 0 ; i < m_vMember.size(); ++i )
	{
		if (m_vMember[i]->m_eType == pData->m_eType && pData->m_strName.compare( m_vMember[i]->m_strName ) == 0)
		{
			bRet = true;
			++functionCount;
		}
	}
	return bRet;
}

bool SFileData::find(SBaseData* pData)
{
	bool bRet = false;
	functionCount = 0;
	for( size_t i = 0 ; i < m_vGlobal.size(); ++i )
	{
		if (m_vGlobal[i]->m_eType == pData->m_eType && pData->m_strName.compare( m_vGlobal[i]->m_strName ) == 0)
		{
			bRet = true;
			++functionCount;
		}
	}
	return bRet;
}

CCodeAnalysis* CCodeAnalysis::m_pInstance = NULL;

CCodeAnalysis::CCodeAnalysis()
	: m_bInitFlag(false)
{	
	m_strClass.append(_T("ClassObjectTypes = {\n"));
}

CCodeAnalysis::~CCodeAnalysis()
{
	typedef std::map<TString, SFileData*> TDataMap;
	BEATS_SAFE_DELETE_MAP(m_mStrPool, TDataMap);
}

void CCodeAnalysis::Init( const std::vector<TString>& scanPathList )
{
	TCHAR curWorkingPath[MAX_PATH];
	GetModuleFileName(NULL, curWorkingPath, MAX_PATH);
	PathRemoveFileSpec(curWorkingPath);
	TString fullPathStr(curWorkingPath);
	fullPathStr.append(_T("\\")).append(SCAN_DATA_NAME);
	for(size_t i = 0; i < scanPathList.size(); ++i)
	{
		ScanKeyInDirectory(scanPathList[i].c_str());
	}
	m_bInitFlag = true;
}

bool CCodeAnalysis::SaveToPkg( const TCHAR* pFileName )
{
	bool bRet = false;
	TString strResult;
	std::map<TString, SFileData*>::iterator iter = m_mStrPool.begin();
	for (; iter != m_mStrPool.end(); ++iter)
	{
		int _nNum = iter->second->m_vGlobal.size();
		if( _nNum > 0 )
		{
			TString data;
			for(int m = 0; m < _nNum; ++m)
			{
				data.append(iter->second->m_vGlobal[m]->m_str);
				data.append(_T("\n"));
			}
			strResult.append(data);
		}
		_nNum = iter->second->m_vClass.size();
		if( _nNum > 0 )
		{
			for (int i = 0; i < _nNum; ++i)
			{
				if( iter->second->m_vClass[i]->m_eType == EDT_CLASSTYPE )
				{	
					TString data;
					data.append(_T("class "));
					data.append(iter->second->m_vClass[i]->m_str);
					data.append(_T("\n{\n"));
					for(size_t j = 0; j < iter->second->m_vClass[i]->m_vMember.size(); ++j)
					{
						data.append(_T("\t"));
						data.append(iter->second->m_vClass[i]->m_vMember[j]->m_str);
						data.append(_T("\n"));
					}
					data.append(_T("};\n"));
					strResult.append(data);
				}
				else if( iter->second->m_vClass[i]->m_eType == EDT_ENUMTYPE )
				{
					strResult.append(iter->second->m_vClass[i]->m_str);
					strResult.append(_T("\n"));
				}
			}
		}
	}
	
	CSerializer serializer;
	serializer << strResult;
	size_t uBufferLength = serializer.GetWritePos();
	char* pBuffer = new char[uBufferLength];
	CStringHelper::GetInstance()->ConvertToCHAR((TCHAR*)serializer.GetBuffer(), pBuffer, uBufferLength);
	CSerializer ansicSerializer;
	ansicSerializer.SetBuffer((BYTE*)pBuffer, uBufferLength);
	ansicSerializer.SetWritePos(strlen(pBuffer));
	ansicSerializer.Deserialize(pFileName, _T("wt"));

	return bRet;
}

bool CCodeAnalysis::SaveToLua( const TCHAR* pFileName )
{
	bool bRet = false;
	TString strR;
	TString strResult;
	//strResult.append(_T("require ('oop')\n"));
	std::map<TString, SFileData*>::iterator iter = m_mStrPool.begin();
	m_strClass.append(_T("}\n"));
	strResult.append(_T("require ('oop')\n"));
	TString strInclude;
	strInclude.append(_T("outputInclude = [[/****************************/\n"));

	strInclude.append(_T("#include \"Reflect.h\"\n"));

	for (; iter != m_mStrPool.end(); ++iter)
	{

		TCHAR szOut[MAX_PATH] = _T("");
		PathRelativePathTo(szOut,
			pFileName,
			FILE_ATTRIBUTE_DIRECTORY,
			iter->second->m_strFilePath.c_str(),
			FILE_ATTRIBUTE_NORMAL);
		TString strRet = ReplaceString( szOut, _T("\\"), _T("/") );
		strInclude.append(_T("#include \""));
		strInclude.append(strRet);
		strInclude.append(_T("\"\n"));
		int _nNum = iter->second->m_vClass.size();
		if( _nNum > 0 )
		{
			for (int i = 0; i < _nNum ; ++i)
			{
				if (iter->second->m_vClass[i]->m_eType == EDT_CLASSTYPE)
				{
					TString strName = iter->second->m_vClass[i]->m_strName; 
					TString headName = strName;
					headName.append( _T("_c = class(") );
					headName.append( strName );
					headName.append(_T(")\n"));
					strResult.append(headName);
					for(size_t j = 0; j < iter->second->m_vClass[i]->m_vMember.size(); ++j)
					{
						if( iter->second->m_vClass[i]->m_vMember[j]->m_eType == EDT_FUNCTION )
						{
							TString funParam;
							funParam.append(_T("(") );
							for( size_t m = 0; m < iter->second->m_vClass[i]->m_vMember[j]->m_vParamValue.size() ; ++m )
							{
								if ( m > 0 )
								{
									funParam.append( _T(", ") );
								}
								funParam.append(iter->second->m_vClass[i]->m_vMember[j]->m_vParamValue[m]);
							}
							funParam.append( _T(")\n") );
							TString funcName = _T("function ");
							funcName.append(strName);
							funcName.append(_T("_c:"));
							if (iter->second->m_vClass[i]->m_vMember[j]->m_strNewName.empty() )
							{
								funcName.append(iter->second->m_vClass[i]->m_vMember[j]->m_strName);
							}
							else
							{
								funcName.append(iter->second->m_vClass[i]->m_vMember[j]->m_strNewName);
							}
							funcName.append(_T("_c"));
							funcName.append(funParam);
							funcName.append(_T("\t"));
							funcName.append(_T("self:"));
							funcName.append(iter->second->m_vClass[i]->m_vMember[j]->m_strName);
							funcName.append(funParam);
							funcName.append(_T("end\n"));
							strResult.append(funcName);
						}
					}
				}
			}
		}
	}	
	strInclude.append(_T("]]\n"));
	strR = strInclude + m_strClass + strResult;
	CSerializer serializer;
	serializer << strR;
	size_t uBufferLength = serializer.GetWritePos();
	char* pBuffer = new char[uBufferLength];
	CStringHelper::GetInstance()->ConvertToCHAR((TCHAR*)serializer.GetBuffer(), pBuffer, uBufferLength);
	CSerializer ansicSerializer;
	ansicSerializer.SetBuffer((BYTE*)pBuffer, uBufferLength);
	ansicSerializer.SetWritePos(strlen(pBuffer));
	ansicSerializer.Deserialize(pFileName, _T("wt"));

	return bRet;
}

bool CCodeAnalysis::ScanKeyInFile( const TCHAR* pFileName )
{
	bool bRet = false;
	CSerializer serializer;
    FILE* pFile;
	_tfopen_s(&pFile, pFileName, _T("rb"));
    if (pFile != NULL)
    {
        fseek(pFile, 0, SEEK_END);
        size_t fileSize = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);
        char* pData = new char[fileSize];
        fread(pData, fileSize, sizeof(BYTE), pFile);
        fclose(pFile);
		serializer.SetBuffer((const BYTE*)pData, fileSize);
		serializer.SetWritePos(fileSize);
		serializer.SetReadPos(0);
    }
	classCount = 0;
	SFileData* _pFileData = new SFileData();
	while (serializer.GetReadPos() != serializer.GetWritePos())
	{
		std::vector<const char*> vKeyWords;
		vKeyWords.push_back(KEYWORD_STR);
		int nRet = ScanKeyWordInCPlusPlusFile(vKeyWords, &serializer);
		if (nRet == REFLECT_TYPE) // class
		{
			size_t startPos = serializer.GetReadPos();
			int wordId = 0;
			SBufferData EndComment;
			EndComment.pData = "\r\n";
			EndComment.dataLength = strlen((char*)EndComment.pData) * sizeof(char);
			if (serializer.ReadToData(EndComment) != serializer.GetWritePos())
			{
				size_t nowPos = serializer.GetReadPos();
				int nResult = GetReflectType( &serializer, startPos, nowPos + wordId + 2);
				size_t _strLength = strlen(KEYWORD_STR);
				bool bIsKeyWord = IsKeyword(&serializer, _strLength);
				startPos += _strLength;
				serializer.SetReadPos(startPos);
				if ( nResult == CLASS_TYPE )
				{
					if ( bIsKeyWord )
					{
						SClassData* pData = new SClassData();
						if (ReadClass(pData, &serializer, _pFileData))
						{
							_pFileData->m_vClass.push_back(pData);
							bRet = true;
						}
						else
						{
							BEATS_SAFE_DELETE(pData);
						}
					}
				}
				else if (nResult == REFLECT_FUNCTION)
				{
					if ( bIsKeyWord )
					{
						SBaseData* pData = new SBaseData();
						if( ReadFunction(pData, &serializer))
						{
							if (_pFileData->find( pData ))
							{
								pData->m_strNewName = pData->m_strName;
								pData->m_strNewName.append((std::to_wstring(functionCount)));
							}
							else{
								pData->m_strNewName.clear();
							}
							_pFileData->m_vGlobal.push_back(pData);
							bRet = true;
						}
						else
						{
							BEATS_SAFE_DELETE(pData);
						}
					}
				}
				else
				{
					if ( bIsKeyWord )
					{
						SBaseData* pData = new SBaseData();
						if (ReadVariable(pData, &serializer))
						{
							_pFileData->m_vGlobal.push_back(pData);
							bRet = true;
						}
						else
						{
							BEATS_SAFE_DELETE(pData);
						}
					}
				}
			}		

		}
	}
	if ( bRet ) 
	{
		_pFileData->m_strFilePath.assign(pFileName);
		m_mStrPool[pFileName] = _pFileData;
	}
	else
	{
		BEATS_SAFE_DELETE( _pFileData );
	}
	return bRet;
}

bool CCodeAnalysis::ScanKeyInDirectory( const TCHAR* pDirectory )
{
	WIN32_FIND_DATA fileData;
	const TCHAR* pFileStr = _T("*.*");
	TString wildCmpStr(pDirectory);
	size_t pathSize = _tcslen(pDirectory);
	if (pDirectory[pathSize - 1] != '/')
	{
		wildCmpStr.append(_T("/"));
	}
	wildCmpStr.append(pFileStr);
	HANDLE searchHandle = ::FindFirstFile(wildCmpStr.c_str(), &fileData);
	bool bRet = false;
	if (searchHandle != INVALID_HANDLE_VALUE)
	{
		ScanKey(fileData, pDirectory);
		while (::FindNextFile(searchHandle, &fileData))
		{
			ScanKey(fileData, pDirectory);
		}
		FindClose(searchHandle);
		bRet = true;
	}

	BEATS_ASSERT(GetLastError() == ERROR_NO_MORE_FILES, _T("Scan code failed in path %s"), pDirectory);
	return bRet;
}

int CCodeAnalysis::ScanKeyWordInCPlusPlusFile(const std::vector<const char*>& vKeyWords, CSerializer* fileSerializer)
{
	std::vector<SBufferData> keyWords;
	SBufferData data;
	data.pData = "//";
	data.dataLength = strlen((char*)data.pData) * sizeof(char);
	keyWords.push_back(data);
	data.pData = "/*";
	data.dataLength = strlen((char*)data.pData) * sizeof(char);
	keyWords.push_back(data);

	SBufferData quoteSign;
	quoteSign.pData = "\"";
	quoteSign.dataLength = strlen((char*)quoteSign.pData) * sizeof(char);
	keyWords.push_back(quoteSign);

	for ( size_t i = 0 ; i < vKeyWords.size() ; ++i )
	{
		data.pData = (void*)vKeyWords[i];
		data.dataLength = strlen((char*)data.pData) * sizeof(char);
		keyWords.push_back(data);
	}

	SBufferData startComment;
	startComment.pData = "*/";
	startComment.dataLength = strlen((char*)startComment.pData) * sizeof(char);

	SBufferData changeLine;
	changeLine.pData = "\r\n";
	changeLine.dataLength = strlen((char*)changeLine.pData) * sizeof(char);

	int wordId = 0;
	while (fileSerializer->ReadToDataInList(keyWords, false, &wordId) != fileSerializer->GetWritePos())
	{
		if (wordId == EXEGESIS0) // Read "//"
		{
			fileSerializer->SetReadPos(fileSerializer->GetReadPos() + keyWords[wordId].dataLength);
			bool bReadToEnd = fileSerializer->ReadToData(changeLine) == fileSerializer->GetWritePos();//Read to end of line "/r/b"
			if (!bReadToEnd)
			{
				fileSerializer->SetReadPos(fileSerializer->GetReadPos() + changeLine.dataLength);
			}
		}
		else if (wordId == EXEGESIS1) // Read "/*"
		{
			fileSerializer->SetReadPos(fileSerializer->GetReadPos() + keyWords[wordId].dataLength);
			bool bReadToEnd = fileSerializer->ReadToData(startComment) == fileSerializer->GetWritePos();//Read to end of comment "*/"
			if (!bReadToEnd)
			{
				fileSerializer->SetReadPos(fileSerializer->GetReadPos() + startComment.dataLength);
			}
		}
		else if (wordId == EXEGESIS2)// Read " " "
		{
			fileSerializer->SetReadPos(fileSerializer->GetReadPos() + keyWords[wordId].dataLength);
			bool bReadToEnd = fileSerializer->ReadToData(quoteSign) == fileSerializer->GetWritePos();//Read to end of comment "*/"
			if (!bReadToEnd)
			{
				fileSerializer->SetReadPos(fileSerializer->GetReadPos() + quoteSign.dataLength);
			}
		}
		else  // Read goal
		{
			break;
		}
	}
	return wordId;
}

bool CCodeAnalysis::FilterCPlusPlusFileComments( const char* text, char* outBuffer, size_t& bufferLenth )
{
	bufferLenth = 0;
	const char* pReader = text;
	char* pWriter = outBuffer;
	while (*pReader != 0)
	{
		if (memcmp(pReader, "//", 2) == 0)
		{
			while (memcmp(pReader, "\r\n", 2) != 0)
			{
				++pReader;
			}
			pReader += 2;
		}
		else if (memcmp(pReader, "/*", 2) == 0)
		{
			while (memcmp(pReader, "*/", 2) != 0)
			{
				++pReader;
			}
			pReader += 2;
		}
		else
		{
			*(pWriter++) = *(pReader++);
			++bufferLenth;
		}
	}
	*pWriter = 0;
	return true;
}

bool CCodeAnalysis::ScanKey( const WIN32_FIND_DATA& fileData, const TString& fullDirectoryPath )
{
	if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0)
	{
		if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			const TCHAR* pReader = &fileData.cFileName[_tcslen(fileData.cFileName) - 4];
			if (memcmp(pReader + 2, _T(".h"), 2 * sizeof(TCHAR)) == 0)
			{
				TString fileFullPath = fullDirectoryPath;
				fileFullPath.append(_T("/")).append(fileData.cFileName);
				ScanKeyInFile(fileFullPath.c_str());
			}
		}
		else
		{
			if (_tcsicmp(fileData.cFileName, _T(".")) != 0 &&
				_tcsicmp(fileData.cFileName, _T("..")) != 0 )
			{
				TString fileFullPath = fullDirectoryPath;
				fileFullPath.append(_T("/")).append(fileData.cFileName);
				ScanKeyInDirectory(fileFullPath.c_str());
			}
		}
	}
	return true;
}

bool CCodeAnalysis::ReadFunction( SBaseData*& pResult, CSerializer* fileSerializer )
{
	bool bRet = false;
	if ( pResult && fileSerializer )
	{
		pResult->m_eType = EDT_FUNCTION;
		size_t startPos = fileSerializer->GetReadPos();
		std::vector<SBufferData> keyWords;
		SBufferData data;
		data.pData = ";";
		data.dataLength = strlen((char*)data.pData) * sizeof(char);
		keyWords.push_back(data);
		data.pData = "{";
		data.dataLength = strlen((char*)data.pData) * sizeof(char);
		keyWords.push_back(data);
		int wordId = 0;
		if (fileSerializer->ReadToDataInList(keyWords, false, &wordId) != fileSerializer->GetWritePos())
		{
			size_t endPos = fileSerializer->GetReadPos();
			size_t nameLength = endPos - startPos;
			char* funcName = new char[nameLength + 1];
			funcName[nameLength] = 0;
			fileSerializer->SetReadPos(startPos);
			fileSerializer->Deserialize(funcName, nameLength);
			char* funcNewName = new char[nameLength + 1];
			funcNewName[nameLength] = 0;
			FilterCPlusPlusFileComments(funcName, funcNewName, nameLength);
			BEATS_ASSERT(fileSerializer->GetReadPos() == endPos);
			TCHAR* tNameCharBuff = new TCHAR[nameLength + 1];
			tNameCharBuff[nameLength] = 0;
			CStringHelper::GetInstance()->ConvertToTCHAR(funcNewName, tNameCharBuff, nameLength + 1);
			std::vector<TString> filters;
			filters.push_back(_T("\r\n"));
			TString strName = CStringHelper::GetInstance()->FilterString(tNameCharBuff, filters);
			if (wordId == 1)
			{
				int _nCount = 0;
				for ( size_t j =  0; j <strName.length() ; ++j )
				{
					if ( strName[j] == '(' )
					{
						_nCount++;
					}
					if ( strName[j] == ')' )
					{
						_nCount--;
					}
					if ( strName[j] == ')' && _nCount == 0 )
					{
						_nCount = j+1;
						break;
					}
				}
				TString strTemp = strName.substr( 0, _nCount );
				strName = strTemp;
			}
			strName.append(_T(";"));
			pResult->m_str = strName;
			BEATS_SAFE_DELETE_ARRAY(tNameCharBuff);
			BEATS_SAFE_DELETE_ARRAY(funcName);
			BEATS_SAFE_DELETE_ARRAY(funcNewName);
			std::vector<TString> strParamString;
			TString strReplace = ReplaceString( strName, _T("\t"), _T(" ") );
 			
			ReplaceString( strName, _T("\t"), _T(",") );

			strParamString = Split(strName, _T(",") );

			int _nLongth = strParamString.size();
			for (int i = 0; i < _nLongth; ++i)
			{
				TString strRet;
				std::vector<TString> strParamVariableString;
				size_t nResult3 = strParamString[i].find('=');
				if (nResult3 < strName.length())
				{
					strParamString[i] = strParamString[i].substr( 0, nResult3-1);
				}
				strParamVariableString = Split(strParamString[i], _T(" ") );
				strRet = strParamVariableString[strParamVariableString.size()-1];
				if ( i == 0 )
				{   // Read Funciont Name
					int _nPosEnd = 0;
					int _nPosStart = 0;
					int _nCount = _nLongth == 1 ? 1 : 0;
					int _nEndNum = 1;
					TString _strName = strParamString[0];
					for ( int j = _strName.length() - 1 ; j >= 0 ; --j )
					{
						if ( _strName[j] == ')' )
						{
							_nCount--;
						}
						if ( _strName[j] == '(' )
						{
							_nCount++;
						}
						if ( _strName[j] == '(' && _nCount == _nEndNum && _nPosEnd == 0 )
						{
							_nPosEnd = j;
						}
						if ( _nCount == _nEndNum && _strName[j] == ' ' )
						{
							_nPosStart = j+1;
							break;
						}
					}
					std::vector<TString> filters;
					filters.push_back(_T("*"));
					filters.push_back(_T("&"));					
					filters.push_back(_T("\t"));
					filters.push_back(_T("\r\n"));
					filters.push_back(_T(" "));
					TString strName = _strName.substr( _nPosStart, (_nPosEnd - _nPosStart ) );
					pResult->m_strName = CStringHelper::GetInstance()->FilterString(strName.c_str(), filters);
				}
				if ( strRet.compare( _T(");") ) == 0 )
				{   // last param is block off by " "
					std::vector<TString>::iterator findIte = find(strParamVariableString.begin(), strParamVariableString.end(), _T("="));
					if ( findIte != strParamVariableString.end())
					{
						findIte--;
						strRet = *(findIte);
					}
					else
					{
						strRet = strParamVariableString[strParamVariableString.size()-2];
					}
				} 
				else if ( i == _nLongth - 1 )
				{
					if( strRet.find( _T("()")) < strRet.length() )
					{   // no param
						strRet = _T("");
					}
					else if ( _nLongth == 1 )
					{
						strRet = _T("");
					}
					else
					{   // the last param
						strRet[strRet.length() - 2] = '\0';
					}
				} 
				else if (strRet.compare( _T("") ) == 0)
				{
					strRet = strParamVariableString[strParamVariableString.size()-2];
				}
				
				//Read Function param
				std::vector<TString> filters;
				filters.push_back(_T("*"));
				filters.push_back(_T("&"));
				filters.push_back(_T("\t"));
				filters.push_back(_T("\r\n"));
				filters.push_back(_T(" "));
				TString strName = CStringHelper::GetInstance()->FilterString(strRet.c_str(), filters);
				size_t nResult2 = strName.find('=');
				if (nResult2 < strName.length())
				{
					TString strNewName = strName.substr( 0, nResult2 );
					pResult->m_vParamValue.push_back(strNewName);
				}
				else
				{
					pResult->m_vParamValue.push_back(strName);	
				}
			}
			bRet = true;
		}
		else
		{
			BEATS_ASSERT(false,_T("syntactic error!"));
		}
	}
	return bRet;
}

bool CCodeAnalysis::ReadVariable( SBaseData*& pResult, CSerializer* fileSerializer )
{
	bool bRet = false;
	if ( pResult && fileSerializer )
	{
		pResult->m_eType = EDT_PROPERTY;
		size_t startPos = fileSerializer->GetReadPos();
		SBufferData EndComment;
		EndComment.pData = ";";
		EndComment.dataLength = strlen((char*)EndComment.pData) * sizeof(char);
		bool bReadToEnd = fileSerializer->ReadToData(EndComment) == fileSerializer->GetWritePos();//Read to end of Variable ";"
		BEATS_ASSERT(!bReadToEnd,_T("syntactic error!"));
		if (!bReadToEnd)
		{
			fileSerializer->SetReadPos(fileSerializer->GetReadPos() + EndComment.dataLength);
			size_t endPos = fileSerializer->GetReadPos();
			size_t classNameLength = endPos - startPos;
			char* className = new char[classNameLength + 1];
			className[classNameLength] = 0;
			fileSerializer->SetReadPos(startPos);
			fileSerializer->Deserialize(className, classNameLength);
			char* classNewName = new char[classNameLength + 1];
			classNewName[classNameLength] = 0;
			FilterCPlusPlusFileComments(className, classNewName, classNameLength);
			BEATS_ASSERT(fileSerializer->GetReadPos() == endPos);
			TCHAR* tNameCharBuff = new TCHAR[classNameLength + 1];
			tNameCharBuff[classNameLength] = 0;
			CStringHelper::GetInstance()->ConvertToTCHAR(classNewName, tNameCharBuff, classNameLength + 1);
			std::vector<TString> filters;
			filters.push_back(_T("\r\n"));
			TString strName = CStringHelper::GetInstance()->FilterString(tNameCharBuff, filters);
			pResult->m_str = strName;
			bRet = true;

			BEATS_SAFE_DELETE_ARRAY(tNameCharBuff);
			BEATS_SAFE_DELETE_ARRAY(classNewName);
			BEATS_SAFE_DELETE_ARRAY(className);
		}
	}
	return bRet;
}

bool CCodeAnalysis::ReadClass( SClassData*& pResult, CSerializer* fileSerializer, SFileData*& pFileData )
{
	bool bRet = false;
	if ( pResult && fileSerializer )
	{
		std::vector<const char*> vKeyWords;
		size_t beginPos = fileSerializer->GetReadPos();
		vKeyWords.push_back("class");
		vKeyWords.push_back("struct");
		vKeyWords.push_back("enum");
		int nRet = ScanKeyWordInCPlusPlusFile(vKeyWords, fileSerializer);
		if (nRet == CLASS_TYPE) //class
		{
			size_t _strLength = strlen("class");
			bool bIsKeyWord = IsKeyword(fileSerializer, _strLength);
			size_t startPos = fileSerializer->GetReadPos();
			startPos += _strLength;
			fileSerializer->SetReadPos(startPos);

			pResult->m_eType = EDT_CLASSTYPE;
			bRet = ReadClassMember(pResult, fileSerializer, startPos, pFileData);	
			m_strClass.append( _T("\t\"") );
			m_strClass.append(pResult->m_strName);
			m_strClass.append( _T("\",\n") );
		}
		else if (nRet == STRUCT_TYPE) //struct is same to class
		{
			size_t _strLength = strlen("struct");
			bool bIsKeyWord = IsKeyword(fileSerializer, _strLength);
			size_t startPos = fileSerializer->GetReadPos();
			startPos += _strLength;
			fileSerializer->SetReadPos(startPos);

			pResult->m_eType = EDT_CLASSTYPE;
			bRet = ReadClassMember(pResult, fileSerializer, startPos, pFileData);
		}
		else if (nRet == ENUM_TYPE)  //enum 
		{
			size_t _strLength = strlen("enum");
			bool bIsKeyWord = IsKeyword(fileSerializer, _strLength);
			size_t startPos = fileSerializer->GetReadPos();
			startPos += _strLength;
			fileSerializer->SetReadPos(startPos);

			pResult->m_eType = EDT_ENUMTYPE;
			SBufferData EndComment;
			EndComment.pData = ";";
			EndComment.dataLength = strlen((char*)EndComment.pData) * sizeof(char);
			bool bReadToEnd = fileSerializer->ReadToData(EndComment) == fileSerializer->GetWritePos();//Read to end of Variable ";"
			BEATS_ASSERT(!bReadToEnd,_T("syntactic error!"));
			if (!bReadToEnd)
			{
				fileSerializer->SetReadPos(fileSerializer->GetReadPos() + EndComment.dataLength);
				size_t endPos = fileSerializer->GetReadPos();
				size_t classNameLength = endPos - beginPos;
				char* className = new char[classNameLength + 1];
				className[classNameLength] = 0;
				fileSerializer->SetReadPos(beginPos);
				fileSerializer->Deserialize(className, classNameLength);
				char* classNewName = new char[classNameLength + 1];
				classNewName[classNameLength] = 0;
				FilterCPlusPlusFileComments(className, classNewName, classNameLength);
				BEATS_ASSERT(fileSerializer->GetReadPos() == endPos);
				TCHAR* tNameCharBuff = new TCHAR[classNameLength + 1];
				tNameCharBuff[classNameLength] = 0;
				CStringHelper::GetInstance()->ConvertToTCHAR(classNewName, tNameCharBuff, classNameLength + 1);

				pResult->m_str = tNameCharBuff;
				bRet = true;

				BEATS_SAFE_DELETE_ARRAY(tNameCharBuff);
				BEATS_SAFE_DELETE_ARRAY(classNewName);
				BEATS_SAFE_DELETE_ARRAY(className);
			}
		}
	}
	return bRet;
}

bool CCodeAnalysis::ReadClassMember(SClassData*& pResult, CSerializer* fileSerializer, size_t startPos, SFileData*& pFileData)
{
	bool bRet = false;
	SBufferData EndComment;
	EndComment.pData = "{";
	int nCount = 0;
	EndComment.dataLength = strlen((char*)EndComment.pData) * sizeof(char);
	bool bReadToEnd = fileSerializer->ReadToData(EndComment) == fileSerializer->GetWritePos();//Read to end of Variable ";"
	if (!bReadToEnd)
	{
		size_t endPos = fileSerializer->GetReadPos();
		size_t classNameLength = endPos - startPos;
		char* className = new char[classNameLength + 1];
		className[classNameLength] = 0;
		fileSerializer->SetReadPos(startPos);
		fileSerializer->Deserialize(className, classNameLength);
		char* classNewName = new char[classNameLength + 1];
		classNewName[classNameLength] = 0;
		FilterCPlusPlusFileComments(className, classNewName, classNameLength);
		BEATS_ASSERT(fileSerializer->GetReadPos() == endPos);
		TCHAR* tNameCharBuff = new TCHAR[classNameLength + 1];
		tNameCharBuff[classNameLength] = 0;
		CStringHelper::GetInstance()->ConvertToTCHAR(classNewName, tNameCharBuff, classNameLength + 1);
		std::vector<TString> filters;				
		filters.push_back(_T("\t"));
		filters.push_back(_T("\r\n"));
		pResult->m_str = CStringHelper::GetInstance()->FilterString(tNameCharBuff, filters);
		bRet = true;
		if ( pResult->m_str.find(':') < pResult->m_str.length() )
		{
			std::vector<TString> strParamString;
			//CStringHelper::GetInstance()->SplitString(tNameCharBuff, _T(":"), strParamString);
			TString strNameBuff = tNameCharBuff;
			strParamString = Split( strNameBuff, _T(":") );
			std::vector<TString> filters;
			filters.push_back(_T("\r\n"));
			filters.push_back(_T("\t"));
			filters.push_back(_T(" "));
			TString strName = CStringHelper::GetInstance()->FilterString(strParamString[0].c_str(), filters);
			pResult->m_strName = strName;
		}
		else
		{
			std::vector<TString> filters;
			filters.push_back(_T("\r\n"));
			filters.push_back(_T(" "));
			filters.push_back(_T("\t"));
			TString strName = CStringHelper::GetInstance()->FilterString(tNameCharBuff, filters);
			pResult->m_strName = strName;
		}
		BEATS_SAFE_DELETE_ARRAY(tNameCharBuff);
		BEATS_SAFE_DELETE_ARRAY(classNewName);
		BEATS_SAFE_DELETE_ARRAY(className);
		while ( fileSerializer->GetReadPos() != fileSerializer->GetWritePos() ) 
		{
			std::vector<const char*> vKeyWords;
			vKeyWords.push_back(KEYWORD_STR);
			vKeyWords.push_back(KEYWORD_START_STR);
			vKeyWords.push_back(KEYWORD_END_STR);
			int nRet = ScanKeyWordInCPlusPlusFile(vKeyWords, fileSerializer);
			if ( nRet == REFLECT_TYPE )
			{
				size_t startPos = fileSerializer->GetReadPos();
				int wordId = 0;
				std::vector<SBufferData> keyWords;
				SBufferData data;
				data.pData = "\n";
				data.dataLength = strlen((char*)data.pData) * sizeof(char);
				keyWords.push_back(data);
				data.pData = "\r\n";
				data.dataLength = strlen((char*)data.pData) * sizeof(char);
				keyWords.push_back(data);
				if (fileSerializer->ReadToDataInList(keyWords, false, &wordId) != fileSerializer->GetWritePos())
				{
					size_t nowPos = fileSerializer->GetReadPos();
					int nResult = GetReflectType( fileSerializer, startPos, nowPos + wordId + 1);				
					size_t _strLength = strlen(KEYWORD_STR);
					bool bIsKeyWord = IsKeyword(fileSerializer, _strLength);
					startPos += _strLength;
					fileSerializer->SetReadPos(startPos);
					if ( nResult == CLASS_TYPE )
					{
						if ( bIsKeyWord )
						{
							SClassData* pData = new SClassData();
							pResult->m_nCount++;
							if (ReadClass(pData, fileSerializer, pFileData))
							{
								pFileData->m_vClass.push_back(pData);
								bRet = true;
							}
							else
							{
								BEATS_SAFE_DELETE(pData);
							}
						}
					}
					else if (nResult == REFLECT_FUNCTION)
					{
						if ( bIsKeyWord )
						{
							SBaseData* pData = new SBaseData();
							if( ReadFunction(pData, fileSerializer))
							{
								if (pResult->find( pData ))
								{
									pData->m_strNewName = pData->m_strName;
									pData->m_strNewName.append((std::to_wstring(functionCount)));
								}
								else{
									pData->m_strNewName.clear();
								}
								pResult->m_vMember.push_back(pData);
								bRet = true;
							}
							else
							{
								BEATS_SAFE_DELETE(pData);
							}
						}
					}
					else
					{
						if ( bIsKeyWord )
						{
							SBaseData* pData = new SBaseData();
							if (ReadVariable(pData, fileSerializer))
							{
								pResult->m_vMember.push_back(pData);
								bRet = true;
							}
							else
							{
								BEATS_SAFE_DELETE(pData);
							}
						}
					}	
				}
			}
			else if ( nRet == REFLECT_END )
			{
				nCount--;
				if (nCount == 0)
				{
					if (pResult->m_nCount == 0)
					{
						pResult->m_nCount--;
						break;
					}
				}
				size_t _strLength = strlen(KEYWORD_END_STR);
				size_t startPos = fileSerializer->GetReadPos();
				startPos += _strLength;
				fileSerializer->SetReadPos(startPos);
			}
			else if ( nRet == REFLECT_START )
			{
				nCount++;
				size_t _strLength = strlen(KEYWORD_START_STR);
				size_t nPos = fileSerializer->GetReadPos();
				nPos += _strLength;
				fileSerializer->SetReadPos(nPos);
			}
		} 
	}
	return bRet;
}

bool CCodeAnalysis::IsKeyword( CSerializer* pSerailizer, size_t length )
{
	bool bRet = true;
	//Check front.
	if (pSerailizer->GetReadPos() > 1)
	{
		BYTE byteData = pSerailizer->GetBuffer()[pSerailizer->GetReadPos() - 1];
		bRet = !(byteData != '\n' && byteData != ' ' && byteData != '\t');
	}
	if (bRet)
	{
		//Check after, there must be some characters after keywords.
		BYTE byteData = pSerailizer->GetBuffer()[pSerailizer->GetReadPos() + length];
		bRet = !(byteData != '\n' && byteData != ' ' && byteData != '\t' && byteData != '{');
	}
	return bRet;
}


//function for Spliting string
std::vector<TString> CCodeAnalysis::Split(TString str,TString pattern)
{
	size_t pos;
	std::vector<TString> result;
	str+=pattern;
	size_t size=str.size();

	for(size_t i=0; i<size; i++)
	{
		pos=str.find(pattern,i);
		if(pos<size)
		{
			TString s=str.substr(i,pos-i);
			result.push_back(s);
			i=pos+pattern.size()-1;
		}
	}
	return result;
}

TString CCodeAnalysis::ReplaceString(TString str,TString pattern, TString goal)
{
	size_t pos;
	TString result;
	str+=pattern;
	size_t size=str.size();

	for(size_t i=0; i<size; i++)
	{
		pos=str.find(pattern,i);
		if(pos<size)
		{
			TString s=str.substr(i,pos-i);
			result.append(s);
			if ( pos < size - 1 )
			{
				result.append(goal);
			}
			i=pos+pattern.size()-1;
		}
	}
	return result;
}

int CCodeAnalysis::GetReflectType( CSerializer* fileSerializer, size_t startPos, size_t endPos )
{
	size_t nameLength = endPos - startPos;
	char* strNameBuff = new char[nameLength + 1];
	strNameBuff[nameLength] = 0;
	fileSerializer->SetReadPos(startPos);
	fileSerializer->Deserialize(strNameBuff, nameLength);
	char* steNewName = new char[nameLength + 1];
	steNewName[nameLength] = 0;
	FilterCPlusPlusFileComments(strNameBuff, steNewName, nameLength);

	TCHAR* tNameCharBuff = new TCHAR[nameLength + 1];
	tNameCharBuff[nameLength] = 0;
	CStringHelper::GetInstance()->ConvertToTCHAR(steNewName, tNameCharBuff, nameLength + 1);
	TString tBuff = tNameCharBuff;
	BEATS_SAFE_DELETE_ARRAY(tNameCharBuff);
	BEATS_SAFE_DELETE_ARRAY(steNewName);
	BEATS_SAFE_DELETE_ARRAY(strNameBuff);
	fileSerializer->SetReadPos(startPos);
	int iRet = REFLECT_VARIABLE;
	if ( tBuff.find( _T("class") ) < tBuff.length() ||
		tBuff.find( _T("struct") ) < tBuff.length() ||
		tBuff.find( _T("enum") ) < tBuff.length() )
	{
		iRet = REFLECT_TYPE;
	}
	else if ( tBuff.find( _T("(") ) < tBuff.length() && tBuff.find( _T(")") ) < tBuff.length() )
	{
		iRet = REFLECT_FUNCTION;
	}
	return iRet;
}