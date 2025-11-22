//*************************************
//
//　2Dポリゴン処理[Object2D.h]
//　Author:Hatori Kensaku
//
//*************************************

// 二十インクルード防止
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

// インクルード防止
#include "Object.h"
#include "manager.h"
#include "scene.h"
#include "TagComp.hpp"

// オブジェクトクラスを継承した2Dポリゴン用のクラスを宣言
class CObject2D :public CObject
{
public:
	// コンストラクタ・デストラクタ
	CObject2D(int Priority = 3);
	~CObject2D();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObject2D* Create(const D3DXVECTOR3 Pos, const D3DXQUATERNION Quat);

	// セッター
	void SetPosition(const D3DXVECTOR3 Pos);
	void SetRotasion(const D3DXQUATERNION Quat);
	void SetSize(const D3DXVECTOR2 Size);
	void SetUv(const D3DXVECTOR2 Uv, const float SizeU, const float SizeV);
	void SetCol(const D3DXCOLOR Col);
	void SetTexIndx(const int Indx);

	//// ゲッター
	//D3DXVECTOR3 GetPosition(void) {return CManager::GetScene()->GetReg().get<Transform>(m_Entity).Pos; };
	//D3DXQUATERNION GetRotasion(void) { return CManager::GetScene()->GetReg().get<Transform>(m_Entity).Quat; };
	//D3DXVECTOR2 GetSize(void) { return CManager::GetScene()->GetReg().get<Object2DComponent>(m_Entity).Size; };
private:
};
#endif // !_OBJECT2D_H_