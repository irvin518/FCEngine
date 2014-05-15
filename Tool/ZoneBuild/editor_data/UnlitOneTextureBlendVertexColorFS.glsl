uniform lowp    sampler2D texture1;

varying mediump vec2 vCoord0;
varying mediump vec4 vColor0;

void main()
{
	mediump vec4 texColor1 = texture2D(texture1, vCoord0);

	gl_FragColor = texColor1 * vColor0;
}

