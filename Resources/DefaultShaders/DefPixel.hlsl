Texture2D shaderTexture		: register( t0 );
Texture2D normalMap			: register (t1);
SamplerState SampleType		: register( s0 );

struct PS_IN
{
	float4 pos_SV		: SV_POSITION;
	float4 pos_WS		: POSITION;
	float2 texCoord		: TEXCOORD;
	float3 normal		: NORMAL;
	float4 lightViewPos	: TEXCOORD2;
	float3 tangent		: TANGENT;
	float3 bitangent    : TANGENT2;
	
};

struct PS_OUT
{
	float4 normal			: SV_Target0;
	float4 diffuse			: SV_Target1;
	float4 pos				: SV_Target2;
	float4 lightViewPos		: SV_Target3;
};

PS_OUT PS_main( in PS_IN input)
{
	PS_OUT output;
	//Texture space 
	float3x3 texSpace = float3x3(input.tangent, input.bitangent, input.normal);

	float4 nMap = normalMap.Sample(SampleType, input.texCoord);

	//Range from -1 to 1.
	nMap = (2.f * nMap) - 1.f;

	//Check for orthogonal tangent to normal.

	output.normal = float4(normalize(mul(nMap, texSpace)), 0.f);


	// Output G-buffer values
	output.diffuse = shaderTexture.Sample(SampleType, input.texCoord);
	//output.diffuse = float4(1, 1, 1, 1);//shaderTexture.Sample(SampleType, input.texCoord); !För debugging av nMap
	output.pos = input.pos_WS;
	output.lightViewPos = input.lightViewPos;

	return output;
};