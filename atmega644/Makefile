###############################################################################
# Makefile for the project XBH_ATmega644
###############################################################################

## General Flags
PROJECT = XBH_ATmega644
MCU = atmega644
TARGET = XBH_ATmega644.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON) -I ../common
CFLAGS += -Wall -gdwarf-2 -std=gnu99  -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections 


## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=XBH_ATmega644.map -Wl,-gc-sections



## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"source/networkcard" -I"source" 

## Objects that must be built in order to link
OBJECTS = analog.o base64.o cmd.o dhcpc.o dnsc.o http_get.o httpd.o lcd.o main.o ntp.o sendmail.o stack.o telnetd.o timer.o udp_lcd.o usart.o wol.o enc28j60.o udp_xbh.o tcp_xbh.o xbh.o xbd_multipacket.o i2c.o xbh_utils.o xbh_xbdcomm.o

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: XBH_version $(TARGET) XBH_ATmega644.hex XBH_ATmega644.eep XBH_ATmega644.lss size

## Compile
analog.o: source/analog.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

base64.o: source/base64.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

cmd.o: source/cmd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

dhcpc.o: source/dhcpc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

dnsc.o: source/dnsc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

http_get.o: source/http_get.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

httpd.o: source/httpd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd.o: source/lcd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: source/main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ntp.o: source/ntp.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sendmail.o: source/sendmail.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

stack.o: source/stack.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

telnetd.o: source/telnetd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer.o: source/timer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

udp_lcd.o: source/udp_lcd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usart.o: source/usart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

wol.o: source/wol.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

enc28j60.o: source/networkcard/enc28j60.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

udp_xbh.o: source/udp_xbh.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

tcp_xbh.o: source/tcp_xbh.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

xbh.o: ../common/xbh.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

xbh_utils.o: source/xbh_utils.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

xbh_xbdcomm.o: source/xbh_xbdcomm.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<


xbd_multipacket.o: source/xbd_multipacket.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<


i2c.o: source/i2c.c
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

size: ${TARGET}
	@echo
	@avr-size  ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) XBH_ATmega644.elf XBH_ATmega644.hex XBH_ATmega644.eep XBH_ATmega644.lss XBH_ATmega644.map XBH_version.h

XBH_version:
	../xbh-version .. | cmp -s - source/XBH_version.h || ../xbh-version .. > source/XBH_version.h
