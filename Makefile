all: main
#./src/init.c ./src/menu.c 
main: ./src/main.c
	gcc ./src/*.c  -o ./build/game -lncursesw