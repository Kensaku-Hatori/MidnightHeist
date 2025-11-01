//*************************************
//
//　ビルボード処理[ObjectBilBoard.h]
//　Author:Hatori Kensaku
//
//*************************************

// 二重インクルード防止
#ifndef _OBJECTBILLBOARD_H_
#define _OBJECTBILLBOARD_H_

// インクルード
#include "manager.h"

// オブジェクトの派生クラスを定義[ビルボード]
class CObjectBillBoard : public CObject
{
public:
	// コンストラクタ・デストラクタ
	CObjectBillBoard(const int Priority = 3);
	~CObjectBillBoard();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// セッター
	void SetPosition(D3DXVECTOR3 Pos) { m_Pos = Pos; };
	void SetSize(D3DXVECTOR2 Size) { m_Size = Size; };
	void SetCol(D3DXCOLOR Col) { m_Col = Col; };
	void SetTexIndx(int Indx) { m_nTexIndx = Indx; };

	// ゲッター
	D3DXVECTOR3 GetPos(void) { return m_Pos; };
	D3DXVECTOR2 GetSize(void) { return m_Size; };
	D3DXCOLOR GetCol(void) { return m_Col; };

	// 生成
	static CObjectBillBoard* Create(D3DXVECTOR3 Pos, D3DXVECTOR2 Size);
private:
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVertex;	// 頂点バッファ
	D3DXMATRIX m_mtxWorld;				// マトリックス
	D3DXVECTOR3 m_Pos;					// 位置
	D3DXVECTOR2 m_Size;					// 大きさ
	D3DXCOLOR m_Col;					// 色
	int m_nTexIndx;						// テクスチャへのインデックス
};
#endif // !_OBJECTBILLBOARD_H_