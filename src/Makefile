link_flags = -lglfw -lGLEW -lGL
compile_flags = -O2 -std=c++11
main = main
tester = testing/tester
sources = main.cpp testing/tester.cpp game/game.cpp graphics/graphics.cpp graphics/util.cpp graphics/shader.cpp graphics/keystate.cpp
objects = $(sources:.cpp=.o)
depends = $(sources:.cpp=.d)

$(main) : $(filter-out $(tester).o, $(objects))
	g++ $(compile_flags) $^ -o $@ $(link_flags)

$(tester) : $(filter-out $(main).o, $(objects))
	g++ $(compile_flags) $^ -o $@ $(link_flags)

-include $(depends)

%.o : %.cpp
	g++ $(compile_flags) -c -MMD $< -o $@

.PHONY : test
test : $(tester)
	@./$(tester)

.PHONY : clean
clean :
	@rm -f $(main) $(tester) $(depends) $(objects)
