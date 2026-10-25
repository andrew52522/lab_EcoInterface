# Native macOS Apple Silicon build.  Works with Apple Command Line Tools.
# Override ECO_FRAMEWORK as needed; no full Xcode installation required.
ROOT := $(abspath ../../..)
# Clone peerf-eco/Eco.Education NEXT TO this repository.
EDUCATION ?= $(abspath $(ROOT)/../Eco.Education)
CALC_A := $(EDUCATION)/Eco.Pro.Training/001.InsideACOM/Lessons/Lesson02/Eco.CalculatorA
ECO_FRAMEWORK ?= $(HOME)/Downloads/Eco_labs/Eco.Framework
CC := clang
AR := ar
COMP_ID := 48749720D2434B819DCFE025D17DEB17
ARCH := arm64
BUILDDIR := $(ROOT)/BuildFiles/Mac/$(ARCH)
OBJDIR := $(BUILDDIR)/Objects
STATIC_DIR := $(BUILDDIR)/StaticRelease
APP_DIR := $(BUILDDIR)/UnitTest
LIBRARY := $(STATIC_DIR)/lib$(COMP_ID).a
UNITTEST := $(APP_DIR)/EcoMaxMin
CALC_LIBRARY := $(STATIC_DIR)/libEcoCalculatorA.a

CORE := $(ECO_FRAMEWORK)/Eco.Core1/SharedFiles
BUS := $(ECO_FRAMEWORK)/Eco.InterfaceBus1/SharedFiles
MEM := $(ECO_FRAMEWORK)/Eco.MemoryManager1/SharedFiles
FS := $(ECO_FRAMEWORK)/Eco.FileSystemManagement1/SharedFiles
SYS := $(CORE)

SYS_LIB := $(ECO_FRAMEWORK)/Eco.System1/BuildFiles/Mac/$(ARCH)/StaticRelease/lib00000000000000000000000053595333.a
FS_LIB  := $(ECO_FRAMEWORK)/Eco.FileSystemManagement1/BuildFiles/Mac/$(ARCH)/StaticRelease/lib00000000000000000000000046534D31.a
BUS_LIB := $(ECO_FRAMEWORK)/Eco.InterfaceBus1/BuildFiles/Mac/$(ARCH)/StaticRelease/lib00000000000000000000000042757331.a
MEM_LIB := $(ECO_FRAMEWORK)/Eco.MemoryManager1/BuildFiles/Mac/$(ARCH)/StaticRelease/lib0000000000000000000000004D656D31.a

INCLUDES := -I"$(ROOT)/HeaderFiles" -I"$(ROOT)/SharedFiles" \
            -I"$(CORE)" -I"$(BUS)" -I"$(MEM)" -I"$(FS)" -I"$(SYS)" \
            -I"$(CALC_A)/HeaderFiles" -I"$(CALC_A)/SharedFiles"
DEFINES := -DECO_LIB -DECO_APPLE -DECO_MAC -DECO_AARCH64 -DUGUID_UTILITY -DERR_ECO_SUCCESES=ERR_ECO_SUCCESS
CFLAGS := -arch $(ARCH) -std=c89 -Wall -Wextra -Wno-unused-parameter -O2 $(DEFINES) $(INCLUDES)

OBJ_MAIN := $(OBJDIR)/CEcoMaxMin.o
OBJ_FACTORY := $(OBJDIR)/CEcoMaxMinFactory.o
OBJ_TEST := $(OBJDIR)/EcoMaxMinTest.o
OBJ_CALC_A := $(OBJDIR)/CEcoCalculatorA.o
OBJ_CALC_A_FACTORY := $(OBJDIR)/CEcoCalculatorAFactory.o

.PHONY: all test check-env clean help
all: $(UNITTEST)

help:
	@echo 'Use make all or make test. Required: ECO_FRAMEWORK points to Eco.Framework.'

check-env:
	@for f in "$(CORE)/IEcoBase1.h" "$(BUS)/IEcoInterfaceBus1.h" \
	  "$(MEM)/IEcoMemoryAllocator1.h" "$(SYS)/IEcoSystem1.h" \
	  "$(SYS_LIB)" "$(FS_LIB)" "$(BUS_LIB)" "$(MEM_LIB)" \
	  "$(CALC_A)/SharedFiles/IEcoCalculatorX.h" \
	  "$(CALC_A)/SharedFiles/IdEcoCalculatorA.h" \
	  "$(CALC_A)/SourceFiles/CEcoCalculatorA.c" \
	  "$(CALC_A)/SourceFiles/CEcoCalculatorAFactory.c"; do \
	  if [ ! -f "$$f" ]; then echo "MISSING: $$f"; exit 1; fi; \
	done
	@echo 'Eco.Framework headers and macOS arm64 libraries: present'

$(OBJDIR) $(STATIC_DIR) $(APP_DIR):
	@mkdir -p "$@"

$(OBJ_MAIN): $(ROOT)/SourceFiles/CEcoMaxMin.c $(ROOT)/HeaderFiles/CEcoMaxMin.h $(ROOT)/SharedFiles/IEcoMaxMin.h $(CALC_A)/SharedFiles/IEcoCalculatorX.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c "$<" -o "$@"

$(OBJ_FACTORY): $(ROOT)/SourceFiles/CEcoMaxMinFactory.c $(ROOT)/HeaderFiles/CEcoMaxMinFactory.h $(ROOT)/HeaderFiles/CEcoMaxMin.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c "$<" -o "$@"

$(OBJ_TEST): $(ROOT)/UnitTestFiles/SourceFiles/EcoMaxMin.c $(ROOT)/SharedFiles/IEcoMaxMin.h $(CALC_A)/SharedFiles/IdEcoCalculatorA.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c "$<" -o "$@"

$(OBJ_CALC_A): $(CALC_A)/SourceFiles/CEcoCalculatorA.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c "$<" -o "$@"

$(OBJ_CALC_A_FACTORY): $(CALC_A)/SourceFiles/CEcoCalculatorAFactory.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c "$<" -o "$@"

$(CALC_LIBRARY): $(OBJ_CALC_A) $(OBJ_CALC_A_FACTORY) | $(STATIC_DIR)
	$(AR) rcs "$@" $(OBJ_CALC_A) $(OBJ_CALC_A_FACTORY)

$(LIBRARY): $(OBJ_MAIN) $(OBJ_FACTORY) | $(STATIC_DIR)
	$(AR) rcs "$@" $(OBJ_MAIN) $(OBJ_FACTORY)

$(UNITTEST): $(OBJ_TEST) $(LIBRARY) $(CALC_LIBRARY) | $(APP_DIR)
	$(CC) -arch $(ARCH) -o "$@" $(OBJ_TEST) $(LIBRARY) $(CALC_LIBRARY) \
	  "$(BUS_LIB)" "$(MEM_LIB)" "$(FS_LIB)" "$(SYS_LIB)" -framework Foundation -framework Cocoa

test: check-env $(UNITTEST)
	@"$(UNITTEST)"; result=$$?; \
	  if [ "$$result" -eq 0 ]; then echo 'PASS: Eco.MaxMin + contained Eco.CalculatorA UnitTest'; \
	  else echo "FAIL: EcoMain exit code $$result"; exit "$$result"; fi

clean:
	rm -rf "$(BUILDDIR)"
