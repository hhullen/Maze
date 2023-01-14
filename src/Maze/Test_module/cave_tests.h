#include <gtest/gtest.h>

#include "../Model_module/cave/cave_gen.h"

using s21::CaveGen;

string kCaveSize10Path = "Maze/Test_module/test_10.cave";
string kCaveSize20Path = "Maze/Test_module/test_20.cave";

TEST(cave_test_01, constructor) {
  CaveGen cave;
  cave.FillRandomly(false);
  EXPECT_FALSE(cave.IsBordered());
}

TEST(cave_test_02, load) {
  CaveGen cave;
  cave.Load(kCaveSize10Path);
  EXPECT_EQ(cave.get_cave()->size(), 10);
  EXPECT_EQ(cave.get_cave()->front().size(), 10);
  cave.Load(kCaveSize20Path);
  EXPECT_EQ(cave.get_cave()->size(), 20);
  EXPECT_EQ(cave.get_cave()->front().size(), 20);
  cave.Load(kCaveSize10Path);
  EXPECT_EQ(cave.get_cave()->size(), 10);
  EXPECT_EQ(cave.get_cave()->front().size(), 10);
}

TEST(cave_test_03, load) {
  CaveGen cave;
  cave.Load(kCaveSize10Path);
  for (int i = 0; i < 1000; ++i) {
    cave.RenderNext();
  }
  EXPECT_FALSE(cave.RenderNext());
  EXPECT_EQ(cave.get_cave()->size(), 10);
  EXPECT_EQ(cave.get_cave()->front().size(), 10);
}

TEST(cave_test_04, pametrized) {
  CaveGen cave;
  cave.Load(kCaveSize10Path);

  for (int i = 0; i < 1000; ++i) {
    cave.RenderNext();
  }

  EXPECT_FALSE(cave.RenderNext());
  EXPECT_EQ(cave.get_cave()->size(), 10);
  EXPECT_EQ(cave.get_cave()->front().size(), 10);
  cave.set_size(-50, -1);
  EXPECT_EQ(cave.get_cave()->size(), 10);
  EXPECT_EQ(cave.get_cave()->front().size(), 10);
  cave.set_size(600, 1000);
  EXPECT_EQ(cave.get_cave()->size(), 50);
  EXPECT_EQ(cave.get_cave()->front().size(), 50);
  cave.set_init_chance(500);
  EXPECT_EQ(cave.get_chance(), 100);
  cave.set_init_chance(-500);
  EXPECT_EQ(cave.get_chance(), 0);
}