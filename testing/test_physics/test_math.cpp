#include <gtest/gtest.h>

#include "engine/physics/Math.h"

// =============================================================================
// resolveAABBCollision
// =============================================================================

TEST(MathResolveAABB, OverlappingBoxesReturnsCollision)
{
    Math::AABB a{{-1.0f, -1.0f}, {1.0f, 1.0f}};
    Math::AABB b{{0.0f, -1.0f}, {2.0f, 1.0f}};

    auto result = Math::resolveAABBCollision(a, b);
    EXPECT_TRUE(result.isColliding);
    EXPECT_GT(result.penetration, 0.0f);
}

TEST(MathResolveAABB, SeparatedBoxesNoCollision)
{
    Math::AABB a{{-1.0f, -1.0f}, {1.0f, 1.0f}};
    Math::AABB b{{5.0f, -1.0f}, {7.0f, 1.0f}};

    auto result = Math::resolveAABBCollision(a, b);
    EXPECT_FALSE(result.isColliding);
}

TEST(MathResolveAABB, EdgeTouchingNoCollision)
{
    // Overlap is exactly 0 on X axis -> no collision (uses <= 0 check)
    Math::AABB a{{-1.0f, -1.0f}, {1.0f, 1.0f}};
    Math::AABB b{{1.0f, -1.0f}, {3.0f, 1.0f}};

    auto result = Math::resolveAABBCollision(a, b);
    EXPECT_FALSE(result.isColliding);
}

TEST(MathResolveAABB, NormalPointsAlongLeastPenetrationAxis)
{
    // A: [-1,-1] to [1,1], B: [0.5,-1] to [2.5,1]
    // X overlap: min(1-0.5, 2.5-(-1)) = min(0.5, 3.5) = 0.5
    // Y overlap: min(1-(-1), 1-(-1)) = min(2, 2) = 2.0
    // Least penetration is X (0.5), normal should be along X
    Math::AABB a{{-1.0f, -1.0f}, {1.0f, 1.0f}};
    Math::AABB b{{0.5f, -1.0f}, {2.5f, 1.0f}};

    auto result = Math::resolveAABBCollision(a, b);
    EXPECT_TRUE(result.isColliding);
    EXPECT_FLOAT_EQ(result.penetration, 0.5f);
    // Normal pushes A away from B: A is to the left, so normal is -X
    EXPECT_FLOAT_EQ(result.normal.x, -1.0f);
    EXPECT_FLOAT_EQ(result.normal.y, 0.0f);
}

TEST(MathResolveAABB, NormalPointsPositiveWhenAIsToRight)
{
    // A: [0.5,-1] to [2.5,1], B: [-1,-1] to [1,1]
    // overlapX1 = A.max.x - B.min.x = 2.5-(-1) = 3.5
    // overlapX2 = B.max.x - A.min.x = 1-0.5 = 0.5
    // overlapX = 0.5, signX = +1 (overlapX2 is smaller, push A right, away from B)
    Math::AABB a{{0.5f, -1.0f}, {2.5f, 1.0f}};
    Math::AABB b{{-1.0f, -1.0f}, {1.0f, 1.0f}};

    auto result = Math::resolveAABBCollision(a, b);
    EXPECT_TRUE(result.isColliding);
    EXPECT_FLOAT_EQ(result.penetration, 0.5f);
    EXPECT_FLOAT_EQ(result.normal.x, 1.0f);
    EXPECT_FLOAT_EQ(result.normal.y, 0.0f);
}

TEST(MathResolveAABB, FullyContainedBox)
{
    Math::AABB big{{-5.0f, -5.0f}, {5.0f, 5.0f}};
    Math::AABB small{{-1.0f, -1.0f}, {1.0f, 1.0f}};

    auto result = Math::resolveAABBCollision(big, small);
    EXPECT_TRUE(result.isColliding);
    EXPECT_GT(result.penetration, 0.0f);
}

TEST(MathResolveAABB, VerticalOverlapPrimary)
{
    // A: [-1,-1] to [1,1], B: [-1,0.5] to [1,2.5]
    // X overlap: min(1-(-1), 1-(-1)) = 2.0
    // Y overlap: min(1-0.5, 2.5-(-1)) = min(0.5, 3.5) = 0.5
    // overlapY1 = A.max.y - B.min.y = 1-0.5 = 0.5 (smaller)
    // signY = -1.0 (push A upward, away from B which is below)
    Math::AABB a{{-1.0f, -1.0f}, {1.0f, 1.0f}};
    Math::AABB b{{-1.0f, 0.5f}, {1.0f, 2.5f}};

    auto result = Math::resolveAABBCollision(a, b);
    EXPECT_TRUE(result.isColliding);
    EXPECT_FLOAT_EQ(result.penetration, 0.5f);
    EXPECT_FLOAT_EQ(result.normal.x, 0.0f);
    EXPECT_FLOAT_EQ(result.normal.y, -1.0f);
}

// =============================================================================
// resolveCircleCollision
// =============================================================================

TEST(MathResolveCircle, OverlappingCircles)
{
    auto result = Math::resolveCircleCollision({0.0f, 0.0f}, 1.0f, {1.0f, 0.0f}, 1.0f);
    EXPECT_TRUE(result.isColliding);
    EXPECT_NEAR(result.penetration, 1.0f, 0.001f);
    // Normal should point from B to A (i.e., -X direction since A is to left)
    EXPECT_NEAR(result.normal.x, -1.0f, 0.001f);
    EXPECT_NEAR(result.normal.y, 0.0f, 0.001f);
}

TEST(MathResolveCircle, SeparatedCircles)
{
    auto result = Math::resolveCircleCollision({0.0f, 0.0f}, 1.0f, {5.0f, 0.0f}, 1.0f);
    EXPECT_FALSE(result.isColliding);
}

TEST(MathResolveCircle, ExactlyTouchingNoCollision)
{
    // Distance = 2.0, sum of radii = 2.0, uses >= so no collision
    auto result = Math::resolveCircleCollision({0.0f, 0.0f}, 1.0f, {2.0f, 0.0f}, 1.0f);
    EXPECT_FALSE(result.isColliding);
}

TEST(MathResolveCircle, ConcentricCirclesArbitraryNormal)
{
    auto result = Math::resolveCircleCollision({0.0f, 0.0f}, 2.0f, {0.0f, 0.0f}, 3.0f);
    EXPECT_TRUE(result.isColliding);
    EXPECT_FLOAT_EQ(result.penetration, 5.0f);
    // Arbitrary axis chosen for concentric
    EXPECT_FLOAT_EQ(result.normal.x, 1.0f);
    EXPECT_FLOAT_EQ(result.normal.y, 0.0f);
}

TEST(MathResolveCircle, DiagonalOverlap)
{
    // Centers at (0,0) and (1,1), distance = sqrt(2) ~ 1.414
    // Radii sum = 2.0, penetration = 2.0 - 1.414 ~ 0.586
    auto result = Math::resolveCircleCollision({0.0f, 0.0f}, 1.0f, {1.0f, 1.0f}, 1.0f);
    EXPECT_TRUE(result.isColliding);
    EXPECT_NEAR(result.penetration, 2.0f - std::sqrt(2.0f), 0.001f);
    // Normal from B to A = (0-1, 0-1) normalized = (-1/sqrt2, -1/sqrt2)
    float invSqrt2 = 1.0f / std::sqrt(2.0f);
    EXPECT_NEAR(result.normal.x, -invSqrt2, 0.001f);
    EXPECT_NEAR(result.normal.y, -invSqrt2, 0.001f);
}

TEST(MathResolveCircle, NormalIsNormalized)
{
    auto result = Math::resolveCircleCollision({0.0f, 0.0f}, 2.0f, {1.5f, 1.5f}, 2.0f);
    EXPECT_TRUE(result.isColliding);
    float length = std::sqrt(result.normal.x * result.normal.x + result.normal.y * result.normal.y);
    EXPECT_NEAR(length, 1.0f, 0.001f);
}

// =============================================================================
// resolveAABBCircleCollision
// =============================================================================

TEST(MathResolveAABBCircle, CircleOutsideOverlapping)
{
    // AABB [-1,-1] to [1,1], circle at (1.5, 0) radius 1.0
    // Closest point on AABB = (1, 0), distance = 0.5 < 1.0
    auto result = Math::resolveAABBCircleCollision({{-1.0f, -1.0f}, {1.0f, 1.0f}}, {1.5f, 0.0f}, 1.0f);
    EXPECT_TRUE(result.isColliding);
    EXPECT_NEAR(result.penetration, 0.5f, 0.001f);
    EXPECT_NEAR(result.normal.x, 1.0f, 0.001f);
    EXPECT_NEAR(result.normal.y, 0.0f, 0.001f);
}

TEST(MathResolveAABBCircle, CircleSeparated)
{
    auto result = Math::resolveAABBCircleCollision({{-1.0f, -1.0f}, {1.0f, 1.0f}}, {5.0f, 0.0f}, 1.0f);
    EXPECT_FALSE(result.isColliding);
}

TEST(MathResolveAABBCircle, CircleCenterInsideAABB)
{
    // Circle center at (0,0) inside AABB [-2,-2] to [2,2], radius 0.5
    // Nearest edge distances: right=2, left=2, top=2, bottom=2
    // All equal, should pick right (first checked)
    auto result = Math::resolveAABBCircleCollision({{-2.0f, -2.0f}, {2.0f, 2.0f}}, {0.0f, 0.0f}, 0.5f);
    EXPECT_TRUE(result.isColliding);
    EXPECT_FLOAT_EQ(result.penetration, 2.5f); // radius + minDist = 0.5 + 2.0
}

TEST(MathResolveAABBCircle, CircleAtCornerNoCollision)
{
    // AABB [-1,-1] to [1,1], circle at (2,2) radius 1.0
    // Closest point = (1,1), distance = sqrt(2) ~ 1.414 > 1.0
    auto result = Math::resolveAABBCircleCollision({{-1.0f, -1.0f}, {1.0f, 1.0f}}, {2.0f, 2.0f}, 1.0f);
    EXPECT_FALSE(result.isColliding);
}

TEST(MathResolveAABBCircle, CircleAtCornerColliding)
{
    // AABB [-1,-1] to [1,1], circle at (1.5, 1.5) radius 1.0
    // Closest point = (1,1), distance = sqrt(0.5) ~ 0.707 < 1.0
    auto result = Math::resolveAABBCircleCollision({{-1.0f, -1.0f}, {1.0f, 1.0f}}, {1.5f, 1.5f}, 1.0f);
    EXPECT_TRUE(result.isColliding);
    EXPECT_NEAR(result.penetration, 1.0f - std::sqrt(0.5f), 0.001f);
}

TEST(MathResolveAABBCircle, CircleCenterInsideNearLeftEdge)
{
    // AABB [-2,-2] to [2,2], circle center at (-1.5, 0), radius 0.5
    // Distance to edges: right=3.5, left=0.5, top=2, bottom=2
    // Nearest is left (0.5)
    auto result = Math::resolveAABBCircleCollision({{-2.0f, -2.0f}, {2.0f, 2.0f}}, {-1.5f, 0.0f}, 0.5f);
    EXPECT_TRUE(result.isColliding);
    EXPECT_FLOAT_EQ(result.penetration, 1.0f); // 0.5 + 0.5
    EXPECT_FLOAT_EQ(result.normal.x, -1.0f);
    EXPECT_FLOAT_EQ(result.normal.y, 0.0f);
}
