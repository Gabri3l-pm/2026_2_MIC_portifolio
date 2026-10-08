/*
 * SPI.h
 *
 * Created: 10/8/2026 10:46:12 AM
 *  Author: Gabriel Demossi P. Machado
 */ 

#ifndef SPI_H_
#define SPI_H_

void SPI_master_config();
uint8_t SPI_transceive(uint8_t pTxByte);

#endif /* SPI_H_ */