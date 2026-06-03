//================================================================
//
// リザルトの処理[Result.h]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// 二重インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// インクルード
//****************************************************************
#include "scene.h"

//****************************************************************
// クラスを定義
//****************************************************************
class CResult : public CScene
{
public:
	// コンストラクタ・デストラクタ
	CResult() : CScene(CScene::MODE_RESULT) {};
	~CResult() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>結果</returns>
	HRESULT Init(void);
	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name=""></param>
	void Uninit(void);
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
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