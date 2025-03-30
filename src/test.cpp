#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "PGMimageProcessor.h"
#include <filesystem>

TEST_CASE("ConnectedComponent basic functionality", "[ConnectComponent]"){
    ConnectedComponent cc(1);
    REQUIRE(cc.getId() == 1);
    REQUIRE(cc.getPixelCount() == 0);

    cc.addPixel(10, 20);
    REQUIRE(cc.getPixelCount == 1);
}

TEST_CASE("PGMimageProcessor file loading", "[PGMimageProcessor]"){
    REQUIRE_NOTHROW(PGMimageProcessor("test.pgm"));

    PGMimageProcessor process("test.pgm");
    REQUIRE(processor.getComponentCount() == 0);
}

TEST_CASE("Component extraction", "[PGMimageProcessor]"){
    PGMimageProcessor process("test.pgm");
    int count = processor.extracrComponents(128, 10);
    REQUIRE(count > 0);
    REQUIRE(processor.getComponentCount() == count);
}