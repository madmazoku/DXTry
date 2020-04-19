/************************************************
    Type definitions
************************************************/
struct VertexTypeColor
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct PixelTypeColor
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct VertexTypeTex
{
    float4 position : POSITION;
    float4 tex : TEXCOORD0;
};

struct PixelTypeTex
{
    float4 position : SV_POSITION;
    float4 tex : TEXCOORD0;
};

/************************************************
    Global constant buffer
************************************************/
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};
