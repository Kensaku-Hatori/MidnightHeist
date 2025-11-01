#include "pch.h"
#include "pysicssystem.h"

HRESULT CPysicsSystem::Init(void)
{
    // è’ìÀê›íË
    m_pConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
    m_pDispatcher = std::make_unique<btCollisionDispatcher>(m_pConfiguration.get());
    m_pBroadPhase = std::make_unique<btDbvtBroadphase>();
    m_pSolver = std::make_unique<btSequentialImpulseConstraintSolver>();

    // ï®óùê¢äEÇê∂ê¨
    m_pDynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
        m_pDispatcher.get(),
        m_pBroadPhase.get(),
        m_pSolver.get(),
        m_pConfiguration.get()
        );

    // é∏îs
    if (m_pDynamicsWorld) return E_FAIL;

    // èdóÕê›íË
    m_pDynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));

    return S_OK;
}

void CPysicsSystem::Update(void)
{
    if (m_pDynamicsWorld)
    {
        m_pDynamicsWorld->stepSimulation(btScalar(GetFPS()), 10, GetDeltaTime());
    }
}

void CPysicsSystem::Draw(void)
{
}

void CPysicsSystem::Uninit(void)
{
    m_RigidBodies.clear();
    m_pDynamicsWorld.reset();
    m_pSolver.reset();
    m_pBroadPhase.reset();
    m_pDispatcher.reset();
    m_pConfiguration.reset();
}

btRigidBody* CPysicsSystem::CreateRigidBody(float mass, const btVector3& pos, btCollisionShape* shape,void* Ptr, const bool IsCheckCollision, const bool IsMove)
{
    btVector3 inertia(0, 0, 0);
    if (mass != 0.0f)
    {
        shape->calculateLocalInertia(mass, inertia);
    }

    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
    btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, shape, inertia);
    auto RB = std::make_unique<btRigidBody>(info);

    if (IsCheckCollision == true)
    {
        RB->setUserPointer(Ptr);
    }
    if (IsMove == true)
    {
        RB->setLinearFactor(btVector3(1, 1, 1));
        RB->setActivationState(DISABLE_DEACTIVATION);
    }
    m_pDynamicsWorld->addRigidBody(RB.get());
    m_RigidBodies.push_back(std::move(RB));

    return m_RigidBodies.back().get();
}