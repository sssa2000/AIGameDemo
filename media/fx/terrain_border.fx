//--------------------------------------------------------------------------------------
// File: BasicHLSL.fx
//
// The effect file for the BasicHLSL sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------


texture g_MeshTexture;              // Color texture for mesh

float    g_fTime;                   // App's time in seconds
float4x4 g_mWorld;                  // World matrix for object
float4x4 g_mViewProjection;    // View * Projection matrix
float3 g_camerapos;
float4x4 ViewMatrix; 
float4 threshold; //x=begin y=end
//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler = 
sampler_state
{
    Texture = <g_MeshTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
	AddressU = Wrap;
    AddressV = Wrap;
};


//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
	float4 PosWS: TEXCOORD1;
	float ViewZ: TEXCOORD2;
};



//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION, 
                         float3 vNormal : NORMAL,
                         float2 vTexCoord0 : TEXCOORD0,
                         uniform int nNumLights,
                         uniform bool bAnimate )
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;
    float4 vAnimatedPos = vPos;
	float4 worldPosition = mul(vAnimatedPos, g_mWorld);
	Output.PosWS=worldPosition.xyzw;

    Output.Position = mul(worldPosition, g_mViewProjection);
    
    // Transform the normal from object space to world space    
    vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld)); // normal (world space)
 
	Output.TextureUV = vTexCoord0+float2(g_fTime*0.01,g_fTime*0.005f);
	Output.ViewZ=dot(float4(worldPosition.xyz, 1), ViewMatrix._13_23_33_43);
    return Output;    
}


//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT RenderScenePS( VS_OUTPUT In) 
{ 
    PS_OUTPUT Output;

	float4 tex=tex2D(MeshTextureSampler, In.TextureUV);
	
	float fakeParam = max(In.ViewZ - threshold.x, 0) * 0.01f;

	float f = 1.0 / exp(fakeParam);
	float t = lerp(1 - threshold.y, 1, f);
	Output.RGBColor.rgb  =tex.rgb;
	Output.RGBColor.a  =tex.a * t;

    return Output;
}


//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------
technique RenderSceneWithTexture1Light
{
    pass P0
    {          
        VertexShader = compile vs_3_0 RenderSceneVS( 1, true );
        PixelShader  = compile ps_3_0 RenderScenePS(  ); // trivial pixel shader (could use FF instead if desired)
    }
}
