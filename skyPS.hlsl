#include "structs.hlsli"


TextureCube SkyTexture : register(t0);
SamplerState samplr : register(s0);


float4 main(VertexToPixel_Sky input) : SV_TARGET
{

	return SkyTexture.Sample(samplr, input.sampleDir);
}