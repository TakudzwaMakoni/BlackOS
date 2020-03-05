#ifndef TRINKETS_NAVIGATION_HELPERS_H
#define TRINKETS_NAVIGATION_HELPERS_H

/**
 * NavigationHelpers
 *
 * Copyright (C) 2020, Takudzwa Makoni <https://github.com/TakudzwaMakoni>
 *
 * This Program is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This Program is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with This Program. If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

#include <filesystem>
#include <list>
#include <string>
#include <time.h>
#include <vector>

namespace BlackOS {
namespace Trinkets {

std::string stripSubstring(std::string str, std::string const &substring);
std::string timestampToDateTime(time_t const rawtime);
std::string pathPermissions(std::string const &path);
std::string pathLastModifiedDate(std::string const &path);
std::vector<std::string> loadFields(std::list<std::string> const &children,
                                    std::string const &parentPath);
std::string pathType(std::filesystem::path const &path);
} // namespace Trinkets
} // namespace BlackOS

#endif
