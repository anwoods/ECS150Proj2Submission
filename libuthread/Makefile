# Target library
lib := libuthread.a

# #all: $(lib)

#TODO: Phase 1.1
# targets := test
objs    := preempt.o 
objs 	+= context.o uthread.o queue.o
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror
CFLAGS  += -g
PANDOC  := pandoc
ifneq ($(V),1)
Q = @
endif

all: $(lib)

#queue.o: queue.c queue.h
%.o: %.c %.h
	@echo "CC   $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

$(lib): $(objs)
	ar rcs $@ $^

%.html: %.md
	@echo "MD   $@"    
	$(Q)$(PANDOC) -o $@ $<

clean:
	@echo "CLEAN   $@"
	$(Q)rm -f *.html $(objs) *.a
#q: queue.c queue.h test_queue.c
#	gcc -Wall -Wextra -Werror -o q queue.c queue.h test_queue.c

#create: preempt.c preempt.h context.c context.h uthread.c uthread.h queue.c queue.h uthread_hello.c
#	gcc -Wall -Wextra -Werror -o create preempt.c preempt.h context.c context.h uthread.c uthread.h queue.c queue.h uthread_hello.c

#yield: preempt.c preempt.h context.c context.h uthread.c uthread.h queue.c queue.h uthread_yield.c
#	gcc -Wall -Wextra -Werror -o yield preempt.c preempt.h context.c context.h uthread.c uthread.h queue.c queue.h uthread_yield.c

#clean:
#	rm -f q create yield
