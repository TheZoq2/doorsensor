SOURCES=dynamicstring.cpp wifi.cpp

TEST_SOURCE=unit.cpp

all:
	make build
	make upload

build:
	make -f arduino.mk

upload:
	make -f arduino.mk reset


test:
	g++ $(SOURCES) $(TEST_SOURCE) --std=c++11 -o test.out -Wall -g -Werror=return-type
	./test.out -b

monitor:
	make -f arduino.mk monitor
