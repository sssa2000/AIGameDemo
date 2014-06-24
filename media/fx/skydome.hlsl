

float    g_fTime;                   // App's time in seconds
float4x4 g_mWorld;                  // World matrix for object
float4x4 g_mViewProjection;    // View * Projection matrix



//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
texture g_MeshTexture;
sampler MeshTextureSampler = 
sampler_state
{
    Texture = <g_MeshTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
	AddressU = Clamp;
    AddressV = Clamp;
};

texture g_CloudTexture;
sampler CloudTextureSampler = 
sampler_state
{
    Texture = <g_CloudTexture>;
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
    float4 Position   	: POSITION;   // vertex position 
    float4 TextureUV0  	: TEXCOORD0;
	float4 TextureUV1  	: TEXCOORD1;
	float4 Color		: TEXCOORD2;
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION, 
                         float2 vTexCoord0 : TEXCOORD0,
						 float4 vColor:COLOR0,
                         uniform int nNumLights,
                         uniform bool bAnimate )
{
    VS_OUTPUT Output;
     
    // Transform the position from object space to homogeneous projection space
	float4 worldPosition = mul(vPos, g_mWorld);
    Output.Position = mul(worldPosition, g_mViewProjection);
    
	Output.TextureUV0 = float4(0.0,1.0 - vPos.y / 25.0 + 0.3,0.0,1.0);
	Output.TextureUV1 = float4(
		vPos.x / 50.0 + g_fTime / 20.0,
		vPos.z / 50.0,
		vPos.x / 60.0 + g_fTime / 10.0,
		vPos.z / 60.0);

	Output.Color = vColor;
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
	
	float4 horizon = tex2D(MeshTextureSampler, In.TextureUV0.xy);
	float4 clouds_0 = tex2D(CloudTextureSampler, In.TextureUV1.xy);
	float4 clouds_1 = tex2D(CloudTextureSampler, In.TextureUV1.zw);
	float4 clouds = (clouds_0 + clouds_1) * horizon.w;

	Output.RGBColor = In.Color *(1.0 - clouds.x) + clouds;// 0.5f * horizon * (1.0 - clouds.x) + 1.5f * clouds;
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

