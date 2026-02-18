# Makefile for Cross-Platform Raylib Game Development
# Compatible with Windows, Linux, and macOS
# Supports both C and C++ source files

# Project name (change this to your game's name)
PROJECT_NAME = game

# Compilers
CC = gcc
CXX = g++

# Detect operating system
ifeq ($(OS),Windows_NT)
    PLATFORM = WINDOWS
    PLATFORM_OS = WINDOWS
    # Windows-specific settings
    RAYLIB_PATH ?= C:/raylib/raylib
    INCLUDE_PATH = $(RAYLIB_PATH)/src
    LIBRARY_PATH = $(RAYLIB_PATH)/src
    # Windows linking
    LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
    EXT = .exe
    RM = del /Q
    # Windows mkdir command
    MKDIR = if not exist
    MKDIR_END = mkdir
    PATH_SEP = \\
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        PLATFORM = LINUX
        PLATFORM_OS = LINUX
        # Linux-specific settings
        RAYLIB_PATH ?= /usr/local
        INCLUDE_PATH = $(RAYLIB_PATH)/include
        LIBRARY_PATH = $(RAYLIB_PATH)/lib
        LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
        EXT =
        RM = rm -f
        MKDIR = mkdir -p
        MKDIR_END =
        PATH_SEP = /
    endif
    ifeq ($(UNAME_S),Darwin)
        PLATFORM = OSX
        PLATFORM_OS = OSX
        # macOS-specific settings
        RAYLIB_PATH ?= /usr/local
        INCLUDE_PATH = $(RAYLIB_PATH)/include
        LIBRARY_PATH = $(RAYLIB_PATH)/lib
        LDLIBS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
        EXT =
        RM = rm -f
        MKDIR = mkdir -p
        MKDIR_END =
        PATH_SEP = /
    endif
endif

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99 -I$(INCLUDE_PATH) -D$(PLATFORM)
CXXFLAGS = -Wall -Wextra -std=c++17 -I$(INCLUDE_PATH) -D$(PLATFORM)
LDFLAGS = -L$(LIBRARY_PATH)

# Debug/Release flags
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CFLAGS += -g -O0 -DDEBUG
    CXXFLAGS += -g -O0 -DDEBUG
    BUILD_DIR = build/debug
else
    CFLAGS += -O2 -DNDEBUG
    CXXFLAGS += -O2 -DNDEBUG
    BUILD_DIR = build/release
endif

# Source files - automatically detect C and C++ files
SRC_DIR = src
C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
CXX_SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.cc) $(wildcard $(SRC_DIR)/*.cxx)

# Object files
C_OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
CXX_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CXX_SOURCES))
CXX_OBJECTS += $(patsubst $(SRC_DIR)/%.cc,$(BUILD_DIR)/%.o,$(filter %.cc,$(CXX_SOURCES)))
CXX_OBJECTS += $(patsubst $(SRC_DIR)/%.cxx,$(BUILD_DIR)/%.o,$(filter %.cxx,$(CXX_SOURCES)))

ALL_OBJECTS = $(C_OBJECTS) $(CXX_OBJECTS)

# Determine which linker to use (C++ if any C++ files exist, otherwise C)
ifneq ($(strip $(CXX_SOURCES)),)
    LINKER = $(CXX)
    LINK_FLAGS = $(CXXFLAGS)
else
    LINKER = $(CC)
    LINK_FLAGS = $(CFLAGS)
endif

# Output
OUTPUT = $(BUILD_DIR)/$(PROJECT_NAME)$(EXT)

# Default target
all: $(BUILD_DIR) $(OUTPUT)
	@echo Build complete for $(PLATFORM)!
	@echo Language: $(if $(strip $(CXX_SOURCES)),C++ and C,C only)
	@echo Run: $(OUTPUT)

# Create build directory - Windows compatible
$(BUILD_DIR):
ifeq ($(OS),Windows_NT)
	@$(MKDIR) "build" $(MKDIR_END) "build" 2>nul || echo Directory exists
	@$(MKDIR) "$(BUILD_DIR)" $(MKDIR_END) "$(BUILD_DIR)" 2>nul || echo Directory exists
else
	@$(MKDIR) $(BUILD_DIR)
endif

# Build executable
$(OUTPUT): $(ALL_OBJECTS)
	@echo Linking $(PROJECT_NAME) with $(LINKER)...
	@$(LINKER) $(ALL_OBJECTS) -o $(OUTPUT) $(LDFLAGS) $(LDLIBS)

# Compile C source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo Compiling C file: $<...
	@$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ source files (.cpp)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo Compiling C++ file: $<...
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C++ source files (.cc)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc
	@echo Compiling C++ file: $<...
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C++ source files (.cxx)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cxx
	@echo Compiling C++ file: $<...
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	@echo Cleaning build files...
ifeq ($(OS),Windows_NT)
	@if exist build\debug\*.o del /Q build\debug\*.o 2>nul
	@if exist build\release\*.o del /Q build\release\*.o 2>nul
	@if exist build\debug\*.exe del /Q build\debug\*.exe 2>nul
	@if exist build\release\*.exe del /Q build\release\*.exe 2>nul
else
	@$(RM) $(BUILD_DIR)/*.o $(OUTPUT)
endif
	@echo Clean complete!

# Clean everything including build directory
distclean:
	@echo Removing all build files...
ifeq ($(OS),Windows_NT)
	@if exist build rmdir /S /Q build 2>nul
else
	@rm -rf build
endif
	@echo All clean!

# Run the game
run: all
	@echo Running $(PROJECT_NAME)...
	@./$(OUTPUT)

# Debug build
debug:
	@$(MAKE) DEBUG=1

# List source files (helpful for debugging the Makefile)
list:
	@echo === Source Files ===
	@echo C files: $(C_SOURCES)
	@echo C++ files: $(CXX_SOURCES)
	@echo.
	@echo === Object Files ===
	@echo C objects: $(C_OBJECTS)
	@echo C++ objects: $(CXX_OBJECTS)
	@echo.
	@echo === Linker ===
	@echo Using: $(LINKER)

# Help target
help:
	@echo === Raylib Game Makefile ===
	@echo Platform: $(PLATFORM)
	@echo Supports: C (.c) and C++ (.cpp, .cc, .cxx)
	@echo.
	@echo Available targets:
	@echo   make          - Build the game (release mode)
	@echo   make debug    - Build with debug symbols
	@echo   make run      - Build and run the game
	@echo   make clean    - Remove object files and executable
	@echo   make distclean - Remove entire build directory
	@echo   make list     - List all source and object files
	@echo   make help     - Show this help message
	@echo.
	@echo Configuration:
	@echo   RAYLIB_PATH=$(RAYLIB_PATH)
	@echo   DEBUG=$(DEBUG)
	@echo   C Standard: c99
	@echo   C++ Standard: c++17

# Phony targets
.PHONY: all clean distclean run debug list help