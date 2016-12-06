ASFLAGS=-32
GCC_COMPILE_OPTS=-m32 -O3 -ffreestanding -nostdinc -fno-asynchronous-unwind-tables -g0 
GCC_LINK_OPTS=-T link.ld -nostdlib

C_SRCS=utils.c screen.c kernel.c
K_OBJECTS=boot.o utils.o screen.o kernel.o
KERNEL=myos.bin

all: $(KERNEL)

$(KERNEL): $(K_OBJECTS)
	gcc $(GCC_COMPILE_OPTS) $(GCC_LINK_OPTS) $(K_OBJECTS) -o $@
	strip --remove-section=.note --remove-section=.comment \
	--remove-section=.note.gnu.build-id \
	--remove-section=.eh_frame --remove-section=.eh_frame_hdr $@

%.o: %.s
	as $(ASFLAGS) $< -o $@
%.o: %.c
	gcc $(GCC_COMPILE_OPTS) $< -c

clean:
	rm -f $(K_OBJECTS) $(KERNEL)

test: $(KERNEL)
	qemu-system-i386 -kernel $(KERNEL)

depend: .depend

.depend: $(C_SRCS)
	@rm -f ./.depend
	gcc $(GCC_COMPILE_OPTS) -MM $^ -MF  ./.depend

include .depend
