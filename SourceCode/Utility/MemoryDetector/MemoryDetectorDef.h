#ifndef FCENGINETOOL_MEMORYDETECTOR_MEMORYDETECTORDEF_H__INCLUDE
#define FCENGINETOOL_MEMORYDETECTOR_MEMORYDETECTORDEF_H__INCLUDE

struct SMemoryRecord
{
    void* m_pAddress;
    size_t m_size;
    size_t m_allocId;
    size_t m_freeId;
    size_t m_allocTime;
    size_t m_freeTime;
    size_t m_usage;
    bool m_isFree;

    SMemoryRecord(void* address, size_t size, size_t allocTime, size_t allocId, size_t usage = 0)
        : m_pAddress(address)
        , m_size(size)
        , m_allocId(allocId)
        , m_freeId(0)
        , m_allocTime(allocTime)
        , m_freeTime(0)
        , m_usage(usage)
        , m_isFree(false)
    {

    }

    SMemoryRecord()
        : m_pAddress(NULL)
        , m_size(0)
        , m_allocId(0)
        , m_freeId(0)
        , m_allocTime(0)
        , m_freeTime(0)
        , m_usage(0)
        , m_isFree(false)
    {

    }

    void Init(void* address, size_t size, size_t allocTime, size_t allocId, size_t usage = 0)
    {
        m_pAddress = address;
        m_size = size;
        m_allocId = allocId;
        m_freeId = 0;
        m_allocTime = allocTime;
        m_freeTime = 0;
        m_usage = usage;
        m_isFree = false;
    }
};

// This structure stores information, primarily the virtual address range, about
// a given module and can be used with the Set template because it supports the
// '<' operator (sorts by virtual address range).
struct SModuleInfo {
    BOOL operator < (const struct SModuleInfo &other) const
    {
        return addrhigh < other.addrlow ? TRUE : FALSE;
    }

    SIZE_T addrhigh;                 // Highest address within the module's virtual address space (i.e. base + size).
    SIZE_T addrlow;                  // Lowest address within the module's virtual address space (i.e. base address).
};

typedef std::map<void*, SMemoryRecord*> TRecordMap;
typedef std::map<size_t, std::set<SMemoryRecord*>> TRecordU32Map;

enum EMemoryFrameRecordType
{
    eMFRT_All,
    eMFRT_Address,
    eMFRT_Location,
    eMFRT_Size,
    eMFRT_Time,

    eMFRT_Count,
    eMFRT_Force32Bit = 0xFFFFFFFF
};

struct SMemoryFrameRecord
{
    TRecordMap recordMap;

    //for fast view
    TRecordU32Map recordMapForLocation;
    TRecordU32Map recordMapForSize;
    TRecordU32Map recordMapForTime;
    
    EMemoryFrameRecordType type;
    size_t                 totalItemCount;
    size_t                 startPos; //Available when this frame record is just a part of real one.
    SMemoryFrameRecord()
        : type(eMFRT_Address)
        , totalItemCount(0)
        , startPos(0)
    {

    }
    void Clear()
    {
        recordMap.clear();
        recordMapForLocation.clear();
        recordMapForSize.clear();
        recordMapForTime.clear();
    }
    bool IsEmpty()
    {
        bool bEmpty = false;
        switch (type)
        {
        case eMFRT_All:
        case eMFRT_Address:
            bEmpty = recordMap.size() == 0;
            break;
        case eMFRT_Location:
            bEmpty = recordMapForLocation.size() == 0;
            break;
        case eMFRT_Size:
            bEmpty = recordMapForSize.size() == 0;
            break;
        case eMFRT_Time:
            bEmpty = recordMapForTime.size() == 0;
            break;
        default:
            BEATS_ASSERT(false, _T("Never reach here!"));
            break;
        }
        return bEmpty;
    }
    SMemoryFrameRecord& operator= (const SMemoryFrameRecord& rRef)
    {
        recordMap = rRef.recordMap;
        recordMapForLocation = rRef.recordMapForLocation;
        recordMapForSize = rRef.recordMapForSize;
        recordMapForTime = rRef.recordMapForTime;
        return *this;
    }

};

#define R2VA(modulebase, rva)  (((PBYTE)modulebase) + rva) // Relative Virtual Address to Virtual Address conversion.

#endif