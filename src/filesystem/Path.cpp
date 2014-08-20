#include "filesystem/Path.hpp"

using namespace filesystem;

Path::Path(std::string path)
  : m_path(path)
{
}

std::string Path::path() const
{
  return m_path;
}
