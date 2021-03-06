
#define NUM_CLIP_DISTANCES 8
#define WALL_STUMP_FROM_CENTER 0.725f

layout(location = 0) out vec4 g_OutColor;
layout(location = 1) out vec4 g_Normal;

in VS_OUT
{
	vec3 WorldPosition;
	vec3 ObjectPosition;
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoords;
} fs_in;

layout(binding = 0) uniform sampler2D g_DiffuseMap;
layout(binding = 1) uniform sampler2D g_NormalMap;
layout(binding = 2) uniform sampler2D g_SpecularMap;
//layout(binding = 2) uniform sampler2D g_DissolveMap;

//layout(std140, binding = 0) uniform PerFrame
//{
//	mat4 g_ViewProjection;
//	vec3 g_CameraPosition;
//	float g_Padding;
//	vec3 g_CameraLookAt;
//	float g_Padding2;
//	vec4 g_ClipDistances[NUM_CLIP_DISTANCES];
//};
//
//layout(std140, binding = 1) uniform PerObject
//{
//	vec4 g_Color;
//	float g_HasTexture;
//	float g_HasNormalMap;
//};

layout(std140, binding = 0) uniform CameraBuffer
{
	mat4 g_ProjectionView;
	mat4 g_View;
	mat4 g_Projection;
	mat4 g_InverseView;
	mat4 g_InverseProjection;
	vec3 g_CameraPosition;
};

layout(std140, binding = 2) uniform DefferedMaterialBuffer
{
	vec4 g_Color;
	float g_Specular;
	float g_HasDiffuseMap;
	float g_HasNormalMap;
	float g_HasSpecularMap;
	float g_DissolvePercentage;
};

vec3 mod289(vec3 x)
{
	return x - floor(x * (1.0f / 289.0f)) * 289.0f;
} 

vec4 mod289(vec4 x) 
{
	return x - floor(x * (1.0f / 289.0f)) * 289.0f;
}

vec4 permute(vec4 x)
{
	return mod289(((x * 34.0f) + 1.0f) * x);
}

vec4 taylorInvSqrt(vec4 r)
{
	return 1.79284291400159f - 0.85373472095314f * r;
}

vec3 fade(vec3 t)
{
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float snoise(vec3 v)
{ 
	const vec2  C = vec2(1.0f / 6.0f, 1.0f / 3.0f);
	const vec4  D = vec4(0.0f, 0.5f, 1.0f, 2.0f);

	//First corner
	vec3 i  = floor(v + dot(v, C.yyy));
	vec3 x0 = v - i + dot(i, C.xxx);
	
	//Other corners
	vec3 g = step(x0.yzx, x0.xyz);
	vec3 l = 1.0f - g;
	vec3 i1 = min(g.xyz, l.zxy);
	vec3 i2 = max(g.xyz, l.zxy);
	
	//   x0 = x0 - 0.0 + 0.0 * C.xxx;
	//   x1 = x0 - i1  + 1.0 * C.xxx;
	//   x2 = x0 - i2  + 2.0 * C.xxx;
	//   x3 = x0 - 1.0 + 3.0 * C.xxx;
	vec3 x1 = x0 - i1 + C.xxx;
	vec3 x2 = x0 - i2 + C.yyy;
	vec3 x3 = x0 - D.yyy;
	
	//Permutations
	i = mod289(i); 
	vec4 p = permute(permute(permute( 
		  i.z + vec4(0.0f, i1.z, i2.z, 1.0f))
		+ i.y + vec4(0.0f, i1.y, i2.y, 1.0f)) 
		+ i.x + vec4(0.0f, i1.x, i2.x, 1.0f));
	
	//Gradients: 7x7 points over a square, mapped onto an octahedron.
	//The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
	float n_ = 0.142857142857f; // 1.0/7.0
	vec3  ns = n_ * D.wyz - D.xzx;
	
	vec4 j = p - 49.0f * floor(p * ns.z * ns.z);  //  mod(p,7*7)
	
	vec4 x_ = floor(j * ns.z);
	vec4 y_ = floor(j - 7.0f * x_ );    // mod(j,N)
	
	vec4 x = x_ *ns.x + ns.yyyy;
	vec4 y = y_ *ns.x + ns.yyyy;
	vec4 h = 1.0f - abs(x) - abs(y);
	
	vec4 b0 = vec4(x.xy, y.xy);
	vec4 b1 = vec4(x.zw, y.zw);
	
	//vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
	//vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
	vec4 s0 = floor(b0) * 2.0f + 1.0f;
	vec4 s1 = floor(b1) * 2.0f + 1.0f;
	vec4 sh = -step(h, vec4(0.0f));
	
	vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy;
	vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww;
	
	vec3 p0 = vec3(a0.xy, h.x);
	vec3 p1 = vec3(a0.zw, h.y);
	vec3 p2 = vec3(a1.xy, h.z);
	vec3 p3 = vec3(a1.zw, h.w);
	
	//Normalise gradients
	vec4 norm = taylorInvSqrt(vec4(dot(p0, p0), dot(p1, p1), dot(p2, p2), dot(p3, p3)));
	p0 *= norm.x;
	p1 *= norm.y;
	p2 *= norm.z;
	p3 *= norm.w;
	
	//Mix final noise value
	vec4 m = max(0.6f - vec4(dot(x0, x0), dot(x1, x1), dot(x2, x2), dot(x3, x3)), 0.0f);
	m = m * m;
	return 42.0f * dot(m*m, vec4(dot(p0, x0), dot(p1, x1), dot(p2, x2), dot(p3, x3)));
}

#define EPSILON 0.1f

vec3 EncodeNormals(vec3 normal)
{
	return (normalize(normal) + vec3(1.0f)) * 0.5f;
}

float EncodeSpecular(float specular)
{
	return specular / 256.0f;
}

void main()
{
	//SPECULAR
	float specular = (texture(g_SpecularMap, fs_in.TexCoords).r * g_HasSpecularMap) + (EncodeSpecular(g_Specular) * (1.0f - g_HasSpecularMap));
    
	//COLOR
	vec3 mappedColor = texture(g_Texture, fs_in.TexCoords).rgb * g_HasTexture;
	vec3 uniformColor = g_Color.rgb * (1.0f - g_HasTexture);
	g_OutColor = vec4(uniformColor + mappedColor, specular);

	if (fs_in.ObjectPosition.y - fs_in.WorldPosition.y < WALL_STUMP_FROM_CENTER)
	{
		float minDissolve = g_DissolvePercentage * ((snoise(fs_in.WorldPosition.xyz) / 2.0f) + 0.5f);

		float isNotUp = abs(dot(fs_in.Normal, vec3(0.0f, 1.0f, 0.0f)));
		vec3 cameraForward = normalize(g_CameraLookAt.xyz - g_CameraPosition.xyz);
		vec3 objectToLookAt = vec3(g_CameraLookAt.x, 0.0f, g_CameraLookAt.z) - vec3(fs_in.ObjectPosition.x, 0.0f, fs_in.ObjectPosition.z);
		float distanceToLookAt = abs(dot(objectToLookAt, vec3(cameraForward.x, 0.0f, cameraForward.z)));

		if (distanceToLookAt < minDissolve)
		{
			if (distanceToLookAt < minDissolve - EPSILON)
			{
				discard;
			}

			float lerpValue = 0.5f + (((distanceToLookAt + EPSILON) - minDissolve) / EPSILON) / 2.0f;
			g_OutColor.rgb = g_OutColor.rgb * lerpValue;
		}
	}
	
	//NORMAL
	vec3 mappedNormal = (texture(g_NormalMap, fs_in.TexCoords).xyz * 2.0f) - vec3(1.0f);
	
	mat3 tbn = mat3(fs_in.Tangent, fs_in.Binormal, fs_in.Normal);
	mappedNormal = tbn * mappedNormal;

	vec3 normal = (fs_in.Normal * (1.0f - g_HasNormalMap)) + (mappedNormal * g_HasNormalMap);
	normal = EncodeNormals(normal);

	g_Normal = vec4(normal, 1.0f);
}