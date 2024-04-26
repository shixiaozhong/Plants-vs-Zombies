#include "../src/include/CoreMinimal.h"
#include "gtest/gtest.h"

TEST(Vector2D, Distance) {
  Vector2D p1{3, 0};
  Vector2D p2{0, 4};
  EXPECT_EQ(5, Vector2D::Distance(p1, p2));
}

TEST(Vector2D, Rotate) {
  Vector2D p{2, 0};
  auto res = Vector2D::RotateVector(30, p);
  std::cout << res << "\n";
}