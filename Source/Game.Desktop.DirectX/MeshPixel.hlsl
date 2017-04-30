#include "Globals.hlsli"

Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 Texcoord : TEXCOORD0;
	float3 Normal : NORMAL0;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main( PS_INPUT input) : SV_Target
{
	float4 texColor = txDiffuse.Sample(samLinear, input.Texcoord) * MeshColor;
	float intensity = max(0.0f, dot(input.Normal, -1.0f * LightDirection.xyz));

	if (texColor.a < 0.05f)
	{
		discard;
	}

	float4 outputColor = max(texColor * intensity, texColor * AmbientLight);
	outputColor.a = texColor.a;
	return outputColor;
}
