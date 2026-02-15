#include <gtest/gtest.h>
#include "engine/ecs/Entity.h"

TEST(EntityTest, AddAndGetComponent)
{
    Entity entity(1);
    struct Position
    {
        float x, y;
    };
    Position pos{10.0f, 20.0f};
    entity.addComponent(pos);

    const Position &retrievedPos = entity.getComponent<Position>();
    EXPECT_EQ(retrievedPos.x, 10.0f);
    EXPECT_EQ(retrievedPos.y, 20.0f);
}