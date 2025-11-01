#pragma once
class CPysicsSystem
{
public:
	CPysicsSystem() {}
	~CPysicsSystem() {}

	HRESULT Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);

	btRigidBody* CreateRigidBody(float mass, const btVector3& pos, btCollisionShape* shape, void* Ptr = nullptr,const bool IsCheckCollision = true,const bool IsMove = true);
private:
	// メンバ変数
	std::unique_ptr<btDiscreteDynamicsWorld> m_pDynamicsWorld;				// 物理世界
	std::unique_ptr<btDbvtBroadphase> m_pBroadPhase;					// 衝突判定のクラス
	std::unique_ptr<btDefaultCollisionConfiguration> m_pConfiguration;	// 衝突判定を実行するクラス
	std::unique_ptr<btSequentialImpulseConstraintSolver> m_pSolver;		// 制約ソルバー
	std::unique_ptr<btCollisionDispatcher> m_pDispatcher;				// 衝突判定を検出するクラス
	std::vector<std::unique_ptr<btRigidBody>>              m_RigidBodies;    // 登録された剛体たち
};