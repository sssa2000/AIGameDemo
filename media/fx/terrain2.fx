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
float3	g_camera_pos;

float4 towerInfo[2]; //x,y,z=center  w=r
float4 robotInfo;
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

texture g_DetailTex; 
sampler DetailTexSampler = 
sampler_state
{
    Texture = <g_DetailTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
	AddressU = Wrap;
    AddressV = Wrap;
};

texture g_NormalTex; 
sampler NormalTexSampler = 
sampler_state
{
    Texture = <g_NormalTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
	AddressU = Clamp;
    AddressV = Clamp;
};

//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
	float4 ViewVecWS  : TEXCOORD0;

    float2 TextureUV1  : TEXCOORD1;  // vertex texture coords 
	float2 TextureUV2  : TEXCOORD2;  // vertex texture coords 
	float4 TextureUV3  : TEXCOORD3;  // vertex texture coords 
	float4 WorldPos   : TEXCOORD4;	//world pos of vertex
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
    float3 vNormalWorldSpace; 
    
    // Transform the position from object space to homogeneous projection space
	float4 worldPosition = mul(vPos, g_mWorld);
    Output.Position = mul(worldPosition, g_mViewProjection);
    Output.WorldPos=worldPosition;
	
    // Transform the normal from object space to world space    
    vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld)); // normal (world space)
    
    // Compute simple directional lighting equation
    Output.ViewVecWS=float4(normalize(g_camera_pos.xyz-worldPosition.xyz),0);
	
    
	Output.TextureUV1 = vTexCoord0.xy;//float2(vPos.x / 128.0 - 0.5,vPos.z / 128.0 - 0.5);
	Output.TextureUV2 = float2(vPos.x / 73.0,vPos.z / 73.0);
	Output.TextureUV3.x = clamp(1.3 - (vPos.y - 7.0) / 73.0,0.0,1.0);
	Output.TextureUV3.y = clamp(0.1 + vPos.y / 15.0,0.0,1.0);
	Output.TextureUV3.z = vPos.y;
	Output.TextureUV3.w=1;
	
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

	float3 normal = tex2D(NormalTexSampler,In.TextureUV1.xy).xyz * 2.0 - 1.0;
	
	float3 color_0 = tex2D(DiffuseTexSampler,In.TextureUV2.xy).xyz * In.TextureUV3.x;
	float3 color_1 = tex2D(DetailTexSampler,In.TextureUV2.xy).xyz * In.TextureUV3.y;
	float f= dot(normal.xzy,In.ViewVecWS.xyz);
	float3 tex=(color_0 + color_1);
	float3 res = lerp(tex,f,0.1);
    //return float4(res,1);
	
	/*
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
		res.g=0.9f;
	}
	*/
    return float4(res,1);
	
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

