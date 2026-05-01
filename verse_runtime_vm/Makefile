# Unreal Engine 6 - Verse Virtual Machine Build Script
# Targets: Windows 12 / Project Helix Native

CXX = clang++
CXXFLAGS = -std=c++20 -O3 -Wall -Wextra -fPIC -fno-rtti -fno-exceptions

# Conceptually linking against LLVM for the JIT compiler backend
INCLUDES = -I./src -I/usr/local/opt/llvm/include
LDFLAGS = -L/usr/local/opt/llvm/lib -lLLVM

OUT_DIR = bin
VM_LIB = $(OUT_DIR)/libverse_vm.a

# Source Files
SRCS = src/compiler/VerseJITCompiler.cpp \
       src/execution/TransactionalMemory.cpp

OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean dirs

all: dirs $(VM_LIB)

dirs:
	mkdir -p $(OUT_DIR)

# Compile the Verse VM into a static library for the main UE6 Core to consume
$(VM_LIB): $(OBJS)
	ar rcs $@ $^
	@echo "======================================================="
	@echo " [SUCCESS] Verse Virtual Machine compiled to $(VM_LIB)"
	@echo "======================================================="

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f src/compiler/*.o src/execution/*.o
	rm -rf $(OUT_DIR)
	@echo "[CLEAN] Verse VM artifacts removed."
