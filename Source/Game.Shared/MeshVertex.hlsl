struct VertexInput
{
	float4 position : POSITION;
};

float4 main( VertexInput input ) : SV_POSITION
{
	return input.position;
}