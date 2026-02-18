#include <gtest/gtest.h>
#include "engine/ecs/ComponentPool.h"

TEST(ComponentPoolTest, AddAndGet)
{
    ComponentPool<int> pool;
    pool.add(1, 42);

    EXPECT_EQ(pool.get(1), 42);
}

TEST(ComponentPoolTest, HasReturnsTrueAfterAdd)
{
    ComponentPool<int> pool;
    pool.add(0, 10);

    EXPECT_TRUE(pool.has(0));
}

TEST(ComponentPoolTest, HasReturnsFalseForMissing)
{
    ComponentPool<int> pool;

    EXPECT_FALSE(pool.has(0));
    EXPECT_FALSE(pool.has(999));
}

TEST(ComponentPoolTest, AddMultipleEntities)
{
    ComponentPool<float> pool;
    pool.add(0, 1.0f);
    pool.add(1, 2.0f);
    pool.add(2, 3.0f);

    EXPECT_FLOAT_EQ(pool.get(0), 1.0f);
    EXPECT_FLOAT_EQ(pool.get(1), 2.0f);
    EXPECT_FLOAT_EQ(pool.get(2), 3.0f);
}

TEST(ComponentPoolTest, RemoveEntity)
{
    ComponentPool<int> pool;
    pool.add(0, 10);
    pool.add(1, 20);

    EXPECT_TRUE(pool.remove(0));
    EXPECT_FALSE(pool.has(0));
    EXPECT_TRUE(pool.has(1));
    EXPECT_EQ(pool.get(1), 20);
}

TEST(ComponentPoolTest, RemoveReturnsFalseForMissing)
{
    ComponentPool<int> pool;

    EXPECT_FALSE(pool.remove(0));
}

TEST(ComponentPoolTest, RemoveLastEntity)
{
    ComponentPool<int> pool;
    pool.add(5, 50);

    EXPECT_TRUE(pool.remove(5));
    EXPECT_FALSE(pool.has(5));
}

TEST(ComponentPoolTest, RemoveMiddleSwapsWithLast)
{
    ComponentPool<int> pool;
    pool.add(0, 100);
    pool.add(1, 200);
    pool.add(2, 300);

    // Remove entity 0 — entity 2's data should move to index 0
    pool.remove(0);

    EXPECT_FALSE(pool.has(0));
    EXPECT_TRUE(pool.has(1));
    EXPECT_TRUE(pool.has(2));
    EXPECT_EQ(pool.get(1), 200);
    EXPECT_EQ(pool.get(2), 300);
}

TEST(ComponentPoolTest, GetReturnsModifiableReference)
{
    ComponentPool<int> pool;
    pool.add(0, 10);

    pool.get(0) = 99;
    EXPECT_EQ(pool.get(0), 99);
}

TEST(ComponentPoolTest, ConstGetReturnsCorrectValue)
{
    ComponentPool<int> pool;
    pool.add(0, 42);

    const auto &constPool = pool;
    EXPECT_EQ(constPool.get(0), 42);
}

TEST(ComponentPoolTest, DenseToEntityTracksCorrectly)
{
    ComponentPool<int> pool;
    pool.add(5, 50);
    pool.add(10, 100);

    const auto &entities = pool.getDenseToEntity();
    ASSERT_EQ(entities.size(), 2u);
    EXPECT_EQ(entities[0], 5u);
    EXPECT_EQ(entities[1], 10u);
}

TEST(ComponentPoolTest, DenseToEntityUpdatesAfterRemove)
{
    ComponentPool<int> pool;
    pool.add(0, 10);
    pool.add(1, 20);
    pool.add(2, 30);

    pool.remove(0);

    const auto &entities = pool.getDenseToEntity();
    EXPECT_EQ(entities.size(), 2u);
}

TEST(ComponentPoolTest, RangeBasedForLoop)
{
    ComponentPool<int> pool;
    pool.add(3, 30);
    pool.add(7, 70);
    pool.add(11, 110);

    int count = 0;
    for (auto entityId : pool)
    {
        EXPECT_TRUE(pool.has(entityId));
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST(ComponentPoolTest, NonContiguousEntityIDs)
{
    ComponentPool<int> pool;
    pool.add(0, 1);
    pool.add(100, 2);
    pool.add(50, 3);

    EXPECT_TRUE(pool.has(0));
    EXPECT_TRUE(pool.has(100));
    EXPECT_TRUE(pool.has(50));
    EXPECT_FALSE(pool.has(1));

    EXPECT_EQ(pool.get(0), 1);
    EXPECT_EQ(pool.get(100), 2);
    EXPECT_EQ(pool.get(50), 3);
}

TEST(ComponentPoolTest, AddAfterRemove)
{
    ComponentPool<int> pool;
    pool.add(0, 10);
    pool.remove(0);
    pool.add(0, 99);

    EXPECT_TRUE(pool.has(0));
    EXPECT_EQ(pool.get(0), 99);
}

TEST(ComponentPoolTest, StructComponent)
{
    struct Vec2
    {
        float x, y;
    };

    ComponentPool<Vec2> pool;
    pool.add(0, {1.0f, 2.0f});

    EXPECT_FLOAT_EQ(pool.get(0).x, 1.0f);
    EXPECT_FLOAT_EQ(pool.get(0).y, 2.0f);

    pool.get(0).x = 5.0f;
    EXPECT_FLOAT_EQ(pool.get(0).x, 5.0f);
}
