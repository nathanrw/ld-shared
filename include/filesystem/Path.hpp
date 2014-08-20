#pragma once

#include <string>

namespace filesystem {

  /**
   * Type safe representation of a path.
   */
  class Path {
  public:
    explicit Path(std::string path);
    std::string path() const;
  private:
    std::string m_path;
  };  

}
