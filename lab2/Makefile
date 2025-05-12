# Compiler 변수 설정: g++ 사용
CXX=g++

# 컴파일러 플래그 설정
CXXFLAGS = -std=c++17 -g 

LDFLAGS = -lpthread /usr/lib/libgtest.a /usr/lib/libgtest_main.a 

# Source files
SOURCES = test.cpp skiplist_impl.cpp test_util.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
EXECUTABLE = test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
