#include "stdafx.h"
#include "CAnimationImporter.h"
#include "CAnimationManager.h"
#include "CArmatureData.h"
#include "CAnimationData.h"
#include "CTextureData.h"
#include "CBoneData.h"
#include "CMoveData.h"
#include "Json.h"
#include "Utility/BeatsUtility/StringHelper.h"

static const char *VERSION = "version";
static const char *ARMATURES = "armatures";
static const char *ARMATURE = "armature";
static const char *BONE = "b";
static const char *DISPLAY = "d";
static const char *ANIMATIONS = "animations";
static const char *ANIMATION = "animation";
static const char *MOVEMENT = "mov";
static const char *FRAME = "f";
static const char *TEXTURE_ATLAS = "TextureAtlas";
static const char *SUB_TEXTURE = "SubTexture";
static const char *A_NAME = "name";
static const char *A_DURATION = "dr";
static const char *A_FRAME_INDEX = "fi";
static const char *A_DURATION_TO = "to";
static const char *A_DURATION_TWEEN = "drTW";
static const char *A_LOOP = "lp";
static const char *A_MOVEMENT_SCALE = "sc";
static const char *A_MOVEMENT_DELAY = "dl";
static const char *A_DISPLAY_INDEX = "dI";
static const char *A_PARENT = "parent";
static const char *A_SKEW_X = "kX";
static const char *A_SKEW_Y = "kY";
static const char *A_SCALE_X = "cX";
static const char *A_SCALE_Y = "cY";
static const char *A_Z = "z";
static const char *A_EVENT = "evt";
static const char *A_SOUND = "sd";
static const char *A_SOUND_EFFECT = "sdE";
static const char *A_TWEEN_EASING = "twE";
static const char *A_EASING_PARAM = "twEP";
static const char *A_TWEEN_ROTATE = "twR";
static const char *A_IS_ARMATURE = "isArmature";
static const char *A_DISPLAY_TYPE = "displayType";
static const char *A_MOVEMENT = "mov";
static const char *A_X = "x";
static const char *A_Y = "y";
static const char *A_WIDTH = "width";
static const char *A_HEIGHT = "height";
static const char *A_PIVOT_X = "pX";
static const char *A_PIVOT_Y = "pY";
static const char *A_COCOS2D_PIVOT_X = "cocos2d_pX";
static const char *A_COCOS2D_PIVOT_Y = "cocos2d_pY";
static const char *A_BLEND_TYPE = "bd";
static const char *A_BLEND_SRC = "bd_src";
static const char *A_BLEND_DST = "bd_dst";
static const char *A_ALPHA = "a";
static const char *A_RED = "r";
static const char *A_GREEN = "g";
static const char *A_BLUE = "b";
static const char *A_ALPHA_OFFSET = "aM";
static const char *A_RED_OFFSET = "rM";
static const char *A_GREEN_OFFSET = "gM";
static const char *A_BLUE_OFFSET = "bM";
static const char *A_COLOR_TRANSFORM = "colorTransform";
static const char *A_TWEEN_FRAME = "tweenFrame";
static const char *CONTOUR = "con";
static const char *CONTOUR_VERTEX = "con_vt";
static const char *FL_NAN = "NaN";
static const char *FRAME_DATA = "frame_data";
static const char *MOVEMENT_BONE_DATA = "mov_bone_data";
static const char *MOVEMENT_DATA = "mov_data";
static const char *ANIMATION_DATA = "animation_data";
static const char *DISPLAY_DATA = "display_data";
static const char *SKIN_DATA = "skin_data";
static const char *BONE_DATA = "bone_data";
static const char *ARMATURE_DATA = "armature_data";
static const char *CONTOUR_DATA = "contour_data";
static const char *TEXTURE_DATA = "texture_data";
static const char *VERTEX_POINT = "vertex";
static const char *COLOR_INFO = "color";
static const char *CONFIG_FILE_PATH = "config_file_path";
static const char *CONTENT_SCALE = "content_scale";

 using namespace _2DSkeletalAnimation;

CAnimationImporter* CAnimationImporter::m_pInstance = nullptr;

CAnimationImporter::CAnimationImporter()
{

}

CAnimationImporter::~CAnimationImporter()
{

}
void CAnimationImporter::ImportData(const TString& file)
{
    size_t len = file.size() * 2 + 1;
    char *filename = new char[len];
    CStringHelper::GetInstance()->ConvertToCHAR(file.c_str(), filename, len);
    const char* jsonstr = GetStringFromFile(filename);
    BEATS_SAFE_DELETE_ARRAY(filename);
    Json* jsonRoot = Json_create (jsonstr);
    Json* jsonChild = jsonRoot->child;
    while (jsonChild) 
    {
        if(jsonChild->type == Json_Number)
        {
            BEATS_ASSERT(jsonChild->valueFloat > 0.f)
        }
        else if(jsonChild->type == Json_Array)
        {
            if (strcmp(jsonChild->name, ARMATURE_DATA) == 0)
            {
                DecodeArmature(jsonChild);
            }
            else if (strcmp(jsonChild->name, ANIMATION_DATA) == 0)
            {
                DecodeAnimation(jsonChild);
            }
            else if (strcmp(jsonChild->name, TEXTURE_DATA) == 0)
            {
                DecodeTexture(jsonChild);
            }
        }
        jsonChild = jsonChild->next;
    }
    Json_dispose(jsonRoot);
    BEATS_SAFE_DELETE_ARRAY(jsonstr);
}

void CAnimationImporter::DecodeArmature(Json* json)
{
    BEATS_ASSERT(json);
    Json* child = json->child;
    while (child)
    {
        if(child->type == Json_Object)
        {
            Json* name = Json_getItem(child,A_NAME);
            Json* bonedata = Json_getItem(child,BONE_DATA);
            CArmatureData* pArmatureData = new CArmatureData();
            pArmatureData->SetName(name->valueString);
            CAnimationManager::GetInstance()->AddArmatureData(pArmatureData);
            DecodeBoneData(bonedata, pArmatureData);
        }
        child = child->next;
    }
}

void CAnimationImporter::DecodeBoneData(Json* json, CArmatureData* pArmatureData)
{
    BEATS_ASSERT(json);
    BEATS_ASSERT(json->type == Json_Array);

    Json* child = json->child;
    while (child)
    {
        if(child->type == Json_Object)
        {
            Json* name = Json_getItem(child,A_NAME);
            Json* x = Json_getItem(child,A_X);
            Json* y = Json_getItem(child,A_Y);
            Json* z = Json_getItem(child,A_Z);
            Json* cX = Json_getItem(child,A_SCALE_X);
            Json* cY = Json_getItem(child,A_SCALE_Y);
            Json* kX = Json_getItem(child,A_SKEW_X);
            Json* kY = Json_getItem(child,A_SKEW_Y);

            Json* display_data = Json_getItem(child, DISPLAY_DATA);

            CBoneData* pBoneData = new CBoneData();

            pBoneData->SetName(name->valueString);
            pBoneData->SetPositionX(x->valueFloat);
            pBoneData->SetPositionY(y->valueFloat);
            pBoneData->SetZOrder(z->valueInt);
            pBoneData->SetScaleX(cX->valueFloat);
            pBoneData->SetScaleY(cY->valueFloat);
            pBoneData->SetSkewX(kX->valueFloat);
            pBoneData->SetSkewY(kY->valueFloat);

            pArmatureData->AddBoneData(pBoneData);

            DecodeDisplayData(display_data, pBoneData);
        }
        child = child->next;
    }
}

void CAnimationImporter::DecodeDisplayData(Json* json, CBoneData* pBoneData)
{
    BEATS_ASSERT(json && pBoneData);
    BEATS_ASSERT(json->type == Json_Array);

    Json* child = json->child;
    while (child)
    {
        if(child->type == Json_Object)
        {
            Json* name = Json_getItem(child,A_NAME);
            Json* displayType = Json_getItem(child,A_DISPLAY_TYPE);
            Json* skin_data = Json_getItem(child,SKIN_DATA);
            SDisplayData displaydata;

            displaydata.name = name->valueString;
            displaydata.type = (EDisplayType)displayType->valueInt;

            DecodeSkinData(skin_data,&displaydata);
            pBoneData->AddDisplayData(displaydata);
        }
        child =child->next;
    }
}

void CAnimationImporter::DecodeSkinData(Json* json, SDisplayData* pDisplayData)
{
    BEATS_ASSERT(json && pDisplayData);
    BEATS_ASSERT(json->type == Json_Array);

    Json* child = json->child;
    while (child)
    {
        if(child->type == Json_Object)
        {
            Json* x = Json_getItem(child,A_X);
            Json* y = Json_getItem(child,A_Y);

            Json* cX = Json_getItem(child,A_SCALE_X);
            Json* cY = Json_getItem(child,A_SCALE_Y);

            Json* kX = Json_getItem(child, A_SKEW_X);
            Json* kY = Json_getItem(child, A_SKEW_Y);

            pDisplayData->skindata.x = x->valueFloat;
            pDisplayData->skindata.y = y->valueFloat;
            pDisplayData->skindata.scaleX = cX->valueFloat;
            pDisplayData->skindata.scaleY = cY->valueFloat;
            pDisplayData->skindata.skewX = kX->valueFloat;
            pDisplayData->skindata.skewY = kY->valueFloat;
        }
        child =child->next;
    }
}

void CAnimationImporter::DecodeAnimation(Json* json)
{
    BEATS_ASSERT(json);
    Json* child = json->child;
    while (child)
    {
        if(child->type == Json_Object)
        {
            Json* name = Json_getItem(child,A_NAME);
            Json* movdata = Json_getItem(child,MOVEMENT_DATA);
            CAnimationData* pAnimationData = new CAnimationData;
            pAnimationData->SetName(name->valueString);
            CAnimationManager::GetInstance()->AddAnimationData(pAnimationData);
            DecodeMovData(movdata, pAnimationData);
        }
        child = child->next;
    }
}

void CAnimationImporter::DecodeMovData(Json* json, CAnimationData* pAnimationData)
{
    BEATS_ASSERT(json);
    BEATS_ASSERT(json->type == Json_Array);

    Json* child = json->child;
    while (child)
    {
        if(child->type == Json_Object)
        {
            Json* name = Json_getItem(child,A_NAME);
            Json* dr = Json_getItem(child,A_DURATION);
            Json* to = Json_getItem(child,A_DURATION_TO);
            Json* drTW = Json_getItem(child,A_DURATION_TWEEN);
            Json* movebonedata = Json_getItem(child,MOVEMENT_BONE_DATA);
            
            CMoveData* pMoveData = new CMoveData;
            pMoveData->SetName(name->valueString);
            pMoveData->SetDuration(dr->valueInt);
            pMoveData->SetDurationTo(to->valueInt);
            pMoveData->SetDuraionTween(drTW->valueInt);
            pAnimationData->AddMoveData(pMoveData);

            DecodeMovBoneData(movebonedata, pMoveData);
        }
        child = child->next;
    }
}

void CAnimationImporter::DecodeMovBoneData(Json *json, CMoveData* pMoveData)
{
    BEATS_ASSERT(json);
    Json* child = json->child;
    while (child)
    {
        if(child->type == Json_Object)
        {
            Json* name = Json_getItem(child,A_NAME);
            Json* dl = Json_getItem(child,A_MOVEMENT_DELAY);
            Json* framedata = Json_getItem(child,FRAME_DATA);
            SMoveBoneData* pMoveBoneData = new SMoveBoneData;
            pMoveBoneData->name = name->valueString;
            pMoveBoneData->duration = dl->valueFloat;
            pMoveData->AddMoveBoneData(pMoveBoneData);

            DecodeFrameData(framedata, pMoveBoneData);
        }
        child = child->next;
    }
}

void CAnimationImporter::DecodeFrameData(Json* json, SMoveBoneData* pMoveBoneData)
{
    BEATS_ASSERT(json);
    Json* child = json->child;
    while (child)
    {
        if(child->type == Json_Object)
        {
            Json* dI = Json_getItem(child,A_DISPLAY_INDEX);
            Json* x = Json_getItem(child,A_X);
            Json* y = Json_getItem(child,A_Y);
            Json* z = Json_getItem(child,A_Z);
            Json* cX = Json_getItem(child,A_SCALE_X);
            Json* cY = Json_getItem(child,A_SCALE_Y);
            Json* kX = Json_getItem(child,A_SKEW_X);
            Json* kY = Json_getItem(child,A_SKEW_Y);
            Json* fi = Json_getItem(child,A_FRAME_INDEX);

            SFrameData* framedata = new SFrameData;
            framedata->dI = dI->valueInt;
            framedata->x = x->valueFloat;
            framedata->y = y->valueFloat;
            framedata->zOrder = z->valueInt;
            framedata->scaleX = cX->valueFloat;
            framedata->scaleY = cY->valueFloat;
            framedata->skewX = kX->valueFloat;
            framedata->skewY = kY->valueFloat;
            framedata->frameID = fi->valueInt;

            pMoveBoneData->frameList.push_back(framedata);
        }
        child = child->next;
    }
}

void CAnimationImporter::DecodeTexture(Json* json)
{
    BEATS_ASSERT(json);
    Json* child = json->child;
    while (child)
    {
        if(child->type == Json_Object)
        {
            Json* name = Json_getItem(child,A_NAME);
            Json* width = Json_getItem(child,A_WIDTH);
            Json* height = Json_getItem(child,A_HEIGHT);
            Json* pX = Json_getItem(child,A_PIVOT_X);
            Json* pY = Json_getItem(child,A_PIVOT_Y);
            CTextureData* texturedata = new CTextureData;
            texturedata->m_strName = name->valueString;
            texturedata->m_fWidth = width->valueFloat;
            texturedata->m_fHeight = height->valueFloat;
            texturedata->m_fpX = pX->valueFloat;
            texturedata->m_fpY = pY->valueFloat;

            std::vector<CTextureData*>& texturelist= CAnimationManager::GetInstance()->GetTextureData();
            texturelist.push_back(texturedata);

        }
        child = child->next;
    }
}

const char* CAnimationImporter::GetStringFromFile(const char* file)
{
    char* pBuffer = nullptr;
    size_t size = 0;
    FILE *fp = nullptr;
    fp = fopen(file, "rt");
    BEATS_ASSERT(fp);
    fseek(fp,0,SEEK_END);
    size = ftell(fp);
    fseek(fp,0,SEEK_SET);

    pBuffer = new char[(sizeof(char) * (size + 1))];
    pBuffer[size] = '\0';

    size = fread(pBuffer, sizeof(unsigned char), size, fp);
    fclose(fp);
    return pBuffer;
}
