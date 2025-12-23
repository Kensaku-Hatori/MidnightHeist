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

	// メンバ関数
	void Init(void)override;
	void Update(void)override;
	void Uninit(void)override;
private:
	// 定数用の構造体
	struct Config {
		// デフォルト
		struct Default {
			// 視点・注視点
			static const D3DXVECTOR3 PosV;
			static const D3DXVECTOR3 PosR;
		};
		// プレイヤーが登場した時
		struct FirstAnimation {
			// 近さ
			static constexpr float Zoom = 1400.0f;
		};
		// プレイヤーがピッキングしているとき
		struct PickingZoom {
			// 近さ
			static constexpr float Zoom = 1200.0f;
		};
	};
};