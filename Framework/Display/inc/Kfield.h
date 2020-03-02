#ifndef BLACKOS_KFIELD_H
#define BLACKOS_KFIELD_H

/**
 *  Copyright 2019-07-27 by Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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

#include "Directives.h"
#include <string>

namespace BlackOS {
namespace Display {

/// BlackOS Field Object
struct Kfield {
public:
  Kfield(std::string const &name, directive const script,
         std::string const &message = "");
  std::string name() const;
  directive script() const;
  std::string message() const;
  void setName(const std::string &name);

  //  ~Kfield();

private:
  std::string _name;
  directive _script = BlackOS::Display::Directives::doNothing;
  std::string _message;
};
} // namespace Display
} // namespace BlackOS
#endif // BLACKOS_KFIELD_H
