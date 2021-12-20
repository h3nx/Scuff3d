struct PS_IN
{
	float4 pos			: SV_POSITION;
	float4 worldPos		: POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 biTangent	: BITANGENT;
	float2 uv			: TEXCOORD;
};

float4 main(PS_IN input) : SV_TARGET
{
	return float4(input.color.x,input.color.y,0,1);
}