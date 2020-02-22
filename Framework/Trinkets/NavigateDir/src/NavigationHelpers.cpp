#include "../inc/NavigationHelpers.h"

#include <filesystem>
#include <string>
#include <sys/stat.h>
#include <time.h>

namespace BlackOS {
namespace Trinkets {
std::string stripSubstring(std::string str, std::string const &substring) {
  size_t pos = std::string::npos;
  // Search for the substring in string in a loop untill nothing is found
  while ((pos = str.find(substring)) != std::string::npos) {
    // If found then erase it from string
    str.erase(pos, substring.length());
  }
  return str;
}

std::string timestampToDateTime(time_t const rawtime) {
  struct tm *dt;
  char buffer[80];
  dt = localtime(&rawtime);
  strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M  %Z", dt);
  return std::string(buffer);
}

std::string pathLastModifiedDate(std::string const &path) {
  struct stat result;
  if (stat(path.c_str(), &result) == 0) {
    return timestampToDateTime(result.st_mtime);
  } else {
    return "unknown";
  }
}

std::string pathPermissions(std::string const &path) {
  namespace fs = std::filesystem;
  fs::perms p = fs::status(path).permissions();

  std::string uR = (p & fs::perms::owner_read) != fs::perms::none ? "r" : "-";
  std::string uW = (p & fs::perms::owner_write) != fs::perms::none ? "w" : "-";
  std::string uX = (p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-";

  std::string gR = (p & fs::perms::group_read) != fs::perms::none ? "r" : "-";
  std::string gW = (p & fs::perms::group_write) != fs::perms::none ? "w" : "-";
  std::string gX = (p & fs::perms::group_exec) != fs::perms::none ? "x" : "-";

  std::string oR = (p & fs::perms::others_read) != fs::perms::none ? "r" : "-";
  std::string oW = (p & fs::perms::others_write) != fs::perms::none ? "w" : "-";
  std::string oX = (p & fs::perms::others_exec) != fs::perms::none ? "x" : "-";

  return uR + uW + uX + gR + gW + gX + oR + oW + oX;
}

std::vector<BlackOS::DisplayKernel::Kfield>
loadFields(std::list<std::string> const &children,
           std::string const &parentPath) {

  // get largest string length
  size_t max = 0;
  for (std::string const &entry : children) {
    std::string name = stripSubstring(entry, parentPath + "/");
    if (name.size() > max) {
      max = name.size();
    }
  }

  // generate fields
  std::vector<BlackOS::DisplayKernel::Kfield> fields;
  for (std::string const &entry : children) {
    std::stringstream ss;
    ss << std::left << std::setw(max + 3) << std::setfill(' ')
       << stripSubstring(entry, parentPath + "/") << std::setw(12)
       << pathPermissions(entry) << pathLastModifiedDate(entry);
    BlackOS::DisplayKernel::Kfield fieldEntry(
        ss.str(), BlackOS::DisplayKernel::Directives::doNothing);
    fields.push_back(fieldEntry);
  }
  return fields;
}

} // namespace Trinkets
} // namespace BlackOS
