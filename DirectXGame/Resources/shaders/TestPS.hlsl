struct PixelshaderOutPut
{
    float4 color : SV_TARGET0;
};

PixelshaderOutPut main()
{
    PixelshaderOutPut output;
    output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    return output;
}