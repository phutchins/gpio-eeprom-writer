// Compile: cc -o eepromProgrammer eepromProgrammer.c -lbcm2835

#include <bcm2835.h>
#include <stdio.h>
#include <string.h>

// Header 11 - GPIO0 - SHIFT_CLK
// Header 13 - GPIO2 - SHIFT_DATA
// Header 15 - GPIO3 - SHIFT_LATCH

#define SHIFT_CLK RPI_V2_GPIO_P1_11
#define SHIFT_DATA RPI_V2_GPIO_P1_13
#define SHIFT_LATCH RPI_V2_GPIO_P1_15

#define MAX 1000

 
// Convert HEX to Binary
const char * hexToBin(char * hexString) {
  printf("Got hex string: %s\n", hexString);

  char binarynum[MAX], hexa[MAX];
  char binstr[MAX];
  long int i = 0;
 
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
  return *binstr;
}

void setPinUp(int pinNum) {
  bcm2835_gpio_set_pud(pinNum, BCM2835_GPIO_PUD_UP);

  return;
}

void setPinDown(int pinNum) {
  bcm2835_gpio_set_pud(pinNum, BCM2835_GPIO_PUD_DOWN);

  return;
}

void setPinOff(int pinNum) {
  bcm2835_gpio_set_pud(pinNum, BCM2835_GPIO_PUD_OFF);

  return;
}

void clearPin(int pinNum) {
  bcm2835_gpio_fsel(pinNum, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_clr(pinNum);
}

void initPinSet(int pinNum) {
  printf("Initializing pin number %i\n", pinNum);
  sleep(1);
  bcm2835_gpio_fsel(pinNum, BCM2835_GPIO_FSEL_OUTP);
  //bcm2835_gpio_set_pud(pinNum, BCM2835_GPIO_PUD_OFF);
  bcm2835_gpio_set(pinNum);
  //setPinOff(pinNum);
  sleep(1);
  clearPin(pinNum);

  return;
}

// Clock pulse is up, down up or up, off, up ?
void pulseClock() {
  // Pulse the clock to shift the data one bit 
  bcm2835_gpio_set_pud(SHIFT_CLK, BCM2835_GPIO_PUD_UP);
  sleep(1);
  clearPin(SHIFT_CLK);
  //bcm2835_gpio_set_pud(SHIFT_CLK, BCM2835_GPIO_PUD_OFF);
  sleep(1);
  bcm2835_gpio_set_pud(SHIFT_CLK, BCM2835_GPIO_PUD_UP);
  sleep(1);

  return;
}

// Latch pulse is DOWN, UP, DOWN?
void pulseLatch() {
  // Pulse the latch
  bcm2835_gpio_set_pud(SHIFT_LATCH, BCM2835_GPIO_PUD_DOWN);
  sleep(1);
  bcm2835_gpio_set_pud(SHIFT_LATCH, BCM2835_GPIO_PUD_UP);
  sleep(1);
  bcm2835_gpio_set_pud(SHIFT_LATCH, BCM2835_GPIO_PUD_DOWN);
  sleep(1);

  return;
}

// Load bits into the shift register
void pushBits(char * bits) {
  long int i = 0;
  printf("Pushing bits %s\n", bits);

  while (bits[i]) {
    switch (bits[i]) {
      case '0':
        printf("Bit is 0\n");
        setPinOff(SHIFT_DATA);
      case '1':
        printf("Bit is 1\n");
        setPinUp(SHIFT_DATA);
    }

    sleep(1);

    pulseClock();

    i++;
  }

  return;
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

main() {
  // Set each bit on SHIFT_DATA
  // Then pulse SHIFT_CLK
  // Repeat for each bit
  // When done...
  // Bring SHIFT_LATCH LOW then HIGH then LOW

  // Use this to iterate through hex values 0-255
  //char hexString[MAX] = "55";
  //char binString[MAX];
  //const char* binString = hexToBin(hexString);
  //printf("Binary string is: %s\n", binString);
  //pushBits(binString);

  printf("Init BCM...\n");

  // Init the BCM module
  if (!bcm2835_init()) return 1;

  // Set GPIO mode to set
  initPinSet(SHIFT_DATA);
  initPinSet(SHIFT_CLK);
  initPinSet(SHIFT_LATCH);

  pushBits("01010101");
  //pushBits("11111111");

  pulseLatch();

  sleep(5);

  // Reset everything back to default
  gpio_reset();

  bcm2835_close();
  printf("... done!\n");
  return 0;
}

