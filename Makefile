default: clean all

all: main.cpp src/SerialPort.cpp     
	g++ -I include/ main.cpp src/SerialPort.cpp -o SerialPort

clean: 
	rm SerialPort.exe 