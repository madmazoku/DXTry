#include "Common.hlsli"

float4 main(PixelTypeColor input) : SV_TARGET
{
    return input.color;
}