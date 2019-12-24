#ifndef MENUGENERATOR_H
#define MENUGENERATOR_H
#include "../../inc/Kmenu.h"
namespace BlackOSDisplay {
namespace TestHelpers {
using Kmenu_sptr = std::shared_ptr<BlackOSDisplay::Kmenu>;
Kmenu_sptr testMenuWithPagination(int pagination);
Kmenu_sptr testMenuInitialisedWithSizeAndPos(int const sizeY, int const sizeX,
                                             int const posY, int const posX);
} // namespace TestHelpers
} // namespace BlackOSDisplay
#endif
