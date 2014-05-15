varying mediump vec4 vColor0;

//uniform mediump	vec4 brushPos;
//uniform lowp	float brushSize;

void main()
{
	//float strengthUnclamped = length(gl_TexCoord[5].xyz - brushPos.xyz) / brushSize;
	//float strength = strengthUnclamped > 1.0f ? 0.0f : 1.f - strengthUnclamped;
	//float on = abs(strengthUnclamped - 1.f) < 0.05f ? 1.f : 0.f;

	//gl_FragColor = on * vec4(0.0, 1.0, 0.0, 0.5);

	gl_FragColor = vColor0;
}

