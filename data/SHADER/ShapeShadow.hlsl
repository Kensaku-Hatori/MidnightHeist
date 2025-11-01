// グローバル変数宣言ブロック
float4x4 g_mtxWorld; // ワールドマトリックス
float4x4 g_View; // 射影変換マトリックス
float4x4 g_Proj; // プロェクションマトリックス

// 頂点シェーダ出力構造体
struct VS_OUTPUT
{
    float4 Pos : POSITION;
    float4 Depth : TEXCOORD1;
    float2 Uv : TEXCOORD;
};

// オブジェクト(テクスチャ)
texture g_ObjectTexture;

// テクスチャサンプラーを宣言
sampler2D ObjectSampler = sampler_state
{
	// サンプラーを設定
    Texture = <g_ObjectTexture>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

// オブジェクト(テクスチャ)
texture g_TargetTexture;

// テクスチャサンプラーを宣言
sampler2D TargetSampler = sampler_state
{
	// サンプラーを設定
    Texture = <g_TargetTexture>;
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

    float4 in_pos = input.Pos; // 位置

	//座標変換(ローカル座標と掛け合わせ)
    float4 WorldPos = mul(in_pos, g_mtxWorld);
    output.Pos = mul(WorldPos, g_View);
    output.Pos = mul(output.Pos, g_Proj);
    output.Depth = output.Pos;

    return output;
}

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_Write(VS_OUTPUT input) : COLOR
{
	// 深度をそのまま書き込む（明るいほど遠い）
    return input.Depth.z / input.Depth.w;
}

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_Comp(VS_OUTPUT input) : COLOR
{
	// 物体（障害物）側の深度テクスチャを取得
    float ObjectDepth = tex2D(ObjectSampler, input.Uv).x;
    // 物体（障害物）側の深度テクスチャを取得
    float TargetDepth = tex2D(TargetSampler, input.Uv).x;

	// プレイヤーが物体の裏側にある場合
    if (TargetDepth < ObjectDepth)
    {
		// シルエットカラーで描く
        return float4(1.0f, 0.0f, 0.0f, 1.0f);
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
    pass P0_Write
    {
        VertexShader = compile vs_3_0 VS_main();
        PixelShader = compile ps_3_0 PS_Write();
    }
    pass P1_Compare
    {
        VertexShader = compile vs_3_0 VS_main();
        PixelShader = compile ps_3_0 PS_Comp();
    }
}
