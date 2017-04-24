//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer cbMeshVertex : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Position : POSITION;
    float2 Texcoord : TEXCOORD0;
	float3 Normal : NORMAL0;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 Texcoord : TEXCOORD0;
	float3 Normal : NORMAL0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    //output.Position = mul( input.Position, World );
    //output.Position = mul( output.Position, View );
    //output.Position = mul( output.Position, Projection );
	//float4 pos;
	//pos.xy = input.Position.xyz;
	output.Position = input.Position;
	output.Position.z = 0.0f;
    output.Texcoord = input.Texcoord;
	output.Normal = input.Normal;
    
    return output;
}
