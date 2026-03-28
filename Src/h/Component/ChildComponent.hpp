//****************************************************************
//
// 親子関係のコンポーネント[ChildrenComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// シンプルな親子関係コンポーネントを宣言
struct ChildrenComponent {
	// イニシャライズコンストラクタ
	ChildrenComponent(const std::vector<entt::entity> Children = {}) : Children(Children) {}
	// 子供のリスト
	std::vector<entt::entity> Children;
};