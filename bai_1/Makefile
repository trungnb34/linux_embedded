all : 
	gcc -c add.c -o add.o
	gcc -c sub.c -o sub.o
	gcc -c main.c -o main.o
	ar rcs libmymath.a add.o sub.o
	gcc -o myprogram_static main.o libmymath.a

	gcc -fPIC -c add.c -o add1.o
	gcc -fPIC -c sub.c -o sub1.o
	gcc -shared -o libmymath.so add1.o sub1.o
	gcc -o myprogram_shared main.o libmymath.so
	cp libmymath.so /usr/lib 
	ldconfig
static:
	gcc -c add.c -o add.o
	gcc -c sub.c -o sub.o
	gcc -c main.c -o main.o
	ar rcs libmymath.a add.o sub.o
	gcc -o myprogram_static main.o libmymath.a

shared:
	gcc -fPIC -c add.c -o add.o
	gcc -fPIC -c sub.c -o sub.o
	gcc -c main.c -o main.o
	gcc -shared -o libmymath.so add.o sub.o
	gcc -o myprogram_shared main.o libmymath.so
	cp libmymath.so /usr/lib 
	ldconfig
clean:
	rm add.o sub.o libmymath.a myprogram_shared myprogram_static main.o add1.o sub1.o libmymath.so
