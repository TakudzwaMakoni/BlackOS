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

#include "../inc/Kfield.h"

namespace BlackOS {
namespace Display {

Kfield::Kfield(const std::string &name, const directive script,
               const std::string &message)
    : _name(name), _script(script), _message(message) {}
std::string Kfield::name() const { return this->_name; }
directive Kfield::script() const { return this->_script; }
std::string Kfield::message() const { return this->_message; }
void Kfield::setName(const std::string &name) { _name = name; }
// Kfield::~Kfield(){};
} // namespace Display
} // namespace BlackOS
