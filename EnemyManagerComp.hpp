//****************************************************************
//
// 敵の管理のコンポーネント[EnemyManagerComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "Factories.h"
#include "manager.h"
#include "scene.h"

// テクスチャ情報
struct EnemyManagerComp {
	// イニシャライズコンストラクタ
	EnemyManagerComp(const std::string& Path = "") {
		// 名前空間
		using namespace nlohmann;
		using namespace std;
		using namespace Factories;

		// jsonファイルを開く
		ifstream ifs(Path);

		if (!ifs)
		{
			MessageBox(NULL, "ファイルが読み込めませんでした", "終了メッセージ", MB_OK);
			return;
		}
		if (ifs.is_open() == false)
		{
			MessageBox(NULL, "ファイルが読み込めませんでした", "終了メッセージ", MB_OK);
			return;
		}

		// jsonデータを宣言
		ordered_json jsonData;
		// jsonを読み込む
		ifs >> jsonData;

		// SetStage配列の中にあるデータを代入存在しなかったら終わる
		for (const auto& Info : jsonData)
		{
			std::vector<EnemyState::PatrolMap> SetInfo;
			D3DXVECTOR3 Pos;
			D3DXQUATERNION Quat;

			// 位置情報を取り出す
			Pos.x = Info["Transform"]["Position"]["x"];
			Pos.y = Info["Transform"]["Position"]["y"];
			Pos.z = Info["Transform"]["Position"]["z"];

			// 回転情報を取り出す
			Quat.w = Info["Transform"]["Rotation"]["w"];
			Quat.x = Info["Transform"]["Rotation"]["x"];
			Quat.y = Info["Transform"]["Rotation"]["y"];
			Quat.z = Info["Transform"]["Rotation"]["z"];

			SetInfo.resize(Info["PointList"].size());

			for (int PointCount = 0; PointCount < static_cast<int>(Info["PointList"].size()); PointCount++)
			{
				std::string PointIdx = std::to_string(PointCount);
				std::string PointKey = "P" + PointIdx;
				SetInfo[PointCount].Idx = Info["PointList"][PointKey]["Idx"];
				SetInfo[PointCount].CoolDown = Info["PointList"][PointKey]["CoolDown"];
			}
			EnemyList.push_back(Factories::makeEnemy(CManager::GetScene()->GetReg(), Pos, SetInfo));
		}
	};
	std::vector<entt::entity> EnemyList;
};