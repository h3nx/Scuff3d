
cbuffer ObjectBuffer : register(b0)
{
	float4x4 WorldMatrix;
	float4x4 ViewMatrix;
	float4x4 ProjectionMatrix;
	float4x4 LightViewMatrix;
	float4x4 LightProjectionMatrix;
};

cbuffer FrameBuffer : register(b1)
{
	float4 direction;
	float3 camPos;
};


struct GS_IN
{
	float4 pos		: POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
};

struct GS_OUT
{
	float4 pos_SV		: SV_POSITION;
	float4 pos_WS		: POSITION;
	float2 texCoord		: TEXCOORD;
	float3 normal		: NORMAL;
	float4 lightViewPos	: TEXCOORD2;
	float3 tangent		: TANGENT;
	float3 bitangent	: TANGENT2;
	
};

[maxvertexcount(12)]
void GS_main( triangle GS_IN IN[3], inout TriangleStream< GS_OUT > output )
{
	for (int i = 0; i < 3; i++)
	{
		GS_OUT element;

		element.normal = mul(WorldMatrix, IN[i].normal);
		element.pos_SV = IN[i].pos;

		element.pos_SV = mul(WorldMatrix, element.pos_SV);
		float3 camToVert = normalize(element.pos_SV - camPos);
		
		if (dot(element.normal, camToVert ) < 0.f) //Backfaceculling
		{
			
			element.pos_WS = element.pos_SV;
			element.pos_SV = mul(ViewMatrix, element.pos_SV);
			element.pos_SV = mul(ProjectionMatrix, element.pos_SV);

			// Used in shadow mapping
			element.lightViewPos = mul(WorldMatrix, IN[i].pos);
			element.lightViewPos = mul(LightViewMatrix, element.lightViewPos);
			element.lightViewPos = mul(LightProjectionMatrix, element.lightViewPos);

			element.texCoord = IN[i].texCoord;

			//NormalMap
			element.tangent = mul(WorldMatrix, IN[i].tangent);
			element.bitangent = cross(IN[i].normal, IN[i].tangent);

			output.Append(element);
		}
	}
	output.RestartStrip();
}



/*
		/Trodde först detta var problemet, men gjorde ingen skillnad, sparade just in case. 
		IN[i].tangent = normalize(IN[i].tangent - dot(IN[i].tangent, IN[i].normal)*IN[i].normal);
		element.normal = normalize(mul(WorldMatrix, IN[i].normal));

		element.tangent = normalize(mul(WorldMatrix, IN[i].tangent));
		element.bitangent = normalize(mul(WorldMatrix, element.bitangent));

*/