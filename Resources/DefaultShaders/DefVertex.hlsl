struct VS_IN
{
	float3 pos		: POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
};

struct VS_OUT
{
	float4 pos		: POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
};


VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	
	output.pos = float4(input.pos, 1.f);
	output.texCoord = input.texCoord;
	output.normal = input.normal;
	output.tangent = input.tangent;
	return output;
}