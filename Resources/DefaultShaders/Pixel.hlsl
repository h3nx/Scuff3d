// Textures
Texture2D NormalTexture			: register (t0);
Texture2D DiffuseTexture		: register (t1);
texture2D PositionTexture		: register (t2);
texture2D lightViewPosTexture	: register (t3);
texture2D ShadowMappingTexture	: register (t4);

// Samplers
SamplerState SampleType		: register(s0); // Wrap
SamplerState ClampSampler	: register(s1); // Clamp

cbuffer defferedLightInfo : register(b0)
{
	float4 camPos;
	float4 ambientColor;
	float4 diffuseLightColor;
	float4 lightPos;
};

struct PS_IN
{
	float4 pos			: SV_POSITION;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
};

float4 main( in PS_IN input) : SV_Target
{
	float4 normal = NormalTexture.Sample(SampleType, input.texCoord);
	float4 color = DiffuseTexture.Sample(SampleType, input.texCoord); 
	float4 position = PositionTexture.Sample(SampleType, input.texCoord);
	float4 lpos = lightViewPosTexture.Sample(SampleType, input.texCoord);

	float2 projectTexCoord;
	float depthValue;
	float lightDepthValue;
	float lightIntensity;
	float4 textureColor;


	
	projectTexCoord.x = lpos.x / lpos.w / 2.0f + 0.5f;
	projectTexCoord.y = -lpos.y / lpos.w / 2.0f + 0.5f;

	
	lightDepthValue = lpos.z / lpos.w;
	lightDepthValue -= 0.00001f;
	//return projectTexCoord;// pow(depthValue, 300); //pow(lightDepthValue, 300);// normalize(normal); //diffuse * saturate(dot(normalize(normal), normalize(lightPos - position)));
	//return normal; //!Debugging

	//THIS IS HOW IT SHOULD WORK
	float4 ambient = ambientColor*color;
	float4 diffuse;
	float3 lightRay = lightPos - position;
	

	//Shadowmap
	if (saturate(projectTexCoord.x) == projectTexCoord.x && saturate(projectTexCoord.y) == projectTexCoord.y) //Check if between 0 and 1( saturate does this). 
	{
		depthValue = ShadowMappingTexture.Sample(ClampSampler, projectTexCoord).r;


		if (lightDepthValue <= depthValue) //If true not in shadow 
		{
			float3 lightRay = lightPos - position;

			float lightIntensity = 1.0f;
			float dotValue = dot(normalize(normal), normalize(lightRay));
			if ( dotValue > 0) //Check if light hits front side i.e angle between vectors less then 90 degrees. 
			{
				diffuse = saturate( float4(color.xyz * diffuseLightColor * dotValue, 1.f));
				
			}
			

			return saturate(diffuse * lightIntensity + ambient);
		}
	}

	return ambient;
}

//if (length(lightRay) > 25)
	//	return ambient;
	//float dotValue = dot(normalize(normal), normalize(lightRay));
	//if (dotValue > 0) //Check if light hits front side i.e angle between vectors less then 90 degrees. 
	//{
	//	diffuse = saturate(float4(color.xyz * diffuseLightColor * dotValue, 1.f));
	//	//lightIntensity = max(6.f / length(lightRay), 0.2f); 
	//}
	//return saturate(diffuse + ambient);

	//lightIntensity = max(6.f / length(lightRay), 0.2f); 
	//else
				//lightIntensity = 0.2f;
