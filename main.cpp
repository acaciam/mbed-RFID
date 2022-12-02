//Test of cheap 13.56 Mhz RFID-RC522 module from eBay
//This code is based on Martin Olejar's MFRC522 library. Minimal changes
//Adapted for Nucleo STM32 F401RE. Should work on other Nucleos too

//Connect as follows:
//RFID pins        ->  Nucleo header CN5 (Arduino-compatible header)
//----------------------------------------
//RFID IRQ=pin5    ->   Not used. Leave open
//RFID MISO=pin4   ->   Nucleo SPI_MISO=PA_6=D12
//RFID MOSI=pin3   ->   Nucleo SPI_MOSI=PA_7=D11
//RFID SCK=pin2    ->   Nucleo SPI_SCK =PA_5=D13
//RFID SDA=pin1    ->   Nucleo SPI_CS  =PB_6=D10
//RFID RST=pin7    ->   Nucleo         =PA_9=D8
//3.3V and Gnd to the respective pins                              
                              
#include "mbed.h"
#include "MFRC522.h"
// Nucleo Pin for MFRC522 reset (pick another D pin if you need D8)
#define MF_RESET    D8
#define SPI_MOSI PB_5
#define SPI_MISO PB_4
#define SPI_SCK PB_3

 #define UART_RX     PA_3
 #define UART_TX     PA_2

DigitalOut LedGreen(LED1);
DigitalOut LedBlue(LED2);
DigitalOut LedRed(LED3);
DigitalOut LedE(PC_0);


BufferedSerial     pc(UART_TX, UART_RX, 9600);

MFRC522    RfChip   (SPI_MOSI, SPI_MISO, SPI_SCK, PG_2, MF_RESET);

int main(void) {

  // Init. RC522 Chip
  RfChip.PCD_Init();
    pc.set_format(
        /* bits */ 8,
        /* parity */ BufferedSerial::None,
        /* stop bit */ 1
    );

  while (true) {

    // Look for new cards
    if ( ! RfChip.PICC_IsNewCardPresent())
    {
      ThisThread::sleep_for(500ms);
      continue;
    }
    //if new card is present flash light
    else {
        LedBlue = 1;
        ThisThread::sleep_for(500ms);
        LedBlue = 0;
    }
    

    // Select one of the cards
    if ( ! RfChip.PICC_ReadCardSerial())
    {
      ThisThread::sleep_for(500ms);
      continue;
    } //read card successful blink blue led
    else{
        // Print Card UID
        uint8_t ID[] = {0xe3, 0xdf, 0xa6, 0x2e};
        if(RfChip.uid.uidByte[0] == ID[0] && RfChip.uid.uidByte[1] == ID[1] && RfChip.uid.uidByte[2] == ID[2] && RfChip.uid.uidByte[3] == ID[3]){
                printf("Card Match! \n");
                LedGreen = 1;
                ThisThread::sleep_for(500ms);
                LedGreen = 0;
        }
        else{
            printf("Not Matching Card \n");
                    LedRed = 1;
                    ThisThread::sleep_for(500ms);
                    LedRed = 0;
        }
        printf("Card UID: ");
        for (uint8_t i = 0; i < RfChip.uid.size; i++)
        {
            printf(" %X", RfChip.uid.uidByte[i]);
        }
        printf("\n\n\r");
    }

  }
}
