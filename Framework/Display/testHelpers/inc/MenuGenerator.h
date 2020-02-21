#ifndef MENUGENERATOR_H
#define MENUGENERATOR_H

#include "../../inc/Kmenu.h"
namespace BlackOS {
namespace Display {
namespace TestHelpers {
using Kmenu_sptr = std::shared_ptr<Kmenu>;
Kmenu_sptr testMenu();
Kmenu_sptr testMenuWithEightPaginatedFields(int pagination);
Kmenu_sptr testMenuInitialisedWithSizeAndPos(int const sizeY, int const sizeX,
                                             int const posY, int const posX);
} // namespace TestHelpers
} // namespace Display
} // namespace BlackOS
#endif
