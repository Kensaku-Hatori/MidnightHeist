#pragma once

struct RigidBody
{
    btRigidBody* body = nullptr;
    btCollisionShape* shape = nullptr;
    float mass = 1.0f;
    void* UserPointer = nullptr;
    bool enableCollision = true;
    bool canMove = true;
};