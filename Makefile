all:
	g++ main.cpp -o gravitas -I/usr/local/include -L/usr/local/lib  -lsfml-graphics -lsfml-window -lsfml-system