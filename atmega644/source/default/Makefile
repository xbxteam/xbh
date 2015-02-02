###############################################################################
# Makefile for the project LAN
###############################################################################

## General Flags
PROJECT = LAN
MCU = atmega644
TARGET = LAN.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99                    -ffunction-sections  -fdata-sections       -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += -Wl,-gc-sections -Wl,-Map=LAN.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"S:\XBX\trunk\xbh\atmega644\source\..\..\WinAVR-20090313\avr\include" -I"S:\XBX\trunk\xbh\atmega644\source\networkcard" -I"S:\XBX\trunk\xbh\atmega644\source\." -I"S:\XBX\trunk\xbh\atmega644\source\..\..\common" 

## Objects that must be built in order to link
OBJECTS = analog.o base64.o cmd.o dhcpc.o dnsc.o http_get.o lcd.o main.o ntp.o sendmail.o stack.o timer.o udp_lcd.o usart.o wol.o enc28j60.o udp_xbh.o i2c.o xbd_multipacket.o xbh_utils.o xbh_xbdcomm.o tcp_xbh.o xbh.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) LAN.hex LAN.eep LAN.lss size

## Compile
analog.o: ../analog.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

base64.o: ../base64.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

cmd.o: ../cmd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

dhcpc.o: ../dhcpc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

dnsc.o: ../dnsc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

http_get.o: ../http_get.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd.o: ../lcd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ntp.o: ../ntp.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sendmail.o: ../sendmail.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

stack.o: ../stack.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer.o: ../timer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

udp_lcd.o: ../udp_lcd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usart.o: ../usart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

wol.o: ../wol.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

enc28j60.o: ../networkcard/enc28j60.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

udp_xbh.o: ../udp_xbh.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

i2c.o: ../i2c.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

xbd_multipacket.o: ../xbd_multipacket.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

xbh_utils.o: ../xbh_utils.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

xbh_xbdcomm.o: ../xbh_xbdcomm.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

tcp_xbh.o: ../tcp_xbh.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

xbh.o: ../../../common/xbh.c
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
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) LAN.elf dep/* LAN.hex LAN.eep LAN.lss LAN.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

