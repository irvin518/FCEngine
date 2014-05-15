#ifndef  SKELETALANIMATION_CBONEDATA_H__INCLUDE
#define SKELETALANIMATION_CBONEDATA_H__INCLUDE
namespace _2DSkeletalAnimation
{
    enum EDisplayType
    {
        eST_CS_DISPLAY_SPRITE,
        eST_CS_DISPLAY_ARMATURE,
        eST_CS_DISPLAY_PARTICLE,

        eST_CS_DISPLAY_MAX,
    };

    struct SSkinData
    {
        float x;
        float y;
        float skewX;
        float skewY;
        float scaleX;
        float scaleY;
    };

    struct SDisplayData
    {
        EDisplayType type;
        std::string name;
        SSkinData skindata;
    };

    class  CBoneData
    {
    public:
        CBoneData();
        ~CBoneData();

        const std::string& GetName() const;
        void  SetName(const std::string& name);
        const std::string& GetParentName() const;
        void  SetParentName(const std::string& name);

        float GetPositionX() const;
        void SetPositionX(float x);
        float GetPositionY() const;
        void SetPositionY(float y);

        float GetSkewX() const;
        void SetSkewX(float skewX);
        float GetSkewY() const;
        void SetSkewY(float skewY);

        float GetScaleX() const;
        void SetScaleX(float scaleX);
        float GetScaleY() const;
        void SetScaleY(float scaleY);

       int GetZOrder() const;
       void SetZOrder(int zOrder);

       const std::vector<SDisplayData>& GetDisplayDataList() const;
       void AddDisplayData(const SDisplayData& data);
       SDisplayData ReplaceDisplayData(const SDisplayData& displayData, size_t index = 0);
    private:
        std::string m_strName;
        std::string m_parentName;
        float m_fX;	
        float m_fY;
        int m_iZOrder;
        float m_fSkewX;
        float m_fSkewY;
        float m_fScaleX;
        float m_fScaleY;
        std::vector<SDisplayData> m_displayDataList;
    };

}
#endif//SKELETALANIMATION_CBONEDATA_H__INCLUDE
