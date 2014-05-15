#include "stdafx.h"
#include "CFileUnitils.h"

#include "external/unzip/unzip.h"
#include <Shlobj.h>

#define WIN32_MAX_PATH  512

CFileUtils* CFileUtils::m_pInstance = NULL;

CFileUtils::CFileUtils()
{
    m_popupNotify = true;
    Init();
}

CFileUtils::~CFileUtils()
{

}

void CFileUtils::PurgeCachedEntries()
{
    m_fullPathCache.clear();
}

const TString& CFileUtils::GetStringFormFile( const TString& fileName )
{
    m_stringFromFile = _T("");
    CData data = GetData( fileName, true);
    if ( !data.IsNull() )
        m_stringFromFile = (const TCHAR*)data.GetBytes();
    return m_stringFromFile;
}

CData CFileUtils::GetDataFromFile( const TString& fileName )
{
    return GetData( fileName, false );
}

unsigned char* CFileUtils::GetFileDataFormZip( const TString& zipFilePath, const TString& fileName, ssize_t* size )
{
    unsigned char * buffer = NULL;
    unzFile file = NULL;
    *size = 0;

    char *path = new char[ zipFilePath.length() ];
    CStringHelper::GetInstance()->ConvertToCHAR( zipFilePath.c_str(), path, zipFilePath.length() );

    char* name = new char[ fileName.length() ];
    CStringHelper::GetInstance()->ConvertToCHAR( fileName.c_str(), name, fileName.length() );

    do 
    {
        if ( zipFilePath.empty() )
        {
            break;
        }
        file = unzOpen( path);
        if ( !file )
        {
            break;
        }
        int ret = unzLocateFile( file, name, 1 );
        if( UNZ_OK != ret )
        {
            break;
        }

        char filePathA[260];
        unz_file_info fileInfo;
        ret = unzGetCurrentFileInfo(file, &fileInfo, filePathA, sizeof(filePathA), NULL, 0, NULL, 0);
        if( UNZ_OK != ret )
        {
            break;
        }
        ret = unzOpenCurrentFile(file);
        if( UNZ_OK != ret )
        {
            break;
        }
        buffer = (unsigned char*)malloc(fileInfo.uncompressed_size);
        int readedSize = 0;
        readedSize = unzReadCurrentFile(file, buffer, static_cast<unsigned>(fileInfo.uncompressed_size));
        BEATS_ASSERT(readedSize == 0 || readedSize == (int)fileInfo.uncompressed_size, _T("the file size is wrong"));

        *size = fileInfo.uncompressed_size;
        unzCloseCurrentFile(file);
    } while (0);


    if (file)
    {
        unzClose(file);
    }
    BEATS_SAFE_DELETE_ARRAY( path );
    BEATS_SAFE_DELETE_ARRAY( name );

    return buffer;
}

const TString& CFileUtils::FullPathForFilename( const TString& fileName )
{
    m_pathForFileName = _T("");
    if (IsAbsolutePath(fileName))
    {
        m_pathForFileName = fileName;
    }
    else
    {
        // Already Cached ?
        auto cacheIter = m_fullPathCache.find(fileName);
        if( cacheIter != m_fullPathCache.end() )
        {
            m_pathForFileName = cacheIter->second;
        }
        else
        {
            const TString& newFilename = GetNewFilename( fileName );

            TString fullpath;
            bool bBreak = false;
            for (auto searchIt = m_searchPathArray.cbegin(); searchIt != m_searchPathArray.cend(); ++searchIt)
            {
                for (auto resolutionIt = m_searchResolutionsOrderArray.cbegin(); resolutionIt != m_searchResolutionsOrderArray.cend(); ++resolutionIt)
                {
                    fullpath = GetPathForFilename(newFilename, *resolutionIt, *searchIt);

                    if (fullpath.length() > 0)
                    {
                        // Using the filename passed in as key.
                        m_fullPathCache.insert(std::pair<TString, TString>(fileName, fullpath));
                        m_pathForFileName = fullpath;
                        bBreak = true;
                        break;
                    }
                }
                if ( bBreak )
                {
                    break;
                }
                
            }
        }
        // Get the new file name.
        
        BEATS_PRINT(_T("fullPathForFilename: No file found at %s. Possible missing file."), fileName.c_str());

    }
    // XXX: Should it return nullptr ? or an empty string ?
    // The file wasn't found, return the file name passed in.
    return m_pathForFileName;
}

void CFileUtils::LoadFilenameLookupDictionaryFromFile( const TString& filename )
{
    const TString fullPath = FullPathForFilename(filename);
    if (fullPath.length() > 0)
    {
        TValueMap dict = GetValueMapFromFile(fullPath);
        if (!dict.empty())
        {
            TValueMap& metadata =  dict[_T("metadata")].AsValueMap();
            int version = metadata[_T("version")].AsInt();
            if (version != 1)
            {
                BEATS_ASSERT( false, _T("ERROR: Invalid filenameLookup dictionary version: %d. Filename: %s"), version, filename.c_str());
                return;
            }
            SetFilenameLookupDictionary( dict[_T("filenames")].AsValueMap());
        }
    }
}

const TValueMap& CFileUtils::GetValueMapFromFile( const TString& filename ) const
{
#if( FC_PLATFOEM != PLATFORM_IOS ) && ( FC_PLATFOEM != PLATFORM_MAC )
    //this func is not complent,the func is use to param xml
    return m_valueMapFromFile;
#else
    return m_valueMapFromFile;
#endif
}

void CFileUtils::SetFilenameLookupDictionary( const TValueMap& filenameLookupDict )
{
    m_fullPathCache.clear();
    m_filenameLookupDict = filenameLookupDict;
}

const TString& CFileUtils::FullPathFromRelativeFile( const TString& filename, const TString& relativeFile )
{
    m_fullPathFromRelativeFile = _T("");
    m_fullPathFromRelativeFile = relativeFile.substr(0, relativeFile.rfind(_T('/'))+1) + GetNewFilename(filename);
    return m_fullPathFromRelativeFile;
}

void CFileUtils::SetSearchResolutionsOrder( const TStringVector& searchResolutionsOrder )
{
    bool existDefault = false;
    m_fullPathCache.clear();
    m_searchResolutionsOrderArray.clear();
    for(auto iter = searchResolutionsOrder.cbegin(); iter != searchResolutionsOrder.cend(); ++iter)
    {
        TString resolutionDirectory = *iter;
        if (!existDefault && resolutionDirectory == _T(""))
        {
            existDefault = true;
        }

        if (resolutionDirectory.length() > 0 && resolutionDirectory[resolutionDirectory.length()-1] != _T('/'))
        {
            resolutionDirectory += _T("/");
        }

        m_searchResolutionsOrderArray.push_back(resolutionDirectory);
    }
    if (!existDefault)
    {
        m_searchResolutionsOrderArray.push_back( _T(""));
    }
}

void CFileUtils::AddSearchResolutionsOrder( const TString& order )
{
    TString resOrder = order;
    if (!resOrder.empty() && resOrder[resOrder.length()-1] != _T('/'))
        resOrder.append(_T("/"));

    m_searchResolutionsOrderArray.push_back(resOrder);
}

const TStringVector& CFileUtils::GetSearchResolutionsOrder()
{
    return m_searchResolutionsOrderArray;
}

void CFileUtils::SetSearchPaths( const TStringVector& searchPaths )
{
    bool existDefaultRootPath = false;

    m_fullPathCache.clear();
    m_searchPathArray.clear();
    for (auto iter = searchPaths.cbegin(); iter != searchPaths.cend(); ++iter)
    {
        TString prefix;
        TString path;

        if (!IsAbsolutePath(*iter))
        { // Not an absolute path
            prefix = m_defaultResRootPath;
        }
        path = prefix + (*iter);
        if (path.length() > 0 && path[path.length()-1] != '/')
        {
            path += _T("/");
        }
        if (!existDefaultRootPath && path == m_defaultResRootPath)
        {
            existDefaultRootPath = true;
        }
        m_searchPathArray.push_back(path);
    }

    if (!existDefaultRootPath)
    {
        //CCLOG("Default root path doesn't exist, adding it.");
        m_searchPathArray.push_back(m_defaultResRootPath);
    }
}

void CFileUtils::AddSearchPath( const TString& searchpath )
{
    TString prefix;
    if (!IsAbsolutePath(searchpath))
        prefix = m_defaultResRootPath;

    TString path = prefix + searchpath;
    if (path.length() > 0 && path[path.length()-1] != _T('/'))
    {
        path += _T("/");
    }
    m_searchPathArray.push_back(path);
}

const TStringVector& CFileUtils::GetSearchPaths() const
{
    return m_searchPathArray;
}

const TString& CFileUtils::GetWritablePath()
{
#if (FC_PLATFOEM == PLATFORM_WIN32)

    // Get full path of executable, e.g. c:\Program Files (x86)\My Game Folder\MyGame.exe
    TCHAR full_path[WIN32_MAX_PATH + 1];
    ::GetModuleFileName(NULL, full_path, WIN32_MAX_PATH + 1);

    // Debug app uses executable directory; Non-debug app uses local app data directory
#ifndef _DEBUG
    // Get filename of executable only, e.g. MyGame.exe
    char path[WIN32_MAX_PATH];
    CStringHelper::GetInstance()->ConvertToCHAR( full_path, path, WIN32_MAX_PATH );
    char *base_name = strrchr(path, '\\');

    if(base_name)
    {
        TCHAR app_data_path[WIN32_MAX_PATH + 1];

        // Get local app data directory, e.g. C:\Documents and Settings\username\Local Settings\Application Data
        if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, app_data_path)))
        {
            m_writablePath = (TCHAR*)app_data_path;

            TCHAR baseName[ WIN32_MAX_PATH ];
            CStringHelper::GetInstance()->ConvertToTCHAR( base_name, baseName, WIN32_MAX_PATH );
            // Adding executable filename, e.g. C:\Documents and Settings\username\Local Settings\Application Data\MyGame.exe
            m_writablePath += baseName;

            // Remove ".exe" extension, e.g. C:\Documents and Settings\username\Local Settings\Application Data\MyGame
            m_writablePath = m_writablePath.substr(0, m_writablePath.rfind(_T(".")));

            m_writablePath += _T("\\");

            // Create directory
            if (SUCCEEDED(SHCreateDirectoryEx(NULL, m_writablePath.c_str(), NULL)))
            {
                ConvertPathFormatToUnixStyle(m_writablePath);
            }
        }
    }
#else

    // If fetching of local app data directory fails, use the executable one
    TString temp =(TCHAR*)full_path;

    // remove xxx.exe
    m_writablePath =  temp.substr(0, temp.rfind(_T("\\")) + 1);

    ConvertPathFormatToUnixStyle(m_writablePath);
#endif

#else
    struct stat st;
    stat(m_writablePath.c_str(), &st);
    if (!S_ISDIR(st.st_mode)) {
        mkdir(m_writablePath.c_str(), 0744);
    }

#endif

    return m_writablePath;
}

bool CFileUtils::IsFileExist( const TString& filePath ) const
{
    bool bReturn = false;
#if (FC_PLATFOEM == PLATFORM_WIN32)
    if (0 != filePath.length())
    {
        TString strPath = filePath;
        if (!IsAbsolutePath(strPath))
        { // Not absolute path, add the default root path at the beginning.
            strPath.insert(0, m_defaultResRootPath);
        }

        bReturn = (GetFileAttributes(strPath.c_str()) != -1);
    }
#else
    if (0 != filePath.length())
    {
        TString strPath = filePath;
        if (!IsAbsolutePath(strPath))
        { // Not absolute path, add the default root path at the beginning.
            strPath.insert(0, _defaultResRootPath);
        }

        struct stat sts;
        bReturn = (stat(strPath.c_str(), &sts) != -1) ? true : false;
    }
#endif

    return bReturn;
}

void CFileUtils::ConvertPathFormatToUnixStyle( TString& path )
{
    int len = path.length();
    for (int i = 0; i < len; ++i)
    {
        if (path[i] == '\\')
        {
            path[i] = '/';
        }
    }
}

bool CFileUtils::IsAbsolutePath( const TString& path ) const
{
    bool bReturn = false;
#if ( FC_PLATFOEM == PLATFORM_WIN32 )
    if (   path.length() > 2 
        && ( (path[0] >= 'a' && path[0] <= 'z') || (path[0] >= 'A' && path[0] <= 'Z') )
        && path[1] == ':')
    {
        bReturn = true;
    }
#else
    bReturn = (path[0] == '/');
#endif
    return bReturn;
}

void CFileUtils::SetPopupNotify( bool notify )
{
    m_popupNotify = notify;
}

bool CFileUtils::IsPopupNotify() const
{
    return m_popupNotify;
}

bool CFileUtils::WriteToFile( TValueMap& dict, const TString& fullPath )
{
    //this func is not complate, the func use to write path info to a xlm document
    return false;
}

const std::unordered_map<TString, TString>& CFileUtils::GetFullPathCache() const
{
    return m_fullPathCache;
}

bool CFileUtils::Init()
{
#if ( FC_PLATFOEM == PLATFORM_WIN32 )
    if (0 == m_defaultResRootPath.length())
    {
        WCHAR utf16Path[MAX_PATH] = {0};
        GetCurrentDirectoryW(sizeof(utf16Path)-1, utf16Path);
        m_defaultResRootPath = utf16Path;
        ConvertPathFormatToUnixStyle(m_defaultResRootPath);
        m_defaultResRootPath.append(_T("/"));
    }
#else
    // get application path
    char fullpath[256] = {0};
    ssize_t length = readlink("/proc/self/exe", fullpath, sizeof(fullpath)-1);

    if (length > 0)
    {
        fullpath[length] = _T('\0');
        TCHAR wfullPath[MAX_PATH];
        CStringHelper::GetInstance()->ConvertToTCHAR( fullpath, wfullPath, MAX_PATH );
        TString appPath = wfullPath;
        m_defaultResRootPath = appPath.substr(0, appPath.find_last_of(_T("/")));
        m_defaultResRootPath += _T("/Resources/");

        // Set writable path to $XDG_CONFIG_HOME or ~/.config/<app name>/ if $XDG_CONFIG_HOME not exists.
        const char* xdg_config_path = getenv("XDG_CONFIG_HOME");
        std::string xdgConfigPath;
        if (xdg_config_path == NULL) {
            xdgConfigPath = getenv("HOME");
            xdgConfigPath += "/.config";
        } else {
            xdgConfigPath  = xdg_config_path;
        }
        std::string writablePath = xdgConfigPath;
        writablePath += appPath.substr(appPath.find_last_of("/"));
        writablePath += "/";

        TCHAR wrPath[ MAX_PATH ];
        CStringHelper::GetInstance()->ConvertToTCHAR( writablePath, wrPath, MAX_PATH );
        m_writablePath = wrPath;
    }
#endif
    m_searchPathArray.push_back(m_defaultResRootPath);
    m_searchResolutionsOrderArray.push_back(_T(""));
    return true;
}

const TString& CFileUtils::GetNewFilename( const TString& fileName )
{
    m_newFileName = _T("");
    // in Lookup Filename dictionary ?
    auto iter = m_filenameLookupDict.find(fileName);

    if (iter == m_filenameLookupDict.end())
    {
        m_newFileName = fileName;
    }
    else
    {
        m_newFileName = iter->second.AsString();
    }
    return m_newFileName;
}

const TString& CFileUtils::GetPathForFilename( const TString& fileName, const TString& resolutionDirectory, const TString& searchPath )
{
    TString strName = fileName;
    TString strDir = resolutionDirectory;
    TString strPath = searchPath;
#if( FC_PLATFOEM == PLATFORM_WIN32 )
    ConvertPathFormatToUnixStyle( strName );
    ConvertPathFormatToUnixStyle( strDir );
    ConvertPathFormatToUnixStyle( strPath );
#endif
    m_pathForFileName = _T("");
    TString file = strName;
    TString file_path = _T("");
    size_t pos = strName.find_last_of(_T("/"));
    if (pos != std::string::npos)
    {
        file_path = strName.substr(0, pos+1);
        file = strName.substr(pos+1);
    }

    // searchPath + file_path + resourceDirectory
    m_pathForFileName = strPath;
    m_pathForFileName += file_path;
    m_pathForFileName += strDir;

    m_pathForFileName = GetFullPathForDirectoryAndFileName(m_pathForFileName, file);

    //CCLOG("getPathForFilename, fullPath = %s", path.c_str());
    return m_pathForFileName;
}

const TString& CFileUtils::GetFullPathForDirectoryAndFileName( const TString& directory, const TString& filename )
{
    TString strDic = directory;
    TString strName = filename;
#if (FC_PLATFOEM == PLATFORM_WIN32 )

    ConvertPathFormatToUnixStyle(strDic);
    ConvertPathFormatToUnixStyle(strName);
#endif
    m_fullPathForDicAndName = strDic;
    if (strDic.size() && strDic[strDic.size()-1] != _T('/')){
        m_fullPathForDicAndName += _T('/');
    }
    m_fullPathForDicAndName += strName;

    // if the file doesn't exist, return an empty string
    if (!IsFileExist(m_fullPathForDicAndName))
    {
        m_fullPathForDicAndName = _T("");
    }
    return m_fullPathForDicAndName;
}

CData CFileUtils::GetData( const TString& filename, bool bForString )
{
    BEATS_ASSERT( !filename.empty(), _T("Invalid filename!"));
    CData outData;
#if ( FC_PLATFOEM != PLATFORM_WIN32 )
    unsigned char* buffer = NULL;
    ssize_t size = 0;
    const char* mode = NULL;
    if (bForString)
        mode = "rt";
    else
        mode = "rb";

    do
    {
        // Read the file from hardware
        TString fullPath = FullPathForFilename(filename);
        char path[ MAX_PATH ];
        CStringHelper::GetInstance()->ConvertToCHAR( fullPath.c_str(), path, fullPath.length() );
        FILE *fp = fopen( path, mode);
        if(!fp)
        {
            break;
        }
        fseek(fp,0,SEEK_END);
        size = ftell(fp);
        fseek(fp,0,SEEK_SET);

        if (bForString)
        {
            buffer = new unsigned char[ size + 1];
            buffer[size] = '\0';
        }
        else
        {
            buffer = new unsigned char[size];
        }

        size = fread(buffer, sizeof(unsigned char), size, fp);
        fclose(fp);
    } while (0);

    if (NULL == buffer || 0 == size)
    {
        DWORD errorCode = ::GetLastError();
        BEATS_ASSERT( false, _T("Get data from file \"%s\" failed"), filename.c_str() );
    }
    else
    {
        outData.FastSet(buffer, size);
    }
#else
    unsigned char *buffer = NULL;
    size_t size = 0;
    do
    {
        // read the file from hardware
        TString fullPath = CFileUtils::GetInstance()->FullPathForFilename(filename);

        WCHAR wszBuf[MAX_PATH] = {0};
        CStringHelper::GetInstance()->ConvertToWCHAR( fullPath.c_str(), wszBuf, fullPath.length() );

        HANDLE fileHandle = ::CreateFileW(wszBuf, GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL);
        if(fileHandle == INVALID_HANDLE_VALUE)
        {
            break;
        }
        size = ::GetFileSize(fileHandle, NULL);

        if (bForString)
        {
            buffer = new unsigned char[ size + 1];
            buffer[size] = '\0';
        }
        else
        {
            buffer = new unsigned char[ size ];
        }
        DWORD sizeRead = 0;
        BOOL successed = FALSE;
        successed = ::ReadFile(fileHandle, buffer, size, &sizeRead, NULL);
        ::CloseHandle(fileHandle);

        if (!successed)
        {
            BEATS_SAFE_DELETE_ARRAY(buffer);
        }
    } while (0);

    if (buffer == NULL || size == 0)
    {
        // Gets error code.
        DWORD errorCode = 0;
        errorCode = ::GetLastError();
        BEATS_ASSERT( false, _T("Get data from file \"%s\" failed, erroe code is %d"), filename.c_str(), errorCode );
    }
    else
    {
        outData.FastSet(buffer, size);
    }
#endif

    return outData;
}

// const TValueVector& CFileUtils::GetValueVectorFromFile( const TString& fileName )
// {
//     
// }
