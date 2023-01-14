#include <gtest/gtest.h>

#include "../Model_module/maze/maze_gen.h"
#include "../Model_module/maze/route_finder.h"

using s21::MazeGen;
using s21::RouteFinder;

string kMazeSize10Path = "Maze/Test_module/test_10.maze";
string kMazeSize20Path = "Maze/Test_module/test_20.maze";

TEST(maze_test_01, constructor) {
  MazeGen m;
  m.GenerateRandomly();
}

TEST(maze_test_02, load) {
  MazeGen m;
  m.Load(kMazeSize10Path);
  EXPECT_EQ(m.get_right_walls()->size(), 10);
  EXPECT_EQ(m.get_bottom_walls()->size(), 10);
  m.Load(kMazeSize20Path);
  EXPECT_EQ(m.get_right_walls()->size(), 20);
  EXPECT_EQ(m.get_bottom_walls()->size(), 20);
  m.Load(kMazeSize10Path);
  EXPECT_EQ(m.get_right_walls()->size(), 10);
  EXPECT_EQ(m.get_bottom_walls()->size(), 10);
}

TEST(maze_test_04, pametrized) {
  MazeGen m;
  m.Load(kMazeSize10Path);
  EXPECT_EQ(m.get_right_walls()->size(), 10);
  EXPECT_EQ(m.get_bottom_walls()->size(), 10);
  m.Clear();
  m.Load(kMazeSize20Path);
  EXPECT_EQ(m.get_right_walls()->size(), 20);
  EXPECT_EQ(m.get_bottom_walls()->size(), 20);
  m.Clear();
  m.set_size(30);
  EXPECT_EQ(m.get_right_walls()->size(), 30);
  EXPECT_EQ(m.get_bottom_walls()->size(), 30);
  m.Load(kMazeSize10Path);
  EXPECT_EQ(m.get_right_walls()->size(), 10);
  EXPECT_EQ(m.get_bottom_walls()->size(), 10);
  EXPECT_THROW(m.set_size(-21), std::invalid_argument);
}

TEST(maze_test_05, route) {
  MazeGen m;
  EXPECT_TRUE(m.Load(kMazeSize10Path));

  RouteFinder router;
  router.set_maze(m.get_right_walls(), m.get_bottom_walls());
  router.set_start(6, 4);
  router.set_end(4, 4);
  router.FindRoute();
  EXPECT_TRUE(router.IsFound());
}