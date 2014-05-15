#ifdef GL_FRAGMENT_PRECISION_HIGH
	// define maxp based on user preference
	#ifdef GLITCH_USE_HIGHP
		#define MAXP highp
	#else
		#define MAXP mediump
	#endif
#else
	// highp not supported, force maxp as medium
	#define MAXP mediump
#endif

attribute highp vec4 Position;

#if !defined(GLITCH_DEBUG_SUBSTITUTE)
attribute lowp vec4 Color0;
#endif

#if defined(TEXTURED) || (defined(LIGHTMAP) && !defined(GLITCH_DEBUG_SUBSTITUTE))
attribute MAXP vec2 TexCoord0;
varying MAXP vec2 vTexCoord0;
uniform MAXP mat4 TexCoord0_transform;
#endif

#if defined(LIGHTMAP) && defined(TEXTURED) && !defined(GLITCH_DEBUG_SUBSTITUTE)
attribute MAXP vec2 TexCoord1;
varying MAXP vec2 vTexCoord2;
uniform MAXP mat4 TexCoord1_transform;
#    define LightMapCoord TexCoord1
#    define LightMapCoord_transform TexCoord1_transform
#elif defined(LIGHTMAP)
#    define LightMapCoord TexCoord0
#    define LightMapCoord_transform TexCoord0_transform
#endif

#if defined(MULTITEXTURED) && !defined(GLITCH_DEBUG_SUBSTITUTE)
varying MAXP vec2 vTexCoord1;
uniform highp mat4 matWorldViewIT;
#endif //MULTITEXTURED

uniform highp mat4 WorldViewProjectionMatrix;

#if !defined(GLITCH_DEBUG_SUBSTITUTE)
uniform mediump vec4 DiffuseColor;
#endif

#if defined(FOG) && !defined(GLITCH_DEBUG_SUBSTITUTE)
uniform highp mat4 WorldViewT;
varying lowp float fogFactor;
uniform mediump vec2 fogstartend;
#endif //FOG

#if defined(SKINNED)
uniform highp mat4 BoneMatrices[48];
uniform lowp vec4 WeightMask;
attribute mediump vec4 SkinWeights;
attribute mediump vec4 SkinIndices;
#endif //SKINNED

#if (defined LIGHTING || defined MULTITEXTURED) && !defined(GLITCH_DEBUG_SUBSTITUTE)
attribute highp vec3 Normal;
uniform highp mat4 matWorld;
uniform highp mat4 matWorldIT;
#endif

#if defined(LIGHTING) && !defined(GLITCH_DEBUG_SUBSTITUTE)
// Light0
uniform highp vec4 Light0Position;
uniform vec3 Light0Attenuation;
uniform vec4 Light0Ambientcolormultiplied;
uniform vec4 Light0Diffusecolormultiplied;
uniform vec4 Light0Specularcolormultiplied;

// MaterialColor
uniform vec4 EmissionColor;
uniform vec4 AmbientColor;
uniform vec4 SpecularColor;
uniform float Shininess;
uniform vec4 SceneAmbientLight;
#endif //LIGHTING

#if !defined(GLITCH_DEBUG_SUBSTITUTE)
varying lowp vec4 vColor0;
#endif

void main(void)
{
#if defined SKINNED
    vec4 weights = SkinWeights * WeightMask;
    vec3 netPosition = vec3(0.0, 0.0, 0.0);
#   if (defined LIGHTING || defined MULTITEXTURED) && !defined(GLITCH_DEBUG_SUBSTITUTE)
    vec3 netNormal = vec3(0.0, 0.0, 0.0);
#   endif
	vec4 indices = SkinIndices;
    for (int i = 0; i < 4; i++)
    {
        int index = int(indices[i]);
        netPosition += weights[i] * (BoneMatrices[index] * vec4(Position.xyz, 1)).xyz;
#   if (defined LIGHTING || defined MULTITEXTURED) && !defined(GLITCH_DEBUG_SUBSTITUTE)
        netNormal += weights[i] * (BoneMatrices[index] * vec4(Normal.xyz, 0)).xyz;
#   endif
    }
    gl_Position = WorldViewProjectionMatrix * vec4(netPosition,1);
#   if (defined LIGHTING || defined MULTITEXTURED) && !defined(GLITCH_DEBUG_SUBSTITUTE)
    vec3 transformedNormal = (matWorldIT * vec4(netNormal, 0.0)).xyz;
    vec3 worldPosition = (matWorld * vec4(netPosition,1)).xyz;
#   endif
#   if defined(FOG) && !defined(GLITCH_DEBUG_SUBSTITUTE)
    float depth = dot(WorldViewT[2], vec4(netPosition,1));
#   endif

#else //No skinning
    gl_Position = WorldViewProjectionMatrix * Position;
#   if (defined LIGHTING || defined MULTITEXTURED) && !defined(GLITCH_DEBUG_SUBSTITUTE)
    vec3 transformedNormal = (matWorldIT * vec4(Normal, 0.0)).xyz;
    vec4 worldPosition = matWorld * Position;
#   endif
#   if defined(FOG) && !defined(GLITCH_DEBUG_SUBSTITUTE)
    float depth = dot(WorldViewT[2], Position);
#   endif
#endif //SKINNED

#if !defined(GLITCH_DEBUG_SUBSTITUTE)
    vColor0 = Color0 * DiffuseColor;
#endif
 
#ifdef TEXTURED
    vTexCoord0 = (TexCoord0_transform * vec4(TexCoord0, 1, 0)).xy;
#endif
#if defined(MULTITEXTURED) && !defined(GLITCH_DEBUG_SUBSTITUTE)
    vec2 n = (matWorldViewIT * vec4(Normal, 1.0)).xy;
    vTexCoord1.x = n.x * 0.5 + 0.5;
    vTexCoord1.y = 0.5 - n.y * 0.5;
#endif //MULTITEXTURED
#if defined(LIGHTMAP) && !defined(GLITCH_DEBUG_SUBSTITUTE)
    vTexCoord2 = (LightMapCoord_transform * vec4(LightMapCoord, 1, 0)).xy;
#endif
 
#if defined(FOG) && !defined(GLITCH_DEBUG_SUBSTITUTE)
    fogFactor = clamp((-depth - fogstartend[0])/(fogstartend[1] - fogstartend[0]), 0.0, 1.0);
#endif //FOG

#if defined(LIGHTING) && !defined(GLITCH_DEBUG_SUBSTITUTE)
    // Clear the light intensity accumulators
    vec3 eye      = vec3 (0.0, 0.0, 1.0);
    vec4 Ambient  = vec4 (0.0, 0.0, 0.0, 0.0);
    vec4 Diffuse  = vec4 (0.0, 0.0, 0.0, 0.0);
    vec4 Specular = vec4 (0.0, 0.0, 0.0, 0.0);
 
    // light 0 (point/directional light)
    vec3 Light0Vec = Light0Position.xyz - worldPosition.xyz * Light0Position.w;
    float d = length(Light0Vec);  // Compute distance between surface and light position
    vec3 VP = normalize(Light0Vec);// Normalize the vector from surface to light position
    float attenuation = 1.0 / (Light0Attenuation.x + Light0Attenuation.y * d + Light0Attenuation.z * d * d);
    vec3  halfVector = normalize(VP + eye);
    float nDotVP = max(0.0, dot(transformedNormal, VP));
    float nDotHV = max(0.0, dot(transformedNormal, halfVector));
    float pf = ( nDotVP > 0.0 ? pow(nDotHV, max(Shininess, 0.0001)) : 0.0 );
    Specular += Light0Specularcolormultiplied * pf * attenuation;
    Ambient  += Light0Ambientcolormultiplied * attenuation;
    Diffuse  += Light0Diffusecolormultiplied * nDotVP * attenuation;

    vec4 color = EmissionColor + (SceneAmbientLight + Ambient) * AmbientColor + Diffuse  * DiffuseColor + Specular * SpecularColor;
    color = clamp( color, 0.0, 1.0 );
    vColor0 = color;
#endif //LIGHTING
}
