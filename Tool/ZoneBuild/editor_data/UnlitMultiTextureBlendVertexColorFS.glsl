uniform lowp    sampler2D texture1;
uniform lowp    sampler2D texture2;
uniform lowp    sampler2D texture3;
uniform lowp    sampler2D texture4;

varying mediump vec2 vCoord0;
varying mediump vec2 vCoord1;
varying mediump vec2 vCoord2;
varying mediump vec2 vCoord3;

varying mediump vec4 vColor0;

void main()
{
	mediump vec4 texColor1 = texture2D(texture1, vCoord0);
	mediump vec4 texColor2 = texture2D(texture2, vCoord1);
	mediump vec4 texColor3 = texture2D(texture3, vCoord2);
	mediump vec4 texColor4 = texture2D(texture4, vCoord3);

	mediump	vec4 layer = texColor1;

 	layer	= vec4((1.0 - texColor2.a) * layer.rgb + texColor2.a * texColor2.rgb, max(layer.a, texColor2.a));
	layer 	= vec4((1.0 - texColor3.a) * layer.rgb + texColor3.a * texColor3.rgb, max(layer.a, texColor3.a));
	layer 	= vec4((1.0 - texColor4.a) * layer.rgb + texColor4.a * texColor4.rgb, max(layer.a, texColor4.a));

	gl_FragColor = layer * vColor0;

	//gl_FragColor = vec4((1.0 - texColor2.a) * texColor1.rgb + texColor2.a * texColor2.rgb, texColor1.a) * vColor0;
}

