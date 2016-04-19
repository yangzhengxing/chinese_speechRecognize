AM_CXXFLAGS = -Wall
CXXFLAGS += -O2


host_cpu=`uname -p`


bin_PROGRAMS = ekho_cmd

ekho_cmd_SOURCES = src/ekho_cmd.cpp
ekho_cmd_LDADD = libekho.a $(LIBEKHO_LIB)

