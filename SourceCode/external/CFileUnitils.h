#ifndef CFILEUTIL_H_INCLUDE
#define CFILEUTIL_H_INCLUDE

#include "external/CData.h"
#include "external/CValue.h"

typedef std::vector<TString> TStringVector;

class CFileUtils
{
    BEATS_DECLARE_SINGLETON( CFileUtils );    
public:

    void PurgeCachedEntries();

    const TString& GetStringFormFile( const TString& fileName );

    CData GetDataFromFile( const TString& fileName );

    unsigned char* GetFileDataFormZip( const TString& zipFilePath, const TString& fileName, ssize_t* size );

    const TString& FullPathForFilename( const TString& fileName );

    void LoadFilenameLookupDictionaryFromFile( const TString& filename );

    void SetFilenameLookupDictionary( const TValueMap& filenameLookupDict );

    const TString& FullPathFromRelativeFile( const TString& filename, const TString& relativeFile );

    void SetSearchResolutionsOrder( const TStringVector& searchResolutionsOrder );

    void AddSearchResolutionsOrder( const TString& order );

    const TStringVector& GetSearchResolutionsOrder();

    void SetSearchPaths( const TStringVector& searchPaths );

    void AddSearchPath( const TString& searchpath );

    const TStringVector& GetSearchPaths() const;

    const TString& GetWritablePath();

    bool IsFileExist( const TString& filePath ) const;

    bool IsAbsolutePath( const TString& path ) const;

    void SetPopupNotify( bool notify );

    bool IsPopupNotify() const;

    const TValueMap& GetValueMapFromFile(const TString& filename) const;

    bool WriteToFile( TValueMap& dict, const TString& fullPath );

    const TValueVector& GetValueVectorFromFile( const TString& fileName );

    const std::unordered_map<TString, TString>& GetFullPathCache() const;

private:
    bool Init();

    const TString& GetNewFilename( const TString& fileName );

    const TString& GetPathForFilename( const TString& fileName, const TString& resolutionDirectory, const TString& searchPath );

    const TString& GetFullPathForDirectoryAndFileName( const TString& directory, const TString& filename );

    CData GetData(const TString& filename, bool forString);

    static void ConvertPathFormatToUnixStyle(TString& path);
private:

    TValueMap m_filenameLookupDict;

    TStringVector m_searchResolutionsOrderArray;

    TStringVector m_searchPathArray;

    TString m_defaultResRootPath;

    std::unordered_map<TString, TString> m_fullPathCache;

    TValueMap m_valueMapFromFile;

    bool m_popupNotify;

    TString m_writablePath;

    TString m_stringFromFile;
    TString m_fullPathFromName;
    TString m_fullPathFromRelativeFile;
    TString m_newFileName;
    TString m_pathForFileName;
    TString m_fullPathForDicAndName;
};

#endif//CFILEUNITIL_H_INCLUDE