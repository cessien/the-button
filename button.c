#include <stdio.h>
#include <ftd2xx.h>

#define PIN_TX  0x01  /* Orange wire on FTDI cable */
#define PIN_RX  0x02  /* Yellow */
#define PIN_RTS 0x10  /* Green */
#define PIN_CTS 0x08  /* Brown */

#define BIT1 PIN_TX  /* TX 2^0 */
#define BIT2 PIN_RX  /* TX 2^1 */
#define BIT3 PIN_CTS /* RX Button Press */
#define BIT4 PIN_RTS

#define STATE_PUSHES_RUNNING 0b00
#define STATE_PUSHES_PAUSED  0x01
#define STATE_PUSHES_UNKNOWN 0x10
#define STATE_BUSY           0x11

int main() {
  unsigned char tx[1] = {0};
  unsigned char rx[1] = {0xff};
  FT_HANDLE handle;
  DWORD tx_bytes;
  DWORD rx_bytes;

  if(FT_Open(0, &handle) != FT_OK) {
      puts("Can't open device");
      return 1;
  }

  FT_SetBitMode(handle, BIT1 | BIT2 | BIT4, 1);
  FT_SetBaudRate(handle, 9600);  /* 9600 * 16 */

  for (;;)  {
    FT_Read(handle, &rx, (DWORD)sizeof(rx), &rx_bytes);
    printf("bits: %x\n", rx[0]);
    //if ((rx[0] & BIT3) == BIT3) {
    //  tx[0] = BIT1 | BIT2;
    //} else {
    tx[0] ^= BIT1 | BIT2 | BIT4;
    //}

    FT_Write(handle, &tx, (DWORD)sizeof(tx), &tx_bytes);
    sleep(1);
  }
}
