#include "structs.hlsli"


cbuffer ExternalData : register(b0)
{
	matrix viewMatrix;
	matrix projectionMatrix;
}


VertexToPixel_Sky main(VertexShaderInput input)
{
	
	

	
	matrix viewNoTranslation = viewMatrix;
	viewNoTranslation._14 = 0;
	viewNoTranslation._24 = 0;
	viewNoTranslation._34 = 0;

	
	matrix vp = mul(projectionMatrix, viewNoTranslation);

	VertexToPixel_Sky output;
	output.screenPosition = mul(vp, float4(input.localPosition, 1.0f));
	output.screenPosition.z = output.screenPosition.w;
	output.sampleDir = input.localPosition;

	return output;
}