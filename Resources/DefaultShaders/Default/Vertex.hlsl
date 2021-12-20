struct VS_IN
{
	float4 pos			: SV_POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 biTangent	: BITANGENT;
	float2 uv			: TEXCOORD;
};

struct VS_OUT {
	float4 pos			: SV_POSITION;
	float4 worldPos		: POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 biTangent	: BITANGENT;
	float2 uv			: TEXCOORD;
};

cbuffer FrameBuffer : register(b0) {
	float4x4 ViewMatrix;
	float4x4 ProjectionMatrix;
};
cbuffer ObjectBuffer : register(b1) {
	float4x4 WorldMatrix;
}


VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos, 1.f);

	output.pos = output.worldPos = mul(WorldMatrix, output.pos);
	output.pos = mul(ViewMatrix, output.pos);
	output.pos = mul(ProjectionMatrix, output.pos);
	

	output.normal = mul(WorldMatrix, input.normal);
	//output.color = mul(WorldMatrix, float4(input.pos, 1.f));
	output.color = input.color;

	return output;
}