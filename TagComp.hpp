//****************************************************************
//
// タグのコンポーネント[TagComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

namespace Tag {
	// 2Dのポリゴンタグ
	struct Object2DComponent {};
	// 3Dポリゴンタグ
	struct Object3DComponent {};
	// オブジェクトXの情報
	struct ObjectXComponent {};
	// タイトルマネージャー
	struct TitleManagerComponent {};
	// タイトルメニュー
	struct TitleMenuComponent {};
	// ポーズマネージャー
	struct PauseManagerComponent {};
	// ポーズメニュー
	struct PauseMenuComponent {};
	// プレイヤー情報
	struct PlayerComponent {};
	// 敵
	struct EnemyComponent {};
	// レーザー
	struct LaserComponent {};
	// マップオブジェクト
	struct MapObjectComponent {};
	// アイテム
	struct ItemComponent {};
	// メッシュフィールド
	struct MeshFieldComponent {};
	// シャドウマップに書き込む
	struct CastShadow {};
	// 物陰として書き込む
	struct CastShapeShadow {};
	// アウトライン描画
	struct RenderingOutLine {};
	// パトロールポイント情報
	struct PatrolPointManager {};
	// スカイボックス情報
	struct SkyBoxComponent {};
	// 円形UI
	struct UICircleComponent {};
}
namespace SequenceTag {
	struct InTitleComp {};
	struct InGameComp {};
}