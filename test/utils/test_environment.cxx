#include "test_environment.h"

#include <stdexcept>

#include "autotools_mvp/logging.h"

namespace com {
namespace autotools_mvp {
namespace testing {
Pathing::Pathing() : Pathing("") {}

Pathing::Pathing(const std::string &executable_path)
    : executable_path_(executable_path) {}

std::filesystem::path Pathing::get_lib_dir_path() const {
  return get_lib_path().parent_path();
}

std::filesystem::path Pathing::get_lib_path() const {
  return executable_path().parent_path().parent_path().parent_path() / "lib" /
         ".libs" / "libautotools_mvp.dylib";
}

std::filesystem::path Pathing::get_stages_lib_dir_path() const {
  return get_stages_lib_path().parent_path();
}

std::filesystem::path Pathing::get_stages_lib_path() const {
  return executable_path().parent_path().parent_path().parent_path() /
         "stages_lib" / "lib" / ".libs" / "libstages_lib.dylib";
}

const std::filesystem::path &Pathing::executable_path() const {
  return executable_path_;
}

std::string TestEnvironment::EXE_PATH;
com::autotools_mvp::LogTag TestEnvironment::TEST_TAG("Testing");
Pathing TestEnvironment::PATHING;

void TestEnvironment::SetUp() {
  com::autotools_mvp::Logging::add_std_cout_logger(TEST_TAG);
  if (TestEnvironment::EXE_PATH.empty()) {
    throw std::runtime_error(
        std::string("Please make sure ") +
        std::string("com::autotools_mvp::testing::TestEnvironment::") +
        std::string("EXE_PATH is set from within main!"));
  } else {
    Logging::info(TEST_TAG, "Got", EXE_PATH, "for path to executable");
    PATHING = Pathing(EXE_PATH);
  }
}
}  // namespace testing
}  // namespace autotools_mvp
}  // namespace com
