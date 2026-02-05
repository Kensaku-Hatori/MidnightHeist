//****************************************************************
//
// 巡回ポイントのコンポーネント[PatrolPointComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

#include "Factories.h"
#include "manager.h"
#include "scene.h"

// 念のため
namespace PatrolPoint {
    // 一つの巡回ポイントの構造体
    struct PatrolPointInfo
    {
        // 位置
        D3DXVECTOR3 Point;
		// 移動できる隣接ノードへのIdx
		std::vector<int> CanMove;
		// 自分自身のIdx
		int Idx;
    };
	// エースター用の構造体
	struct PatrolPointInfoForAStar
	{
		// 自分自身のIdx
		int Idx;
		// 自分自身のノードの親
		int Parent;
		// スタート地点からゴールまでのコスト
		// 今の位置からゴールまでのコスト
		// ヒューリスティック関数のコスト
		float StartGoalCost, MyGoalCost, Heuristic;
	};
}

// テクスチャ情報
struct PatrolPointComponent {
    // イニシャライズコンストラクタ
	PatrolPointComponent(const std::string& Path,const float Radius) {
		// 名前空間
		using namespace nlohmann;
		using namespace std;
		using namespace Factories;

		// jsonファイルを開く
		ifstream ifs(Path);

		// ポイントの大きさ
		PointRadius = Radius;

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

		// クリア
		PatrolPoint.clear();

		// jsonデータを宣言
		ordered_json jsonData;
		// jsonを読み込む
		ifs >> jsonData;

		// SetStage配列の中にあるデータを代入存在しなかったら終わる
		for (const auto& Info : jsonData)
		{
			PatrolPoint::PatrolPointInfo LoadInfo;

			// 位置情報を取り出す
			LoadInfo.Point.x = Info["Point"]["Pos"]["x"];
			LoadInfo.Point.y = Info["Point"]["Pos"]["y"];
			LoadInfo.Point.z = Info["Point"]["Pos"]["z"];

			for (int nCnt = 0;nCnt < static_cast<int>(Info["Point"]["CanMove"].size());nCnt++)
			{
				std::string Key = std::to_string(nCnt);
				LoadInfo.CanMove.push_back(Info["Point"]["CanMove"][Key]);
			}
			// 今のベクターのサイズをIdxとして使用
			LoadInfo.Idx = PatrolPoint.size();
			//Factories::makeMapobject(CManager::GetScene()->GetReg(), "data/MODEL/testBall_00.x", { LoadInfo.Point.x,LoadInfo.Point.y + 100.0f,LoadInfo.Point.z });
			// 連結
			PatrolPoint.push_back(LoadInfo);
		}
		//int test = 7;
		//for (int nCnt = 0; nCnt < static_cast<int>(PatrolPoint[test].CanMove.size()); nCnt++)
		//{
		//	Factories::makeMapobject(CManager::GetScene()->GetReg(), "data/MODEL/testBall_00.x", PatrolPoint[PatrolPoint[test].CanMove[nCnt]].Point);
		//}
    };
	// デストラクタ
	~PatrolPointComponent() {
		// 配列をクリア
		PatrolPoint.clear();
	};
    // コリジョンフラグ
    std::vector<PatrolPoint::PatrolPointInfo> PatrolPoint;
	// 巡回ポイントの当たり判定の大きさ
	float PointRadius;
};