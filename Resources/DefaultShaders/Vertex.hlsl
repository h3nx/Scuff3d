struct VS_IN
{
	float3 pos			: POSITION;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
	float3 tangent		: TANGENT;
};

struct VS_OUT
{
	float4 pos			: SV_POSITION;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos, 1.f);
	output.normal = input.normal;
	output.texCoord = input.texCoord;
	
	return output;
}