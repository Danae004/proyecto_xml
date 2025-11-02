CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g
TARGET = xml_library.exe
SOURCES = XmlDoc.cpp XmlNode.cpp main.cpp
HEADERS = XmlDoc.hpp XmlNode.hpp
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q $(OBJECTS) $(TARGET) test_output.xml test_output_min.xml 2>nul

run: $(TARGET)
	$(TARGET)

.PHONY: clean run