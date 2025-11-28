//****************************************************************
//
// 計算の処理[math.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "math.h"
#include "math_T.h"
#include "modelmanager.h"

//***************************************
// 浮動小数点のランダム
//***************************************
float CMath::randf(float Max, float Min)
{
	if (Max <= 0 || Min <= 0) return 0.0f;
	// 十倍して十分の一にする
	float Out = (rand() % (int)(Max * 10.0f) + (int)(Min * 10.0f)) * 0.1f;
	return Out;
}

//***************************************
// 座標を3Dから2Dに変換する
//***************************************
D3DXVECTOR3 CMath::Get3Dto2DPosition(const D3DXVECTOR3 Position)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DVIEWPORT9 LocalViewPort;
	D3DXMATRIX LocalProjMtx, LocalViewMtx, mtxWorld;

	// 今のカメラの情報を取得
	pDevice->GetViewport(&LocalViewPort);
	pDevice->GetTransform(D3DTS_PROJECTION, &LocalProjMtx);
	pDevice->GetTransform(D3DTS_VIEW, &LocalViewMtx);

	D3DXMatrixIdentity(&mtxWorld);

	D3DXVECTOR3 OutPos;
	// ２Dにする
	D3DXVec3Project(&OutPos, &Position, &LocalViewPort, &LocalProjMtx, &LocalViewMtx, &mtxWorld);
	return OutPos;
}

//***************************************
// 座標を2Dから3Dに変換する
//***************************************
D3DXVECTOR3 CMath::Get2Dto3DPosition(const D3DXVECTOR3 Position)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DVIEWPORT9 LocalViewPort;
	D3DXMATRIX LocalProjMtx, LocalViewMtx, mtxWorld;

	// 今のカメラの情報を取得
	pDevice->GetViewport(&LocalViewPort);
	pDevice->GetTransform(D3DTS_PROJECTION, &LocalProjMtx);
	pDevice->GetTransform(D3DTS_VIEW, &LocalViewMtx);

	D3DXMatrixIdentity(&mtxWorld);

	D3DXVECTOR3 OutPos;
	// 3Dにする
	D3DXVec3Unproject(&OutPos, &Position, &LocalViewPort, &LocalProjMtx, &LocalViewMtx, &mtxWorld);
	return OutPos;
}

//***************************************
// 移動量を計算
//***************************************
D3DXVECTOR3 CMath::CalcMove(D3DXVECTOR3* Pos,float Speed)
{
	CCamera* pCamera = CManager::GetCamera();
	CInputKeyboard* pInput = CManager::GetInputKeyboard();
	D3DXVECTOR3 Move = VEC3_NULL;

	if (pInput->GetPress(DIK_W) == true)
	{
		if (pInput->GetPress(DIK_A) == true)
		{
			Move.x = sinf(pCamera->GetRot().y + (D3DX_PI * 0.75f)) * Speed;
			Move.z = cosf(pCamera->GetRot().y + (D3DX_PI * 0.75f)) * Speed;
		}
		else if (pInput->GetPress(DIK_D) == true)
		{
			Move.x = sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * Speed;
			Move.z = cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * Speed;
		}
		else
		{
			Move.x = sinf(pCamera->GetRot().y + D3DX_PI) * Speed;
			Move.z = cosf(pCamera->GetRot().y + D3DX_PI) * Speed;
		}
	}
	else if (pInput->GetPress(DIK_S) == true)
	{
		if (pInput->GetPress(DIK_A) == true)
		{
			Move.x = sinf(pCamera->GetRot().y + (D3DX_PI * 0.25f)) * Speed;
			Move.z = cosf(pCamera->GetRot().y + (D3DX_PI * 0.25f)) * Speed;
		}
		else if (pInput->GetPress(DIK_D) == true)
		{
			Move.x = sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * Speed;
			Move.z = cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * Speed;
		}
		else
		{
			Move.x = sinf(pCamera->GetRot().y) * Speed;
			Move.z = cosf(pCamera->GetRot().y) * Speed;
		}
	}
	else if (pInput->GetPress(DIK_A) == true)
	{
		Move.x = sinf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * Speed;
		Move.z = cosf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * Speed;
	}
	else if (pInput->GetPress(DIK_D) == true)
	{
		Move.x = sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * Speed;
		Move.z = cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * Speed;
	}
	return *Pos += Move;
}

//***************************************
// ランダムなベクトルを算出
//***************************************
D3DXVECTOR3 CMath::RandVector(D3DXVECTOR3 MaxDir, D3DXVECTOR3 MinDir)
{
	D3DXVECTOR3 randVec;

	// 始点 + 方向 * 強さ
	randVec.x = MinDir.x + (float)(rand()) / RAND_MAX * (MaxDir.x - MinDir.x);
	randVec.y = MinDir.y + (float)(rand()) / RAND_MAX * (MaxDir.y - MinDir.y);
	randVec.z = MinDir.z + (float)(rand()) / RAND_MAX * (MaxDir.z - MinDir.z);

	// 正規化して方向ベクトルにする
	D3DXVec3Normalize(&randVec, &randVec);

	return randVec;
}

//***************************************
// ベクトルのなす角を求める
//***************************************
float CMath::VectersFromAngle(D3DXVECTOR3 VecA, D3DXVECTOR3 VecB,D3DXVECTOR3 Border)
{
	D3DXVec3Normalize(&VecA, &VecA);
	D3DXVec3Normalize(&VecB, &VecB);
	float Dot = D3DXVec3Dot(&VecA, &Border);
	Dot = Dot / fabsf(Dot);
	return (float)acos(D3DXVec3Dot(&VecA, &VecB)) * Dot;
}

//***************************************
// 小数点を第二位までに変換
//***************************************
float CMath::ConvertFloat(float Value,int Format)
{
	int Convert;
	float Out;
	Convert = (int)(Value * powf(10.0f, (float)Format));
	Out = Convert / powf(10.0f, (float)Format);
	return Out;
}

//***************************************
// クォータニオンを変換
//***************************************
btQuaternion CMath::SetQuad(D3DXQUATERNION Set)
{
	return btQuaternion(Set.x, Set.y, Set.z, Set.w);
}

D3DXQUATERNION CMath::SetQuad(btQuaternion Set)
{
	return D3DXQUATERNION(Set.x(), Set.y(), Set.z(), Set.w());
}

//***************************************
// ベクトルを変換
//***************************************
btVector3 CMath::SetVec(D3DXVECTOR3 Set)
{
	return btVector3(Set.x, Set.y, Set.z);
}

D3DXVECTOR3 CMath::SetVec(btVector3 Set)
{
	return D3DXVECTOR3(Set.x(), Set.y(), Set.z());
}

//***************************************
// モデルの大きさを計測
// 戻り値半分の大きさ
//***************************************
D3DXVECTOR3 CMath::CalcModelSize(std::string Path)
{
	// モデルmanagerからインデックスを指定して取得
	CModelManager::MapObject modelinfo = CModelManager::GetModelInfo(Path);

	BYTE* pVtxBuff = NULL;
	WORD SizeFVF = (WORD)D3DXGetFVFVertexSize(modelinfo.modelinfo.pMesh->GetFVF());
	int nNumVtx = modelinfo.modelinfo.pMesh->GetNumVertices();
	modelinfo.modelinfo.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
	D3DXVECTOR3 Max, Min;
	Max = VEC3_NULL;
	Min = VEC3_NULL;

	for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
	{
		D3DXVECTOR3* Size = (D3DXVECTOR3*)pVtxBuff;
		if (Max.x < Size->x)
		{
			Max.x = Size->x;
		}
		if (Max.y < Size->y)
		{
			Max.y = Size->y;
		}
		if (Max.z < Size->z)
		{
			Max.z = Size->z;
		}
		if (Min.x > Size->x)
		{
			Min.x = Size->x;
		}
		if (Min.y > Size->y)
		{
			Min.y = Size->y;
		}
		if (Min.z > Size->z)
		{
			Min.z = Size->z;
		}
		pVtxBuff += SizeFVF;
	}
	D3DXVECTOR3 Size;
	Size = Max;
	Size.y *= 0.5f;

	modelinfo.modelinfo.pMesh->UnlockVertexBuffer();
	return Size;
}

////***************************************
//// 線分と線分の最接点距離
////***************************************
//float CMath::CalcSegSegDist(CCollision::Segment Seg1, CCollision::Segment Seg2, float* S, float* T, D3DXVECTOR3* C1, D3DXVECTOR3* C2)
//{
//	// ベクトルを引く
//	D3DXVECTOR3 Vec1 = Seg1.End - Seg1.Start;
//	D3DXVECTOR3 Vec2 = Seg2.End - Seg2.Start;
//	D3DXVECTOR3 toVec = Seg1.Start - Seg2.Start;
//	// 長さを図る
//	float Length1 = D3DXVec3Dot(&Vec1, &Vec1);
//	float Length2 = D3DXVec3Dot(&Vec2, &Vec2);
//	float toLength = D3DXVec3Dot(&Vec2, &toVec);
//	// 小数点100万分の位置まで考慮する
//	float Epsilon = 1e-6f;
//
//	if (Length1 <= Epsilon && Length2 <= Epsilon)
//	{
//		*S = 0.0f;
//		*T = 0.0f;
//		*C1 = Seg1.Start;
//		*C2 = Seg2.Start;
//		D3DXVECTOR3 VecC = *C1 - *C2;
//		return D3DXVec3Dot(&VecC,&VecC);
//	}
//	if (Length1 <= Epsilon)
//	{
//		*S = 0.0f;
//		*T = toLength / Length2;
//		*T = Clamp(*T, 0.0f, 1.0f);
//	}
//	else
//	{
//		float C = D3DXVec3Dot(&Vec1, &toVec);
//		if (Length2 <= Epsilon)
//		{
//			*T = 0.0f;
//			*S = Clamp(-C / Length1, 0.0f, 1.0f);
//		}
//		else
//		{
//			float B = D3DXVec3Dot(&Vec1, &Vec2);
//			float Ttom = B * (*S) + toLength;
//			if (Ttom < 0.0f)
//			{
//				*T = 0.0f;
//				*S = Clamp(-C / Length1, 0.0f, 1.0f);
//			}
//			else if (Ttom > Length2)
//			{
//				*T = 1.0f;
//				*S = Clamp((B - C) / Length1, 0.0f, 1.0f);
//			}
//			else
//			{
//				*T = Ttom / Length2;
//			}
//			float Denom = (Length1 * Length2) - (B * B);
//			if (Denom != 0.0f)
//			{
//				*S = Clamp((B * toLength) / Denom, 0.0f, 1.0f);
//			}
//			else
//			{
//				*S = 0.0f;
//			}
//			*T = (B * (*S) + toLength) / Length2;
//			if (*T < 0.0f)
//			{
//				*T = 0.0f;
//				*S = Clamp(-C / Length1, 0.0f, 1.0f);
//			}
//			else if (*T > 1.0f)
//			{
//				*T = 1.0f;
//				*S = Clamp((B - C) / Length1, 0.0f, 1.0f);
//			}
//		}
//	}
//	*C1 = Seg1.Start + Vec1 * (*S);
//	*C2 = Seg2.Start + Vec2 * (*T);
//	D3DXVECTOR3 VecC = *C1 - *C2;
//	return D3DXVec3Dot(&VecC,&VecC);
//}
//
/////
///// <<カプセル同士の最接点距離を計算>>
///// <製作者>
///// 羽鳥健作
///// <param name="Cap1"><カプセル１>
///// <param name="Cap2"><カプセル２>
///// <param name="S"><最接点１のスカラー値(出力用)>
///// <param name="T"><最接点２のスカラー値(出力用)>
///// <param name="C1"><最接点１(出力用)>
///// <param name="C2"><最接点２(出力用)>
///// <returns><二つのカプセルの最接点距離をリターン>
///// 
//float CMath::CalcCapCapDist(CCollision::Capsule Cap1, CCollision::Capsule Cap2, float* S, float* T, D3DXVECTOR3* C1, D3DXVECTOR3* C2)
//{
//	// カプセルの終点から始点までのベクトル
//	D3DXVECTOR3 Vec1 = Cap1.End - Cap1.Start;
//	D3DXVECTOR3 Vec2 = Cap2.End - Cap2.Start;
//	// カプセルからもう一方のカプセルまでのベクトル
//	D3DXVECTOR3 toVec = Cap1.Start - Cap2.Start;
//	// それぞれのベクトルのながさ
//	float Length1 = D3DXVec3Dot(&Vec1, &Vec1);
//	float Length2 = D3DXVec3Dot(&Vec2, &Vec2);
//	float toLength = D3DXVec3Dot(&Vec2, &toVec);
//	// 誤差をなくすための一時変数(100万分の1まで考慮)
//	float Epsilon = 1e-6f;
//	// 両方のカプセルが点だったら
//	if (Length1 <= Epsilon && Length2 <= Epsilon)
//	{
//		*S = 0.0f;
//		*T = 0.0f;
//		*C1 = Cap1.Start;
//		*C2 = Cap2.Start;
//		D3DXVECTOR3 VecC = *C1 - *C2;
//		return D3DXVec3Dot(&VecC, &VecC);
//	}
//	// 片方のカプセル１が点だったら
//	if (Length1 <= Epsilon)
//	{
//		*S = 0.0f;
//		*T = toLength / Length2;
//		*T = Clamp(*T, 0.0f, 1.0f);
//	}
//	// それ以外
//	else
//	{
//		float C = D3DXVec3Dot(&Vec1, &toVec);
//		// カプセル２が点だったら
//		if (Length2 <= Epsilon)
//		{
//			*T = 0.0f;
//			*S = Clamp(-C / Length1, 0.0f, 1.0f);
//		}
//		// それ以外
//		else
//		{
//			float B = D3DXVec3Dot(&Vec1, &Vec2);
//			float Ttom = B * (*S) + toLength;
//			if (Ttom < 0.0f)
//			{
//				*T = 0.0f;
//				*S = Clamp(-C / Length1, 0.0f, 1.0f);
//			}
//			else if (Ttom > Length2)
//			{
//				*T = 1.0f;
//				*S = Clamp((B - C) / Length1, 0.0f, 1.0f);
//			}
//			else
//			{
//				*T = Ttom / Length2;
//			}
//			float Denom = (Length1 * Length2) - (B * B);
//			if (Denom != 0.0f)
//			{
//				*S = Clamp((B * toLength) / Denom, 0.0f, 1.0f);
//			}
//			else
//			{
//				*S = 0.0f;
//			}
//			*T = (B * (*S) + toLength) / Length2;
//			if (*T < 0.0f)
//			{
//				*T = 0.0f;
//				*S = Clamp(-C / Length1, 0.0f, 1.0f);
//			}
//			else if (*T > 1.0f)
//			{
//				*T = 1.0f;
//				*S = Clamp((B - C) / Length1, 0.0f, 1.0f);
//			}
//		}
//	}
//	*C1 = Cap1.Start + Vec1 * (*S);
//	*C2 = Cap2.Start + Vec2 * (*T);
//	D3DXVECTOR3 VecC = *C1 - *C2;
//	return D3DXVec3Dot(&VecC, &VecC);
//}

//***************************************
// デストラクタ
//***************************************
CMath::~CMath()
{

}