//****************************************************************
//
// セットアップ読み込みの処理[SetUpLoader.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// クラスを定義
class CSetUpLoader
{
public:
	// コンストラクタ・デストラクタ
	CSetUpLoader() = default;
	~CSetUpLoader() = default;

	// 読み込み
	void LoadToScene(entt::registry& Reg, std::string Path);

	// インスタンス
	static CSetUpLoader& Instance() {
		static CSetUpLoader instance;
		return instance;
	};
private:
};