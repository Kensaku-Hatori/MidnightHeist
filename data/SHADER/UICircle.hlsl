// グローバル変数宣言ブロック
float4x4 g_mtxWorld;    // ワールドマトリックス
float4x4 g_mtxView;     // カメラビュー変換行列
float4x4 g_mtxProj;     // 射影変換行列
float4 g_Center;        // 中心座標
float g_FillAmount;     // 塗りつぶす量
float g_Radius;         // 半径
float g_MaxFillAmount;  // 塗りつぶす最大値(最大6.28)

// 頂点シェーダ出力構造体
struct VS_OUTPUT
{
    float4 pos : POSITION;  // 位置
    float4 Pos : TEXCOORD1; // 位置
    float3 nor : NORMAL;    // 法線
    float4 col : COLOR0;    // 色
};

//**********************************************************************************
// 頂点シェーダプログラムブロック
//**********************************************************************************
VS_OUTPUT VS_main(VS_OUTPUT input)
{
	// 出力用変数
	VS_OUTPUT output = (VS_OUTPUT) 0;
    output.col = input.col;
    output.nor = input.nor;
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
    float2 Origin = g_Center.xy;
    float2 ToPoint = input.Pos.xy - Origin;

    // 半径判定
    float Length = length(ToPoint);
    if (Length > g_Radius)
    {
        return float4(0, 0, 0, 0);   
    }

    // 角度を求めるて修正
    float Angle = atan2(ToPoint.x, ToPoint.y);
    // 負の数だったら
    if (Angle < 0)
    {
        // 修正
        Angle += 6.2831853;
    }

    // 塗りつぶし量
    float FillAngle = clamp(g_MaxFillAmount * g_FillAmount, 0.0f, g_MaxFillAmount);

    // 塗りつぶし判定
    if (Angle <= FillAngle)
    {
        // 塗りつぶす
        return input.col;   
    }
    else
    {
        // 透明にする
        return float4(0, 0, 0, 0);
    }
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

		vertexShader = compile vs_3_0 VS_main();
        pixelShader = compile ps_3_0 PS_main();
    }
}