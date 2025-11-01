float4x4 g_mtxWorld;	// ワールドマトリックス
float4x4 g_LightView;	// ライトビュー変換行列
float4x4 g_LightProj;	// 射影変換行列

struct VS_INPUT
{
	float4 Pos : POSITION;
};

struct VS_OUTPUT
{
	float4 Pos : POSITION;
    float4 Depth : TEXCOORD1;
};

VS_OUTPUT VS_main(VS_INPUT In)
{
	VS_OUTPUT Out;

	float4 wpos = mul(In.Pos, g_mtxWorld);
	// ライト空間での位置（射影テクスチャ座標）
    Out.Pos = mul(wpos, g_LightView);
    Out.Pos = mul(Out.Pos, g_LightProj);
    Out.Depth = Out.Pos;

	return Out;
}

float4 PS_main(VS_OUTPUT In) : COLOR0
{
	// 深度をそのまま書き込む（明るいほど遠い）
	return In.Depth.z / In.Depth.w;
}

technique StandardDraw
{
	pass P0
	{
		VertexShader = compile vs_3_0 VS_main();
		PixelShader  = compile ps_3_0 PS_main();
	}
}