#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "PGMimageProcessor.h"

TEST_CASE("Custom Constructor", "[PGMimageProcessor]") {
    PGMimageProcessor p("pgm_test_files/Shapes.pgm");
    REQUIRE(p.getWidth() > 0);
    REQUIRE(p.getHeight() > 0);
    REQUIRE(p.getComponentCount() == 0);
}

TEST_CASE("Component Extraction", "[PGMimageProcessor]") {
    PGMimageProcessor p("pgm_test_files/Shapes.pgm");
    int count = p.extractComponents(128, 1);
    REQUIRE(count > 0);
    REQUIRE(p.getComponentCount() == count);
}

TEST_CASE("Largest and Smallest Component Size", "[PGMimageProcessor]") {
    PGMimageProcessor p("pgm_test_files/Shapes.pgm");
    p.extractComponents(128, 1);
    REQUIRE(p.getLargestSize() == 520187);
    REQUIRE(p.getSmallestSize() > 0);
}

TEST_CASE("Component Count", "[PGMimageProcessor]") {
    PGMimageProcessor p("pgm_test_files/Shapes.pgm");
    p.extractComponents(128, 1);
    REQUIRE(p.getComponentCount() == 118);
}

TEST_CASE("Filter Components by Size", "[PGMimageProcessor]") {
    PGMimageProcessor p("pgm_test_files/Shapes.pgm");
    p.extractComponents(128, 1);
    int beforeFilter = p.getComponentCount();
    p.filterComponentsBySize(10000, 500000);
    REQUIRE(p.getComponentCount() <= beforeFilter);
}

TEST_CASE("Write Components to File", "[PGMimageProcessor]") {
    PGMimageProcessor p("pgm_test_files/Shapes.pgm");
    p.extractComponents(128, 1);
    REQUIRE(p.writeComponents("output.pgm"));
}