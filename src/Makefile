CC := g++-7
clang_format_program := clang-format-3.9
doc_program := doxygen

link_flags := -lglfw -lGLEW -lGL
compile_flags := -O2 -std=c++17
cov_link_flags := $(link_flags) --coverage
cov_compile_flags := --coverage -std=c++17

main := main
tester := testing/tester
target := target
bin_path := $(target)/bin/
cov_path := $(target)/cov/
doc_path := $(target)/doc/

sources := $(main).cpp $(tester).cpp game/game.cpp graphics/graphics.cpp graphics/shader.cpp graphics/keystate.cpp util/bmp.cpp util/file_io.cpp util/error.cpp
objects := $(addprefix $(bin_path), $(sources:.cpp=.o))
depends := $(addprefix $(bin_path), $(sources:.cpp=.d))
cov_objects := $(addprefix $(cov_path), $(sources:.cpp=.o))
cov_depends := $(addprefix $(cov_path), $(sources:.cpp=.d))
cov_exclude_files := graphics/* util/error.h

stylecheck_files := $(sources) game/game.h graphics/graphics.h graphics/shader.h graphics/keystate.h util/bmp.h util/file_io.h util/error.h
doc_config := $(doc_program).ini

# 
# Don't change anything after this
#

$(bin_path)$(main) : $(filter-out $(bin_path)$(tester).o, $(objects))
	@mkdir -p $(dir $@)
	$(CC) $(compile_flags) $^ -o $@ $(link_flags)

$(bin_path)$(tester) : $(filter-out $(bin_path)$(main).o, $(objects))
	@mkdir -p $(dir $@)
	$(CC) $(compile_flags) $^ -o $@ $(link_flags)

$(cov_path)$(tester) : $(filter-out $(cov_path)$(main).o, $(cov_objects))
	@mkdir -p $(dir $@)
	$(CC) $(cov_compile_flags) $^ -o $@ $(cov_link_flags)

-include $(depends)
-include $(cov_depends)

$(bin_path)%.o : %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(compile_flags) -c -MMD $< -o $@

$(cov_path)%.o : %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(cov_compile_flags) -c -MMD $< -o $@

$(cov_path)html/index.html : $(cov_path)$(tester)
	@lcov -q -o $(cov_path)initcovdata.info -c -i -d $(cov_path) -b $(cov_path) >/dev/null 2>/dev/null				# Generate empty coverage data
	@./$(cov_path)$(tester)														# Run tests
	@gcov $(cov_path)$(tester) -r 2>/dev/null >/dev/null										# Generate gcov coverage data from our run
	@lcov -q -o $(cov_path)runcovdata.info -c -d $(cov_path) -b $(cov_path) >/dev/null 2>/dev/null					# Generate lcov coverage data from gcov coverage data
	@lcov -q -a $(cov_path)initcovdata.info -a $(cov_path)runcovdata.info -o $(cov_path)rescovdata.info >/dev/null 2>/dev/null	# Combine empty coverage and generated coverage files
	@lcov -q -e $(cov_path)rescovdata.info "`pwd`/*" -o $(cov_path)rescovdata.info >/dev/null 2>/dev/null				# Remove all but files in directory, do not generate coverage for stl/other library files.
	@lcov -q --remove $(cov_path)rescovdata.info -o $(cov_path)rescovdata.info $(cov_exclude_files) >/dev/null 2>/dev/null		# Remove all files that need to be excluded
	@genhtml -q -o $(cov_path)html/ $(cov_path)rescovdata.info >/dev/null 2>/dev/null						# Generate HTML from lcov coverage data
	@echo created coverage report index.html in $(cov_path)html/index.html

.PHONY : test
test : $(bin_path)$(tester)
	@./$(bin_path)$(tester)

.PHONY : run
run : $(bin_path)$(main)
	@./$(bin_path)$(main)

.PHONY : coverage
coverage : $(cov_path)html/index.html

.PHONY : showcoverage
showcoverage : coverage
	xdg-open $(cov_path)html/index.html 2>/dev/null

.PHONY : checkstyle
checkstyle :
	@echo "errors\t\tfile"
	@for SOURCENAME in $(stylecheck_files); do\
		$(clang_format_program) -output-replacements-xml $$SOURCENAME | grep -c -n "<replacement " | tr -d '\n';\
		echo "\t:\t$$SOURCENAME";\
	done

.PHONY : fixstyle
fixstyle :
	@for SOURCENAME in $(stylecheck_files); do\
		$(clang_format_program) -i $$SOURCENAME;\
	done

.PHONY : doc
doc :
	$(doc_program) $(doc_config) >/dev/null 2>/dev/null
	@echo created documentation html index.html in $(doc_path)html/index.html

.PHONY : fulldoc
fulldoc :
	@(cat $(doc_config) ; echo "\n EXTRACT_PRIVATE = YES \n EXTRACT_PACKAGE = YES \n EXTRACT_STATIC = YES \n SOURCE_BROWSER = YES \n VERBATIM_HEADERS = YES \n FILE_PATTERNS = *.cpp *.h *.md") | $(doc_program) - >/dev/null 2>/dev/null
	@echo created documentation html index.html in $(doc_path)html/index.html

.PHONY : clean
clean :
	@rm -rf $(target)
