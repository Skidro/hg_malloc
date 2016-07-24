PROGNAME := malloctest

C_SRC := $(wildcard *.c)
C_OBJ := $(patsubst %.c,%.o,$(C_SRC))


all: $(PROGNAME)

$(PROGNAME): $(C_OBJ)
	gcc -Wl,-wrap,malloc,-wrap,free $^ -o $@

debug:
	@echo $(C_SRC) $(C_OBJ)

%.o:%.c
	gcc -c $<

clean:
	rm -rf $(PROGNAME) $(C_OBJ)
