CC	= arm-linux-gcc
LD	= arm-linux-ld
OC	= arm-linux-objcopy

CFLAGS	= -nostdinc -I. -I../include -Wall -Wstrict-prototypes -Wno-trigraphs -O2 -fno-strict-aliasing -fno-common -pipe -mapcs-32 -mcpu=xscale -mshort-load-bytes -msoft-float -fno-builtin -D IN_GUMSTIX -D CHAPTER9
LDFLAGS	= -static -nostdlib -nostartfiles -nodefaultlibs -p -X -T ./main-ld-script
OCFLAGS	= -O binary -R .note -R .comment -S

all: yummyArmOS.img

CSRCFILES = $(wildcard ./*.c)
ASMSRCFILES = $(wildcard ./*.S)
COBJFILES = $(notdir $(patsubst %.c,%.o,$(CSRCFILES)))
ASMOBJFILES = $(notdir $(patsubst %.S,%.o,$(ASMSRCFILES)))

yummyArmOS.elf: $(COBJFILES) $(ASMOBJFILES)
	$(LD) $(LDFLAGS) -o $@ entry.o gpio.o time.o vsprintf.o printf.o string.o serial.o lib1funcs.o yummyArmOS.o memory.o task.o user.o

yummyArmOS.img: yummyArmOS.elf
	$(OC) $(OCFLAGS) $^ $@

%.o: ./%.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: ./%.S
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm *.o
	rm yummyArmOS.elf
	rm yummyArmOS.img
