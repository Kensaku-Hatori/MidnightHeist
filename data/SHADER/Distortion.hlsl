// グローバル変数宣言ブロック
int g_nCnt;
// 波の粗さ
float SineMacro = 1.0f;
// 波の速さ
float Speed = 10.0f;
// 波の波及率
float Ripple = 2.0f;

// 頂点シェーダ出力構造体
struct VS_OUTPUT
{
	float4 pos : POSITION;			// 位置
	float4 col : COLOR0;			// 色
	float2 tex : TEXCOORD;			// テクスチャ座標
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
float4 PS_main(VS_OUTPUT input) : COLOR
{ 
    float Vec = 0.5f - input.tex.y;
    float Distance = length(Vec);

    //if (input.tex.y > 0.1f && input.tex.y < 0.11f || input.tex.y > 0.2f && input.tex.y < 0.21f ||
    //    input.tex.y > 0.3f && input.tex.y < 0.31f || input.tex.y > 0.4f && input.tex.y < 0.41f ||
    //    input.tex.y > 0.5f && input.tex.y < 0.51f || input.tex.y > 0.6f && input.tex.y < 0.61f ||
    //    input.tex.y > 0.7f && input.tex.y < 0.71f || input.tex.y > 0.8f && input.tex.y < 0.81f ||
    //    input.tex.y > 0.9f && input.tex.y < 0.91f)
    //{
    //    input.tex.x += frac(sin(dot(input.tex * g_nCnt, float2(12.9898, 78.233))) * 43758.5453);
    //}
    // 疑似的なノイズ
    float rndY = frac(sin((input.tex.y + g_nCnt) * 123.456) * 98765.4321);

    // 適当判定
    if (rndY < 0.05 && rndY > 0.01)
    {
        input.tex.x += rndY;
    }
    input.tex.x += sin(SineMacro * ((g_nCnt - (Distance * Ripple)) * Speed)) * 0.0001f;
    return tex2D(SceneSampler, float2(input.tex.x, input.tex.y));
}

//**********************************************************************************
// テクニック宣言
//**********************************************************************************
technique StandardDraw
{
	pass P0
	{
		vertexShader = compile vs_3_0 VS_main();
        pixelShader = compile ps_3_0 PS_main();
    }
}