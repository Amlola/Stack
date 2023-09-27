all: MYSTACK

MYSTACK: stack.o Stack_Func.o 
	g++ -o myprog.exe stack.o Stack_Func.o -o MYSTACK -DDUMP

stack.o: stack.cpp
	g++ -c stack.cpp

Stack_Func.o: Stack_Func.cpp
	g++ -c  Stack_Func.cpp

clean:
	rm *.o MYSTACK