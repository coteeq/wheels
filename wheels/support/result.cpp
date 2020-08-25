#include <wheels/support/result.hpp>

namespace wheels {

namespace make_result {

Status Ok() {
  return Status::Ok();
}

detail::Failure CurrentException() {
  return detail::Failure(std::current_exception());
}

detail::Failure Fail(std::error_code error) {
  WHEELS_VERIFY(error, "Expected error");
  return detail::Failure{error};
}

detail::Failure Fail(Error error) {
  WHEELS_VERIFY(error.HasError(), "Expected error");
  return detail::Failure(std::move(error));
}

Status ToStatus(std::error_code error) {
  if (error) {
    return Fail(error);
  } else {
    return Ok();
  }
}

}  // namespace make_result

}  // namespace wheels
