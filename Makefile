# ─────────────────────────────────────────────────────────────────────────────
#  Makefile  –  Student Management System
#  Usage:  make          → compile
#          make run      → compile + run
#          make clean    → remove build artifacts
#          make debug    → compile with debug symbols
# ─────────────────────────────────────────────────────────────────────────────

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
DEBUG    = -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined

TARGET  = student_manager
SRCS    = main.cpp StudentManager.cpp
OBJS    = $(SRCS:.cpp=.o)
HEADERS = Student.h GradedStudent.h LinkedList.h HashTable.h StudentManager.h

# ── Default target ─────────────────────────────────────────────────────────────
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo ""
	@echo "  Build successful → ./$(TARGET)"
	@echo ""

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ── Debug build ────────────────────────────────────────────────────────────────
debug: $(SRCS) $(HEADERS)
	$(CXX) $(DEBUG) -o $(TARGET)_debug $(SRCS)
	@echo "  Debug build → ./$(TARGET)_debug"

# ── Run ────────────────────────────────────────────────────────────────────────
run: all
	./$(TARGET)

# ── Clean ──────────────────────────────────────────────────────────────────────
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET)_debug
	@echo "  Cleaned."

.PHONY: all debug run clean
