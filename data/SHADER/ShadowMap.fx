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
    float Depth : TEXCOORD1;
};

VS_OUTPUT VS_Main(VS_INPUT In)
{
    VS_OUTPUT Out;

    float4 wpos = mul(In.Pos, g_mtxWorld);
    float4 lightPos = mul(wpos, g_LightView);
    lightPos = mul(lightPos, g_LightProj);

    Out.Pos = lightPos;
	
    Out.Depth = Out.Pos.z / Out.Pos.w;
    
    return Out;
}

float4 PS_Main(VS_OUTPUT In) : COLOR0
{
	// 深度をそのまま書き込む（明るいほど遠い）
    return float4(In.Depth.xxx, 1.0f);
}

technique StandardDraw
{
	pass P0
	{
		VertexShader = compile vs_3_0 VS_Main();
		PixelShader  = compile ps_3_0 PS_Main();
	}
}