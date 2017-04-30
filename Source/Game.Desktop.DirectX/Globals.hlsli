cbuffer globalCB : register(b0)
{
	matrix View;
	matrix Projection;
	float4 LightDirection;
	float AmbientLight;
};