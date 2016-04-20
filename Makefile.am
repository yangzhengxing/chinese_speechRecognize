AM_CXXFLAGS = -Wall

host_cpu=`uname -p`

lib_LIBRARIES = libekho.a
bin_PROGRAMS = ekho_cmd

libekho_a_SOURCES = src/ekho.cpp src/ekho_dict.cpp src/number.cpp src/zhy_symbol_map.cpp src/zh_symbol_map.cpp src/symbol_array.cpp
libekho_a_CXXFLAGS = -I$(srcdir)/include -I$(srcdir)/include/utfcpp/source -pthread

ekho_cmd_SOURCES = src/ekho_cmd.cpp
ekho_cmd_CXXFLAGS = -I$(srcdir)/include
ekho_cmd_LDADD = libekho.a -lsonic -lsndfile

