// グローバル変数宣言ブロック
float4x4 g_mtxWorld;
float4x4 g_mtxView;     // カメラビュー変換行列
float4x4 g_mtxProj;     // 射影変換行列
float4 g_Center;
float g_FillAmount;

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
    if (Length > 25.0f)
    {
        return float4(0, 0, 0, 0);   
    }

    // 角度を求める (0～2π)
    float Angle = atan2(ToPoint.x, ToPoint.y);
    if (Angle < 0)
    {
        Angle += 6.2831853; // 負の角度を補正
    }

    // FillAmount に応じた閾値
    float Limit = clamp(6.2831853 * g_FillAmount, 0.0f, 6.2831853);

    // 塗りつぶし判定
    if (Angle <= Limit)
    {
        return input.col; // 塗りつぶし部分   
    }
    else
    {
        return float4(0, 0, 0, 0); // 未塗り部分   
    }
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