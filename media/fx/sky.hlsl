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
};


//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION, 
                         float2 vTexCoord0 : TEXCOORD0,
                         uniform int nNumLights,
                         uniform bool bAnimate )
{
    VS_OUTPUT Output;
     
    // Transform the position from object space to homogeneous projection space
	float4 worldPosition = mul(vPos, g_mWorld);
    Output.Position = mul(worldPosition, g_mViewProjection);
    

	Output.TextureUV = vTexCoord0; 
    
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

   Output.RGBColor = tex2D(MeshTextureSampler, In.TextureUV);

    return Output;
}


//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------
technique RenderSceneWithTexture1Light
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSceneVS( 1, true );
        PixelShader  = compile ps_2_0 RenderScenePS(  ); // trivial pixel shader (could use FF instead if desired)
    }
}

technique RenderSceneWithTexture2Light
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSceneVS( 2, true );
        PixelShader  = compile ps_2_0 RenderScenePS(  ); // trivial pixel shader (could use FF instead if desired)
    }
}

technique RenderSceneWithTexture3Light
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSceneVS( 3, true );
        PixelShader  = compile ps_2_0 RenderScenePS(  ); // trivial pixel shader (could use FF instead if desired)
    }
}

