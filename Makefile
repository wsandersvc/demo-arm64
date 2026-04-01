# Detect platform and set appropriate compiler
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

ifeq ($(UNAME_S),Linux)
    ifeq ($(UNAME_M),x86_64)
        # Linux amd64 - use cross-compiler
        CC = aarch64-linux-gnu-gcc
        CXX = aarch64-linux-gnu-gcc
    else
        # Native ARM64 Linux
        CC = gcc
        CXX = gcc
    endif
else
    # macOS or other platforms
    CC = gcc-13
    CXX = gcc-13
endif


# Compiler flags
CFLAGS = -Wall -Wextra -march=armv8-a -save-temps=obj
CXXFLAGS = -Wall -Wextra -march=armv8-a -std=c++17 -save-temps=obj
LDFLAGS = -lm -lstdc++

# Directories
BUILD_DIR = build
VERAOUT_DIR = veraout
PROJECT_DIR = .

# Target executable
TARGET = $(BUILD_DIR)/arm64_sample

# Source files
SOURCES = main.cpp utils.cpp verademo_vulns.cpp
OBJECTS = $(addprefix $(BUILD_DIR)/,$(SOURCES:.cpp=.o))

# Preprocessed files that will be generated
PREPROCESSED = $(addprefix $(BUILD_DIR)/,$(SOURCES:.cpp=.ii))

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile C++ source files to object files
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) arm64_sample.zip ${VERAOUT_DIR}

# Display preprocessed files
show-preprocessed:
	@echo "Preprocessed files (.ii):"
	@ls -lh $(BUILD_DIR)/*.ii 2>/dev/null || echo "No preprocessed files found. Run 'make' first."

# Build in Docker container (useful for macOS to generate correct ARM64 output)
docker-build:
	docker run --rm -v $(PWD):/app --workdir /app --user root arm64v8/gcc:13.2.0 make

# Veracode package
veracode-package:
	veracode package -s $(PROJECT_DIR) -a -t directory -o veraout

# Phony targets
.PHONY: all clean show-preprocessed docker-build veracode-package
