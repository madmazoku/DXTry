#include "Types.hlsli"

float4 main(PixelInputType input) : SV_TARGET
{
    return input.color;
}