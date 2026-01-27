// グローバル変数宣言ブロック
float4x4 g_mtxWorld; // ワールドマトリックス
float4x4 g_View; // 射影変換マトリックス
float4x4 g_Proj; // プロェクションマトリックス

// CubeMap
textureCUBE g_CubeMap;
// サンプラーを定義
samplerCUBE CubeSampler = sampler_state
{
    Texture = <g_CubeMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
    AddressW = CLAMP;
};

// 頂点入力
struct VS_IN
{
    // 位置
    float3 Pos : POSITION;
};

// 頂点出力
struct VS_OUT
{
    // 位置射影変換後なのでfloat4
    float4 Pos : POSITION;
    // テクスチャ座標
    float3 Tex : TEXCOORD0;
};

//**********************************************************************************
// 頂点シェーダー
//**********************************************************************************
VS_OUT VS(VS_IN IN)
{
    // 出力用の変数
    VS_OUT OUT;

    // マトリックスをかけ合わせる
    OUT.Pos = mul(float4(IN.Pos, 1), g_mtxWorld);
    OUT.Pos = mul(OUT.Pos, g_View);
    OUT.Pos = mul(OUT.Pos, g_Proj);

    // サンプリング方向に使用
    // カメラの中心にあるのでそのまま
    OUT.Tex = IN.Pos;

    return OUT;
}

//**********************************************************************************
// ピクセルシェーダー
//**********************************************************************************
float4 PS(VS_OUT IN) : COLOR
{
    // サンプラーから色を取得
    return texCUBE(CubeSampler, IN.Tex);
}

//**********************************************************************************
// テクニック宣言
//**********************************************************************************
technique StandardDraw
{
    pass P0
    {
        // ゼットバッファへの書き込みを無効化
        ZEnable = FALSE;
        ZWriteEnable = FALSE;
        // 内側から
        CullMode = CW;

        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS();
    }
}