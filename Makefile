VERTICA_SDK=/opt/vertica/sdk

CXXFLAGS=-I $(VERTICA_SDK)/include -fPIC -Wall

all: liboverflowops.so

Vertica.o: $(VERTICA_SDK)/include/Vertica.cpp
	$(CXX) -c $(CXXFLAGS) $?

overflowops.o: overflowops.cpp
	$(CXX) -c $(CXXFLAGS) $?

liboverflowops.so: Vertica.o overflowops.o
	$(CXX) $(CXXFLAGS) -shared -o $@ $?

clean:
	rm -f *.o *.so

install: liboverflowops.so
	cp $^ /tmp
	/opt/vertica/bin/vsql -U vertica -f reinstall.sql

test:
