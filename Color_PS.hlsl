struct PixelTypeColor
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PixelTypeColor input) : SV_TARGET
{
    return input.color;
}