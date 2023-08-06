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

# Needed to create results files
RESULTS=$(patsubst $(PATHT)Test_%.c,$(PATHR)Test_%.txt,$(TSRC))

COMPILE=gcc -c
LINK=gcc
CFLAGS=-I. -I$(PATHI) -I$(PATHU)

# Define phony so make doesn't attempt to build
.PHONY: test
.PHONY: clean

# Define precious files, as Unity clears temporary files when executable runs
.PRECIOUS: $(PATHB)Test_%.out
.PRECIOUS: $(PATHR)%.txt
.PRECIOUS: $(PATHO)%.o

# Checks test result files and outputs failures and ignores
test: $(BUILD_PATHS) $(RESULTS)
	@echo -e "--------------------\nIGNORES:\n--------------------"
	@echo `grep -s IGNORE $(PATHR)*.txt`
	@echo -e "--------------------\nFAILURES:\n--------------------"
	@echo `grep -s FAIL $(PATHR)*.txt`

$(PATHR)%.txt: $(PATHB)%.out
	-./$< > $@ 2>&1

$(PATHB)Test_%.out: $(PATHO)Test_%.o $(PATHO)%.o $(PATHO)unity.o
	$(LINK) -o $@ $^

# Build test object files
$(PATHO)%.o:: $(PATHT)%.c
	$(COMPILE) $(CFLAGS) $< -o $@

# Build src object files
$(PATHO)%.o:: $(PATHS)%.c
	$(COMPILE) $(CFLAGS) $< -o $@

# Build unity object files
$(PATHO)%.o:: $(PATHU)%.c
	$(COMPILE) $(CFLAGS) $< -o $@

# Make the necessary directories to run test if they don't exist
$(PATHB):
	mkdir -p $(PATHB)

$(PATHO):
	mkdir -p $(PATHO)

$(PATHR):
	mkdir -p $(PATHR)

clean:
	rm -f $(PATHO)*.o
	rm -f $(PATHB)*.out
	rm -f $(PATHR)*.txt

