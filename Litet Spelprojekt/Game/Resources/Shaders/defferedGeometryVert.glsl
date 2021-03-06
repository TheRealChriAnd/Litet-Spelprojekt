
#define NUM_CLIP_DISTANCES 8

layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec3 g_Tangent;
layout(location = 3) in vec2 g_TexCoords;
layout(location = 4) in mat4 g_InstanceModel;

out VS_OUT
{
	vec3 WorldPosition;
	vec3 ObjectPosition;
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoords;
} vs_out;

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

layout(std140, binding = 1) uniform PerObject
{
	vec4 g_Color;
	float g_HasTexture;
	float g_HasNormalMap;
	float g_HasSpecularMap;
	float g_DissolvePercentage;
};

#define WALL_STUMP_FROM_CENTER 0.725f

void main()
{
	vec4 worldPos = g_InstanceModel * vec4(g_Position, 1.0);

	//CLIPPING WALLS
	vec3 toLookAt = normalize(g_CameraLookAt - g_InstanceModel[3].xyz);
	vec3 cameraForward = normalize(g_CameraLookAt - g_CameraPosition);
	float dotToLookAtForward = dot(vec3(cameraForward.x, 0.0f, cameraForward.z), vec3(toLookAt.x, 0.0f, toLookAt.z));
	float cutWalls = 1.0f;

	if (dotToLookAtForward > 0.0f)
	{
		vec4 wallClipPlane = vec4(0.0f, -1.0f, 0.0f, g_InstanceModel[3].y - WALL_STUMP_FROM_CENTER);
		cutWalls = dot(worldPos, wallClipPlane);
	}

	gl_ClipDistance[0] = cutWalls;

	//CLIPPING DEPENDING ON LEVEL
	gl_ClipDistance[1] = dot(worldPos, g_ClipDistances[1]);
	gl_ClipDistance[2] = dot(worldPos, g_ClipDistances[2]);

	vec3 normal = (g_InstanceModel * vec4(g_Normal, 0.0f)).xyz;
	vec3 tangent = (g_InstanceModel * vec4(g_Tangent, 0.0f)).xyz;
	
	vs_out.WorldPosition = worldPos.xyz;
	vs_out.ObjectPosition = g_InstanceModel[3].xyz;
	vs_out.Normal = normal;
	vs_out.Tangent = tangent;
	vs_out.Binormal = cross(vs_out.Normal, vs_out.Tangent);
	vs_out.TexCoords = g_TexCoords;

	gl_Position = g_ProjectionView * worldPos;
}