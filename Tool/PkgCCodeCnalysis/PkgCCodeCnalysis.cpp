// PkgCCodeCnalysis.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include "PublicDef.h"
#include "windows.h"
#include "Utility/BeatsUtility/EnumStrGenerator.h"
#include "CodeAnalysis.h"
#include "Utility/BeatsUtility/Serializer.h"
#include "Utility/BeatsUtility/StringHelper.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR szPath[MAX_PATH]; 
	::GetCurrentDirectory(MAX_PATH,szPath);
	TString strPath = szPath;
	strPath.append( _T("\\pkgconfig.ini") );

	CSerializer tmp(strPath.c_str());
	tmp << 0;
	const char* pszData = (const char*)tmp.GetBuffer();
	size_t longth = tmp.GetBufferSize();

	TCHAR* tNameCharBuff = new TCHAR[longth + 1];
	tNameCharBuff[longth] = 0;
	CStringHelper::GetInstance()->ConvertToTCHAR(pszData, tNameCharBuff, longth + 1);

	std::vector<TString> filters;
	filters.push_back(_T("\r\n"));
	filters.push_back(_T("\t"));
	
	std::vector<TString> strParamString;
	TString strName = CStringHelper::GetInstance()->FilterString(tNameCharBuff, filters);
	BEATS_SAFE_DELETE_ARRAY( tNameCharBuff );

	CStringHelper::GetInstance()->SplitString(strName.c_str(), _T(";"), strParamString);
	std::vector<TString> path;
	for( size_t i = 2 ; i < strParamString.size(); ++i )
	{
		path.push_back( strParamString[i] );
	}

	CCodeAnalysis::GetInstance()->Init(path);
	CCodeAnalysis::GetInstance()->SaveToLua(strParamString[0].c_str());//_T("E:\\project\\FCEngine\\Tool\\test\\test.lua"));
	CCodeAnalysis::GetInstance()->SaveToPkg(strParamString[1].c_str());//_T("E:\\project\\FCEngine\\Tool\\test\\test.pkg"));
	return 0;
}

