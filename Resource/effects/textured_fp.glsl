// Do not modify this file directly, plz check *.gx

#define LOWP lowp
#define MEDIUMP mediump
#define HIGHP highp   


#ifdef USE_VERTEX_ANIM
# ifndef USE_VCOLOR
#  define USE_VCOLOR
# endif
#endif


////////////////////////////////////////////////// all

varying MEDIUMP vec2 vTexcoord0;
#if defined USE_VCOLOR
  varying LOWP vec4 vColor;
#endif



uniform LOWP sampler2D DiffuseSampleMap;
void main()
{
	LOWP vec4 col = texture2D(DiffuseSampleMap, vTexcoord0);
#ifdef USE_VCOLOR
	col.rgb *= vColor.rgb;
#endif

	LOWP float alpha;

#if !(defined USE_VERTEX_ANIM) && (defined USE_VCOLOR)
	alpha = col.a * vColor.a;
#else
	alpha = col.a;
#endif

	gl_FragColor = vec4(col.rgb, alpha);
}


