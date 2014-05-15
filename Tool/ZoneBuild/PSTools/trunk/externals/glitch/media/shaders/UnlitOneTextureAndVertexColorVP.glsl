#define TEXTURE_2D 0
#define TEXTURE_3D 1
#define TEXTURE_CUBE_MAP 2
#define TEXTURE_RECT 3

#ifndef TEXTURE_TYPE
#    define TEXTURE_TYPE TEXTURE_2D
#endif

#define TEXCOORD_NONE 0
#define TEXCOORD_SCALE_OFFSET 1
#define TEXCOORD_TRANSFORM 2
#define TEXCOORD_TRANSFORM_3 3
#define TEXCOORD_SCALE_OFFSET_SEPARATE 4

#if !defined(TEXCOORD_MODIFIER) && defined(TEXTURE_MATRIX)
// legacy support for old TEXTURE_MATRIX
#    define TEXCOORD_MODIFIER TEXCOORD_TRANSFORM
#elif !defined(TEXCOORD_MODIFIER)
#    define TEXCOORD_MODIFIER TEXCOORD_NONE
#endif

attribute highp   vec4 Vertex;

#if TEXTURE_TYPE == TEXTURE_3D || TEXTURE_TYPE == TEXTURE_CUBE_MAP
attribute mediump vec3 TexCoord0;
#else
attribute mediump vec2 TexCoord0;
#endif

attribute lowp    vec4 Color0;

uniform   highp   mat4 WorldViewProjectionMatrix;

#if TEXTURE_TYPE == TEXTURE_3D || TEXTURE_TYPE == TEXTURE_CUBE_MAP
varying	  mediump vec3 vCoord0;
#else
varying	  mediump vec2 vCoord0;
#endif
varying	  lowp    vec4 vColor0;

#if (TEXCOORD_MODIFIER == TEXCOORD_TRANSFORM || TEXCOORD_MODIFIER == TEXCOORD_TRANSFORM_3) && (TEXTURE_TYPE == TEXTURE_3D || TEXTURE_TYPE == TEXTURE_CUBE_MAP)
uniform   mediump mat4 TexCoord0_transform;
#elif TEXCOORD_MODIFIER == TEXCOORD_TRANSFORM_3
uniform   mediump mat3 TexCoord0_transform;
#elif (TEXTURE_TYPE == TEXTURE_3D || TEXTURE_TYPE == TEXTURE_CUBE_MAP) && (TEXCOORD_MODIFIER == TEXCOORD_SCALE_OFFSET || TEXCOORD_MODIFIER == TEXCOORD_SCALE_OFFSET_SEPARATE)
uniform mediump vec3 TexCoord0_scale;
uniform mediump vec3 TexCoord0_offset;
#elif TEXCOORD_MODIFIER == TEXCOORD_SCALE_OFFSET
uniform mediump vec4 TexCoord0_scaleoffset;
#elif TEXCOORD_MODIFIER == TEXCOORD_SCALE_OFFSET_SEPARATE
uniform mediump vec2 TexCoord0_scale;
uniform mediump vec2 TexCoord0_offset;
#endif

void main(void)
{
#if (TEXTURE_TYPE == TEXTURE_3D || TEXTURE_TYPE == TEXTURE_CUBE_MAP) && (TEXCOORD_MODIFIER == TEXCOORD_TRANSFORM || TEXCOORD_MODIFIER == TEXCOORD_TRANSFORM_3)
	vCoord0 = (TexCoord0_transform * vec4(TexCoord0, 1.0)).xyz;
#elif (TEXTURE_TYPE == TEXTURE_3D || TEXTURE_TYPE == TEXTURE_CUBE_MAP) && (TEXCOORD_MODIFIER == TEXCOORD_SCALE_OFFSET || TEXCOORD_MODIFIER == TEXCOORD_SCALE_OFFSET_SEPARATE)
	vCoord0 = TexCoord0 * TexCoord0_scale + TexCoord0_offset;
#elif (TEXTURE_TYPE == TEXTURE_2D || TEXTURE_TYPE == TEXTURE_RECT) && TEXCOORD_MODIFIER == TEXCOORD_TRANSFORM_3
	vCoord0 = (TexCoord0_transform * vec3(TexCoord0, 1.0)).xy;
#elif (TEXTURE_TYPE == TEXTURE_2D || TEXTURE_TYPE == TEXTURE_RECT) && TEXCOORD_MODIFIER == TEXCOORD_SCALE_OFFSET
	vCoord0 = TexCoord0 * TexCoord0_scaleoffset.xy + TexCoord0_scaleoffset.zw;
#elif (TEXTURE_TYPE == TEXTURE_2D || TEXTURE_TYPE == TEXTURE_RECT) && TEXCOORD_MODIFIER == TEXCOORD_SCALE_OFFSET_SEPARATE
	vCoord0 = TexCoord0 * TexCoord0_scale + TexCoord0_offset;
#else
	vCoord0 = TexCoord0;
#endif
	vColor0 = Color0;
#if defined(ON_FAR)
	highp vec4 pos = (WorldViewProjectionMatrix * Vertex).xyww;
	// some devices do not interpolate between 1.0 and 1.0 without floating
	// point errors (most notably the Mali-400 MP), making sometimes the
	// resulting value slightly greater than 1.0, so we scale the "z" down to
	// make it slithly less than 1.0 (after perspective division)
	pos.z *= 0.999999;
	gl_Position = pos;
#else
	gl_Position = WorldViewProjectionMatrix * Vertex;
#endif
}
