/**
 *  Copyright 2019 by Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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

#ifndef MENUGENERATOR_H
#define MENUGENERATOR_H

#include "Kmenu.h"
namespace BlackOS {
namespace DisplayKernel {
namespace TestHelpers {
using Kmenu_sptr = std::shared_ptr<Kmenu>;
Kmenu_sptr testMenu();
Kmenu_sptr testMenuWithEightPaginatedFields(int pagination);
Kmenu_sptr testMenuInitialisedWithSizeAndPos(int const sizeY, int const sizeX,
                                             int const posY, int const posX);
} // namespace TestHelpers
} // namespace DisplayKernel
} // namespace BlackOS
#endif
