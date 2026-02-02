//****************************************************************
//
// コンポーネント達[Components.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 剛体コンポーネント
struct RigidBodyComponent{
	// 剛体
	btRigidBody* Body;
	// 当たり判定形状
	btCollisionShape* Shape;
	// 質量
	float Mass;
	// グループ
	int Group;
	// マスク
	int Mask;
};

// 矩形コライダー
struct BoxColliderComponent {
	// 半分の大きさ
	D3DXVECTOR3 HalfSize;
	// ローカル空間の位置
	D3DXVECTOR3 LocalPos;
	// ローカル空間の回転
	D3DXQUATERNION LocalQuat;
};

// カプセルコライダー
struct CapsuleColliderComponent {
	// 高さ
	float Height;
	// 大きさ
	float Radius;
	// ローカル空間の位置
	D3DXVECTOR3 LocalPos;
	// ローカル空間の回転
	D3DXQUATERNION LocalQuat;
};