###############################################################################
# Makefile for the project PROYECTO_ejecucion_AVR
###############################################################################

## General Flags
PROJECT = PROYECTO_ejecucion_AVR
MCU = atmega2560
TARGET = PROYECTO_ejecucion_AVR.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -Os -std=gnu99 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=PROYECTO_ejecucion_AVR.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = PROYECTO_ejecucion_AVR.o UART.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) PROYECTO_ejecucion_AVR.hex PROYECTO_ejecucion_AVR.eep PROYECTO_ejecucion_AVR.lss## Compile
PROYECTO_ejecucion_AVR.o: ../PROYECTO_ejecucion_AVR.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

UART.o: ../UART.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) PROYECTO_ejecucion_AVR.elf dep/* PROYECTO_ejecucion_AVR.hex PROYECTO_ejecucion_AVR.eep PROYECTO_ejecucion_AVR.lss PROYECTO_ejecucion_AVR.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

