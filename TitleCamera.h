//****************************************************************
//
// タイトルカメラの処理[TitleCamera.h]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "camera.h"

// 継承クラスを定義
class CTitleCamera : public CBaceSystem
{
public:
	// コンストラクタ・デストラクタ
	CTitleCamera() = default;
	~CTitleCamera() = default;

	// メンバ関数
	void Init(void)override;
	void Update(void)override;
	void Uninit(void)override;
private:
	// 定数用の構造体
	struct Config {
		// 視点・注視点
		static const D3DXVECTOR3 PosV;
		static const D3DXVECTOR3 PosR;
	};
};