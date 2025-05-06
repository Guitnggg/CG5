struct VertexshaderOutPut
{
    float4 position : SV_POSITION;
};

struct VertexshaderInput
{
    float4 position : POSITION0;
};

VertexshaderOutPut main(VertexshaderInput input)
{
    VertexshaderOutPut output;
    output.position = input.position;
    return output;
}