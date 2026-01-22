// グローバル変数宣言ブロック
float4x4 g_mtxWorld;	// ワールドマトリックス
float4x4 g_View;		// 射影変換マトリックス
float4x4 g_Proj;		// プロェクションマトリックス
float4x4 g_LightView;	// ライトビュー変換行列
float4x4 g_LightProj;	// 射影変換行列
float4 g_Deffuse;		// 色
float4 g_vecLight;      // ライトの向きベクトル
float g_Thickness;      // アウトラインの太さ
float g_Height;         // アウトラインを描画する高さ
float4 g_OutLineColor;  // アウトラインの色

// 頂点シェーダ出力構造体
struct VS_OUTPUT
{
	float4 pos : POSITION;			// 位置
	float3 nor : NORMAL;			// 法線
	float4 col : COLOR0;			// 色
	float4 colModel : COLOR1;		// 色
    float2 tex : TEXCOORD;          // テクスチャ座標
    float4 LightPos : TEXCOORD1;	// ライト空間の位置
    float4 PosPS : TEXCOORD2;       // ライト空間の位置
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

// トゥーンマップ(テクスチャ)
texture g_ToonMap;

// テクスチャサンプラーを宣言
sampler2D ToonMapSampler = sampler_state
{
	// サンプラーを設定
    Texture = <g_ToonMap>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

// モデルのテクスチャ
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

float2 TexelSize = float2(0.00078125, 0.0013888888888889);

float2 offset[4] =
{
    float2(1, 0),
    float2(-1, 0),
    float2(0, 1),
    float2(0, -1),
};

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_Toon(VS_OUTPUT input) : COLOR
{
    float lightIntensity;
	
    // テクスチャカラー取得
    float4 Col = g_Deffuse;
    float4 LightDir = -g_vecLight;
    float4 ToonMap;
    float AmountLight;
    float Shadow = 1.0f;
    float LightPower = 1.0f;
	
    // 射影空間のXY座標をテクスチャ座標に変換
    float2 TransTexCoord;
    TransTexCoord.x = input.LightPos.x / input.LightPos.w / 2.0f + 0.5f;
    TransTexCoord.y = -input.LightPos.y / input.LightPos.w / 2.0f + 0.5f;

    float3 normal = normalize(input.nor);
    float3 lightDir = normalize(g_vecLight.xyz);

    // 光の量を計算
    AmountLight = dot(normal, lightDir);
    // -1から1を0から1に修正
    AmountLight = AmountLight * 0.5f + 0.5f;
    AmountLight = AmountLight * AmountLight;
    
    // トゥーンテクスチャから色をとってくる
    ToonMap = tex2D(ToonMapSampler, float2(AmountLight, 0.0f));
    
    float shadowSum = 0.0f;
    
    if ((saturate(TransTexCoord.x) == TransTexCoord.x) && (saturate(TransTexCoord.y) == TransTexCoord.y))
    {
     	// ライト目線によるZ値の再算出
        float ZValue = input.LightPos.z / input.LightPos.w;
        // リアルZ値抽出
        float SM_Z = tex2D(ShadowSampler, TransTexCoord).x;
    
        if (ZValue >= 0.0f && ZValue <= 1.0f)
        {
            for (int i = 0; i < 4; i++)
            {
                float2 uv = TransTexCoord + offset[i] * TexelSize;

                float SM_Z = tex2D(ShadowSampler, uv).x;

                if (ZValue - 0.02f > SM_Z)
                {
                    shadowSum += 1.0f;
                }
            }
        }
	}
    // 影の割合（0.0 = 光, 1.0 = 完全影）
    float shadowRate = shadowSum / 4;
    // 影の暗さ調整（完全影でも真っ黒にしない）
    Shadow = lerp(1.0f, 0.5f, shadowRate);
    
    return float4(Col.rgb * LightPower * Shadow, Col.a) * ToonMap;
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
    float4 ToonMap;
    float AmountLight = 1.0f;
    float Shadow = 1.0f;
    float LightPower = 1.0f;
	
    // 射影空間のXY座標をテクスチャ座標に変換
    float2 TransTexCoord;
    TransTexCoord.x = input.LightPos.x / input.LightPos.w / 2.0f + 0.5f;
    TransTexCoord.y = -input.LightPos.y / input.LightPos.w / 2.0f + 0.5f;

    float3 normal = normalize(input.nor);
    float3 lightDir = normalize(g_vecLight.xyz);

    // 光の量を計算
    AmountLight = dot(normal, lightDir);
    // -1から1を0から1に修正
    AmountLight = AmountLight * 0.5f + 0.5f;
    AmountLight = AmountLight * AmountLight;
        
    // トゥーンテクスチャから色をとってくる
    ToonMap = tex2D(ToonMapSampler, float2(AmountLight, 0.0f));

    if ((saturate(TransTexCoord.x) == TransTexCoord.x) && (saturate(TransTexCoord.y) == TransTexCoord.y))
    {
     	// ライト目線によるZ値の再算出
        float ZValue = input.LightPos.z / input.LightPos.w;
        // リアルZ値抽出
        float SM_Z = tex2D(ShadowSampler, TransTexCoord).x;
    
        if (ZValue >= 0.0f && ZValue <= 1.0f)
        {
            // 算出点がシャドウマップのZ値よりも大きければ影と判断
            if (ZValue - 0.02f > SM_Z)
            {
                Shadow = 0.5f;
            }
        }
    }	
    return float4(Col.rgb * LightPower * Shadow, Col.a) * ToonMap;
}

//**********************************************************************************
// 頂点シェーダプログラムブロック
//**********************************************************************************
VS_OUTPUT VS_Outline(VS_OUTPUT Input)
{
    // モデル空間に置ける拡大方向を求める
    float4 expanded = mul(float4(Input.nor.xyz, 0.0f), g_mtxWorld);
    // スケールを取り除いた方向を求める
    expanded = normalize(expanded);
    // 押し出す
    expanded *= g_Thickness;
	
	// 出力用の位置
    VS_OUTPUT Out;
    Out = Input;
    
	// ワールド座標に変換
    Out.pos = mul(Input.pos, g_mtxWorld);
    // 押し出す
    Out.pos += expanded;
    Out.PosPS = Out.pos;
	
	// カメラ空間に変換
	Out.pos = mul(Out.pos, g_View);
	Out.pos = mul(Out.pos, g_Proj);
        
	// 出力
	return Out;
}

//**********************************************************************************
// 頂点シェーダプログラムブロック
//**********************************************************************************
float4 PS_Outline(VS_OUTPUT Input) : COLOR
{
    if (Input.PosPS.y > g_Height)
        return g_OutLineColor;
    else
        return float4(0.0f, 0.0f, 0.0f, 0.0f);
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