GCC      = D:\PROGRA~1\BestTech\GCCDEV~1\GCC\H8\bin\h8300-coff-gcc
OBJCOPY  = D:\PROGRA~1\BestTech\GCCDEV~1\GCC\H8\bin\h8300-coff-objcopy
OBJDUMP  = D:\PROGRA~1\BestTech\GCCDEV~1\GCC\H8\bin\h8300-coff-objdump
LDSCRIPT = h8rom.x
TARGET   = main
MAP      = $(TARGET).map

CFLAGS   = -mh -mrelax -Wall -O0
LDFLAGS  = -mh -mrelax -nostartfiles -Wl,--script=$(LDSCRIPT) -Wl,-Map,$(MAP) -Wl,-static

SRCS     =  main.c timer.c sci.c shell.c command.c bmp.c h8crt0.s
OBJS     =  main.o timer.o sci.o shell.o command.o bmp.o h8crt0.o

main.mot : main.exe
	$(OBJCOPY) -O srec main.exe $@
main.exe : $(OBJS)
	$(GCC) -o $@ $(OBJS) $(LDFLAGS)


write:
	h8write -3048 $(TARGET).mot com1

clean:
	del $(OBJS) $(TARGET).exe $(TARGET).mot $(TARGET).map

dump:
	$(OBJDUMP) -D -x main.exe > main.dump

depend:
	$(GCC) -MM $(SRCS) > Makefile.depend


.c.o:
	$(GCC) $(CFLAGS) -c $<
.s.o:
	$(GCC) $(CFLAGS) -c $<

include Makefile.depend
