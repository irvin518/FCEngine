#ifndef ENGINECENTER_H__INCLUDE
#define ENGINECENTER_H__INCLUDE

class CEngineCenter
{
    BEATS_DECLARE_SINGLETON(CEngineCenter);
public:
    bool Initialize(size_t uWindowWidth = 0xFFFFFFFF, size_t uWindowHeight = 0xFFFFFFFF);
    void Update();
    void SetFPS(size_t uFramePerSecond);
    void IncreaseUpdateCounter();
    bool ShouldUpdateThisFrame();
    size_t GetDeltaTime() const; 
private:
    size_t m_uFPS;
    size_t m_uFrameTime;
    size_t m_uDeltaTimeForThisUpdate;
    size_t m_uShouldUpdateCounter;
    size_t m_uUpdateCounter;
    size_t m_uMakeUpTime;
    LARGE_INTEGER m_uTimeFrequency;
    LARGE_INTEGER m_uCurrentTime;
    LARGE_INTEGER m_uLastUpdateTime;
};

#endif