#include <gtest/gtest.h>
#include "engine/ecs/EntityManager.h"

TEST(EntityManagerTest, CreateEntity)
{
    EntityManager entityManager;
    EntityID e1 = entityManager.createEntity();
    EntityID e2 = entityManager.createEntity();

    EXPECT_NE(e1, e2);
}
