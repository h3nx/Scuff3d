struct PS_IN
{
	float4 position			: SV_POSITION;
	float4 depthPosition	: TEXTURE;
};

float4 ShadowMapPS(PS_IN input) : SV_TARGET
{
	float depthValue = input.depthPosition.z / input.depthPosition.w;
	return float4(depthValue, depthValue, depthValue, 1.f);
}