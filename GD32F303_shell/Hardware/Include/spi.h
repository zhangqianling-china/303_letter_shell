#ifndef SPI_H
#define SPI_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f30x.h"



#define GW_SPI0_INIT 0
#define GW_SPI1_INIT 1

uint8_t SpiReadWrite(uint32_t SPIx,uint8_t InData);
void GW_SpiWriteData(uint32_t SPIx,uint8_t byteCount,uint8_t* pData);
uint8_t GW_SpiReadData(uint32_t SPIx);
void GW_SpiInit(void);

#ifdef cplusplus
}
#endif

#endif /* SPI_H */
