# Build settings.
CXX ?= g++
CXXFLAGS ?= -std=c++17 -march=native -Wall
CXXFLAGS += $(INCLUDE_SCRIPT)
LD_INPUT = -lstdc++fs
RELEASE ?= 0
PROFILE ?= 0

# Install settings.
PREFIX ?= ~/.local

OUTPUT = figure

# Check if we are debugging.
ifeq ($(RELEASE), 0)
	CXXFLAGS += -fverbose-asm -ggdb -O0
else
	CXXFLAGS += -O2 -DNDEBUG # -flto
endif

# Check if we are profiling.
ifeq ($(PROFILE), 1)
	CXXFLAGS += -pg
endif

# Build list of sources.
SOURCES = $(shell find . -name "*.cpp")

# Build list of headers.
HEADERS = $(shell find . -name "*.h" -not -path "./build/*")

# Build list of object file targets.
OBJECTS = $(addprefix build/, $(SOURCES:.cpp=.o))
ASM = $(OBJECTS:.o=.s)

# Build list of stub generated targets.
UE_GENERATED = $(addprefix build/generated/, $(HEADERS:.h=.generated.h))

# Build list of compiler-generated dependencies.
DEPS = $(OBJECTS:.o=.d)

# Directories to include in search paths.
INCLUDE_DIRS = . NoUE build/generated
INCLUDE_SCRIPT = $(addprefix -I, $(INCLUDE_DIRS))

# Create stub symbolic link.
$(shell rm -rf ./Figure)
$(shell ln -s . ./Figure)

# Default target.
.PHONY: default
default: all

.PHONY: all
all: build doc

.PHONY: install
install: all
	install -d $(PREFIX)/bin
	install $(OUTPUT) $(PREFIX)/bin

.PHONY: uninstall
uninstall:
	rm $(PREFIX)/bin/$(OUTPUT)

# Remove all generated output.
.PHONY: clean
clean:
	-rm -rf $(OUTPUT) build/ html/ latex/

# Only clean generated dependencies.
.PHONY: cleandep
cleandep:
	-find ./build -name "*.d" -delete

# Rebuild whole project.
.PHONY: remake
remake:
	make clean
	make all

# Build and debug interactive input.
.PHONY: gdb
gdb: $(OUTPUT)
	gdb -tui ./$(OUTPUT)

# Build and debug test file.
.PHONY: test
test: $(OUTPUT)
	gdb -tui --args ./$(OUTPUT) test/test.scm build/test.cc

# Build and run test file.
.PHONY: run
run: $(OUTPUT)
	./$(OUTPUT) test/test.scm build/test.cc

# Code::Blocks targets.
.PHONY: Debug
Debug: $(OUTPUT) doc
	mkdir -p Debug
	cp $(OUTPUT) ./Debug/$(OUTPUT)

.PHONY: Release
Release: $(OUTPUT) doc
	mkdir -p Release
	cp $(OUTPUT) ./Release/$(OUTPUT)

.PHONY: build
build: $(OUTPUT)

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
	cloc . --exclude-dir=html,latex,build,test --not-match-f=json.h

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

$(OUTPUT): $(UE_GENERATED) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(OUTPUT) $(OBJECTS) $(LD_INPUT)

# Include generated dependencies.
-include $(DEPS)

# Compile generated targets.
build/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MD -o $@ -c $<

# Touch stub generated targets.
build/generated/%.generated.h: %.h
	mkdir -p $(@D)
	touch $@

# Export generated assembly.
build/%.s: build/%.o
	mkdir -p $(@D)
	objdump --no-show-raw-insn -drwSC $< > $@
