#include <gtest/gtest.h>

#include "engine/core/ecs/components/Transform.h"
#include "engine/core/ecs/components/RigidBody.h"
#include "engine/core/ecs/components/Collider.h"
#include "engine/physics/CollisionHandler.h"
#include "engine/core/ecs/EntityManager.h"

class CollisionHandlerTest : public ::testing::Test
{
protected:
    EntityManager em;
    std::unique_ptr<CollisionHandler> ch;

    void SetUp() override
    {
        ch = std::make_unique<CollisionHandler>(&em);
    }

    EntityID createEntity(glm::vec2 position, glm::vec2 velocity, float mass = 1.0f, float restitution = 0.5f)
    {
        EntityID e = em.createEntity();
        ECS::Transform t;
        t.position = position;
        em.addComponent<ECS::Transform>(e, t);

        ECS::RigidBody rb;
        rb.velocity = velocity;
        rb.mass = mass;
        rb.restitution = restitution;
        em.addComponent<ECS::RigidBody>(e, rb);

        return e;
    }

    EntityID createStaticEntity(glm::vec2 position)
    {
        EntityID e = em.createEntity();
        ECS::Transform t;
        t.position = position;
        em.addComponent<ECS::Transform>(e, t);
        return e;
    }
};

// =============================================================================
// Positional correction
// =============================================================================

TEST_F(CollisionHandlerTest, EqualMassPositionalCorrection)
{
    // A at left, B at right, normal pushes A left (-X)
    EntityID a = createEntity({0.0f, 0.0f}, {0.0f, 0.0f});
    EntityID b = createEntity({1.0f, 0.0f}, {0.0f, 0.0f});

    Math::CollisionResult result;
    result.isColliding = true;
    result.normal = {-1.0f, 0.0f}; // pushes A left (away from B)
    result.penetration = 0.5f;

    ch->resolveCollision(a, b, result);

    auto &tA = em.getComponent<ECS::Transform>(a);
    auto &tB = em.getComponent<ECS::Transform>(b);

    // Equal mass -> each moves half the penetration
    EXPECT_FLOAT_EQ(tA.position.x, -0.25f);  // pushed -0.25 along normal
    EXPECT_FLOAT_EQ(tB.position.x, 1.25f);   // pushed +0.25 (opposite of normal)
}

TEST_F(CollisionHandlerTest, UnequalMassPositionalCorrection)
{
    // A (light, mass=1) at left, B (heavy, mass=3) at right
    EntityID a = createEntity({0.0f, 0.0f}, {0.0f, 0.0f}, 1.0f);
    EntityID b = createEntity({1.0f, 0.0f}, {0.0f, 0.0f}, 3.0f);

    Math::CollisionResult result;
    result.isColliding = true;
    result.normal = {-1.0f, 0.0f};
    result.penetration = 1.0f;

    ch->resolveCollision(a, b, result);

    auto &tA = em.getComponent<ECS::Transform>(a);
    auto &tB = em.getComponent<ECS::Transform>(b);

    // inverseMassA = 1.0, inverseMassB = 1/3, total = 4/3
    // A moves: normal * (pen * invMassA / totalInvMass) = (-1,0) * (1.0 * 1.0 / (4/3)) = (-0.75, 0)
    // B moves: -normal * (pen * invMassB / totalInvMass) = (1,0) * (1.0 * (1/3) / (4/3)) = (0.25, 0)
    EXPECT_NEAR(tA.position.x, -0.75f, 0.001f);
    EXPECT_NEAR(tB.position.x, 1.25f, 0.001f);
}

TEST_F(CollisionHandlerTest, VerticalCorrection)
{
    // A above, B below. Normal pushes A up (-Y)
    EntityID a = createEntity({0.0f, 0.0f}, {0.0f, 0.0f});
    EntityID b = createEntity({0.0f, 1.0f}, {0.0f, 0.0f});

    Math::CollisionResult result;
    result.isColliding = true;
    result.normal = {0.0f, -1.0f}; // pushes A upward (away from B)
    result.penetration = 0.4f;

    ch->resolveCollision(a, b, result);

    auto &tA = em.getComponent<ECS::Transform>(a);
    auto &tB = em.getComponent<ECS::Transform>(b);

    EXPECT_FLOAT_EQ(tA.position.y, -0.2f);
    EXPECT_FLOAT_EQ(tB.position.y, 1.2f);
    // X should be unchanged
    EXPECT_FLOAT_EQ(tA.position.x, 0.0f);
    EXPECT_FLOAT_EQ(tB.position.x, 0.0f);
}

// =============================================================================
// Velocity impulse response
// =============================================================================

TEST_F(CollisionHandlerTest, HeadOnCollisionReversesVelocities)
{
    // Two equal mass objects approaching each other
    // A at left moving right, B at right moving left
    // Normal pushes A away from B = (-1, 0)
    EntityID a = createEntity({0.0f, 0.0f}, {1.0f, 0.0f}, 1.0f, 1.0f);  // perfect bounce
    EntityID b = createEntity({1.0f, 0.0f}, {-1.0f, 0.0f}, 1.0f, 1.0f); // perfect bounce

    Math::CollisionResult result;
    result.isColliding = true;
    result.normal = {-1.0f, 0.0f}; // pushes A left (away from B)
    result.penetration = 0.0f;

    ch->resolveCollision(a, b, result);

    auto &rbA = em.getComponent<ECS::RigidBody>(a);
    auto &rbB = em.getComponent<ECS::RigidBody>(b);

    // With restitution=1.0, they should swap velocities
    EXPECT_NEAR(rbA.velocity.x, -1.0f, 0.001f);
    EXPECT_NEAR(rbB.velocity.x, 1.0f, 0.001f);
}

TEST_F(CollisionHandlerTest, ZeroRestitutionNoBounciness)
{
    EntityID a = createEntity({0.0f, 0.0f}, {1.0f, 0.0f}, 1.0f, 0.0f);
    EntityID b = createEntity({1.0f, 0.0f}, {-1.0f, 0.0f}, 1.0f, 0.0f);

    Math::CollisionResult result;
    result.isColliding = true;
    result.normal = {-1.0f, 0.0f};
    result.penetration = 0.0f;

    ch->resolveCollision(a, b, result);

    auto &rbA = em.getComponent<ECS::RigidBody>(a);
    auto &rbB = em.getComponent<ECS::RigidBody>(b);

    // With restitution=0.0, both should end up at 0 velocity (perfectly inelastic)
    EXPECT_NEAR(rbA.velocity.x, 0.0f, 0.001f);
    EXPECT_NEAR(rbB.velocity.x, 0.0f, 0.001f);
}

TEST_F(CollisionHandlerTest, MinRestitutionUsed)
{
    // One has restitution 0.0, other has 1.0 -> min = 0.0 is used
    EntityID a = createEntity({0.0f, 0.0f}, {1.0f, 0.0f}, 1.0f, 0.0f);
    EntityID b = createEntity({1.0f, 0.0f}, {-1.0f, 0.0f}, 1.0f, 1.0f);

    Math::CollisionResult result;
    result.isColliding = true;
    result.normal = {-1.0f, 0.0f};
    result.penetration = 0.0f;

    ch->resolveCollision(a, b, result);

    auto &rbA = em.getComponent<ECS::RigidBody>(a);
    auto &rbB = em.getComponent<ECS::RigidBody>(b);

    // min restitution = 0.0, same as perfectly inelastic
    EXPECT_NEAR(rbA.velocity.x, 0.0f, 0.001f);
    EXPECT_NEAR(rbB.velocity.x, 0.0f, 0.001f);
}

TEST_F(CollisionHandlerTest, AlreadySeparatingNoImpulse)
{
    // A moving left (away from B along normal), B moving right (away from A)
    // Normal = (-1, 0) pushes A left
    EntityID a = createEntity({0.0f, 0.0f}, {-2.0f, 0.0f}, 1.0f, 1.0f);
    EntityID b = createEntity({1.0f, 0.0f}, {1.0f, 0.0f}, 1.0f, 1.0f);

    Math::CollisionResult result;
    result.isColliding = true;
    result.normal = {-1.0f, 0.0f};
    result.penetration = 0.0f;

    float velABefore = -2.0f;
    float velBBefore = 1.0f;

    ch->resolveCollision(a, b, result);

    auto &rbA = em.getComponent<ECS::RigidBody>(a);
    auto &rbB = em.getComponent<ECS::RigidBody>(b);

    // relVel = (-2,0) - (1,0) = (-3,0), dot with (-1,0) = 3 > 0 -> separating
    // No impulse should be applied
    EXPECT_FLOAT_EQ(rbA.velocity.x, velABefore);
    EXPECT_FLOAT_EQ(rbB.velocity.x, velBBefore);
}

// =============================================================================
// Static entity (only one has RigidBody)
// =============================================================================

TEST_F(CollisionHandlerTest, OnlyAHasRigidBody)
{
    // A (dynamic) at left, B (static) at right. Normal pushes A left.
    EntityID a = createEntity({0.0f, 0.0f}, {1.0f, 0.0f});
    EntityID b = createStaticEntity({1.0f, 0.0f});

    Math::CollisionResult result;
    result.isColliding = true;
    result.normal = {-1.0f, 0.0f}; // push A left (away from B)
    result.penetration = 0.5f;

    ch->resolveCollision(a, b, result);

    auto &tA = em.getComponent<ECS::Transform>(a);
    auto &tB = em.getComponent<ECS::Transform>(b);

    // Only A has a rigidbody, so all positional correction goes to A
    // A moves: normal * (0.5 * 1.0 / 1.0) = (-0.5, 0)
    EXPECT_FLOAT_EQ(tA.position.x, -0.5f);
    // B doesn't move (inverseMass = 0)
    EXPECT_FLOAT_EQ(tB.position.x, 1.0f);

    // No velocity impulse (only one has rigidbody)
    auto &rbA = em.getComponent<ECS::RigidBody>(a);
    EXPECT_FLOAT_EQ(rbA.velocity.x, 1.0f); // unchanged
}

TEST_F(CollisionHandlerTest, OnlyBHasRigidBody)
{
    // A (static) at left, B (dynamic) at right. Normal pushes A left.
    EntityID a = createStaticEntity({0.0f, 0.0f});
    EntityID b = createEntity({1.0f, 0.0f}, {-1.0f, 0.0f});

    Math::CollisionResult result;
    result.isColliding = true;
    result.normal = {-1.0f, 0.0f};
    result.penetration = 0.5f;

    ch->resolveCollision(a, b, result);

    auto &tA = em.getComponent<ECS::Transform>(a);
    auto &tB = em.getComponent<ECS::Transform>(b);

    // Only B has rigidbody, all correction goes to B (opposite of normal)
    EXPECT_FLOAT_EQ(tA.position.x, 0.0f);
    EXPECT_FLOAT_EQ(tB.position.x, 1.5f); // pushed right by 0.5

    // No velocity impulse
    auto &rbB = em.getComponent<ECS::RigidBody>(b);
    EXPECT_FLOAT_EQ(rbB.velocity.x, -1.0f); // unchanged
}

TEST_F(CollisionHandlerTest, NeitherHasRigidBodyNoOp)
{
    EntityID a = createStaticEntity({0.0f, 0.0f});
    EntityID b = createStaticEntity({1.0f, 0.0f});

    Math::CollisionResult result;
    result.isColliding = true;
    result.normal = {-1.0f, 0.0f};
    result.penetration = 0.5f;

    ch->resolveCollision(a, b, result);

    auto &tA = em.getComponent<ECS::Transform>(a);
    auto &tB = em.getComponent<ECS::Transform>(b);

    EXPECT_FLOAT_EQ(tA.position.x, 0.0f);
    EXPECT_FLOAT_EQ(tB.position.x, 1.0f);
}

// =============================================================================
// Restitution
// =============================================================================

TEST_F(CollisionHandlerTest, DefaultRestitutionIsHalf)
{
    EntityID a = createEntity({0.0f, 0.0f}, {1.0f, 0.0f});
    EntityID b = createEntity({1.0f, 0.0f}, {-1.0f, 0.0f});

    auto &rbA = em.getComponent<ECS::RigidBody>(a);
    auto &rbB = em.getComponent<ECS::RigidBody>(b);
    EXPECT_FLOAT_EQ(rbA.restitution, 0.5f);
    EXPECT_FLOAT_EQ(rbB.restitution, 0.5f);
}

TEST_F(CollisionHandlerTest, HalfRestitutionHeadOn)
{
    // A at left moving right, B at right stationary
    // Normal pushes A left (away from B) = (-1, 0)
    EntityID a = createEntity({0.0f, 0.0f}, {2.0f, 0.0f}, 1.0f, 0.5f);
    EntityID b = createEntity({1.0f, 0.0f}, {0.0f, 0.0f}, 1.0f, 0.5f);

    Math::CollisionResult result;
    result.isColliding = true;
    result.normal = {-1.0f, 0.0f};
    result.penetration = 0.0f;

    ch->resolveCollision(a, b, result);

    auto &rbA = em.getComponent<ECS::RigidBody>(a);
    auto &rbB = em.getComponent<ECS::RigidBody>(b);

    // relVel = (2,0) - (0,0) = (2,0)
    // normalVel = dot((2,0), (-1,0)) = -2.0 < 0 -> approaching
    // restitution = 0.5
    // impulseMagnitude = -(1 + 0.5) * (-2.0) / (1.0 + 1.0) = 3.0 / 2.0 = 1.5
    // A.vel += (-1,0) * (1.5 * 1.0) = (-1.5, 0) -> A.vel = (0.5, 0)
    // B.vel -= (-1,0) * (1.5 * 1.0) = (1.5, 0) -> B.vel = (1.5, 0)
    EXPECT_NEAR(rbA.velocity.x, 0.5f, 0.001f);
    EXPECT_NEAR(rbB.velocity.x, 1.5f, 0.001f);
}

// =============================================================================
// Unequal mass impulse
// =============================================================================

TEST_F(CollisionHandlerTest, HeavyHitsLightTransfersMoreMomentum)
{
    // Heavy object (mass=10) at left moving right, light object (mass=1) at right stationary
    // Normal pushes heavy left (away from light) = (-1, 0)
    EntityID heavy = createEntity({0.0f, 0.0f}, {1.0f, 0.0f}, 10.0f, 1.0f);
    EntityID light = createEntity({1.0f, 0.0f}, {0.0f, 0.0f}, 1.0f, 1.0f);

    Math::CollisionResult result;
    result.isColliding = true;
    result.normal = {-1.0f, 0.0f};
    result.penetration = 0.0f;

    ch->resolveCollision(heavy, light, result);

    auto &rbHeavy = em.getComponent<ECS::RigidBody>(heavy);
    auto &rbLight = em.getComponent<ECS::RigidBody>(light);

    // Heavy object should barely slow down, light should speed up significantly rightward
    EXPECT_GT(rbHeavy.velocity.x, 0.5f);     // still moving right
    EXPECT_GT(rbLight.velocity.x, 1.0f);     // launched rightward (away from heavy)
}
