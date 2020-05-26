src = $(wildcard *.c)
headers = $(wildcard *.h)
objs = $(src:.c=.o)

all:
	$(CC) -fPIC -c -g -Wall -I ../adt $(headers) $(src)
	$(AR) rcs libutils.a $(objs)


clean:
	rm -f *.o *.a *.gch

