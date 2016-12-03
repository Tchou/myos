ASFLAGS=-32
GCC_COMPILE_OPTS=-m32 -ffreestanding -nostdinc
GCC_LINK_OPTS=-T link.ld -nostdlib


K_OBJECTS=boot.o kernel.o
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
