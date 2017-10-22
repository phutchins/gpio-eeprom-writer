// Compile: cc -o eepromProgrammer eepromProgrammer.c -lbcm2835

#include <bcm2835.h>
#include <stdio.h>
#include <string.h>

// Header 11 - GPIO0
// Header 13 - GPIO2
// Header 15 - GPIO3

// Should set these to the pins like RPI_V2_GPIO_P1_03...
//#define SHIFT_DATA 13
//#define SHIFT_CLK 11
//#define SHIFT_LATCH 15
#define SHIFT_DATA RPI_V2_GPIO_P1_13
#define SHIFT_CLK RPI_V2_GPIO_P1_11
#define SHIFT_LATCH RPI_V2_GPIO_P1_15

#define MAX 1000

#define MODE_READ 0
#define MODE_SET 1
#define MODE_CLR 2
#define MODE_INPUT_READ 3

#define PULL_UP 0
#define PULL_DOWN 1
#define NO_PULL 2

#define GPIO_BEGIN 0
#define GPIO_END 1
#define NO_ACTION 2

#define NO_PIN 40 // Some big number that's beyond the connector's pin count
#define DEBUG_OFF 0
#define DEBUG_ON 1

uint8_t  init = NO_ACTION;
uint8_t  pull = NO_PULL;
uint8_t  mode = MODE_READ;
uint8_t  pin_number = NO_PIN;

uint8_t i, len;
uint8_t data, pin, debug_mode = DEBUG_OFF;

void pushBits(char * bits) {
  // Set mode for pin using fsel(pin, BCM2835_GPIO_FSEL_OUTP);
  // Then set pin with bcm2835_gpio_set(pin);

  long int i = 0;

  while (bits[i]) {
    switch (bits[i]) {
      case '0':
        printf("Bit is 0\n");
        // Set data output pin to down
      case '1':
        printf("Bit is 1\n");
        // Set data output pin to up
    }
    // Save data to register? (up, down up?, which pin?)

    bcm2835_gpio_set_pud(pin, BCM2835_GPIO_PUD_UP);
    bcm2835_gpio_set_pud(pin, BCM2835_GPIO_PUD_DOWN);
    bcm2835_gpio_set_pud(pin, BCM2835_GPIO_PUD_UP);
  }
}
 
const char * hexToBin(char * hexString) {
  printf("Got hex string: %s\n", hexString);

  char binarynum[MAX], hexa[MAX];
  char binstr[MAX];
  long int i = 0;
 
  // printf("Enter the value for hexadecimal ");
  // scanf("%s", hexa);
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

main() {
  //#define SHIFT_DATA 13
  //#define SHIFT_CLK 11
  //#define SHIFT_LATCH 15

  // Set each bit on SHIFT_DATA
  // Then pulse SHIFT_CLK
  // Repeat for each bit
  // When done...
  // Bring SHIFT_LATCH LOW then HIGH then LOW

  char hexString[MAX] = "55";
  //char binString[MAX];
  //const char* binString = hexToBin(hexString);
  //printf("Binary string is: %s\n", binString);
  //pushBits(binString);

  printf("Init BCM...\n");

  if (!bcm2835_init()) return 1;

  // GPIO begin here?? (look in gpio example)

  GPIO_BEGIN;

  // Set GPIO mode
  bcm2835_gpio_fsel(SHIFT_DATA, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(SHIFT_CLK, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(SHIFT_LATCH, BCM2835_GPIO_FSEL_OUTP);

  // Set the data bit on the data pin
  bcm2835_gpio_set_pud(SHIFT_DATA, BCM2835_GPIO_PUD_UP);

  // To clear (instead of down??)
  //bcm2835_gpio_fsel(SHIFT_DATA, BCM2835_GPIO_FSEL_OUTP);
  //bcm2835_gpio_clr(SHIFT_DATA);
  //bcm2835_gpio_fsel(SHIFT_CLK, BCM2835_GPIO_FSEL_OUTP);
  //bcm2835_gpio_clr(SHIFT_CLK);
  //bcm2835_gpio_fsel(SHIFT_LATCH, BCM2835_GPIO_FSEL_OUTP);
  //bcm2835_gpio_clr(SHIFT_LATCH);

  // Pulse the clock to shift the data one bit 
  bcm2835_gpio_set_pud(SHIFT_CLK, BCM2835_GPIO_PUD_UP);
  bcm2835_gpio_set_pud(SHIFT_CLK, BCM2835_GPIO_PUD_DOWN);
  //bcm2835_gpio_fsel(SHIFT_CLK, BCM2835_GPIO_FSEL_OUTP);
  //bcm2835_gpio_clr(SHIFT_CLK);
  bcm2835_gpio_set_pud(SHIFT_CLK, BCM2835_GPIO_PUD_UP);

  // When done passing in data, pulse the latch
  bcm2835_gpio_fsel(SHIFT_LATCH, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_clr(SHIFT_LATCH);
  bcm2835_gpio_set_pud(SHIFT_LATCH, BCM2835_GPIO_PUD_UP);
  bcm2835_gpio_fsel(SHIFT_LATCH, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_clr(SHIFT_LATCH);

  bcm2835_close();
  printf("... done!\n");
  return 0;
}

