attribute highp   vec4 Vertex;
attribute mediump vec2 TexCoord0;
attribute mediump vec4 Color0;

uniform   highp   mat4 WorldViewProjectionMatrix;

varying	  mediump vec2 vCoord0;
varying	  mediump vec4 vColor0;

void main(void)
{
	vCoord0 = vec2(TexCoord0.x, TexCoord0.y);
	vColor0 = Color0;
	gl_Position = WorldViewProjectionMatrix * Vertex;
}

