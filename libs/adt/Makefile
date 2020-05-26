SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)
DEPS = $(OBJS:%.o=%.d)

CFLAGS = -g -fPIC -Wall

# Include all .d Makefiles
-include $(DEPS)

all:
	$(CC) $(CFLAGS) -c -MMD $(SRCS)
	$(AR) rcs libadt.a $(OBJS)

clean:
	rm -f $(OBJS) $(DEPS) *.a 

