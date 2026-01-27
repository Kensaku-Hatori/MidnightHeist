// グローバル変数宣言ブロック
int g_nCnt;
// 波の粗さ
float SineMacro = 1.0f;
// 波の速さ
float Speed = 10.0f;
// 波の波及率
float Ripple = 2.0f;
// ノイズが収束する速さ
float g_NoiseSpeed;
// ノイズの範囲
float g_NoiseMinRange;
float g_NoiseMaxRange;

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
float4 PS_Default(VS_OUTPUT input) : COLOR
{
    return float4(tex2D(SceneSampler, input.tex));
}

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_main(VS_OUTPUT input) : COLOR
{ 
    // カウンタが進んでなかったら
    if (g_nCnt <= 0)
        return float4(tex2D(SceneSampler, input.tex));
    
    // 画面をゆがませる用のベクトルと距離変数
    float Vec = 0.5f - input.tex.y;
    float Distance = length(Vec);

    // 疑似的なノイズ                                      適当          適当
    float rndY = frac(sin((input.tex.y + (float)g_nCnt) * 123.456) * 98765.4321);

    // 適当判定                                    収束する速さ
    if (rndY < g_NoiseMaxRange && rndY > g_NoiseMinRange * (float) (g_nCnt / g_NoiseSpeed))
    {
        // ノイズを走らせる
        input.tex.x += rndY;
    }
    // ゆがませる
    input.tex.x += sin(SineMacro * ((g_nCnt - (Distance * Ripple)) * Speed)) * 0.0001f;
    // 最終的な色を出力
    return tex2D(SceneSampler, float2(input.tex.x, input.tex.y));
}

//**********************************************************************************
// テクニック宣言
//**********************************************************************************
technique StandardDraw
{
	pass P0
	{
        VertexShader = compile vs_3_0 VS_main();
        PixelShader = compile ps_3_0 PS_main();
    }
    pass P1
    {
        VertexShader = compile vs_3_0 VS_main();
        PixelShader = compile ps_3_0 PS_Default();
    }
}