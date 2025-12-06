// グローバル変数宣言ブロック
float4x4 g_mtxWorld;	// ワールドマトリックス
float4x4 g_View;		// 射影変換マトリックス
float4x4 g_Proj;		// プロェクションマトリックス
float4x4 g_LightView;	// ライトビュー変換行列
float4x4 g_LightProj;	// 射影変換行列
float4 g_Deffuse;		// 色
float4 g_vecLight; // ライトの向きベクトル

// 頂点シェーダ出力構造体
struct VS_OUTPUT
{
	float4 pos : POSITION;			// 位置
	float3 nor : NORMAL;			// 法線
	float4 col : COLOR0;			// 色
	float4 colModel : COLOR1;		// 色
    float4 LightPos : TEXCOORD1;	// ライト空間の位置
	float2 tex : TEXCOORD;			// テクスチャ座標
};

// シャドウマップ(テクスチャ)
texture g_ShadowTexture;

// テクスチャサンプラーを宣言
sampler2D ShadowSampler = sampler_state
{
	// サンプラーを設定
    Texture = <g_ShadowTexture>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

// シャドウマップ(テクスチャ)
texture g_ModelTexture;

// テクスチャサンプラーを宣言
sampler2D ModelSampler = sampler_state
{
	// サンプラーを設定
    Texture = <g_ModelTexture>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
    AddressU = WRAP;
    AddressV = WRAP;
};

//**********************************************************************************
// 頂点シェーダプログラムブロック
//**********************************************************************************
VS_OUTPUT VS_main(VS_OUTPUT input)
{
	// 出力用変数
	VS_OUTPUT output = (VS_OUTPUT) 0;
    
	float4 in_pos = input.pos; // 位置
	float3 in_nor = input.nor; // 法線
	float4 in_col = input.col; // 色
	float2 in_tex = input.tex; // テクスチャ座標
    
	//座標変換(ローカル座標と掛け合わせ)
    float4 WorldPos = mul(in_pos, g_mtxWorld);
    output.pos = mul(WorldPos, g_View);
    output.pos = mul(output.pos, g_Proj);
	
	// ライト空間での位置（射影テクスチャ座標）
    output.LightPos = mul(WorldPos, g_LightView);
    output.LightPos = mul(output.LightPos, g_LightProj);

    // 法線に回転行列を掛ける
	float3 norLocal;
	norLocal.x = (in_nor.x * g_mtxWorld._11) + (in_nor.y * g_mtxWorld._21) + (in_nor.z * g_mtxWorld._31);
	norLocal.y = (in_nor.x * g_mtxWorld._12) + (in_nor.y * g_mtxWorld._22) + (in_nor.z * g_mtxWorld._32);
	norLocal.z = (in_nor.x * g_mtxWorld._13) + (in_nor.y * g_mtxWorld._23) + (in_nor.z * g_mtxWorld._33);
    
    // 法線を出力
    output.nor = norLocal;
    
	// 頂点の色の決定
	output.col = in_col;
	output.colModel = g_Deffuse;
    
    // テクスチャ座標
	output.tex = in_tex;

	return output;
}

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_Toon(VS_OUTPUT input) : COLOR
{
    float lightIntensity;
	
    // テクスチャカラー取得
    float4 Col = g_Deffuse;
    float4 LightDir = -g_vecLight;
    float Toon = 1.0f;
    float AmountLight;
    float Shadow = 1.0f;
    float LightPower = 1.0f;
	
    // 射影空間のXY座標をテクスチャ座標に変換
    float2 TransTexCoord;
    TransTexCoord.x = input.LightPos.x / input.LightPos.w / 2.0f + 0.5f;
    TransTexCoord.y = -input.LightPos.y / input.LightPos.w / 2.0f + 0.5f;

    float3 normal = normalize(input.nor);
    float3 lightDir = normalize(g_vecLight.xyz);

    //AmountLight = dot(normal, lightDir);
	
    //// 陰影を3段階に分ける
    //if (AmountLight > 0.9)
    //    Toon = 1.0;
    //else if (AmountLight > 0.6)
    //    Toon = 0.6;
    //else if (AmountLight > 0.4)
    //    Toon = 0.4;
    //else if (AmountLight > 0.2)
    //    Toon = 0.2;
    //else
    //    Toon = 0.0;
    
    if ((saturate(TransTexCoord.x) == TransTexCoord.x) && (saturate(TransTexCoord.y) == TransTexCoord.y))
    {
     	// ライト目線によるZ値の再算出
        float ZValue = input.LightPos.z / input.LightPos.w;
        // リアルZ値抽出
        float SM_Z = tex2D(ShadowSampler, TransTexCoord).x;
    
        if (ZValue >= 0.0f && ZValue <= 1.0f)
        {
            // 算出点がシャドウマップのZ値よりも大きければ影と判断
            if (ZValue - 0.005f > SM_Z)
            {
                Shadow = 0.5f;
            }
        }
	}
    return float4(Col.rgb * LightPower * Toon * Shadow, Col.a);
}

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_ToonTex(VS_OUTPUT input) : COLOR
{
    float lightIntensity;
	
    // テクスチャカラー取得
    float4 TexCol = tex2D(ModelSampler, input.tex);
    float4 Col = TexCol;
    float4 LightDir = -g_vecLight;
    float Toon = 1.0f;
    float AmountLight;
    float Shadow = 1.0f;
    float LightPower = 1.0f;
	
    // 射影空間のXY座標をテクスチャ座標に変換
    float2 TransTexCoord;
    TransTexCoord.x = input.LightPos.x / input.LightPos.w / 2.0f + 0.5f;
    TransTexCoord.y = -input.LightPos.y / input.LightPos.w / 2.0f + 0.5f;

    float3 normal = normalize(input.nor);
    float3 lightDir = normalize(g_vecLight.xyz);

    //AmountLight = dot(normal, lightDir);
	
    //// 陰影を3段階に分ける
    //if (AmountLight > 0.9)
    //    Toon = 1.0;
    //else if (AmountLight > 0.6)
    //    Toon = 0.6;
    //else if (AmountLight > 0.4)
    //    Toon = 0.4;
    //else if (AmountLight > 0.2)
    //    Toon = 0.2;
    //else
    //    Toon = 0.0;
    
    if ((saturate(TransTexCoord.x) == TransTexCoord.x) && (saturate(TransTexCoord.y) == TransTexCoord.y))
    {
     	// ライト目線によるZ値の再算出
        float ZValue = input.LightPos.z / input.LightPos.w;
        // リアルZ値抽出
        float SM_Z = tex2D(ShadowSampler, TransTexCoord).x;
    
        if (ZValue >= 0.0f && ZValue <= 1.0f)
        {
            // 算出点がシャドウマップのZ値よりも大きければ影と判断
            if (ZValue - 0.005f > SM_Z)
            {
                Shadow = 0.5f;
            }
        }
    }	
    return float4(Col.rgb * LightPower * Toon * Shadow, Col.a);
}

//**********************************************************************************
// 頂点シェーダプログラムブロック
//**********************************************************************************
float4 VS_Outline(float4 pos : POSITION, float3 normal : NORMAL) : POSITION
{
    // 法線方向に拡張
	float3 expanded = pos.xyz + normal * 1.05;
	
	// 出力用の位置
	float4 Out;
	
	// ワールド座標に変換
	Out = mul(float4(expanded, 1.0), g_mtxWorld);
	
	// カメラ空間に変換
	Out = mul(Out, g_View);
	Out = mul(Out, g_Proj);
	
	// 出力
	return Out;
}

//**********************************************************************************
// 頂点シェーダプログラムブロック
//**********************************************************************************
float4 PS_Outline() : COLOR
{
	return float4(1, 0.3, 0.5, 1);
}

//**********************************************************************************
// テクニック宣言
//**********************************************************************************
technique StandardDraw
{
	pass P0_Toon
	{
		// 表面だけを描画
		CullMode = CCW;
		vertexShader = compile vs_3_0 VS_main();
		pixelShader = compile ps_3_0 PS_Toon();
	}
	pass P1_ToonTex
	{
		// 表面だけを描画
		CullMode = CCW;
		vertexShader = compile vs_3_0 VS_main();
		pixelShader = compile ps_3_0 PS_ToonTex();
	}
	pass P2_Outline
	{
		// 裏面だけを描画
		CullMode = CW;
		VertexShader = compile vs_3_0 VS_Outline();
		PixelShader = compile ps_3_0 PS_Outline();
	}
}