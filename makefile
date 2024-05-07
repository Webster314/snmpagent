objects=	bin/agent.o \
			bin/ucd_snmp_mib.o \
			bin/host_resources_mib.o \
			bin/ucd_diskio_mib.o \
			bin/if_mib.o

CXX = g++
CXXFLAGS = -Wall
INCLUDES = -Iinclude
LDFLAGS = -lsnmp++ -lagent++

#link
agent: $(objects)
	$(CXX) $(objects) -o ./bin/agent -Wall $(LDFLAGS)
	@echo make done!

run: agent 
	sudo ./bin/agent

# compile
bin/ucd_snmp_mib.o: src/ucd_snmp_mib.cc include/ucd_diskio_mib.h
	$(CXX) -c src/ucd_snmp_mib.cc $(CXXFLAGS) $(INCLUDES) -o bin/ucd_snmp_mib.o

bin/ucd_diskio_mib.o: src/ucd_diskio_mib.cc include/ucd_diskio_mib.h
	$(CXX) -c src/ucd_diskio_mib.cc $(CXXFLAGS) $(INCLUDES) -o bin/ucd_diskio_mib.o

bin/host_resources_mib.o: src/host_resources_mib.cc include/host_resources_mib.h
	$(CXX) -c src/host_resources_mib.cc $(CXXFLAGS) $(INCLUDES) -o bin/host_resources_mib.o

bin/if_mib.o: src/if_mib.cc include/if_mib.h
	$(CXX) -c src/if_mib.cc $(CXXFLAGS) $(INCLUDES) -o bin/if_mib.o

bin/agent.o: src/agent.cc \
			include/ucd_snmp_mib.h include/ucd_diskio_mib.h include/host_resources_mib.h # include/ip_mib.h
	$(CXX) -c src/agent.cc $(CXXFLAGS) $(INCLUDES) -o bin/agent.o

.PHONY: clean
clean:
	rm -f bin/*.o bin/agent