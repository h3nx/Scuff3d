struct VS_IN
{
	float3 pos			: POSITION;
	float3 normal		: NORMAL;
	float4 color		: COLOR;
};

struct VS_OUT {
	float4 pos			: SV_POSITION;
	float3 normal		: NORMAL;
	float4 color		: COLOR;
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

	output.pos = mul(WorldMatrix, output.pos);
	output.pos = mul(ViewMatrix, output.pos);
	output.pos = mul(ProjectionMatrix, output.pos);
	

	output.normal = input.normal;
	//output.color = mul(WorldMatrix, float4(input.pos, 1.f));
	output.color = input.color;

	return output;
}