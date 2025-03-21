#include "autotools_mvp/uuid.h"

#include <iostream>
#include <stdexcept>

namespace com {
namespace autotools_mvp {
UUID::UUID() { uuid_generate(uuid_); }

UUID::UUID(const std::string &user_defined_uuid) {
  int status = uuid_parse(user_defined_uuid.c_str(), uuid_);
  if (status == -1) {
    throw std::runtime_error("Failed to parse user defined uuid of " +
                             user_defined_uuid);
  }
}

UUID::~UUID() { uuid_clear(uuid_); }

UUID UUID::clone() { return (*this); }

UUID::UUID(UUID &&other) {
  uuid_copy(uuid_, other.uuid_);
  other.~UUID();
}

UUID &UUID::operator=(UUID &&rhs) {
  uuid_copy(uuid_, rhs.uuid_);
  rhs.~UUID();

  return (*this);
}

bool UUID::is_initialized() const { return !uuid_is_null(uuid_); }

std::string UUID::as_string() const {
  std::string ret;
  if (is_initialized()) {
    char uuid_str[37];
    uuid_unparse_lower(uuid_, uuid_str);
    ret = std::string(uuid_str);
  }

  return ret;
}

UUID::UUID(const UUID &other) { uuid_copy(uuid_, other.uuid_); }

UUID &UUID::operator=(const UUID &rhs) {
  uuid_copy(uuid_, rhs.uuid_);

  return (*this);
}
} // namespace autotools_mvp
} // namespace com
