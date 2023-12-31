all: MYSTACK

MYSTACK: stack.o Stack_Func.o 
	g++ -g -o myprog.exe stack.o Stack_Func.o -o MYSTACK

stack.o: stack.cpp
	g++ -g -DHASH -DCANARY -DDUMP -c stack.cpp

Stack_Func.o: Stack_Func.cpp
	g++ -g -DHASH -DCANARY -DDUMP -c  Stack_Func.cpp

clean:
	rm *.o MYSTACK