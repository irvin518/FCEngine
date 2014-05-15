// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

//#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <thread>
// TODO: reference additional headers your program requires here
// STL Headers
#include <vector>
#include <map>
#include <list>

#include "PlatformConfig.h"
#include "glfw3.h"
#include "math/kazmath/kazmath/GL/matrix.h"
#include "math/kazmath/kazmath/mat4.h"
#include "math/kazmath/kazmath/vec4.h"
#include "math/kazmath/kazmath/vec3.h"
#include "math/kazmath/kazmath/vec2.h" 

#include "PublicDef.h"

#include "Render/RenderPublic.h"
#include "Resource/ResourcePublic.h"
#include "Utility/BeatsUtility/SharePtr.h"