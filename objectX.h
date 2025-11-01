//*************************************
//
//　モデル処理[ObjectX.h]
//　Author:Hatori Kensaku
//
//*************************************

// 二重インクルード防止
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

// インクルード
#include "main.h"
#include "object.h"

// オブジェクトの派生クラスを定義[モデル]
class CObjectX : public CObject
{
public:
	// コンストラクタ・デストラクタ
	CObjectX(const int Priority = 3);
	~CObjectX();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DrawShape(void);
	void CastShadow(void);
	void DrawShadow(void);
	void DrawOutline(void);

	// ゲッター
	float GetAlpha(void) { return m_fAlpha; };
	D3DXVECTOR3 GetPosition(void) { return m_Pos; };
	D3DXVECTOR3 GetRotasion(void) { return m_Rot; };
	D3DXVECTOR3* GetpPosition(void) { return &m_Pos; };
	D3DXVECTOR3* GetpRotasion(void) { return &m_Rot; };
	D3DXMATRIX* GetMtxParent(void) { return m_pMtxParent; };
	D3DXMATRIX GetMtxRot(void) { return m_mtxRot; };
	D3DXMATRIX GetmtxWorld(void) { return m_mtxWorld; }

	// セッター
	void SetPosition(D3DXVECTOR3 Pos) { m_Pos = Pos; };
	void SetRotasion(D3DXVECTOR3 Rot) { m_Rot = Rot; };
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; };
	void SetAlpha(const float Alpha) { m_fAlpha = Alpha; };
	void SetRotMtx(D3DXMATRIX mtxRot) { m_mtxRot = mtxRot; };
	void SetParentMtx(D3DXMATRIX* mtxParent) { m_pMtxParent = mtxParent; };
	void SetPath(std::string Path) { m_FilePath = Path; }

	// 生成
	static CObjectX* Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, std::string Path);
private:
	D3DXVECTOR3 m_Pos;			// 位置
	D3DXVECTOR3 m_Rot;			// 向き
	D3DXVECTOR3 m_Scale;		// 大きさ
	D3DXMATRIX* m_pMtxParent;	// 親のマトリックス
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	D3DXMATRIX m_mtxRot;		// 回転行列
	std::string m_FilePath;		// モデルへのパス
	float m_fAlpha;				// 透明度
};
#endif // !_OBJECTX_H_