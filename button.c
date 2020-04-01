#include <stdio.h>
#include <stdbool.h>
#include <ftd2xx.h>

#define PIN_TX  0x01  /* Orange wire on FTDI cable */
#define PIN_RX  0x02  /* Yellow */
#define PIN_RTS 0x10  /* Green */
#define PIN_CTS 0x08  /* Brown */

#define BIT1 PIN_TX  /* TX 2^0 */
#define BIT2 PIN_RX  /* TX 2^1 */
#define BIT3 PIN_CTS /* RX Button Press */
#define BIT4 PIN_RTS

#define STATE_PUSHES_ALLOWED 0b00
#define STATE_PUSHES_BLOCKED 0b01
#define STATE_PUSHES_UNKNOWN 0b10
#define STATE_BUSY           0b11

int main() {
  unsigned char tx[1] = {0};
  unsigned char rx;
  bool last_button_state = false;
  FT_HANDLE handle;
  DWORD tx_bytes;

  if(FT_Open(0, &handle) != FT_OK) {
    puts("Can't open device");
    return 1;
  }

  if (FT_SetBitMode(handle, BIT1 | BIT2 | BIT4, 1) != FT_OK) {
    puts("Can't set bitbang mode");
    return 1;
  }
  if (FT_SetBaudRate(handle, 9600) != FT_OK) {  /* 9600 * 16 */
    puts("Can't set baud rate");
    return 1;
  }

  if (FT_GetBitMode(handle, &rx) == FT_OK) {
    last_button_state = (rx | BIT3) == BIT3 ? true : false;
  }

  unsigned long count = 0;
  for (;;)  {
    if (FT_GetBitMode(handle, &rx) == FT_OK) {
      printf("bits(%lu): %x\n", count %4, rx);
    }
    else {
      puts("Can't read bits\n");
      return 1;
    }

    bool button_state = (rx | BIT3) == BIT3 ? true : false;
    if (button_state != last_button_state) {
      // fire off command
      puts("there was a flip!");
    }
    last_button_state = button_state;

    switch(count % 4) {
      case 0:
        tx[0] = ~BIT2 | ~BIT1;
        break;
      case 1:
        tx[0] = ~BIT2 | BIT1;
        break;
      case 2:
        tx[0] = BIT2 | ~BIT1;
        break;
      case 3:
        tx[0] = BIT2 | BIT1;
        break;
    }



    // if ((rx & BIT3) != BIT3) { /* The rx bit is active */
    //   tx[0] = BIT1 | BIT2 | BIT4;
    // } else {
    //   tx[0] ^= BIT1 | BIT2 | BIT4;
    // }

    FT_Write(handle, &tx, (DWORD)sizeof(tx), &tx_bytes);
    sleep(1);
    count++;
  }
}
