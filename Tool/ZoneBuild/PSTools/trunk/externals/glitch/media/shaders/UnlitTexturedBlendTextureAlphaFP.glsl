uniform lowp    sampler2D texture;

#ifdef SPLIT_ALPHA
uniform lowp    sampler2D texture_alpha;
uniform lowp    int       texture_alphachannel;
#endif

varying mediump vec2 vCoord0;
varying	lowp    vec4 vColor0;

void main()
{
#ifdef SPLIT_ALPHA
    lowp vec4 texColor = vec4(texture2D(texture, vCoord0).rgb,
							  texture2D(texture_alpha, vCoord0)[texture_alphachannel]);
#else
    lowp vec4 texColor = texture2D(texture, vCoord0);
#endif
    gl_FragColor = vec4(texColor.rgb *  vColor0.rgb, texColor.a);
}
