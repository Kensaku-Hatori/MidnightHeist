//****************************************************************
//
// リザルトの処理[Result.h]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "scene.h"

// クラスを定義
class CResult : public CScene
{
public:
	// コンストラクタ・デストラクタ
	CResult() : CScene(CScene::MODE_RESULT) {};
	~CResult() = default;

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
	// 定数を設定
	struct Config {
		// セットアップ
		struct SetUp {
			static constexpr const char* Path = "data/TEXT/SetUp/ResultSetUp.json";
		};
	};
};