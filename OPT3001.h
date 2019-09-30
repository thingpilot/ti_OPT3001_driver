/**
 bla bla
 */

#include "mbed.h"

#ifndef MBED_OPT3001_H
#define MBED_OPT3001_H

#define     I2C_FREQ                            100000

// default I2C address with address pin tied to GND
#define OPT3001_DEFAULT_ADDRESS                 (0x44 << 1) //10001000?
#define OPT3001_REG_RESULT                      0x00 
#define OPT3001_REG_CONFIG                      0x01 
#define OPT3001_REG_MAN_ID                      0x7E //manifucture id
#define OPT3001_REG_CHIP_ID                     0x7F //read device id
#define OPT3001_MAN_ID                          0x5449 //address of manifucture id 


class OPT3001:public I2C {
public:
      
    OPT3001( PinName sda, PinName slc) ;                 // constructor

//    ~OPT3001();                                          // destructor
    
    int ReadSignature(void);
    float Readlight( void);                                
    unsigned long ReadSerialNumber(void);            
    
protected:
     uint16_t read2Bytes(int chip_addr, int offset);
};

#endif // MBED_OPT3001_H

