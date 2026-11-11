#include "structs.hlsli"
#include "lights.hlsli"

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
cbuffer ExternalData : register(b0)
{
	Light lights[LIGHT_COUNT];
	int lightCount;
	float3 ambientColor;
	float3 cameraPosition;
	float pad; // For alignment
	float3 colorTint;
	float time;
	float2 uvScale;
	float2 uvOffset;
}


// Example Texture2D and SamplerState definitions in an HLSL pixel shader
Texture2D SurfaceColor : register(t0); // A texture assigned to texture slot 0
Texture2D NormalMap : register(t1);
SamplerState BasicSampler : register(s0); // A sampler assigned to sampler slot 0

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{

	input.uv = input.uv * uvScale + uvOffset;

	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	float3 surface = SurfaceColor.Sample(BasicSampler, input.uv).rgb;
	surface = surface * colorTint;

	input.normal = NormalMapping(NormalMap, BasicSampler, input.uv, input.normal, input.tangent);

	float specularScale = 0.5f;

	float3 totalLight = ambientColor * surface;

	for (int i = 0; i < lightCount; i++)
	{

		Light light = lights[i];

		switch (lights[i].Type)
		{
		case LIGHT_TYPE_DIRECTIONAL:
			totalLight += DirLight(light, input.normal, input.worldPos, cameraPosition, surface, specularScale);
			break;

		case LIGHT_TYPE_POINT:
			totalLight += PointLight(light, input.normal, input.worldPos, cameraPosition, surface, specularScale);
			break;

		case LIGHT_TYPE_SPOT:
			totalLight += SpotLight(light, input.normal, input.worldPos, cameraPosition, surface, specularScale);
			break;
		}
	}

	return float4(totalLight, 1);
}