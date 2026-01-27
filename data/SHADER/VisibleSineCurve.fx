// グローバル変数宣言ブロック
float4x4 g_mtxWorld; // ワールドマトリックス
float4x4 g_mtxView; // カメラビュー変換行列
float4x4 g_mtxProj; // 射影変換行列
float3 g_Center;    // 中心点
// カウンタ
int g_SineCount;
// 波の細かさ
float g_SineMicro;
// 波の波及率
float g_SineRipple;
// 波の速さ
float g_SineSpeed;
// 波の大きさ
float g_SineRadius;

// 頂点シェーダ出力構造体
struct VS_OUTPUT
{
    float4 pos : POSITION; // 位置
    float4 Pos : TEXCOORD1; // 位置
    float4 col : COLOR0; // 色
};

//**********************************************************************************
// 頂点シェーダプログラムブロック
//**********************************************************************************
VS_OUTPUT VS_main(VS_OUTPUT input)
{
	// 出力用変数
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.col = input.col;
    output.pos = mul(input.pos, g_mtxWorld);
    output.pos = mul(output.pos, g_mtxView);
    output.Pos = output.pos;
    output.pos = mul(output.pos, g_mtxProj);

    return output;
}

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_main(VS_OUTPUT input) : COLOR
{ 
    float3 VecCenter = input.Pos.xyz - g_Center;
    float Distance = length(VecCenter);
    
    if(Distance < g_SineRadius)
    {
        float4 Col = input.col * saturate(sin(g_SineMicro * ((g_SineCount - (Distance * g_SineRipple)) * g_SineSpeed)) * 1.0f);
        Col.a -= 1.0f * Distance / g_SineRadius;
        return Col;

    }
    else
    {
        return float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
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
}