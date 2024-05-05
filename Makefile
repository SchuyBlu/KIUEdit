# Author: Schuy
# Makefile for building tests.

PATHI=include/
PATHU=unity/src/
PATHS=src/
PATHT=test/
PATHB=build/test/
PATHO=build/test/objects/
PATHR=build/test/results/

BUILD_PATHS=$(PATHB) $(PATHO) $(PATHR)

TSRC=$(wildcard $(PATHT)*.c)
PSRC=$(wildcard $(PATHS)*.c)

# Needed to create results files
RESULTS=$(patsubst $(PATHT)Test_%.c,$(PATHR)Test_%.txt,$(TSRC))

COMPILE=gcc -c -g
LINK=gcc -g
LIBLINK=-l json-c
DEFINES=-D TEST_BUILD
CFLAGS=-I. -I$(PATHI) -I$(PATHU)
SANITIZE=-fsanitize=address

# Define phony so make doesn't attempt to build
.PHONY: test
.PHONY: clean

# Define precious files, as Unity clears temporary files when executable runs
.PRECIOUS: $(PATHB)Test_%.out
.PRECIOUS: $(PATHR)%.txt
.PRECIOUS: $(PATHO)%.o

# Build 3DS files.
3ds:
	@make -f Makefile.3ds

# Checks test result files and outputs failures and ignores
test: $(BUILD_PATHS) $(RESULTS)
	@echo "--------------------\nIGNORES:\n--------------------"
	@echo `grep -s IGNORE $(PATHR)*.txt`
	@echo "--------------------\nFAILURES:\n--------------------"
	@echo `grep -s FAIL $(PATHR)*.txt`
	@echo "--------------------\nADDRESS SANITIZER:\n--------------------"
	@echo "$$(awk '/==.*==ERROR/,/allocation(s)./' $(PATHR)*.txt)"
	@echo "DONE"

$(PATHR)%.txt: $(PATHB)%.out
	-./$< > $@ 2>&1

# Define Test output rules for executables that have more than
# one dependency
$(PATHB)Test_file.out: $(PATHO)Test_file.o $(PATHO)const.o $(PATHO)file.o $(PATHO)weapons.o $(PATHO)unity.o
	$(LINK) $(SANITIZE) -o $@ $^

$(PATHB)Test_%.out: $(PATHO)Test_%.o $(PATHO)%.o $(PATHO)unity.o
	$(LINK) $(SANITIZE) -o $@ $^

# Build test object files
$(PATHO)%.o:: $(PATHT)%.c
	$(COMPILE) $(SANITIZE) $(CFLAGS) $< -o $@

# Build src object files
$(PATHO)%.o:: $(PATHS)%.c
	$(COMPILE) $(SANITIZE) $(CFLAGS) $(DEFINES) $< -o $@

# Build unity object files
$(PATHO)%.o:: $(PATHU)%.c
	$(COMPILE) $(SANITIZE) $(CFLAGS) $(DEPFLAGS) $< -o $@

# Make the necessary directories to run test if they don't exist
$(PATHB):
	mkdir -p $(PATHB)

$(PATHO):
	mkdir -p $(PATHO)

$(PATHR):
	mkdir -p $(PATHR)

clean:
	@make -sf Makefile.3ds clean
	rm -f $(PATHO)*.o
	rm -f $(PATHB)*.out
	rm -f $(PATHR)*.txt
