

.PHONY : help compilation T M pack

## TDM number
TDM_NUMBER := 07

MODULE = evaluation_context exparith exparith_unary exparith_binary exparith_variable loader_evaluator
TEST_NAME := enonce exparith loader_evaluator

SHELL := bash

##
## HELP
##

help : 
	@echo "Available:"
	@echo "- K => compilation (should not produce any error nor warning)"
	@echo "- T1 / M1 => compilation and test (outpout/memory) test_enonce"
	@echo "- T2 / M2 => compilation and test (outpout/memory) test_exparith"
	@echo "- T3 / M3 => compilation and test (outpout/memory) test_loader_evaluator"
	@echo "- T    => all test on output"
	@echo "- M    => all test on memory"
	@echo "- pack => produce the tgz archive"

##
## COMPILATION
##  automatic of  C++ ansi (98) => .hpp .cpp files
##

# Compiler
CCPP := g++

# Compilation options 
CPP98_FLAG_OFF_UNUSED := -Wno-unused-variable -Wno-unused-parameter
CPP98_FLAGS := -std=c++98 -Wall -Wextra -pedantic -ggdb $(CPP98_FLAG_OFF_UNUSED)

#
# COMPILATION RULES
#

# remove implicit rules
%.o : %.c
%.o : %.cpp
% : %.c
% : %.cpp


# actual rules
%.o : %.cpp $(wildcard *.hpp) $(MAKEFILE_LIST)
	$(CCPP) -c $(CPP98_FLAGS) -o $@ $<

test_% : test_%.cpp $(wildcard *.hpp) $(MODULE:%=%.o) $(MAKEFILE_LIST)
	$(CCPP) $(CPP98_FLAGS) -o $@ $(MODULE:%=%.o) $<

% : %.cpp $(wildcard *.hpp) $(MODULE:%=%.o) $(MAKEFILE_LIST)
	$(CCPP) $(CPP98_FLAGS) -o $@ $(MODULE:%=%.o) $<


# compile all
K : $(TEST_NAME:%=test_%)


##
## TEST
##

VALGRIND := valgrind --leak-check=full --show-leak-kinds=all >/dev/null

OUTPUT_SUFFIX := _out.txt
OUTPUT_EXPECTED_SUFFIX := _out_expected.txt

T_% : test_%
	./test_$* > test_$*$(OUTPUT_SUFFIX)
	diff -s -Z test_$*$(OUTPUT_SUFFIX) test_$*$(OUTPUT_EXPECTED_SUFFIX)


## ERROR:     still reachable: 72,704 bytes in 1 blocks
## comes from std
M_% : test_%
	$(VALGRIND) ./test_$* > /dev/null

T1: T_enonce
T2: T_exparith
T3 : T_loader_evaluator

M1: M_enonce
M2: M_exparith
M3 : M_loader_evaluator

T : $(TEST_NAME:%=T_%)

M : $(TEST_NAME:%=M_%)



##
## CLEAN
##

clean:
	rm -f *.o $(TEST_NAME:%=test_%) $(TEST_NAME:%=test_%$(OUTPUT_SUFFIX))


##
## PRODUCE THE ARCHIVE (to upload on Celene)
## 


#
# Pour faire l'archive de remise
#
TGZ_FILES :=  *.cpp *.hpp ?akefile cr.pdf
TDM_DIR := TDM_$(TDM_NUMBER)

pack :
	[ -e cr.pdf ] || echo > cr.pdf
	cd .. ; tar czf $(TDM_DIR)/$(TDM_DIR).tgz $(TGZ_FILES:%=$(TDM_DIR)/%)


