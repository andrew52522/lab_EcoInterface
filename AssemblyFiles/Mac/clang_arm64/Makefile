# Eco Lab 2: ACOM containment / aggregation demonstrations, Apple Silicon.
ROOT := $(abspath ../../..)
EDUCATION ?= $(abspath $(ROOT)/../Eco.Education)
LESSONS := $(EDUCATION)/Eco.Pro.Training/001.InsideACOM/Lessons
A := $(LESSONS)/Lesson02/Eco.CalculatorA
B := $(LESSONS)/Lesson03/Eco.CalculatorB
C := $(LESSONS)/Lesson05/Eco.CalculatorC
D := $(LESSONS)/Lesson06/Eco.CalculatorD
E := $(LESSONS)/Lesson07/Eco.CalculatorE
ECO_FRAMEWORK ?= $(HOME)/Downloads/Eco_labs/Eco.Framework
CC := clang
AR := ar
MODE ?= A
ARCH := arm64
BUILDDIR := $(ROOT)/BuildFiles/Mac/$(ARCH)/Lab2_$(MODE)
OBJDIR := $(BUILDDIR)/Objects
LIBRARY := $(BUILDDIR)/libEcoMaxMin.a
CALC_LIBRARY := $(BUILDDIR)/libEcoLegacyCalculators.a
UNITTEST := $(BUILDDIR)/EcoMaxMinTest
CORE := $(ECO_FRAMEWORK)/Eco.Core1/SharedFiles
BUS := $(ECO_FRAMEWORK)/Eco.InterfaceBus1/SharedFiles
MEM := $(ECO_FRAMEWORK)/Eco.MemoryManager1/SharedFiles
FS := $(ECO_FRAMEWORK)/Eco.FileSystemManagement1/SharedFiles
SYS_LIB := $(ECO_FRAMEWORK)/Eco.System1/BuildFiles/Mac/$(ARCH)/StaticRelease/lib00000000000000000000000053595333.a
FS_LIB := $(ECO_FRAMEWORK)/Eco.FileSystemManagement1/BuildFiles/Mac/$(ARCH)/StaticRelease/lib00000000000000000000000046534D31.a
BUS_LIB := $(ECO_FRAMEWORK)/Eco.InterfaceBus1/BuildFiles/Mac/$(ARCH)/StaticRelease/lib00000000000000000000000042757331.a
MEM_LIB := $(ECO_FRAMEWORK)/Eco.MemoryManager1/BuildFiles/Mac/$(ARCH)/StaticRelease/lib0000000000000000000000004D656D31.a
INCLUDES := -I"$(ROOT)/HeaderFiles" -I"$(ROOT)/SharedFiles" -I"$(CORE)" -I"$(BUS)" -I"$(MEM)" -I"$(FS)" \
  -I"$(A)/HeaderFiles" -I"$(A)/SharedFiles" -I"$(B)/HeaderFiles" -I"$(B)/SharedFiles" \
  -I"$(C)/HeaderFiles" -I"$(C)/SharedFiles" -I"$(D)/HeaderFiles" -I"$(D)/SharedFiles" \
  -I"$(E)/HeaderFiles" -I"$(E)/SharedFiles"
DEFINES := -DECO_LIB -DECO_APPLE -DECO_MAC -DECO_AARCH64 -DUGUID_UTILITY -DERR_ECO_SUCCESES=ERR_ECO_SUCCESS
CFLAGS := -arch $(ARCH) -std=c89 -Wall -Wextra -Wno-unused-parameter -O2 $(DEFINES) $(INCLUDES)
ifeq ($(MODE),A)
BACKEND := A
BACKEND_FLAG := -DLAB2_BACKEND_A
else ifeq ($(MODE),C)
BACKEND := C
BACKEND_FLAG := -DLAB2_BACKEND_C
else ifeq ($(MODE),D)
BACKEND := D
BACKEND_FLAG := -DLAB2_BACKEND_D
else
$(error MODE must be A, C or D)
endif
CFLAGS += $(BACKEND_FLAG)
CALCS := B D E
ifneq ($(MODE),D)
CALCS += $(MODE)
endif
MAIN_OBJ := $(OBJDIR)/CEcoMaxMin.o
FACT_OBJ := $(OBJDIR)/CEcoMaxMinFactory.o
TEST_OBJ := $(OBJDIR)/EcoMaxMinTest.o
CALC_OBJS := $(foreach x,$(CALCS),$(OBJDIR)/CEcoCalculator$(x).o $(OBJDIR)/CEcoCalculator$(x)Factory.o)
.PHONY: all test test-a test-c test-d check-env clean verify-mode
.NOTPARALLEL: test
all: $(UNITTEST)
check-env:
	@for f in "$(CORE)/IEcoBase1.h" "$(BUS)/IEcoInterfaceBus1.h" "$(MEM)/IEcoMemoryAllocator1.h" \
	"$(CORE)/IEcoSystem1.h" "$(SYS_LIB)" "$(FS_LIB)" "$(BUS_LIB)" "$(MEM_LIB)" \
	"$(A)/SharedFiles/IdEcoCalculatorA.h" "$(B)/SharedFiles/IdEcoCalculatorB.h" \
	"$(C)/SharedFiles/IdEcoCalculatorC.h" "$(D)/SharedFiles/IdEcoCalculatorD.h" "$(E)/SharedFiles/IdEcoCalculatorE.h"; do \
	if [ ! -f "$$f" ]; then echo "MISSING: $$f"; exit 1; fi; done
	@echo "[OK] Eco.Framework and InsideACOM headers present"
$(OBJDIR):
	@mkdir -p "$@"
$(BUILDDIR):
	@mkdir -p "$@"
$(MAIN_OBJ): $(ROOT)/SourceFiles/CEcoMaxMin.c $(ROOT)/HeaderFiles/CEcoMaxMin.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c "$<" -o "$@"
$(FACT_OBJ): $(ROOT)/SourceFiles/CEcoMaxMinFactory.c $(ROOT)/HeaderFiles/CEcoMaxMinFactory.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c "$<" -o "$@"
$(TEST_OBJ): $(ROOT)/UnitTestFiles/SourceFiles/EcoMaxMin.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c "$<" -o "$@"
# Compile the upstream teaching sources from a temporary C89-compatible copy.
# This removes their obsolete // factory comment (instead of hiding the warning).
define CALC_RULES
$(OBJDIR)/CEcoCalculator$(1).o: $$($(1))/SourceFiles/CEcoCalculator$(1).c | $(OBJDIR)
	$(CC) $(CFLAGS) -c "$$<" -o "$$@"
$(OBJDIR)/CEcoCalculator$(1)Factory.o: $$($(1))/SourceFiles/CEcoCalculator$(1)Factory.c | $(OBJDIR)
	@sed 's@//deleteCEcoCalculator$(1)Factory(&pCMe->m_VtblICF);@/* old unused call */@' "$$<" > "$(OBJDIR)/CEcoCalculator$(1)Factory.c"
	$(CC) $(CFLAGS) -c "$(OBJDIR)/CEcoCalculator$(1)Factory.c" -o "$$@"
endef
$(foreach x,A B C D E,$(eval $(call CALC_RULES,$(x))))
$(LIBRARY): $(MAIN_OBJ) $(FACT_OBJ) | $(BUILDDIR)
	$(AR) rcs "$@" $(MAIN_OBJ) $(FACT_OBJ)
$(CALC_LIBRARY): $(CALC_OBJS) | $(BUILDDIR)
	$(AR) rcs "$@" $(CALC_OBJS)
$(UNITTEST): $(TEST_OBJ) $(LIBRARY) $(CALC_LIBRARY) | $(BUILDDIR)
	$(CC) -arch $(ARCH) -o "$@" $(TEST_OBJ) $(LIBRARY) $(CALC_LIBRARY) \
	"$(BUS_LIB)" "$(MEM_LIB)" "$(FS_LIB)" "$(SYS_LIB)" -framework Foundation -framework Cocoa
verify-mode: check-env $(UNITTEST)
	@"$(UNITTEST)"; code=$$?; \
	if [ "$$code" -ne 0 ]; then echo "[FAIL] mode=$(MODE) unit test exit code $$code"; exit "$$code"; fi
	@echo "[PASS] mode=$(MODE): Max/Min, X/Y, QueryInterface, D contains B, E aggregates B"
test-a:
	@$(MAKE) --no-print-directory MODE=A verify-mode
test-c:
	@$(MAKE) --no-print-directory MODE=C verify-mode
test-d:
	@$(MAKE) --no-print-directory MODE=D verify-mode
test: test-a test-c test-d
	@echo '=================================================='
	@echo 'PASS: Lab 2 ACOM / A B C D E / containment + aggregation'
	@echo 'PASS: all 3 build variants and their UnitTests'
	@echo '=================================================='
clean:
	rm -rf "$(ROOT)/BuildFiles/Mac/$(ARCH)/Lab2_A" "$(ROOT)/BuildFiles/Mac/$(ARCH)/Lab2_C" "$(ROOT)/BuildFiles/Mac/$(ARCH)/Lab2_D"
