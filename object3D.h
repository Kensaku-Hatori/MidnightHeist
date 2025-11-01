//*************************************
//
//　3Dポリゴン処理[Object3D.h]
//　Author:Hatori Kensaku
//
//*************************************

// 二重インクルード防止
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

// インクルード
#include "manager.h"

// オブジェクトの派生クラスを定義[３Dポリゴン]
class CObject3D : public CObject
{
public:
	// コンストラクタ・デストラクタ
	CObject3D(int Priority = 3);
	~CObject3D();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// セッター
	void SetPosition(D3DXVECTOR3 Pos) { m_Pos = Pos; };
	void SetRotasion(D3DXVECTOR3 Rot) { m_Rot = Rot; };
	void SetRotasionDest(D3DXVECTOR3 Rot) { m_RotDest = Rot; };
	void SetSize(D3DXVECTOR2 Size) { m_fWidth = Size.x,m_fHeigth = Size.y; };
	void SetCol(D3DXCOLOR Col) { m_Col = Col; };
	void SetUV(D3DXVECTOR2 UVMax, D3DXVECTOR2 UVMin) { m_UVMax = UVMax;m_UVMin = UVMin; };
	void ResisterTexIndx(const int TexIndx) { m_nTexIndx = TexIndx; };

	// ゲッター
	D3DXVECTOR3 GetPos(void) { return m_Pos; };
	D3DXVECTOR3 GetRot(void) { return m_Rot; };
	D3DXVECTOR2 GetUvMax(void) { return m_UVMax; };
	D3DXVECTOR2 GetUvMin(void) { return m_UVMin; };
	float GetWidth(void) { return m_fWidth; };
	float GetHeight(void) { return m_fHeigth; };

	// 生成
	static CObject3D* Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot);
private:
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVertex;	// 頂点バッファ
	D3DXMATRIX m_mtxWorld;				// マトリックス
	D3DXVECTOR3 m_Pos;					// 位置
	D3DXVECTOR3 m_Rot;					// 向き
	D3DXVECTOR3 m_RotDest;				// 目標の向き
	D3DXVECTOR2 m_UVMax;				// テクスチャ座標の最大値
	D3DXVECTOR2 m_UVMin;				// テクスチャ座標の最小値
	D3DXCOLOR m_Col;					// 色
	float m_fWidth;						// 横幅
	float m_fHeigth;					// 高さ
	int m_nTexIndx;						// テクスチャへのインデックス
};
#endif // !