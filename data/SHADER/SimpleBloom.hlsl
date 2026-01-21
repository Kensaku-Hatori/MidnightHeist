// グローバル変数宣言
float g_Filter = 0.4f;

// 頂点シェーダ出力構造体
struct VS_OUTPUT
{
    float4 pos : POSITION; // 位置
    float4 col : COLOR0; // 色
    float2 tex : TEXCOORD; // テクスチャ座標
};

// シャドウマップ(テクスチャ)
texture g_SceneTexture;

// テクスチャサンプラーを宣言
sampler2D SceneSampler = sampler_state
{
	// サンプラーを設定
    Texture = <g_SceneTexture>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

// シャドウマップ(テクスチャ)
texture g_BrightTexture;

// テクスチャサンプラーを宣言
sampler2D BrightSampler = sampler_state
{
	// サンプラーを設定
    Texture = <g_BrightTexture>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

// シャドウマップ(テクスチャ)
texture g_BloomTexture;

// テクスチャサンプラーを宣言
sampler2D BloomSampler = sampler_state
{
	// サンプラーを設定
    Texture = <g_BloomTexture>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

//**********************************************************************************
// 頂点シェーダプログラムブロック
//**********************************************************************************
VS_OUTPUT VS_main(VS_OUTPUT input)
{
	// 出力用変数
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    float4 in_pos = input.pos; // 位置
    float4 in_col = input.col; // 色
    float2 in_tex = input.tex; // テクスチャ座標
        
	// 頂点の色の決定
    output.col = in_col;
    
    // テクスチャ座標
    output.tex = in_tex;

    return output;
}

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_Filter(VS_OUTPUT input) : COLOR
{
    float4 Col = tex2D(SceneSampler, input.tex);
    // 輝度計算
    float luminance = dot(Col.rgb, float3(0.299, 0.587, 0.114));
    
    if(luminance < g_Filter)
    {
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        return Col;
    }
}

float2 TexelSize = float2(0.0078125 * 0.25f, 0.013888888888889 * 0.25f);

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_BloomWidth(VS_OUTPUT input) : COLOR
{
    float4 sum = 0;

    sum += tex2D(BrightSampler, input.tex) * 0.2;
    sum += tex2D(BrightSampler, input.tex + TexelSize * float2(1, 0)) * 0.15;
    sum += tex2D(BrightSampler, input.tex + TexelSize * float2(-1, 0)) * 0.15;

    sum = saturate(sum);
    
    return sum;
}

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_BloomHeight(VS_OUTPUT input) : COLOR
{
    float4 sum = 0;

    sum += tex2D(BrightSampler, input.tex) * 0.2;
    sum += tex2D(BrightSampler, input.tex + TexelSize * float2(0, 1)) * 0.15;
    sum += tex2D(BrightSampler, input.tex + TexelSize * float2(0, -1)) * 0.15;

    sum = saturate(sum);
    
    return sum;
}

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_Multiply(VS_OUTPUT input) : COLOR
{
    float4 scene = tex2D(SceneSampler, input.tex);
    float4 bloom = tex2D(BloomSampler, input.tex);

    return scene + bloom;
}

//**********************************************************************************
// テクニック宣言
//**********************************************************************************
technique StandardDraw
{
    pass P0
    {
        vertexShader = compile vs_3_0 VS_main();
        pixelShader = compile ps_3_0 PS_Filter();
    }
    pass P1
    {
        vertexShader = compile vs_3_0 VS_main();
        pixelShader = compile ps_3_0 PS_BloomWidth();
    }
    pass P2
    {
        vertexShader = compile vs_3_0 VS_main();
        pixelShader = compile ps_3_0 PS_BloomHeight();
    }
    pass P3
    {
        vertexShader = compile vs_3_0 VS_main();
        pixelShader = compile ps_3_0 PS_Multiply();
    }
}