# Build settings.
CXX ?= g++
CXXFLAGS ?= -std=c++11 -march=native -Wall
CXXFLAGS += $(INCLUDE_SCRIPT)
LD_INPUT = -lstdc++fs
RELEASE ?= 0
PROFILE ?= 0

# Check if we are debugging.
ifeq ($(RELEASE), 0)
	CXXFLAGS += -fverbose-asm -ggdb -O0
else
	CXXFLAGS += -O2 -DNDEBUG -flto
endif

# Check if we are profiling.
ifeq ($(PROFILE), 1)
	CXXFLAGS += -pg
endif

# Build list of sources.
SOURCES = $(shell find . -name "*.cc")

# Build list of headers.
HEADERS = $(shell find . -name "*.h")

# Build list of object file targets.
OBJECTS = $(addprefix build/, $(SOURCES:.cc=.o))
ASM = $(OBJECTS:.o=.s)

# Build list of compiler-generated dependencies.
DEPS = $(OBJECTS:.o=.d)

# Directories to include in search paths.
INCLUDE_DIRS = .
INCLUDE_SCRIPT = $(addprefix -I, $(INCLUDE_DIRS))

# Default target.
.PHONY: all
all: build doc

# Remove all generated output.
.PHONY: clean
clean:
	-rm -rf figure build/ html/ latex/

# Only clean generated dependencies.
.PHONY: cleandep
cleandep:
	-find ./build -name "*.d" -delete

# Rebuild whole project.
.PHONY: remake
remake:
	make clean
	make all

.PHONY: build
build: figure

.PHONY: doc
doc: html/index.html

# Export generated assembly.
.PHONY: asm
asm: $(ASM)

# Generate documentation.
html/index.html: Doxyfile $(SOURCES) $(HEADERS)
ifneq (, $(shell which doxygen))
	doxygen Doxyfile
endif

# Print lines of code.
.PHONY: cloc
cloc:
	cloc . --exclude-dir=html,latex,build --not-match-f=json.h

# Analyze codebase with cppcheck. Only runs if cppcheck is in PATH.
.PHONY: cppcheck
cppcheck:
ifneq (, $(shell which cppcheck))
	cppcheck -j 2 -ivendor -q --enable=warning,portability $(INCLUDE_SCRIPT) .
endif

# Beautify code.
.PHONY: astyle
astyle:
	astyle $(SOURCES) $(HEADERS)

figure: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o figure $(OBJECTS) $(LD_INPUT)

# Include generated dependencies.
-include $(DEPS)

# Compile generated targets.
build/%.o: %.cc
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MD -o $@ -c $<

# Export generated assembly.
build/%.s: build/%.o
	mkdir -p $(@D)
	objdump --no-show-raw-insn -drwSC $< > $@
