CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS =

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
EXECUTABLE = findcomp

MAIN_SOURCE = $(wildcard $(SRC_DIR)/*.cpp)
MAIN_SOURCES := $(filter-out $(SRC_DIR)/test.cpp, $(MAIN_SOURCE))
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(MAIN_SOURCES))

TEST_SOURCES = $(SRC_DIR)/test.cpp
TEST_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SOURCES))

.PHONY: all clean tests

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(filter-out $(BUILD_DIR)/main.o, $(OBJECTS)) $(TEST_OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE) $(TEST_EXECUTABLE)