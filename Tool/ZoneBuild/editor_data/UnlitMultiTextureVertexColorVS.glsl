attribute highp   vec4 Vertex;

attribute mediump vec2 TexCoord0;
attribute mediump vec2 TexCoord1;
attribute mediump vec2 TexCoord2;
attribute mediump vec2 TexCoord3;

attribute mediump vec4 Color0;

uniform   highp   mat4 WorldViewProjectionMatrix;

varying	  mediump vec2 vCoord0;
varying	  mediump vec2 vCoord1;
varying	  mediump vec2 vCoord2;
varying	  mediump vec2 vCoord3;

varying	  mediump vec4 vColor0;

void main(void)
{
	vCoord0 = vec2(TexCoord0.x, TexCoord0.y);
	vCoord1 = vec2(TexCoord1.x, TexCoord1.y);
	vCoord2 = vec2(TexCoord2.x, TexCoord2.y);
	vCoord3 = vec2(TexCoord3.x, TexCoord3.y);

	vColor0 = Color0;

	gl_Position = WorldViewProjectionMatrix * Vertex;
}

