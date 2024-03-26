CXX=clang++
# Directory structure
SRC_DIR=src
TEST_DIR=test
BUILD_DIR=./build

# Compiler flags
CXXFLAGS=-std=c++11 -O2 -march=native -pthread
INC=-I/Library/gurobi1100/macos_universal2/include
LDFLAGS=
LDLIBS=-L/Library/gurobi1100/macos_universal2/lib -lgurobi_c++ -lgurobi110

# Find all the C++ files in src and test directories
OPT_SRCS=$(SRC_DIR)/optimal.cpp $(SRC_DIR)/graph.cpp $(SRC_DIR)/milp.cpp $(SRC_DIR)/solution.cpp
APP_SRCS=$(SRC_DIR)/approximate.cpp $(SRC_DIR)/graph.cpp $(SRC_DIR)/solution.cpp $(SRC_DIR)/heuristic.cpp
TEST_SRCS=$(TEST_DIR)/gurobi.cpp

# Create object file names by replacing .cpp with .o
OPT_OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(OPT_SRCS))
APP_OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(APP_SRCS))
TEST_OBJS=$(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SRCS))

# Targets
OPT_TARGET=$(BUILD_DIR)/optimal
APP_TARGET=$(BUILD_DIR)/approximate
TEST_TARGET=$(BUILD_DIR)/gurobi_test

# Build targets
opt: $(OPT_TARGET)
app: $(APP_TARGET)
test: $(TEST_TARGET)

$(OPT_TARGET): $(OPT_OBJS)
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(APP_TARGET): $(APP_OBJS)
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

# Compile test files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: opt app clean test
