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




float4x4 g_mWorld;                  // World matrix for object
float4x4 g_mViewProjection;    // View * Projection matrix
float range=1.0f;
//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
texture g_DiffuseTex; 
sampler DiffuseTexSampler = 
sampler_state
{
    Texture = <g_DiffuseTex>;
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
    float2 TextureUV1  : TEXCOORD1;  // vertex texture coords 
	float4 WorldPos   : TEXCOORD2;	//world pos of vertex
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION,                          
                         float2 vTexCoord0 : TEXCOORD0,
						 float3 vNormal : NORMAL,
                         uniform int nNumLights,
                         uniform bool bAnimate )
{
    VS_OUTPUT Output;
    
	float4 worldPosition =vPos * float4(range ,0.1f,range ,1); 
	worldPosition=mul(worldPosition, g_mWorld);
	
    Output.Position = mul(worldPosition, g_mViewProjection);
    Output.WorldPos=worldPosition;
	
    // Transform the normal from object space to world space    
	Output.TextureUV1 = vTexCoord0.xy;//float2(vPos.x / 128.0 - 0.5,vPos.z / 128.0 - 0.5);

	
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
float4 RenderScenePS( VS_OUTPUT In) : COLOR0
{ 
	float4 t = tex2D(DiffuseTexSampler,In.TextureUV1.xy);
	return t;
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

technique RenderSceneWithTexture2Light
{
    pass P0
    {          
        VertexShader = compile vs_3_0 RenderSceneVS( 2, true );
        PixelShader  = compile ps_3_0 RenderScenePS(  ); // trivial pixel shader (could use FF instead if desired)
    }
}

technique RenderSceneWithTexture3Light
{
    pass P0
    {          
        VertexShader = compile vs_3_0 RenderSceneVS( 3, true );
        PixelShader  = compile ps_3_0 RenderScenePS(  ); // trivial pixel shader (could use FF instead if desired)
    }
}

