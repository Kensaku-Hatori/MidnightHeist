//****************************************************************
//
// ゲームカメラの処理[GameCamera.h]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "camera.h"

// 継承クラスを定義
class CGameCamera : public CBaceSystem
{
public:
	// コンストラクタ・デストラクタ
	CGameCamera() = default;
	~CGameCamera() = default;

	void Init(void)override;
	void Update(void)override;
	void Uninit(void)override;
private:
	struct Config {
		static const D3DXVECTOR3 PosV;
		static const D3DXVECTOR3 PosR;
	};
};