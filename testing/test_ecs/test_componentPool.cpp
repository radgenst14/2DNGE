#include <gtest/gtest.h>
#include "engine/ecs/ComponentPool.h"

TEST(ComponentPoolTest, AddAndGetComponent)
{
    ComponentPool<int> pool;
    EntityID entityId = 1;
    int componentValue = 42;

    pool.add(entityId, componentValue);
    int &retrievedComponent = pool.get(entityId);

    EXPECT_EQ(retrievedComponent, componentValue);
}