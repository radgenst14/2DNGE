#include <gtest/gtest.h>
#include <SDL.h>
#include "../../engine/core/SceneManager.h"

// Concrete test scene implementation
class TestScene : public IScene
{
public:
    bool initCalled = false;
    bool cleanupCalled = false;
    int updateCount = 0;

    void init() override { initCalled = true; }
    void input(SDL_Event &) override {}
    void update(float) override { updateCount++; }
    void render() override {}
    void cleanup() override { cleanupCalled = true; }
};

TEST(SceneManagerTest, InitiallyNoActiveScene)
{
    SceneManager sm;
    EXPECT_EQ(sm.getActiveScene(), nullptr);
}

TEST(SceneManagerTest, SetSceneCallsInit)
{
    SceneManager sm;
    auto scene = std::make_unique<TestScene>();
    auto *ptr = scene.get();

    sm.setScene(std::move(scene));

    EXPECT_TRUE(ptr->initCalled);
}

TEST(SceneManagerTest, GetActiveSceneReturnsCurrentScene)
{
    SceneManager sm;
    auto scene = std::make_unique<TestScene>();
    auto *ptr = scene.get();

    sm.setScene(std::move(scene));

    EXPECT_EQ(sm.getActiveScene(), ptr);
}

TEST(SceneManagerTest, SetSceneCallsCleanupOnPrevious)
{
    SceneManager sm;

    auto scene1 = std::make_unique<TestScene>();
    auto *ptr1 = scene1.get();
    sm.setScene(std::move(scene1));

    auto scene2 = std::make_unique<TestScene>();
    sm.setScene(std::move(scene2));

    EXPECT_TRUE(ptr1->cleanupCalled);
}

TEST(SceneManagerTest, SwitchSceneUpdatesActive)
{
    SceneManager sm;

    sm.setScene(std::make_unique<TestScene>());
    auto scene2 = std::make_unique<TestScene>();
    auto *ptr2 = scene2.get();

    sm.setScene(std::move(scene2));

    EXPECT_EQ(sm.getActiveScene(), ptr2);
}
