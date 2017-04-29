Texture2D txTexture : register( t0 );
SamplerState samLinear : register( s0 );

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 Texcoord : TEXCOORD0;
};

cbuffer cbQuad : register( b0 )
{
	float4 Color;
	float4 Textured;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main( PS_INPUT input) : SV_Target
{
	if (Textured.x != 0.0)
	{
		return txTexture.Sample( samLinear, input.Texcoord ) * Color;
	}
	else
	{
		return Color;
	}
}
