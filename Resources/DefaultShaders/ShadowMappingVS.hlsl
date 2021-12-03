cbuffer ContantBuffer : register(b0)
{
	float4x4 WorldMatrix;
	float4x4 ViewMatrix;  // The view matrix of the light
	float4x4 ProjectionMatrix; // ProjectionMatrix for the light
	float4x4 LightViewMatrix;
	float4x4 LightProjectionMatrix;
};

struct VS_IN
{
	float3 pos			: POSITION;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
	float3 tangent	: TANGENT;
};

struct PS_IN
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE;
};


PS_IN ShadowMapVS(VS_IN input)
{
	PS_IN output;
	output.position = float4(input.pos, 1.f);

	output.position = mul(WorldMatrix, output.position);
	output.position = mul(LightViewMatrix, output.position);
	output.position = mul(LightProjectionMatrix, output.position);
	output.depthPosition = output.position;

	return output;
}