main: CSVRow.cpp ImageUtils.cpp
	g++ main.cpp -o main CSVRow.cpp ImageUtils.cpp -lcurl `pkg-config --cflags --libs opencv`

clean:
	-rm -f main; make