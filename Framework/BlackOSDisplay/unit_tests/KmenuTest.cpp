// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include "../inc/Kmenu.h"
#include "../inc/Directives.h"
#include "../inc/Kfield.h"
#include "../testHelpers/inc/MenuGenerator.h"
#include <catch2/catch.hpp>

namespace BlackOSDisplay {
auto const &menu = TestHelpers::testMenuWithPagination(3);
TEST_CASE("test paginated fields map", "[pagination]") { REQUIRE(1 == 1); }
} // namespace BlackOSDisplay
