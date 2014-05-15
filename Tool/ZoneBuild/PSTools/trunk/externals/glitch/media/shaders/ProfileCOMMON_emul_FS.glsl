#ifdef GL_FRAGMENT_PRECISION_HIGH
	// define maxp based on user preference
	#ifdef GLITCH_USE_HIGHP
		#define MAXP highp
	#else
		#define MAXP mediump
	#endif
#else
	// force maxp as medium
	#define MAXP mediump

	// don't use bias, even if the user specify it, unless he/she explicitly force it
	#ifdef GLITCH_FORCE_USE_BIAS
		#ifndef GLITCH_USE_BIAS
			#define GLITCH_USE_BIAS
		#endif
	#else
		#ifdef GLITCH_USE_BIAS
			#undef GLITCH_USE_BIAS
		#endif
	#endif
#endif

#if defined(GLITCH_USE_BIAS) && !defined(GLITCH_DEBUG_SUBSTITUTE)
	#define TEXTURE2D(sampler, coord, bias) texture2D(sampler, coord, bias)
#else
	#define TEXTURE2D(sampler, coord, bias) texture2D(sampler, coord)
#endif

#ifdef TEXTURED
uniform lowp sampler2D Sampler0;
#    ifdef SPLIT_ALPHA
uniform lowp sampler2D Sampler0_alpha;
#    endif
varying MAXP vec2 vTexCoord0;
#    if defined(GLITCH_USE_BIAS) && !defined(GLITCH_DEBUG_SUBSTITUTE)
uniform mediump float Sampler0_bias;
#    endif
#endif
#if defined(MULTITEXTURED) && !defined(GLITCH_DEBUG_SUBSTITUTE)
uniform lowp sampler2D Sampler1;
varying MAXP vec2 vTexCoord1;
uniform lowp float envmapIntensity;
#    ifdef GLITCH_USE_BIAS
uniform mediump float Sampler1_bias;
#    endif
#endif //MULTITEXTURED
#if defined(LIGHTMAP) && !defined(GLITCH_DEBUG_SUBSTITUTE)
varying MAXP vec2 vTexCoord2;
uniform lowp sampler2D Sampler2;
#    ifdef GLITCH_USE_BIAS
uniform mediump float Sampler2_bias;
#    endif
#endif

#if defined(FOG) && !defined(GLITCH_DEBUG_SUBSTITUTE)
uniform lowp vec4 fogcolor;
varying lowp float fogFactor;
#endif //FOG

#if defined(GLITCH_DEBUG_SUBSTITUTE)
uniform lowp vec4 DiffuseColor;
#else
varying lowp vec4 vColor0;
#endif

#if defined(ALPHATEST) && !defined(GLITCH_DEBUG_SUBSTITUTE)
uniform lowp float AlphaRef;
#endif

void main()
{
#if defined(GLITCH_DEBUG_SUBSTITUTE)
	lowp vec4 color = DiffuseColor;
#else
	lowp vec4 color = vColor0;
#endif
#ifdef TEXTURED
	#ifdef SPLIT_ALPHA
	color *= vec4(TEXTURE2D(Sampler0, vTexCoord0, Sampler0_bias).rgb,
				  TEXTURE2D(Sampler0_alpha, vTexCoord0, Sampler0_bias).g);
	#else
	color *= TEXTURE2D(Sampler0, vTexCoord0, Sampler0_bias);
	#endif
#endif
#if defined(LIGHTMAP) && !defined(GLITCH_DEBUG_SUBSTITUTE)
	color *= TEXTURE2D(Sampler2, vTexCoord2, Sampler2_bias);
#endif

#if defined(ALPHATEST) && !defined(GLITCH_DEBUG_SUBSTITUTE)
	if (color.a <= AlphaRef)
	{
		discard;
	}
#endif

#if defined(MULTITEXTURED) && !defined(GLITCH_DEBUG_SUBSTITUTE)
	color.rgb += TEXTURE2D(Sampler1, vTexCoord1, Sampler1_bias).rgb * envmapIntensity;
#endif //MULTITEXTURED	

#if defined(FOG) && !defined(GLITCH_DEBUG_SUBSTITUTE)
#    ifdef ADDITIVEBLEND
    // Additiveblend does not need to blend with the fog color, just reduce it's
    // additive contribution to the upcomming blending.
    color.rgb = mix(color.rgb, vec3(0.0, 0.0, 0.0), fogFactor * length(color.rgb) * color.a);
#    else
    color.rgb = mix(color.rgb, fogcolor.rgb, fogFactor);
#    endif
#endif //FOG
	gl_FragColor = color;
}
