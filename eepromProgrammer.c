// Compile: cc -o eepromProgrammer eepromProgrammer.c -lbcm2835

#include <bcm2835.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Header 11 - GPIO17 - SHIFT_CLK
// Header 13 - GPIO27 - SHIFT_DATA
// Header 15 - GPIO22 - SHIFT_LATCH
// Header 16 - GPIO23 - EEPROM_WRITE_ENABLE 

#define SHIFT_CLK RPI_V2_GPIO_P1_11
#define SHIFT_DATA RPI_V2_GPIO_P1_13
#define SHIFT_LATCH RPI_V2_GPIO_P1_15
#define EEPROM_WRITE_ENABLE RPI_V2_GPIO_P1_16

#define MAX 1000
 
// Convert HEX to Binary
int hexToBin(char* hexString, char** binString) {
  char binarynum[MAX];
  char hexa[MAX];
  char binstr[MAX] = "";
  long int i = 0;

  if (hexString[0] == '0' && hexString[1] == 'x') {
    i = 2;
  }

  while (hexString[i]) {
    switch (hexString[i]) {
      case '0':
        strcat(binstr, "0000"); break;
      case '1':
        strcat(binstr, "0001"); break;
      case '2':
        strcat(binstr, "0010"); break;
      case '3':
        strcat(binstr, "0011"); break;
      case '4':
        strcat(binstr, "0100"); break;
      case '5':
        strcat(binstr, "0101"); break;
      case '6':
        strcat(binstr, "0110"); break;
      case '7':
        strcat(binstr, "0111"); break;
      case '8':
        strcat(binstr, "1000"); break;
      case '9':
        strcat(binstr, "1001"); break;
      case 'A':
        strcat(binstr, "1010"); break;
      case 'B':
        strcat(binstr, "1011"); break;
      case 'C':
        strcat(binstr, "1100"); break;
      case 'D':
        strcat(binstr, "1101"); break;
      case 'E':
        strcat(binstr, "1110"); break;
      case 'F':
        strcat(binstr, "1111"); break;
      case 'a':
        strcat(binstr, "1010"); break;
      case 'b':
        strcat(binstr, "1011"); break;
      case 'c':
        strcat(binstr, "1100"); break;
      case 'd':
        strcat(binstr, "1101"); break;
      case 'e':
        strcat(binstr, "1110"); break;
      case 'f':
        strcat(binstr, "1111"); break;
      default:
        printf("\n Invalid hexa digit %c ", hexString[i]);
          return 0;
    }
    i++;
  }

  *binString = binstr;

  return 1;
}

void initPinOutUp(int pinNum) {
  bcm2835_gpio_fsel(pinNum, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_write(pinNum, HIGH);
  return;
}

void setPinUp(int pinNum) {
  bcm2835_gpio_write(pinNum, HIGH);
  return;
}

void setPinDown(int pinNum) {
  bcm2835_gpio_write(pinNum, LOW);
  return;
}

void setPinOff(int pinNum) {
  bcm2835_gpio_set_pud(pinNum, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set(pinNum);

  return;
}

void clearPin(int pinNum) {
  bcm2835_gpio_set_pud(pinNum, BCM2835_GPIO_PUD_OFF);
  //bcm2835_gpio_fsel(pinNum, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_clr(pinNum);
}

void initPinSet(int pinNum) {
  printf("Initializing pin number %i\n", pinNum);
  bcm2835_gpio_fsel(pinNum, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_set(pinNum);

  return;
}

// Clock pulse is LOW -> HIGH -> LOW
void pulseClock() {
  // Pulse the clock to shift the data one bit 
  clearPin(SHIFT_CLK);
  setPinUp(SHIFT_CLK);
  clearPin(SHIFT_CLK);

  return;
}

// Latch pulse is LOW -> HIGH -> LOW
void pulseLatch() {
  // Pulse the latch
  clearPin(SHIFT_LATCH);
  setPinUp(SHIFT_LATCH);
  clearPin(SHIFT_LATCH);

  return;
}

void pulseWrite() {
  clearPin(EEPROM_WRITE_ENABLE);
  setPinUp(EEPROM_WRITE_ENABLE);
  bcm2835_delay(200);
  clearPin(EEPROM_WRITE_ENABLE);

  return;
}

// Load bits into the shift register
void pushBits(char * bits) {
  long int i = 0;
  printf("Pushing bits %s\n", bits);

  while (bits[i]) {
    if (bits[i] == '0') {
      setPinDown(SHIFT_DATA);
    }

    if (bits[i] == '1') {
        setPinUp(SHIFT_DATA);
    }

    i++;

    pulseClock();
  }

  return;
}

int shiftOutHex(char * hexData) {
  char* binString = NULL;

  if (!hexToBin(hexData, &binString)) {
    printf("Error converting hex to binary\n");
    return 1;
  }; 

  printf("Converted %s to binary as %s, pushing bits...\n", hexData, binString);

  pushBits(binString);

  return 0;
}

void gpio_reset(void) {
  bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_03, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_05, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_07, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_26, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_24, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_21, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_19, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_23, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_10, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_11, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_12, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_13, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_15, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_16, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_18, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_22, BCM2835_GPIO_PUD_OFF);

  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_03, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_05, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_07, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_26, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_24, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_21, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_19, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_23, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_10, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_11, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_12, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_13, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_15, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_16, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_18, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(RPI_V2_GPIO_P1_22, BCM2835_GPIO_FSEL_INPT);
}

int main() {
  // Set each bit on SHIFT_DATA
  // Then pulse SHIFT_CLK
  // Repeat for each bit
  // When done...
  // Bring SHIFT_LATCH LOW then HIGH then LOW

  char* binString = NULL;

  printf("Init BCM...\n");

  // Init the BCM module
  if (!bcm2835_init()) return 1;

  printf("SHIFT_DATA pin: %i\n", SHIFT_DATA);
  printf("SHIFT_CLK pin: %i\n", SHIFT_CLK);
  printf("SHIFT_LATCH pin: %i\n", SHIFT_LATCH);

  // Set GPIO mode to set
  initPinOutUp(SHIFT_DATA);
  initPinOutUp(SHIFT_CLK);
  initPinOutUp(SHIFT_LATCH);
  initPinOutUp(EEPROM_WRITE_ENABLE);

  //byte data[] = { 0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b, 0x77, 0x1f, 0x4e, 0x3d, 0x4f, 0x47 };

  pushBits("00000000000");
  pulseLatch();
  pulseWrite();
  sleep(1);

  //for (int i = 0; i < 255; i++) {
  //  char hex[5];
  //  sprintf(hex, "%x", i);
  //
  //  shiftOutHex(hex);
  //  pulseLatch();
  //  pulseWrite();
  //  sleep(1);
  //}

  // Reset everything back to default
  //gpio_reset();

  bcm2835_close();

  printf("... done!\n");

  return 0;
}

