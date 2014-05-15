attribute highp   vec4 Vertex;
attribute mediump vec4 Color0;

uniform   highp   mat4 WorldViewProjectionMatrix;

varying	  mediump vec4 vColor0;

void main(void)
{
	vColor0 = Color0;
	gl_Position = WorldViewProjectionMatrix * Vertex;

	//gl_TexCoord[5] = vec4(Vertex.x, Vertex.y, Vertex.z, 0.f);
}

