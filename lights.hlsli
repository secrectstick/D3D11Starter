#ifndef __GGP_LIGHTING__
#define __GGP_LIGHTING__



#define LIGHT_COUNT 32 

#define LIGHT_TYPE_DIRECTIONAL	0
#define LIGHT_TYPE_POINT		1
#define LIGHT_TYPE_SPOT			2

struct Light
{
    int Type;
    float3 Direction; // 16 bytes
    float Range;
    float3 Position; // 32 bytes
    float Intensity;
    float3 Color; // 48 bytes
    float SpotInnerAngle;
    float SpotOuterAngle;
    float2 Padding; // 64 bytes
};

float3 NormalMapping(Texture2D map, SamplerState samplr, float2 uv, float3 normal, float3 tangent)
{
	
    float3 unpackedNormal= map.Sample(samplr, uv).rgb * 2.0f - 1.0f;

	
    float3 N = normal;
    float3 T = normalize(tangent - N * dot(tangent, N));
    float3 B = cross(T, N);

	
    float3x3 TBN = float3x3(T, B, N);

	
    return normalize(mul(unpackedNormal, TBN));
}


float SpecularPhong(float3 normal, float3 lightDir, float3 camDir)
{
    float3 refl = reflect(-lightDir, normal);
    return pow(max(dot(camDir, refl), 0), 128.0f);
}

float Attenuate(Light light, float3 worldPos)
{
    float dist = distance(light.Position, worldPos);
    float att = saturate(1.0f - (dist * dist / (light.Range * light.Range)));
    return att * att;
}


float3 DirLight(Light light, float3 normal, float3 worldPos, float3 camPos, float3 surface, float specularScale)
{
	// normalize light direction and camera direction
    float3 lightDir = normalize(-light.Direction);
    float3 camDir = normalize(camPos - worldPos);

	// calc light amounts
    float diffuseTerm = saturate(dot(normal, lightDir));
    float specularTerm= SpecularPhong(normal, lightDir, camDir) * specularScale;

	// Combine all the light info
    return (surface * diffuseTerm + specularTerm) * light.Intensity * light.Color;
}


float3 PointLight(Light light, float3 normal, float3 worldPos, float3 camPos, float3 surface, float specularScale)
{
	// normalize light direction and camera direction
    float3 lightDir = normalize(light.Position - worldPos);
    float3 camDir = normalize(camPos - worldPos);

	// stuff further away should be less bright
    float attenuateTerm = Attenuate(light, worldPos);
    
    // calc light amounts
    float diffuseTerm = saturate(dot(normal, lightDir));
    float specularTerm= SpecularPhong(normal, lightDir, camDir) * specularScale;

	// combine all the light info
    return (surface * diffuseTerm + specularTerm) * attenuateTerm * light.Intensity * light.Color;
}


float3 SpotLight(Light light, float3 normal, float3 worldPos, float3 camPos, float3 surface, float specularScale)
{
    
    float3 lightDir = normalize(light.Position - worldPos);
    float pixelAngle = saturate(dot(-lightDir, light.Direction));
    
    // Get cosines of angles and calculate range
    float cosOuter = cos(light.SpotOuterAngle);
    float cosInner = cos(light.SpotInnerAngle);
    float falloffRange = cosOuter - cosInner;
    
    // Linear falloff over the range, clamp 0-1, apply to light calc
    float spotTerm = saturate((cosOuter - pixelAngle) / falloffRange);
    float3 color = PointLight(light, normal, worldPos, camPos, surface, specularScale) * spotTerm;
    return color;

}










#endif