//****************************************************************
//
// 親子関係のコンポーネント[ParentComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// シンプルな親子関係コンポーネントを宣言
struct ParentComponent {
	// イニシャライズコンストラクタ
	ParentComponent(const entt::entity ParentEntity = entt::null) : Parent(ParentEntity) {
		static int Idx = 0;
		nIdx = Idx;
		Idx++;
	}
	// 生成準
	int nIdx;
	// 親のエンティティ
	entt::entity Parent;
};