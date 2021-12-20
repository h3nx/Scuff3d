struct PS_IN
{
	float4 pos			: SV_POSITION;
	float3 normal		: NORMAL;
	float4 color		: COLOR;
};

float4 main(PS_IN input) : SV_TARGET
{
	return float4(input.normal,1);
}