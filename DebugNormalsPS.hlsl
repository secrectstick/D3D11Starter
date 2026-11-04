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
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	return float4(input.normal, 1);
}