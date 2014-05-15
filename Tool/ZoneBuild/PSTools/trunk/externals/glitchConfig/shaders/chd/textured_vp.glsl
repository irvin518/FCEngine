// Do not modify this file directly, plz check *.gx
// -*- mode: c++; -*-
// common.h
#define SHADER_DEVICE_HI_END  0
#define SHADER_DEVICE_HI      10
#define SHADER_DEVICE_MID     20
#define SHADER_DEVICE_LOW     30
#define SHADER_DEVICE_LOW_END 40

// what is Shader_*** macro ?
// It is macro defined in CCustomColladaFactory
// so we can modify it from xml config

// shader running only in win32 version
#ifndef Shader_Win32
#  define Shader_Win32 0
#endif

#ifndef Shader_Debug
#  define Shader_Debug 0
#endif

#if Shader_Win32
# undef Shader_Debug
# define Shader_Debug 1 // Force enable debug
#endif

// shader running only in editor (atlasing)
#ifndef Shader_Editor
# define Shader_Editor 0
#endif

// performance scale, 0 is the highest level. bigger then lower
#ifndef Shader_Device
#  define Shader_Device 0
#endif

// Alpha split from original texture (ETC)
#ifndef Shader_SplitAlphaTex
#  define Shader_SplitAlphaTex 0
#endif

#ifndef Shader_NoHIGHP
#  define Shader_NoHIGHP 0
#endif

#ifndef Shader_UseTexBias
#  define Shader_UseTexBias 0
#endif

#if (defined GLITCH_USE_BIAS) || (defined GLITCH_FORCE_USE_BIAS) // force bias
#  undef Shader_UseTexBias
#  define Shader_UseTexBias 1
#endif

////////////////////////////////////////////////// Presicion
#define LOWP lowp
#define MEDIUMP mediump
#if Shader_NoHIGHP
#  define HIGHP mediump
#else
#  define HIGHP highp
#endif

////////////////////////////////////////////////// Macros

#define SATURATE(x) clamp((x), 0.0, 1.0)

////////////////////////////////////////////////// ALL

uniform HIGHP mat4 wvp;
uniform HIGHP mat4 worldviewt;
uniform HIGHP mat4 worldviewit;
uniform HIGHP mat4 view;
uniform HIGHP mat4 viewt;
uniform HIGHP mat4 viewi;
uniform HIGHP mat4 viewit;
uniform HIGHP mat4 world;
uniform HIGHP mat4 worldi;
uniform HIGHP mat4 worldit;
uniform HIGHP mat4 viewprojection;

uniform HIGHP vec3 EyePos;
uniform HIGHP vec3 EyePosOs;
uniform LOWP vec3 LightDir; // !Notice, LightDir not normalized in glitch!
uniform LOWP vec3 LightDirOs;
uniform MEDIUMP vec4 LightColorMultiplied;
uniform LOWP vec4 LightColor;


#ifdef DCC_MAX
#  define _C(SEM) : SEM
#else
#  define _C(_)
#endif

/*
  Use GLshort for UV, see options in data/game/convert_meshes.py, 
  params +=' geometry_texture_format=SHORT '
  comment it if using float for uv.
*/
#ifndef DCC_MAX
//#  define USE_TEX_MAT
#endif

#ifdef USE_TEX_MAT
#  define DECL_TEX_COORD(coord, coord_scaleoffset) uniform MEDIUMP vec4 coord_scaleoffset; attribute MEDIUMP vec2 coord
#  define TEX_COORD(coord, coord_scaleoffset) (coord*coord_scaleoffset.xy+coord_scaleoffset.zw)
#else
#  define DECL_TEX_COORD(coord, _) attribute MEDIUMP vec2 coord
#  define TEX_COORD(coord, _) coord
#endif




////////////////////////////////////////////////// VERTEX

uniform HIGHP float global_Fog_Start;
uniform HIGHP float global_Fog_End;
LOWP float fog(HIGHP vec4 WorldPosition)
{
	MEDIUMP float viewDistance = dot(viewt[2], WorldPosition); // column major here
	return SATURATE((-viewDistance-global_Fog_Start)/(global_Fog_End-global_Fog_Start));
}

// toning.h
// near and far fog

#ifndef Shader_Toned
#  define Shader_Toned 0
#endif


// tone options
#if Shader_Toned
#  ifndef Shader_DisableNearTone
#    define Shader_DisableNearTone 0
#  endif

#  ifndef Shader_UseColorGain
#    define Shader_UseColorGain 0  // Caution, vary slow!
#  endif
#endif



#if Shader_Toned
  varying LOWP vec3 vToneGain;
  varying LOWP vec3 vToneOffset;
#endif




#if Shader_Toned
  uniform LOWP vec3 global_ToneGain_Near;
  uniform LOWP vec3 global_ToneOffset_Near;
  uniform LOWP vec3 global_ToneGain_Far;
  uniform LOWP vec3 global_ToneOffset_Far;

  void tone(LOWP float fogFactor)
  {
#if Shader_DisableNearTone
	  vToneGain = global_ToneGain_Far * (1.0 - fogFactor);
	  vToneOffset = global_ToneOffset_Far * fogFactor;
#else
	  vToneGain = mix(global_ToneGain_Near, global_ToneGain_Far, fogFactor);
	  vToneOffset = mix(global_ToneOffset_Near, global_ToneOffset_Far, fogFactor);
#endif
  }
#else
# define tone(x)
#endif



#ifdef USE_VERTEX_ANIM
# ifndef USE_VCOLOR
#  define USE_VCOLOR
# endif
#endif

////////////////////////////////////////////////// all

varying MEDIUMP vec2 vTexcoord0;
#if defined USE_VCOLOR || defined USE_PCOLOR
  varying LOWP vec4 vColor;
#endif

////////////////////////////////////////////////// vertex

attribute HIGHP vec3 position _C(POSITION);
attribute MEDIUMP vec2 texcoord0 _C(TEXCOORD0);


#ifdef USE_VCOLOR
# ifdef DCC_MAX
  attribute LOWP vec3 color0;
  attribute LOWP float alpha0;
# else
  attribute LOWP vec4 color0;
# endif
#endif

#ifdef USE_PCOLOR
  uniform LOWP vec3 Color;
  uniform LOWP float ColorIntensity;
  uniform LOWP float Alpha;
#endif

#ifdef USE_UV_TILE_OFFSET
  uniform MEDIUMP vec2 UVTile;
  uniform MEDIUMP vec2 UVOffset;
#endif


#ifdef USE_VERTEX_ANIM
# ifdef DCC_MAX
#  define WEIGHT_ATTR alpha0
# else
#  define WEIGHT_ATTR color0.a
# endif
  uniform HIGHP float Time;
# define TIME Time
# define WORLD_MAT world
// -*- mode: c++; -*-


#ifndef WEIGHT_ATTR
  attribute LOWP float alpha0;
# define WEIGHT_ATTR alpha0
#endif

#ifndef TIME
  uniform HIGHP float Time;
# define TIME Time
#endif

#ifndef WORLD_MAT
  uniform HIGHP mat4 world;
# define WORLD_MAT world
#endif


#ifndef WORLD_UP
# define WORLD_UP vec3(0.0, 0.0, 1.0)
#endif

#if defined DCC_MAX || Shader_Editor
  const HIGHP vec3 global_VertexMovementForce = vec3(100.0, 100.0, 100.0);
#else
  uniform HIGHP vec3 global_VertexMovementForce;
#endif

// polynomial approximation of a sin wave like function
// f(x) = ax^3 + bax^2 + cx + d;
// f(0) = 0, f(1) = 0, f(-1) = 0
// 6 instructions (frac,+,*)
// sin(w) ~= CheapWave(r / PIMul2) * 20.72f
HIGHP float cheapWave(HIGHP float r)
{
	float x = fract(r);
	return x * (x - 0.5) * (x - 1.0);
}

HIGHP vec3 animate(HIGHP vec3 worldP)
{
	const HIGHP vec3 worldUp = WORLD_UP;
	HIGHP vec3 worldCenter = (WORLD_MAT*vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	HIGHP vec3 worldDirFromCenter = worldP-worldCenter;
	HIGHP vec3 worldTangent = cross(worldDirFromCenter, worldUp);
	worldTangent.x += 0.001; // prevent 0 length
	worldTangent = normalize(worldTangent);
	HIGHP float frequencyOffset = worldTangent.x+worldTangent.y;
	HIGHP float maxAmplitude = global_VertexMovementForce.y*WEIGHT_ATTR;

	const HIGHP float PIMul2 = 1.0 / (3.14159265 * 2.0);
	HIGHP float frequencyOffsetForCheapWave = (frequencyOffset+TIME)*PIMul2;
	HIGHP float tangentAmplitude = cheapWave(global_VertexMovementForce.x + frequencyOffsetForCheapWave)*maxAmplitude;
	HIGHP float verticalAmplitude = cheapWave(global_VertexMovementForce.z + frequencyOffsetForCheapWave)*maxAmplitude;
	
	//apply the vertex offset
	worldP.xyz += worldTangent*tangentAmplitude;
	worldP.xyz += worldUp*verticalAmplitude;
	return worldP;
}
#endif // USE_VERTEX_ANIM

void main()
{
	HIGHP vec4 worldP = world*vec4(position, 1.0);

#ifdef USE_VERTEX_ANIM
	worldP.xyz = animate(worldP.xyz);
#endif

	gl_Position = viewprojection*worldP;

	vTexcoord0 = texcoord0;

#ifdef USE_UV_TILE_OFFSET
# ifndef DCC_MAX
	vTexcoord0.y = vTexcoord0.y - 1.0; // correct difference between Max and other tool (image orignal problem)
# endif
	vTexcoord0 = (vTexcoord0+UVOffset)*UVTile;
#endif

#ifdef USE_VCOLOR
# ifdef DCC_MAX
	LOWP vec4 vertexColor = vec4(color0, alpha0);
# else
	LOWP vec4 vertexColor = color0;
# endif
#endif

#if defined USE_VCOLOR && defined USE_PCOLOR
	vColor = vertexColor*vec4(Color*ColorIntensity, Alpha);
#elif defined USE_PCOLOR
	vColor = vec4(Color*ColorIntensity, Alpha);
#elif defined USE_VCOLOR
	vColor = vertexColor;
#endif

#ifndef DISABLE_TONE
	tone(fog(worldP));
#endif
}

// -*- mode: c++; -*-
