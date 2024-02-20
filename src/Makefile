CXX=g++
CXXFLAGS=-std=c++17 -c -Wall -Werror -Wextra 
GCOVFLAGS =--coverage
LDFLAGS=-lgtest
SOURCES=support/s21_graph.cc \
		support/s21_matrix.cc \
		ant/ant_colony.cc \
		gauss/gauss.cc \
		winograd/winograd.cc \
		ant/ant_interface.cc \
		gauss/gauss_interface.cc \
		winograd/winograd_interface.cc
SRCTEST=support/s21_graph.cc \
		support/s21_matrix.cc \
		ant/ant_colony.cc \
		gauss/gauss.cc \
		winograd/winograd.cc
HEADERS=support/s21_graph.h\
		support/s21_matrix.h\
		support/s21_simple_timer.h\
		support/s21_screen.h\
		ant/ant_colony.h\
		ant/ant_interface.h\
		gauss/gauss.h\
		gauss/gauss_interface.h\
		winograd/winograd.h\
		winograd/winograd_interface.h
OBJECTS=$(SOURCES:.cc=.o)

OS=$(shell uname)
ifeq ($(OS), Linux)
LEAKS=CK_FORK=no valgrind -s --tool=memcheck --leak-check=full --show-leak-kinds=all --show-reachable=yes ./test.out
OPEN=xdg-open
else
LEAKS=leaks --atExit -- ./test.out
OPEN=open
endif

all: $(OBJECTS)
	cp examples/*.txt .
	$(CXX) main.cc $(SOURCES) -o main.out
	./main.out
	$(RM) main.out *.txt

ant: $(OBJECTS)
	cp examples/v*.txt . 
	$(CXX) ./ant/ant_main.cc $(SOURCES) -o ./ant_main.out
	./ant_main.out
	$(RM) ./ant_main.out v*.txt

gauss: $(OBJECTS)
	cp examples/g*.txt . 
	$(CXX) ./gauss/gauss_main.cc $(SOURCES) -o gauss_main.out
	./gauss_main.out
	$(RM) gauss_main.out g*.txt

winograd: $(OBJECTS)
	cp examples/m*.txt . 
	$(CXX) ./winograd/winograd_main.cc $(SOURCES) -o winograd_main.out
	./winograd_main.out
	$(RM) winograd_main.out m*.txt

test: $(OBJECTS)
	$(CXX) test.cc $(SRCTEST) -o test.out $(LDFLAGS)
	./test.out

gcov_report: 
	$(CXX) test.cc $(SRCTEST) $(GCOVFLAGS) -o gcov_rep.out $(LDFLAGS) 
	./gcov_rep.out
	lcov -c -d . -o coverage.info --no-external #--ignore-errors mismatch
	genhtml -o ./report coverage.info
	# $(OPEN) ./report/index.html
	$(RM) *.out *.gcno *.gcda *.gcov *.info

clean:
	rm -rf ./report
	$(RM) ant/*.o gauss/*.o winograd/*.o support/*.o
	$(RM) *.o *.out *.gcno *.gcda *.gcov *.info *.dSYM *.a *.dot *.txt

style:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i $(SOURCES) $(HEADERS)
	clang-format -n $(SOURCES) $(HEADERS)
	$(RM) .clang-format

cppcheck:
	cppcheck --language=c++ --std=c++17 --suppress=unusedFunction \
	--suppress=missingInclude $(SOURCES) $(HEADERS)

leaks: test
	$(LEAKS)

CH: style cppcheck
