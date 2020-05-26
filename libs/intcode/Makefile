src = $(wildcard *.c)
headers = $(wildcard *.h)
objs = $(src:.c=.o)

all:
	$(CC) -fPIC -c -g -Wall -I ../utils -I ../adt $(headers) $(src)
	$(AR) rcs libintcode.a $(objs)

clean:
	rm -f *.o *.a *.gch

