CXX = g++
CXXFLAGS = -g -Wall -L.
RM = clean

LIB_OBJS = string.cpp field.cpp port.cpp ip.cpp
MAIN_OBGS = main.cpp
EXEC = firewall.exe
EXEC_LIB = firewall

all::firewall.exe

firewall.exe: firewall
	$(CXX) $(CXXFLAGS) main.cpp -o firewall.exe -lfirewall -linput

firewall:
	$(CXX) $(CXXFLAGS) -fPIC -shared $(LIB_OBJS) -o libfirewall.so

$(RM):
	rm -rf *.o $(EXEC) $(EXEC_LIB)