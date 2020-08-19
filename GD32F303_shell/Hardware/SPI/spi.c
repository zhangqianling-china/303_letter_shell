#include "spi.h"

uint8_t SpiReadWrite(uint32_t SPIx,uint8_t InData)
{
		uint8_t retry = 0;

		/* Loop while DR register in not emplty */
		while (spi_i2s_flag_get(SPIx, SPI_FLAG_TBE) == RESET)
		{
			retry ++;
			if(retry > 200) return 0;	
		}

		/* Send byte through the SPI peripheral */
		spi_i2s_data_transmit(SPIx, InData);

		retry = 0;
		/* Wait to receive a byte */
		while (spi_i2s_flag_get(SPIx, SPI_FLAG_RBNE) == RESET)
		{
			retry ++;
			if(retry > 200) return 0;			
		}

		/* Return the byte read from the SPI bus */
		return spi_i2s_data_receive(SPIx);

}

void GW_SpiWriteData(uint32_t SPIx,uint8_t byteCount,uint8_t* pData)
{
		uint8_t i;

		for(i=0;i<byteCount;i++)
		{
		 SpiReadWrite(SPIx,pData[i]);
		}
}


uint8_t GW_SpiReadData(uint32_t SPIx)
{
		return (SpiReadWrite(SPIx,0));
}

void GW_SpiInit(void)
{
	spi_parameter_struct spi_init_struct;
	
#if GW_SPI0_INIT
		rcu_periph_clock_enable(RCU_GPIOA);
		rcu_periph_clock_enable(RCU_SPI0);
	    /* SPI0 GPIO config:SCK/PA5, MISO/PA6, MOSI/PA7 */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_7);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
	
#endif
	
#if GW_SPI1_INIT
		rcu_periph_clock_enable(RCU_GPIOB);
		rcu_periph_clock_enable(RCU_SPI1);
    /* SPI1 GPIO config:SCK/PB13, MISO/PB14, MOSI/PB15 */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 |GPIO_PIN_15);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_14);	
	
#endif
	  /* SPI parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_8;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
#if GW_SPI0_INIT
		spi_init(SPI0, &spi_init_struct);
		spi_crc_polynomial_set(SPI0,7);
		spi_crc_on(SPI0);
		spi_enable(SPI0);
#endif

#if GW_SPI1_INIT
		spi_init(SPI1, &spi_init_struct);
		spi_crc_polynomial_set(SPI1,7);
		spi_crc_on(SPI1);
		spi_enable(SPI1);
#endif
    
}


