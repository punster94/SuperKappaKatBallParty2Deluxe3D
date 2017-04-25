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
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
    return txDiffuse.Sample( samLinear, input.Texcoord );
}
