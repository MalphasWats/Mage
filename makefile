#
# Makefile template for ATtiny85
# Derived from AVR Crosspack template
#

DEVICE     = attiny85
DEVDUDE    = t85
CLOCK      = 8000000
PROGRAMMER = -c linuxspi -P /dev/spidev0.0
OBJECTS    = utils.o beep.o oled.o mage.o     # Add more objects for each .c file here
C_FLAGS    = -Wl,--gc-sections -Wl,--relax -ffunction-sections -fdata-sections -fno-inline-small-functions -fpack-struct -fshort-enums -mshort-calls
# fuse settings:
# use http://www.engbedded.com/fusecalc
#FUSES      = -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m  # 1mhz
FUSES      = -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

AVRDUDE = sudo avrdude -b 14400 $(PROGRAMMER) -p $(DEVDUDE)
AVRDUDE_FAST = sudo avrdude -b 200000 $(PROGRAMMER) -p $(DEVDUDE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(C_FLAGS)

# symbolic targets:
all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

fast: all
	$(AVRDUDE_FAST) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

install:	flash fuse

# if you use a bootloader, change the command below appropriately:
load:	all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E utils.c beep.c oled.c mage.c
