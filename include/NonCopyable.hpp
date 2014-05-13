#pragma once

/**
 * Base class for objects which are not copyable.
 **/
struct NonCopyable {
  NonCopyable() = default;
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable& that) = delete;
};
