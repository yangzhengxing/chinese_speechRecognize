AM_CXXFLAGS = -Wall

host_cpu=`uname -p`

lib_LIBRARIES = libekho.a
bin_PROGRAMS = ekho_cmd

libekho_a_SOURCES = src/ekho.cpp src/ekho_dict.cpp src/number.cpp src/zhy_symbol_map.cpp src/zh_symbol_map.cpp src/symbol_array.cpp
libekho_a_CXXFLAGS = -I$(srcdir)/common -I$(srcdir)/common/utfcpp/source -I$(srcdir)/include -pthread -O0 -std=c++11

ekho_cmd_SOURCES = sample/ekho_cmd.cpp
ekho_cmd_CXXFLAGS = -I$(srcdir)/common -I$(srcdir)/include -O0 -std=c++11
ekho_cmd_LDADD = libekho.a -lsonic -lsndfile

