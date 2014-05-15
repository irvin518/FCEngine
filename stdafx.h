// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#pragma warning(disable:4611) // for load png warning

#define WIN32_DEV
//#define WIN64_DEV
//#define ANDROID_DEV
//#define IOS_DEV

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
#include <windows.h>

#include <thread>

#include "PlatformConfig.h"
#include "glfw3.h"


//STL headers
#include <list>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

#include "PublicDef.h"

#include "math/kazmath/kazmath/GL/matrix.h"
#include "math/kazmath/kazmath/mat4.h"
#include "math/kazmath/kazmath/vec4.h"
#include "math/kazmath/kazmath/vec3.h"
#include "math/kazmath/kazmath/vec2.h"
#include "math/kazmath/kazmath/mat4.h"
#include "Utility/BeatsUtility/SharePtr.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Render/RenderPublic.h"
#include "Resource/ResourcePublic.h"
#include "Utility/BeatsUtility/ComponentSystem/ComponentPublic.h"
#include "Utility/PerformDetector/PerformDetector.h"
