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



attribute HIGHP vec3 position;
attribute MEDIUMP vec2 texcoord0;
#ifdef USE_VCOLOR
attribute LOWP vec4 color0;
#endif

uniform HIGHP mat4 wvp;
uniform HIGHP mat4 world;
uniform HIGHP mat4 viewprojection;

#ifdef USE_VERTEX_ANIM
uniform HIGHP vec3 VertexMovementForce;
uniform HIGHP float Time;

HIGHP float cheapWave(HIGHP float r)
{
	float x = fract(r);
	return x * (x - 0.5) * (x - 1.0);
}
#define WORLD_UP vec3(0.0, 0.0, 1.0)

HIGHP vec3 animate(HIGHP vec3 worldP)
{
	const HIGHP vec3 worldUp = WORLD_UP;
	HIGHP vec3 worldCenter = (world*vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	HIGHP vec3 worldDirFromCenter = worldP-worldCenter;
	HIGHP vec3 worldTangent = cross(worldDirFromCenter, worldUp);
	worldTangent.x += 0.001; // prevent 0 length
	worldTangent = normalize(worldTangent);
	HIGHP float frequencyOffset = worldTangent.x+worldTangent.y;
	HIGHP float maxAmplitude = VertexMovementForce.y*color0.a;

	const HIGHP float PIMul2 = 1.0 / (3.14159265 * 2.0);
	HIGHP float frequencyOffsetForCheapWave = (frequencyOffset+Time)*PIMul2;
	HIGHP float tangentAmplitude = cheapWave(VertexMovementForce.x + frequencyOffsetForCheapWave)*maxAmplitude;
	HIGHP float verticalAmplitude = cheapWave(VertexMovementForce.z + frequencyOffsetForCheapWave)*maxAmplitude;
	
	//apply the vertex offset
	worldP.xyz += worldTangent*tangentAmplitude;
	worldP.xyz += worldUp*verticalAmplitude;
	return worldP;
}
#endif

void main()
{
	HIGHP vec4 worldP = world*vec4(position, 1.0);

#ifdef USE_VERTEX_ANIM
	worldP.xyz = animate(worldP.xyz);
#endif

	gl_Position = viewprojection*worldP;

#ifdef USE_VCOLOR
	vColor = color0;
#endif	
	vTexcoord0 = texcoord0;

}