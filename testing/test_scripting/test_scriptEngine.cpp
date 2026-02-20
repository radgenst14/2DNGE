#include <gtest/gtest.h>
#include "../../engine/scripting/ScriptEngine.h"
#include <string>

static const std::string TEST_SCRIPT =
    std::string(PROJECT_SOURCE_DIR) + "/testing/test_scripting/test_script.py";

TEST(ScriptEngineTest, InterpreterInitializes)
{
    ScriptEngine se;
    EXPECT_TRUE(se.isInitialized());
}

TEST(ScriptEngineTest, ExecuteSimpleExpression)
{
    ScriptEngine se;
    auto result = se.execute("1 + 2");
    EXPECT_EQ(result.cast<int>(), 3);
}

TEST(ScriptEngineTest, LoadScriptFile)
{
    ScriptEngine se;
    bool loaded = se.loadScript(TEST_SCRIPT);
    EXPECT_TRUE(loaded);
}

TEST(ScriptEngineTest, CallFunctionNoArgs)
{
    ScriptEngine se;
    se.loadScript(TEST_SCRIPT);
    auto result = se.callFunction("greet");
    EXPECT_EQ(result.cast<std::string>(), "hello from python");
}

TEST(ScriptEngineTest, CallFunctionWithArgs)
{
    ScriptEngine se;
    se.loadScript(TEST_SCRIPT);
    auto result = se.callFunction("add", 3, 4);
    EXPECT_EQ(result.cast<int>(), 7);
}