PROGRAM = supportpp

INCLUDEDIRS = \
	-I/usr/local/include

LIBDIRS = \
	-L/usr/local/lib

LIBS = -ltomcrypt -ltommath -lcurl

CXXSOURCES = main.cc 
CXXREMOVE = main.o
CXXOBJECTS = $(CXXSOURCES:.cc=.o)  # expands to list of object files
CXXFLAGS = -DESRI_UNIX $(INCLUDEDIRS) -DUSE_LTM -DLTM_DESC -lstdc++fs
CXX = g++ -std=c++17

LDFLAGS = $(LIBDIRS) $(LIBS)

main: $(PROGRAM)

$(PROGRAM): $(CXXOBJECTS) 
	$(CXX) -o $@ $(CXXOBJECTS) $(CXXFLAGS) $(LDFLAGS)

clean:
	$(RM) -f $(CXXREMOVE) $(PROGRAM)