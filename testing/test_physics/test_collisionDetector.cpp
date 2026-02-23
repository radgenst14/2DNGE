#include <gtest/gtest.h>

#include "engine/core/ecs/components/Collider.h"
#include "engine/core/ecs/components/Transform.h"
#include "engine/physics/CollisionDetector.h"
#include "engine/core/ecs/EntityManager.h"

class CollisionTest : public ::testing::Test
{
protected:
    EntityManager em;
    std::unique_ptr<CollisionDetector> cd;

    void SetUp() override
    {
        cd = std::make_unique<CollisionDetector>(&em);
    }

    EntityID createBoxEntity(glm::vec2 position, glm::vec2 size, glm::vec2 scale = {1.0f, 1.0f})
    {
        EntityID e = em.createEntity();
        ECS::Transform t;
        t.position = position;
        t.scale = scale;
        em.addComponent<ECS::Transform>(e, t);

        ECS::Collider c;
        c.type = ECS::ColliderType::Box;
        c.size = size;
        em.addComponent<ECS::Collider>(e, c);
        return e;
    }

    EntityID createCircleEntity(glm::vec2 position, float radius)
    {
        EntityID e = em.createEntity();
        ECS::Transform t;
        t.position = position;
        em.addComponent<ECS::Transform>(e, t);

        ECS::Collider c;
        c.type = ECS::ColliderType::Circle;
        c.radius = radius;
        em.addComponent<ECS::Collider>(e, c);
        return e;
    }
};

// ---- Box-Box ----

TEST_F(CollisionTest, BoxBoxOverlapping)
{
    EntityID a = createBoxEntity({0.0f, 0.0f}, {2.0f, 2.0f});
    EntityID b = createBoxEntity({0.0f, 0.0f}, {2.0f, 2.0f});

    EXPECT_TRUE(cd->checkCollision(a, b));
}

TEST_F(CollisionTest, BoxBoxSeparatedX)
{
    EntityID a = createBoxEntity({0.0f, 0.0f}, {2.0f, 2.0f});
    EntityID b = createBoxEntity({5.0f, 0.0f}, {2.0f, 2.0f});

    EXPECT_FALSE(cd->checkCollision(a, b));
}

TEST_F(CollisionTest, BoxBoxSeparatedY)
{
    EntityID a = createBoxEntity({0.0f, 0.0f}, {2.0f, 2.0f});
    EntityID b = createBoxEntity({0.0f, 5.0f}, {2.0f, 2.0f});

    EXPECT_FALSE(cd->checkCollision(a, b));
}

TEST_F(CollisionTest, BoxBoxEdgeTouching)
{
    // Box A: center(0,0), size(2,2) -> AABB [-1,-1] to [1,1]
    // Box B: center(2,0), size(2,2) -> AABB [1,-1] to [3,1]
    // Edge touching (min.x == max.x) -> no collision with strict < check
    EntityID a = createBoxEntity({0.0f, 0.0f}, {2.0f, 2.0f});
    EntityID b = createBoxEntity({2.0f, 0.0f}, {2.0f, 2.0f});

    EXPECT_FALSE(cd->checkCollision(a, b));
}

TEST_F(CollisionTest, BoxBoxPartialOverlap)
{
    // Box A: center(0,0), size(2,2) -> AABB [-1,-1] to [1,1]
    // Box B: center(1,1), size(2,2) -> AABB [0,0] to [2,2]
    EntityID a = createBoxEntity({0.0f, 0.0f}, {2.0f, 2.0f});
    EntityID b = createBoxEntity({1.0f, 1.0f}, {2.0f, 2.0f});

    EXPECT_TRUE(cd->checkCollision(a, b));
}

TEST_F(CollisionTest, BoxBoxDifferentSizes)
{
    // Small box inside big box
    EntityID big = createBoxEntity({0.0f, 0.0f}, {10.0f, 10.0f});
    EntityID small = createBoxEntity({0.0f, 0.0f}, {1.0f, 1.0f});

    EXPECT_TRUE(cd->checkCollision(big, small));
}

TEST_F(CollisionTest, BoxBoxWithScale)
{
    // Box at origin, size(1,1), scale(4,4) -> effective size (4,4) -> AABB [-2,-2] to [2,2]
    EntityID a = createBoxEntity({0.0f, 0.0f}, {1.0f, 1.0f}, {4.0f, 4.0f});
    // Box at (3,0), size(2,2), scale(1,1) -> AABB [2,-1] to [4,1]
    // a max.x = 2, b min.x = 2, so 2 > 2 is false -> no collision
    EntityID b = createBoxEntity({3.0f, 0.0f}, {2.0f, 2.0f});

    EXPECT_FALSE(cd->checkCollision(a, b));
}

// ---- Circle-Circle ----

TEST_F(CollisionTest, CircleCircleOverlapping)
{
    EntityID a = createCircleEntity({0.0f, 0.0f}, 1.0f);
    EntityID b = createCircleEntity({1.0f, 0.0f}, 1.0f);

    EXPECT_TRUE(cd->checkCollision(a, b));
}

TEST_F(CollisionTest, CircleCircleSeparated)
{
    EntityID a = createCircleEntity({0.0f, 0.0f}, 1.0f);
    EntityID b = createCircleEntity({5.0f, 0.0f}, 1.0f);

    EXPECT_FALSE(cd->checkCollision(a, b));
}

TEST_F(CollisionTest, CircleCircleExactlyTouching)
{
    // Distance = 2.0, sum of radii = 2.0
    // checkCircleCollision uses <, not <=, so exactly touching = no collision
    EntityID a = createCircleEntity({0.0f, 0.0f}, 1.0f);
    EntityID b = createCircleEntity({2.0f, 0.0f}, 1.0f);

    EXPECT_FALSE(cd->checkCollision(a, b));
}

TEST_F(CollisionTest, CircleCircleConcentric)
{
    EntityID a = createCircleEntity({0.0f, 0.0f}, 5.0f);
    EntityID b = createCircleEntity({0.0f, 0.0f}, 1.0f);

    EXPECT_TRUE(cd->checkCollision(a, b));
}

TEST_F(CollisionTest, CircleCircleDiagonal)
{
    // Distance = sqrt(1+1) ~ 1.414, sum of radii = 2.0
    EntityID a = createCircleEntity({0.0f, 0.0f}, 1.0f);
    EntityID b = createCircleEntity({1.0f, 1.0f}, 1.0f);

    EXPECT_TRUE(cd->checkCollision(a, b));
}

TEST_F(CollisionTest, CircleCircleDifferentRadii)
{
    // Distance = 3.0, sum = 0.5 + 2.0 = 2.5 < 3.0 -> no collision
    EntityID a = createCircleEntity({0.0f, 0.0f}, 0.5f);
    EntityID b = createCircleEntity({3.0f, 0.0f}, 2.0f);

    EXPECT_FALSE(cd->checkCollision(a, b));
}

// ---- Box-Circle ----

TEST_F(CollisionTest, BoxCircleOverlapping)
{
    EntityID box = createBoxEntity({0.0f, 0.0f}, {2.0f, 2.0f});
    EntityID circle = createCircleEntity({1.5f, 0.0f}, 1.0f);

    EXPECT_TRUE(cd->checkCollision(box, circle));
}

TEST_F(CollisionTest, BoxCircleSeparated)
{
    EntityID box = createBoxEntity({0.0f, 0.0f}, {2.0f, 2.0f});
    EntityID circle = createCircleEntity({5.0f, 0.0f}, 1.0f);

    EXPECT_FALSE(cd->checkCollision(box, circle));
}

TEST_F(CollisionTest, BoxCircleCircleInsideBox)
{
    EntityID box = createBoxEntity({0.0f, 0.0f}, {10.0f, 10.0f});
    EntityID circle = createCircleEntity({0.0f, 0.0f}, 0.5f);

    EXPECT_TRUE(cd->checkCollision(box, circle));
}

TEST_F(CollisionTest, BoxCircleCornerCase)
{
    // Box AABB: [-1,-1] to [1,1]
    // Circle at corner (2, 2) with radius 1.0
    // Closest point on AABB to circle center = (1,1)
    // Distance = sqrt(1+1) ~ 1.414 > 1.0 -> no collision
    EntityID box = createBoxEntity({0.0f, 0.0f}, {2.0f, 2.0f});
    EntityID circle = createCircleEntity({2.0f, 2.0f}, 1.0f);

    EXPECT_FALSE(cd->checkCollision(box, circle));
}

TEST_F(CollisionTest, BoxCircleReversedOrder)
{
    // Collision should be symmetric regardless of argument order
    EntityID box = createBoxEntity({0.0f, 0.0f}, {2.0f, 2.0f});
    EntityID circle = createCircleEntity({1.5f, 0.0f}, 1.0f);

    EXPECT_TRUE(cd->checkCollision(circle, box));
}

// ---- No collider ----

TEST_F(CollisionTest, NoColliderReturnsFalse)
{
    EntityID a = em.createEntity();
    em.addComponent<ECS::Transform>(a, ECS::Transform{});
    // No collider on entity a

    EntityID b = createBoxEntity({0.0f, 0.0f}, {2.0f, 2.0f});

    EXPECT_FALSE(cd->checkCollision(a, b));
}

TEST_F(CollisionTest, NeitherHasCollider)
{
    EntityID a = em.createEntity();
    em.addComponent<ECS::Transform>(a, ECS::Transform{});
    EntityID b = em.createEntity();
    em.addComponent<ECS::Transform>(b, ECS::Transform{});

    EXPECT_FALSE(cd->checkCollision(a, b));
}

// ---- Collider offset ----

TEST_F(CollisionTest, BoxBoxWithOffset)
{
    // Box A at (0,0), size(2,2), offset(0,0) -> AABB [-1,-1] to [1,1]
    EntityID a = createBoxEntity({0.0f, 0.0f}, {2.0f, 2.0f});

    // Box B at (3,0), size(2,2), offset(-2,0) -> center becomes (1,0), AABB [0,-1] to [2,1]
    EntityID b = em.createEntity();
    ECS::Transform tb;
    tb.position = {3.0f, 0.0f};
    em.addComponent<ECS::Transform>(b, tb);
    ECS::Collider cb;
    cb.type = ECS::ColliderType::Box;
    cb.size = {2.0f, 2.0f};
    cb.offset = {-2.0f, 0.0f};
    em.addComponent<ECS::Collider>(b, cb);

    EXPECT_TRUE(cd->checkCollision(a, b));
}
