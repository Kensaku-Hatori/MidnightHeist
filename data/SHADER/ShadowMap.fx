//****************************************************************
//
// テクスチャに深度を書き込むシェーダーの処理[ShadowMap.fx]
// Author Kensaku Hatori
//
//****************************************************************

// グローバル変数宣言ブロック
float4x4 g_mtxWorld;	// ワールドマトリックス
float4x4 g_LightView;	// ライトビュー変換行列
float4x4 g_LightProj;	// 射影変換行列

// 頂点シェーダー入力用構造体
struct VS_INPUT
{
	float4 Pos : POSITION;  // 位置
};

// 頂点シェーダー出力構造体
struct VS_OUTPUT
{
	float4 Pos : POSITION;      // 位置
    float Depth : TEXCOORD1;    // 深度
};

//**********************************************************************************
// 頂点シェーダプログラムブロック
//**********************************************************************************
VS_OUTPUT VS_Main(VS_INPUT In)
{
    VS_OUTPUT Out;

    // 座標変換
    float4 wpos = mul(In.Pos, g_mtxWorld);
    float4 lightPos = mul(wpos, g_LightView);
    lightPos = mul(lightPos, g_LightProj);
    // ライトから見た位置
    Out.Pos = lightPos;
	// 深度を計算
    Out.Depth = Out.Pos.z / Out.Pos.w;
    
    return Out;
}

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_Main(VS_OUTPUT In) : COLOR0
{
	// 深度をそのまま書き込む（明るいほど遠い）
    return float4(In.Depth.xxx, 1.0f);
}

//**********************************************************************************
// テクニック宣言
//**********************************************************************************
technique StandardDraw
{
    // パス0
	pass P0
	{
		VertexShader = compile vs_3_0 VS_Main();
		PixelShader  = compile ps_3_0 PS_Main();
	}
}