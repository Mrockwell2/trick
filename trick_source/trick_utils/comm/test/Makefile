
include ${TRICK_HOME}/share/trick/makefiles/Makefile.common
include ${TRICK_HOME}/share/trick/makefiles/Makefile.tricklib

INCLUDE_DIRS   = -I$(GTEST_HOME) -I$(GTEST_HOME)/include -I$(TRICK_HOME)/include

# Use the trick_comm library only.  libtrick.a would also work.
TRICK_LIBS     := ${TRICK_LIB_DIR}/libtrick_comm.a
TRICK_EXEC_LINK_LIBS += -L${GTEST_HOME}/lib64 -L${GTEST_HOME}/lib -lgtest -lgtest_main
TRICK_CXXFLAGS += ${INCLUDE_DIRS} -g -Wall -Wextra ${TRICK_SYSTEM_CXXFLAGS} ${TRICK_TEST_FLAGS}

# Automatically determine all executable names produced by this Makefile.
TESTS = comm_test
TEST_CPP =  $(subst $(OBJ_DIR)/,,$(subst .o,.cpp,$(CPP_OBJS)))

default : all

all : objects $(TESTS)

# Set XML test results name
test : all
	@ for i in $(TESTS) ; do \
         ./$$i --gtest_output=xml:${TRICK_HOME}/trick_test/TrickComm.xml; \
      done

clean : clean_test

clean_test :
	$(RM) -rf $(TESTS)

$(TESTS) : $(CPP_OBJS)
	$(TRICK_CXX) $(TRICK_CXXFLAGS) -o $@ $^ $(TRICK_LIBS) $(TRICK_EXEC_LINK_LIBS)

