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
float4 g_MaterialAmbientColor;      // Material's ambient color
float4 g_MaterialDiffuseColor;      // Material's diffuse color
int g_nNumLights;

float3 g_LightDir;               // Light's direction in world space
float4 g_LightDiffuse;           // Light's diffuse color
float4 g_LightAmbient;              // Light's ambient color

texture g_MeshTexture;              // Color texture for mesh

float    g_fTime;                   // App's time in seconds
float4x4 g_mWorld;                  // World matrix for object
float4x4 g_mViewProjection;    // View * Projection matrix

float4 towerInfo[2]; //x,y,z=center  w=r
float4 robotInfo;

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
	float4 WorldPos   : TEXCOORD1;	//world pos of vertex
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
    
    // Transform the position from object space to homogeneous projection space
	float4 worldPosition = mul(vPos, g_mWorld);
    Output.Position = mul(worldPosition, g_mViewProjection);
    Output.WorldPos=worldPosition;
	Output.TextureUV=float2(vPos.x/64,vPos.z/64);//vTexCoord0;
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

	float3 res=g_MaterialDiffuseColor;
	
	//draw robot shadow on terrain
	float f1=length(In.WorldPos.xz-robotInfo.xy);
	float t1=(f1-robotInfo.z);
	if(t1<0)
		res.xyz=float3(0.2,0.3,0.3);
		
	float factor=length(In.WorldPos.xz-towerInfo[0].xz);
	float t=abs(factor-towerInfo[0].w);
	if(t<0.1f)
		res.g=0.9f;
	else
	{
		factor=length(In.WorldPos.xz-towerInfo[1].xz);
	t=abs(factor-towerInfo[1].w);
	if(t<0.1f)
		res.xyz=float3(0.3,0.3,0.3);
	}

   float4 tex=float4(0,tex2D(MeshTextureSampler, In.TextureUV).r,0,1);
   Output.RGBColor = float4(res,1)+tex*0.03f;

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

