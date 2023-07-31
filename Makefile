CFLAGS = -std=c++23 # -o2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXrandr # -lXxf86vm -lXi

game: *.cpp *.hpp
	g++ $(CFLAGS) -o game *.cpp $(LDFLAGS)


.PHONY: test clean

test: game
	./game

clean:
	rm -f game
