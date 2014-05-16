#ifndef FCENGINEEDITOR_RESOURCEShlwapiMANAGER_H__INCLUDE
#define FCENGINEEDITOR_RESOURCEShlwapiMANAGER_H__INCLUDE

class CResourceShlwapiManager
{
    BEATS_DECLARE_SINGLETON(CResourceShlwapiManager);
public:
    const TCHAR* PathFindExtension(const TCHAR* pszPath);
    const TCHAR* PathFindFileName(const TCHAR* pszPath);
    BOOL IsPathFileExists(const TCHAR* pszPath);
private:
    TString m_strResultCache;
};

#endif