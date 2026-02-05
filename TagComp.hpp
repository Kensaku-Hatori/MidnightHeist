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
	struct Object2D {};
	// 3Dポリゴンタグ
	struct Object3D {};
	// オブジェクトXの情報
	struct ObjectX {};
	// タイトルマネージャー
	struct TitleManager {};
	// タイトルメニュー
	struct TitleMenu {};
	// ポーズマネージャー
	struct PauseManager {};
	// ポーズメニュー
	struct PauseMenu {};
	// プレイヤー情報
	struct Player {};
	// 敵
	struct Enemy {};
	// レーザー
	struct Laser {};
	// マップオブジェクト
	struct MapObject {};
	// アイテム
	struct Item {};
	// メッシュフィールド
	struct MeshField {};
	// シャドウマップに書き込む
	struct CastShadow {};
	// 物陰として書き込む
	struct CastShapeShadow {};
	// アウトライン描画
	struct RenderingOutLine {};
	// パトロールポイント情報
	struct PatrolPointManager {};
	// スカイボックス情報
	struct SkyBox {};
	// 円形UI
	struct UICircle {};
	// 扇形視界
	struct SightFan {};
	// 音の大きさ
	struct VisibleSound {};
	// スタッツマネージャー
	struct StutsManager {};
	// タイマー
	struct Timer {};
	// ゲートマネージャー
	struct GateManager {};
}

// 画面遷移用のタグ
namespace SequenceTag {
	struct InTitle {};
	struct InGame {};
}

// 物理エンジン用
namespace Pysics {
	// 剛体
	struct RigitBody {};
	// 矩形の当たり判定
	struct BoxCollider {};
	// カプセルの当たり判定
	struct CapsuleCollider {};
}