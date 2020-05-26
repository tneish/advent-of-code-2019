
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))
DEPS = $(OBJS:%.o=%.d)
BINS = $(subst src/,,$(OBJS))
BINS ::= $(subst .o,,$(BINS))

INCLUDELIBS = -I libs/adt -I libs/intcode -I libs/utils
LDLIBS = -ladt -lintcode -lutils -lm

LDFLAGS = -L libs/adt -L libs/intcode -L libs/utils
CFLAGS = -g -Wall

all: info libadt libintcode libutils $(BINS)

$(BINS): $(OBJS)
	$(CC) -Wall $(LDFLAGS) -o $@ src/$@.o $(LDLIBS)

# Include all .d Makefiles
-include $(DEPS)

$(OBJS): %.o: %.c
	# -MMD flag also creates dependency Makefile
	$(CC) $(CFLAGS) $(INCLUDELIBS) -MMD -c $< -o $@


info:
	$(info $$(SRCS) = $(SRCS))
	$(info $$(OBJS) = $(OBJS))
	$(info $$(DEPS) = $(DEPS))
	$(info $$(BINS) = $(BINS))

.PHONY : clean
clean: libadtclean libutilsclean libintcodeclean
	-rm $(BINS) $(OBJS) $(DEPS)

libadt:
	$(MAKE) -C libs/adt

libadtclean:
	$(MAKE) -C libs/adt clean

libutils:
	$(MAKE) -C libs/utils

libutilsclean:
	$(MAKE) -C libs/utils clean

libintcode:
	$(MAKE) -C libs/intcode

libintcodeclean:
	$(MAKE) -C libs/intcode clean