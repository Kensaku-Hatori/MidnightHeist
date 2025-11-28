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
	// プレイヤー情報
	struct PlayerComponent {};
	// 敵
	struct EnemyComponent {};
	// レーザー
	struct LaserComponent {};
	// マップオブジェクト
	struct MapObjectComponent {};
	// メッシュフィールド
	struct MeshFieldComponent {};
}