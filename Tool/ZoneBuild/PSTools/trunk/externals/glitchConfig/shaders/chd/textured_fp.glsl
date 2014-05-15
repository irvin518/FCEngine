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




////////////////////////////////////////////////// FRAGMENT

#define SAMPLERCUBE(x) uniform LOWP samplerCube x
#define SAMPLER2D_RAW(x) uniform LOWP sampler2D x

#if Shader_SplitAlphaTex

#  if Shader_UseTexBias

#    define SAMPLER2D(x, x_bias, x_alpha) uniform LOWP sampler2D x; uniform LOWP sampler2D x_alpha; uniform LOWP float x_bias
#    define SAMPLER2D_NOALPHA(x, x_bias) uniform LOWP sampler2D x; uniform LOWP float x_bias

#  else // !Shader_UseTexBias

#    define SAMPLER2D(x, _, x_alpha) uniform LOWP sampler2D x; uniform LOWP sampler2D x_alpha
#    define SAMPLER2D_NOALPHA(x, _) uniform LOWP sampler2D x

#  endif

#else // !Shader_SplitAlphaTex

#  if Shader_UseTexBias
#    define SAMPLER2D(x, x_bias, _) uniform LOWP sampler2D x; uniform LOWP float x_bias
#    define SAMPLER2D_NOALPHA(x, x_bias) uniform LOWP sampler2D x; uniform LOWP float x_bias
#  else
#    define SAMPLER2D(x, _, _) uniform LOWP sampler2D x
#    define SAMPLER2D_NOALPHA(x, _) uniform LOWP sampler2D x
#  endif

#endif // Shader_SplitAlphaTex


#if Shader_SplitAlphaTex

#  if Shader_UseTexBias
#    define HANDLE_TEX2D_READ(sampler, uv, bias, sampler_alpha) vec4(texture2D(sampler, uv, bias).rgb, texture2D(sampler_alpha, uv, bias).r)
#    define HANDLE_TEX2D_READ_NOALPHA(sampler, uv, bias) texture2D(sampler, uv, bias)
#  else
#    define HANDLE_TEX2D_READ(sampler, uv, _, sampler_alpha) vec4(texture2D(sampler, uv).rgb, texture2D(sampler_alpha, uv).r)
#    define HANDLE_TEX2D_READ_NOALPHA(sampler, uv, _) texture2D(sampler, uv)
#  endif

#else // !Shader_SplitAlphaTex

#  if Shader_UseTexBias
#    define HANDLE_TEX2D_READ(sampler, uv, bias, _) texture2D(sampler, uv, bias)
#    define HANDLE_TEX2D_READ_NOALPHA(sampler, uv, bias) texture2D(sampler, uv, bias)
#  else
#    define HANDLE_TEX2D_READ(sampler, uv, _, _) texture2D(sampler, uv)
#    define HANDLE_TEX2D_READ_NOALPHA(sampler, uv, _) texture2D(sampler, uv)
#  endif


#endif // Shader_SplitAlphaTex


#define HANDLE_TEX2D_READ_RAW(sampler, uv) texture2D(sampler, uv)
#define HANDLE_TEXCUBE_READ(sampler, uvw) textureCube(sampler, uvw)

#define HANDLE_COLOR_READ(color) color

#define HANDLE_COLOR_WRITE(target, color) target = color

LOWP float grayScaled(LOWP vec3 color)
{
	return dot(color, vec3(0.299, 0.587, 0.114));
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




LOWP vec3 gain(LOWP float g, LOWP vec3 x)
{
	LOWP vec3 f = (1.0/g-2.0)*(1.0-2.0*x);
	LOWP vec3 v1 = x/(f+1.0);
	LOWP vec3 v2 = (f-x)/(f-1.0);
	return mix(v1, v2, step(0.5, x));
}

#if Shader_Toned

  uniform LOWP float global_Tone_GainFactor;
  uniform LOWP float global_Tone_Saturation;

  LOWP vec3 tone(LOWP vec3 color)
  {
	  LOWP vec3 tonedColor = SATURATE(vToneGain*color + vToneOffset);

#if Shader_UseColorGain
	  LOWP float lum = grayScaled(tonedColor);
	  LOWP vec3 gray = vec3(lum, lum, lum);
	  tonedColor = gain(global_Tone_GainFactor, tonedColor);
	  tonedColor = mix(gray, tonedColor, global_Tone_Saturation);
#endif
	  return tonedColor;
  }
#else
#  define tone(color) color
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

// -*- mode: c++; -*-

////////////////////////////////////////////////// fragment
#ifdef USE_SUN_OCCLUSION
// -*- mode: c++; -*-

#ifndef Shader_Debug
# define Shader_Debug 0
#endif

#if Shader_Debug
  uniform LOWP int global_DEBUG_NoDiffuse;
  uniform LOWP int global_DEBUG_NoSpec;
  uniform LOWP int global_DEBUG_NoFresnel;
# define DEBUG_DiffuseOverride vec3(1.0, 1.0, 1.0)
# define DEBUG_SpecOverride vec3(0.0, 0.0, 0.0)
# define DEBUG_FresOverride vec3(0.0, 0.0, 0.0)
#endif // Shader_Debug

#ifdef USE_CEL
# ifdef DCC_MAX
#   define RAMP(x) clamp(x, 0.01, 0.99)
# else
#   define RAMP(x) x
# endif
	SAMPLER2D_RAW(RampSampler);
#endif // USE_CEL

LOWP vec3 get_diff_and_backdiff(LOWP vec3 N, LOWP vec3 L, LOWP vec3 ambColor, LOWP vec3 diffColor, LOWP vec3 backDiffColor)
{ 
#if Shader_Debug
	if (bool(global_DEBUG_NoDiffuse))
		return DEBUG_DiffuseOverride;
#endif
	LOWP float NdotL = max(dot(N, L), 0.0);
#ifdef USE_CEL
	LOWP float diffCoef = HANDLE_TEX2D_READ_RAW(RampSampler, RAMP(vec2(NdotL, NdotL))).r;
	NdotL *= diffCoef;
#endif
	LOWP vec3 diff = max(NdotL, 0.0)*diffColor;
	diff += max(dot(N, -L), 0.0)*backDiffColor;
	return diff+ambColor;
}
LOWP vec3 get_specular(LOWP float NdotH, MEDIUMP float shininess, LOWP vec3 specColor, LOWP float specIntensity)
{
#if Shader_Debug
	if (bool(global_DEBUG_NoSpec))
		return DEBUG_SpecOverride;
#endif
	return pow(NdotH, shininess)*specColor*specIntensity;
}
LOWP vec3 get_specular(LOWP vec3 N, LOWP vec3 H, MEDIUMP float shininess, LOWP vec3 specColor, LOWP float specIntensity)
{
	LOWP float NdotH = dot(N, H)*0.5+0.5;
	return get_specular(NdotH, shininess, specColor, specIntensity);
}
LOWP vec3 get_fresnel(LOWP float NdotH, MEDIUMP float fresnelPower, LOWP vec3 fresnelColor, LOWP float fresnelIntensity)
{
#if Shader_Debug
	if (bool(global_DEBUG_NoFresnel))
		return DEBUG_FresOverride;
#endif
	LOWP float invNdotH = 1.0-NdotH;
	return pow(invNdotH, fresnelPower)*fresnelColor*fresnelIntensity;
}
LOWP vec3 get_fresnel(LOWP vec3 N, LOWP vec3 H, MEDIUMP float fresnelPower, LOWP vec3 fresnelColor, LOWP float fresnelIntensity)
{
	LOWP float NdotH = dot(N, H)*0.5+0.5;
	return get_fresnel(NdotH, fresnelPower, fresnelColor, fresnelIntensity);
}
LOWP vec3 get_rim(LOWP vec3 N, LOWP vec3 V, LOWP vec3 rimColor, LOWP float rimLevel)
{
	LOWP float NdotV = max(dot(N, V), 0.0);
	return rimColor * pow((1.0 - NdotV), rimLevel);
}

#ifndef Shader_SunOcclusion
# define Shader_SunOcclusion 0
#endif


#if Shader_SunOcclusion

# if Shader_Debug
  uniform LOWP int global_DEBUG_NoSunOcclusion;
# endif

# define DECL_SUN_OCCLUSION( SunOcclusion ) uniform LOWP float SunOcclusion;
# if Shader_Debug
#   define HANDLE_SUN_OCCLUSION( rgb, SunOcclusion ) if (!bool(global_DEBUG_NoSunOcclusion)) rgb *= SunOcclusion;
# else
#   define HANDLE_SUN_OCCLUSION( rgb, SunOcclusion ) rgb *= SunOcclusion;
# endif

#else // !Shader_SunOcclusion

# define DECL_SUN_OCCLUSION( SunOcclusion )
# define HANDLE_SUN_OCCLUSION( _, _ )

#endif // Shader_SunOcclusion


DECL_SUN_OCCLUSION(SunOcclusion)
#endif

#if Shader_Editor
  GLITCH_UNIFORM_PROPERTIES(DiffuseMapSampler, (texcoord = texcoord0, semantic = texture))
#endif

SAMPLER2D(DiffuseMapSampler, DiffuseMapSampler_bias, DiffuseMapSampler_alpha);

#ifdef USE_OBJECT_ALPHA
  uniform LOWP float ObjectAlpha;
#endif

#ifdef USE_FX0
  uniform LOWP float FX0_Width;
  uniform LOWP float FX0_MoveDir;
  uniform LOWP float FX0_BaseIllum;
#endif

void main()
{
	LOWP vec4 col = HANDLE_TEX2D_READ(DiffuseMapSampler, 
									  vTexcoord0, 
									  DiffuseMapSampler_bias, 
									  DiffuseMapSampler_alpha);
#ifdef USE_ALPHA_TEST
	if (col.a < 0.5)
		discard;
#endif

#if defined USE_PCOLOR || defined USE_VCOLOR
	col.rgb *= vColor.rgb;
#endif

	LOWP float alpha;

#ifdef NO_ALPHA
	alpha = 1.0;
#elif !(defined USE_VERTEX_ANIM) && (defined USE_VCOLOR || defined USE_PCOLOR)
	alpha = col.a * vColor.a;
#else
	alpha = col.a;
#endif

#ifdef USE_OBJECT_ALPHA
	alpha *= ObjectAlpha;
#endif

#ifdef USE_FX0
	LOWP float F = cos(mod(length(vTexcoord0-vec2(0.5, -0.5)), FX0_Width)/FX0_Width*6.28318+FX0_MoveDir)*FX0_BaseIllum+(1.0-FX0_BaseIllum);
	col.rgb *= F;
	alpha *= F;
#endif

#ifdef USE_SUN_OCCLUSION
HANDLE_SUN_OCCLUSION(col.rgb, SunOcclusion)
#endif

#ifdef DISABLE_TONE
	gl_FragColor = vec4(col.rgb, alpha);
#else
	gl_FragColor = vec4(tone(col.rgb), alpha);
#endif
}
