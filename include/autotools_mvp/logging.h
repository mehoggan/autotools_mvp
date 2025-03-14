#ifndef COM_VISUALIZATION_LOGGING_H_INCLUDED
#define COM_VISUALIZATION_LOGGING_H_INCLUDED

#define BOOST_LOG_DYN_LINK 1

#include <gtest/gtest_prod.h>

#include <atomic>
#include <boost/filesystem/path.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/trivial.hpp>
#include <boost/weak_ptr.hpp>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>

#define AUTOTOOLS_MVP_TRACE boost::log::trivial::severity_level::trace
#define AUTOTOOLS_MVP_DEBUG boost::log::trivial::severity_level::debug
#define AUTOTOOLS_MVP_INFO boost::log::trivial::severity_level::info
#define AUTOTOOLS_MVP_WARN boost::log::trivial::severity_level::warning
#define AUTOTOOLS_MVP_ERROR boost::log::trivial::severity_level::error
#define AUTOTOOLS_MVP_FATAL boost::log::trivial::severity_level::fatal

namespace com {
namespace autotools_mvp {
/**
 * @brief A class used to uniqlly identify a specific log.
 *
 * A generic class used to identify a specific log and its source.
 */
struct LogTag {
 public:
  /**
   * @brief ctor for a \ref LogTag.
   *
   * @param[in] tag A string used in the logs to identify the source and
   *                to uniquly identify a log.
   */
  explicit LogTag(const std::string &tag);

  /**
   * @brief A getter for the string used to represent the \ref LogTag.
   *
   * A member getter function of \ref LogTag that returns the string
   * representation of the \ref LogTag.
   *
   * @return A null terminated string representation of \ref LogTag.
   */
  const char *tag() const;

  /**
   * @brief A utility output operator.
   *
   * A utility output operator for a \ref LogTag.
   *
   * @param[in] os The stream to write to.
   * @param[in] tag The \ref LogTag to write to \os.
   *
   * @return The stream written to.
   */
  friend std::ostream &operator<<(std::ostream &os, const LogTag &tag) {
    os << tag.tag();
    return os;
  }

  /**
   * @brief A utility output operator.
   *
   * @param[in] ss The stream to write to.
   * @param[in] tag The \ref LogTag to write to \p ss.
   *
   * @return The stream \p ss after being written to.
   */
  friend std::stringstream &operator<<(std::stringstream &ss,
                                       const LogTag &tag) {
    ss << tag.tag();
    return ss;
  }

  /**
   * @brief A utility operator to test for equality between \ref LogTag.
   *
   * @param[in] lhs The \ref LogTag on the left hand side of the '==`.
   * @param[in] rhs The \ref LogTag on the right hand side of the '=='.
   *
   * @return true if \p lhs tag_ is a string equivalent to \p rhs tag_.
   */
  friend bool operator==(const LogTag &lhs, const LogTag &rhs) {
    return lhs.tag() == rhs.tag();
  }

 private:
  std::string tag_;
};
}  // namespace autotools_mvp
}  // namespace com

namespace std {
template <>
struct hash<com::autotools_mvp::LogTag> {
  size_t operator()(const com::autotools_mvp::LogTag &obj) const {
    return hash<std::string>{}(obj.tag());
  }
};
}  // namespace std

namespace com {
namespace autotools_mvp {
/**
 * @brief A class used to log to various sinks or sources.based on a tag.
 *
 * A generic class used to log to various sinks or sources based of a
 * \ref LogTag.
 */
class Logging {
 private:
  template <typename T>
  static void log_string_builder(std::stringstream &ss, T &&arg) {
    ss << std::forward<T>(arg);
  }

  template <typename T, typename... Ts>
  static void log_string_builder(std::stringstream &ss, T &&arg, Ts &&...args) {
    ss << std::forward<T>(arg) << " ";
    log_string_builder(ss, args...);
  }

 public:
  typedef boost::log::sinks::synchronous_sink<
      boost::log::sinks::text_ostream_backend>
      TextSink;
  typedef std::unordered_map<LogTag, boost::weak_ptr<TextSink>> Dict;

  /**
   * @brief Used to create a stdout source tied to a \ref LogTag.
   *
   * A static function used to create a stdout source tied to a tag.
   *
   * @param[in] tag The \ref LogTag used to identify logs going to
                    stdout stream.
   * @param[in] level A filter for a give sink and the \ref tag.
   *
   * @return true if \p tag was not already added to logger, false
   *         otherwise.
   */
  static bool add_std_cout_logger(
      const LogTag &tag,
      boost::log::trivial::severity_level level = AUTOTOOLS_MVP_INFO);

  /**
   * @brief Used to create a stderr source tied to a \ref LogTag.
   *
   * A static function used to create a stderr source tied to a tag.
   *
   * @param[in] tag The \ref LogTag used to identify logs going to
                stderr stream.
   * @param[in] level A filter for a give sink and the \ref tag.
   *
   * @return true if \p tag was not already added to logger, false
   *         otherwise.
   */
  static bool add_std_cerr_logger(
      const LogTag &tag,
      boost::log::trivial::severity_level level = AUTOTOOLS_MVP_ERROR);

  /**
   * @brief Used to create a stdlog source tied to a \ref LogTag.
   *
   * @param[in] tag The \ref LogTag used to identify logs going to
                    stdlog stream.
   * @param[in] level A filter for a give sink and the \ref tag.
   *
   * @return true if \p tag was not already added to logger, false
   *         otherwise.
   */
  static bool add_std_log_logger(
      const LogTag &tag,
      boost::log::trivial::severity_level level = AUTOTOOLS_MVP_TRACE);

  /**
   * @brief Used to create a file source tied to a \ref LogTag.
   *
   * A static function used to create a file source tied to a tag.
   *
   * @param[in] tag The \ref LogTag used to identify logs going to
                    a specific file stream.
   * @param[in] log_path The path to the file to log to.
   * @param[in] level A filter for a give sink and the \ref tag.
   *
   * @return
   */
  static bool add_file_logger(
      const LogTag &tag, const boost::filesystem::path &log_path,
      boost::log::trivial::severity_level level = AUTOTOOLS_MVP_INFO);

  /**
   * @brief A utility function used to generate a log tag for a specific
   *        class.
   *
   * @tparam T The type for the instance to generate a \ref LogTag for.
   * @param[in] instance The instance to use RTTI on for the \ref LogTag.
   *
   * @return A \ref LogTag generated by RTTI on \p instance.
   */
  template <typename T>
  static LogTag LogTag_for_this(const T &instance) {
    std::stringstream ss;
    ss << typeid(T).name() << "_" << (&(instance));
    return LogTag(ss.str());
  }

  /**
   * @brief If you want to log without a empty \ref LogTag call this.
   *
   * Used to set up common attributes for loggers to enable Logging
   * without a \ref LogTag.
   * @return true if Logging setup or on first call, false otherwise.
   */
  static bool init();

  /**
   * @brief Utility function for creating a tmp directory for where to
   *        write log files.
   *
   * @param[in] model The format to be used in creating a tmp directory
   *
   * @return A \ref boost::filesystem::path to where the tmp directory
   *         was created on the local filesystem.
   */
  static boost::filesystem::path mktmpdir(
      const boost::filesystem::path &model = "%%%%-%%%%-%%%%-%%%%");

  /**
   * @brief A key function for Logging at the \ref AUTOTOOLS_MVP_TRACE
   *        level.
   *
   * This static function logs any user defined type that can be written
   * to a \ref std::ostream and will use the tag for filtering based on
   * the call to \ref Logging::add_std_cout_logger.
   *
   * @tparam ...Ts A list of mixed types which all must have support for
   *               writting to a string stream.
   * @param[in] tag A \ref LogTag used in filtering.
   * @param[in] args A variadic list of args that can be written to a
                     \ref std::ostream.
   */
  template <typename... Ts>
  static void trace(const LogTag &tag, Ts &&...args) {
    std::stringstream ss;
    Logging::log_string_builder(ss, args...);
    write_severity_log(tag, AUTOTOOLS_MVP_TRACE, ss.str());
  }

  /**
   * @brief A key function for Logging at the \ref AUTOTOOLS_MVP_DEBUG
   *        level.
   *
   * This static function logs any user defined type that can be written
   * to a \ref std::ostream and will use the tag for filtering based on
   * the call to \ref Logging::add_std_cout_logger.
   *
   * @tparam ...Ts A list of mixed types which all must have support for
   *               writting to a string stream.
   * @param[in] tag A \ref LogTag used in filtering.
   * @param[in] args A variadic list of args that can be written to a
   *                 \ref std::ostream.
   */
  template <typename... Ts>
  static void debug(const LogTag &tag, Ts &&...args) {
    std::stringstream ss;
    Logging::log_string_builder(ss, args...);
    write_severity_log(tag, AUTOTOOLS_MVP_DEBUG, ss.str());
  }

  /**
   * @brief A key function for Logging at the \ref AUTOTOOLS_MVP_INFO
   *        level.
   *
   * This static function logs any user defined type that can be written
   * to a \ref std::ostream and will use the tag for filtering based on
   * the call to \ref Logging::add_std_cout_logger.
   *
   * @tparam ...Ts A list of mixed types which all must have support for
   *               writting to a string stream.
   * @param[in] tag A \ref LogTag used in filtering.
   * @param[in] args A variadic list of args that can be written to a
   *                 \ref std::ostream.
   */
  template <typename... Ts>
  static void info(const LogTag &tag, Ts &&...args) {
    std::stringstream ss;
    Logging::log_string_builder(ss, args...);
    write_severity_log(tag, AUTOTOOLS_MVP_INFO, ss.str());
  }

  /**
   * @brief A key function for Logging at the \ref AUTOTOOLS_MVP_WARN
   *        level.
   *
   * This static function logs any user defined type that can be written
   * to a \ref std::ostream and will use the tag for filtering based on
   * the call to \ref Logging::add_std_cout_logger.
   *
   * @tparam ...Ts A list of mixed types which all must have support for
   *               writting to a string stream.
   * @param[in] tag A \ref LogTag used in filtering.
   * @param[in] args A variadic list of args that can be written to a
   *                 \ref std::ostream.
   */
  template <typename... Ts>
  static void warn(const LogTag &tag, Ts &&...args) {
    std::stringstream ss;
    Logging::log_string_builder(ss, args...);
    write_severity_log(tag, AUTOTOOLS_MVP_WARN, ss.str());
  }

  /**
   * @brief A key function for Logging at the \ref AUTOTOOLS_MVP_ERROR
   *        level.
   *
   * This static function logs any user defined type that can be written
   * to a \ref std::ostream and will use the tag for filtering based on
   * the call to \ref Logging::add_std_cerr_logger.
   *
   * @tparam ...Ts A list of mixed types which all must have support for
   *               writting to a string stream.
   * @param[in] tag A \ref LogTag used in filtering.
   * @param[in] args A variadic list of args that can be written to a
   *             \ref std::ostream.
   */
  template <typename... Ts>
  static void error(const LogTag &tag, Ts &&...args) {
    std::stringstream ss;
    Logging::log_string_builder(ss, args...);
    write_severity_log(tag, AUTOTOOLS_MVP_ERROR, ss.str());
  }

  /**
   * @brief A key function for Logging at the \ref AUTOTOOLS_MVP_FATAL
   *        level.
   *
   * This static function logs any user defined type that can be written
   * to a \ref std::ostream and will use the tag for filtering based on
   * the call to \ref Logging::add_std_cerr_logger.
   *
   * @tparam ...Ts A list of mixed types which all must have support for
   *               writting to a string stream.
   * @param tag A \ref LogTag used in filtering.
   * @param args A variadic list of args that can be written to a
   *             \ref std::ostream.
   */
  template <typename... Ts>
  static void fatal(const LogTag &tag, Ts &&...args) {
    std::stringstream ss;
    Logging::log_string_builder(ss, args...);
    write_severity_log(tag, AUTOTOOLS_MVP_FATAL, ss.str());
  }

  /**
   * @brief Removes all sinks and clears the tags.
   *
   * @return true if successful, false otherwise.
   */
  static bool clear_all();

 private:
  static void write_severity_log(const LogTag &tag,
                                 boost::log::trivial::severity_level level,
                                 const std::string &message);

 private:
  FRIEND_TEST(TestLogging, test_clear_all);

 private:
  LogTag tag_;

 private:
  static std::atomic<bool> init_;
  static Dict loggers_;
  static std::mutex loggers_mutex_;
};

/**
 * @brief Adds a logger for a type \t T to the logger.
 *
 * A template function that takes a type T and generates a unique string
 * to be used in a LogTag.
 *
 * @tparam T The type for \p t for which RTTI is used on to get a
 *           \ref LogTag.
 * @param[in] t An instance of \tref T used to generate a \ref LogTag.
 * @param[in] cout_level The level at which to log to std::cout.
 * @param[in] cerr_level The level at which to log to std::cerr.
 *
 * @return A \ref LogTag generated using RTTI for \p t.
 */
template <class T>
LogTag add_std_cout_std_err_logger_for_type_instance(
    const T &t,
    boost::log::trivial::severity_level cout_level = AUTOTOOLS_MVP_INFO,
    boost::log::trivial::severity_level cerr_level = AUTOTOOLS_MVP_ERROR) {
  LogTag LOG_TAG = Logging::LogTag_for_this(t);
  Logging::add_std_cout_logger(LOG_TAG, cout_level);
  Logging::add_std_cerr_logger(LOG_TAG, cerr_level);
  return LOG_TAG;
}
}  // namespace autotools_mvp
}  // namespace com
#endif
