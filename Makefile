CFLAGS = -std=c++23 # -o2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: main.cpp
	g++ $(CFLAGS) -o VulkanTest main.cpp $(LDFLAGS)


.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest

