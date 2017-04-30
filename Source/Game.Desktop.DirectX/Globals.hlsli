cbuffer globalCB : register(b0)
{
	matrix View;
	matrix Projection;
	float4 LightDirection;
	float AmbientLight;
};

cbuffer cbPerMesh : register(b1)
{
	matrix World;
	float4 MeshColor;
}