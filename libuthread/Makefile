# Target library
lib := libuthread.a

objs    := preempt.o context.o
objs 	+= uthread.o queue.o
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror
CFLAGS  += -g
PANDOC  := pandoc
ifneq ($(V),1)
Q = @
endif

all: $(lib)

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