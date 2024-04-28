CXX=clang++
# Directory structure
SRC_DIR=src
OPT_DIR=optimal
APP_DIR=approximate
TEST_DIR=test
BUILD_DIR=./build

# Compiler flags
CXXFLAGS=-std=c++11 -O2 -march=native -pthread
OPT_INC=-I/Library/gurobi1100/macos_universal2/include
APP_INC=
OPT_LDFLAGS=
APP_LDFLAGS=
OPT_LDLIBS=-L/Library/gurobi1100/macos_universal2/lib -lgurobi_c++ -lgurobi110
APP_LDLIBS=

# Find all the C++ files in src and test directories
OPT_SRCS=$(SRC_DIR)/$(OPT_DIR)/solve.cpp $(SRC_DIR)/$(OPT_DIR)/graph.cpp $(SRC_DIR)/$(OPT_DIR)/milp.cpp $(SRC_DIR)/$(OPT_DIR)/solution.cpp
APP_SRCS=$(SRC_DIR)/$(APP_DIR)/main.cpp $(SRC_DIR)/$(APP_DIR)/graph.cpp $(SRC_DIR)/$(APP_DIR)/state.cpp $(SRC_DIR)/$(APP_DIR)/utils.cpp $(SRC_DIR)/$(APP_DIR)/builders.cpp $(SRC_DIR)/$(APP_DIR)/modifiers.cpp $(SRC_DIR)/$(APP_DIR)/solvers.cpp $(SRC_DIR)/$(APP_DIR)/ea.cpp

TEST_SRCS=$(TEST_DIR)/gurobi.cpp

# Create object file names by replacing .cpp with .o
OPT_OBJS=$(patsubst $(SRC_DIR)/$(OPT_DIR)/%.cpp,$(BUILD_DIR)/$(OPT_DIR)/%.o,$(OPT_SRCS))
APP_OBJS=$(patsubst $(SRC_DIR)/$(APP_DIR)/%.cpp,$(BUILD_DIR)/$(APP_DIR)/%.o,$(APP_SRCS))
TEST_OBJS=$(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SRCS))

# Targets
OPT_TARGET=$(BUILD_DIR)/$(OPT_DIR)/solve
APP_TARGET=$(BUILD_DIR)/$(APP_DIR)/solve
TEST_TARGET=$(BUILD_DIR)/gurobi_test

# Build targets
app: $(APP_TARGET)
opt: $(OPT_TARGET)
test: $(TEST_TARGET)

$(OPT_TARGET): $(OPT_OBJS)
	$(CXX) $(CXXFLAGS) $(OPT_INC) $^ -o $@ $(OPT_LDFLAGS) $(OPT_LDLIBS)

$(APP_TARGET): $(APP_OBJS)
	$(CXX) $(CXXFLAGS) $(APP_INC) $^ -o $@ $(APP_LDFLAGS) $(APP_LDLIBS)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(OPT_INC) $^ -o $@ $(OPT_LDFLAGS) $(OPT_LDLIBS)

# Compile source files
$(BUILD_DIR)/$(OPT_DIR)/%.o: $(SRC_DIR)/$(OPT_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)/$(OPT_DIR)
	$(CXX) $(CXXFLAGS) $(OPT_INC) -c $< -o $@

# Compile source files
$(BUILD_DIR)/$(APP_DIR)/%.o: $(SRC_DIR)/$(APP_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)/$(APP_DIR)
	$(CXX) $(CXXFLAGS) $(APP_INC) -c $< -o $@

# Compile test files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OPT_INC) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: opt app clean test all
