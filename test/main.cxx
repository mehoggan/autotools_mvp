#include <autotools_mvp/logging.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <iostream>

#include "utils/test_environment.h"

int main(int argc, char *argv[]) {
  const std::string exe_path = argv[0];
  com::autotools_mvp::testing::TestEnvironment::EXE_PATH = exe_path;

  com::autotools_mvp::LogTag main_tag("main");
  com::autotools_mvp::Logging::add_std_cout_logger(main_tag);

  ::testing::AddGlobalTestEnvironment(
      new com::autotools_mvp::testing::TestEnvironment);
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
