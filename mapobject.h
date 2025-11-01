//****************************************************************
//
// マップに置くオブジェクトの処理[mapobject.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _MAPOBJECT_H_
#define _MAPOBJECT_H_

// インクルード
#include "object.h"
#include "modelmanager.h"
#include "sound.h"

// マップに置くオブジェクトのクラスを定義
class CMapObject : public CObject
{
public:
	// 定数を設定
	struct Config {
		static constexpr float CoreSize = 10.0f;	// マウスとの当たり判定時に使う情報
	};

	// コンストラクタ・デストラクタ
	CMapObject();
	~CMapObject();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void UpdateRB(void);
	void Draw(void);
	void DrawObject(void);
	void DrawShape(void);
	void CastShadow(void);
	void DrawShadow(void);
	void DrawOutline(void);
	void CalcSize(void);

	// 高さをメッシュフィールドに合わせる
	void UpdateHeight(void);

	// セッター
	void SetPos(D3DXVECTOR3 Pos);
	void SetRot(D3DXVECTOR3 Rot);
	void SetScale(D3DXVECTOR3 Scale);
	void SetRotFromQuad(btQuaternion Quad);
	void AddRot(D3DXVECTOR3 Vec, float Scaler);
	void SetQuadValue(btVector3 Value) { m_QuadValue = D3DXVECTOR3(Value); }
	void SetModelPath(std::string Path) { m_ModelPath = Path; };
	void SetStatic(const bool Static);
	void SetMass(const float Mass);
	btQuaternion SetQuad(D3DXQUATERNION Set);
	D3DXQUATERNION SetQuad(btQuaternion Set);

	// ゲッター
	bool GetStatic(void) const { return m_IsStatic; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; };
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetScale(void) { return m_Scale; }
	D3DXQUATERNION GetQuad(void) { return m_Quad; }
	float GetMass(void) { return m_Mass; }
	btVector3 GetQuadValue(void) { return { m_QuadValue.x,m_QuadValue.y,m_QuadValue.z }; }
	std::string GetModelPath(void) { return m_ModelPath; };

	// Mouseとの当たり判定
	bool CollisionMouse(void);
	bool CollisionMousetoMesh(float* Distance);
	bool CollisionRaytoMesh(D3DXVECTOR3 Origin, D3DXVECTOR3 Dir, float* Distance);

	// 生成
	static CMapObject* Create(D3DXVECTOR3 Pos,D3DXVECTOR3 Rot, std::string Path);
private:
	// メンバ変数
	bool m_IsStatic;				// 静的か動的か
	float m_Mass;
	D3DXVECTOR3 m_Size;
	D3DXVECTOR3 m_RBOffset;
	D3DXVECTOR3 m_QuadValue;
	D3DXQUATERNION m_Quad;
	D3DXVECTOR3 m_Pos;				// 位置
	D3DXVECTOR3 m_Scale = { 1.0f,1.0f,1.0f };			// 拡大率
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	std::unique_ptr<btCollisionShape> m_CollisionShape;
	std::unique_ptr<btRigidBody> m_RigitBody;		// リジットボディー
	std::unique_ptr<CEmitter> m_Sound;		// リジットボディー
	std::string m_ModelPath;		// モデルへのパス
};
#endif // !_MAPOBJECT_H_