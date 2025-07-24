#include "Test.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelshaderOutPut{
    float32_t4 color : SV_TARGET0;
};

PixelshaderOutPut main(VertexShaderOutput input)
{
    PixelshaderOutPut output;
    
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    float32_t2 correct = input.texcoord * (1.0f - input.texcoord.yx);
    float vignette = correct.x * correct.y * 16.0f;
    vignette = saturate(pow(vignette, 0.8f));
    output.color.rgb *= vignette;
    
    return output;
}