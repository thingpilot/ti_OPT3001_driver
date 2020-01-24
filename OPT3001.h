/**
  * @file    OPT3001.h
  * @version 0.1.0
  * @author  
  * @brief   Header file of the OPT3001 driver module
  * 
  * Example: 
  * code
  * #include "OPT3001/OPT3001.h"
  *
  * OPT3001 OPT3001(SDA,SCL); 
  *  
  * int main() {
  *     float lght_lux=OPT3001.Readlight();
  * }
  * @endcode
  */

/** Define to prevent recursive inclusion
 */
#pragma once

/** Includes
 */
#include "mbed.h"

#ifndef MBED_OPT3001_H
#define MBED_OPT3001_H

/** Uncomment if needed
*/
//#define     I2C_FREQ                            100000


/** Register Map.
 *  Device ID Register (offset = 7Fh) [reset = 3001h]
 *  Manufacturer ID Register (offset = 7Eh) [reset = 5449h]
 */
#define OPT3001_REG_RESULT                      0x00 
#define OPT3001_REG_CONFIG                      0x01
#define OPT3001_REG_MAN_ID                      0x7E //manifucture id
#define OPT3001_REG_DEV_ID                      0x7F //read device id

/** Default HDC1080 I2C address. This should be set according to the 
 *  configuration of the hardware address pins. 
 */
#define OPT3001_DEFAULT_ADDRESS                 (0x44 << 1) 
/** Configure Register changed for calibrating the device,
 * Default values (offset = 01h) [reset = C810h]- for full scale lux measurement
 * Changed to (offset = 01h) [reset = AE0Ch]
 */
#define OPT3001_CFG_DEFAULT_1                   0xAE
#define OPT3001_CFG_DEFAULT                     0x0C

class OPT3001:public I2C 
{
    public:
        /** Class constructor 
         */
        OPT3001( PinName sda, PinName slc);

        /** Class destructor
		 */
        // ~OPT3001(); 
        
        int ReadSignature(void);
        float Readlight( void);                                
        unsigned long ReadDeviceId(void);            
        
    protected:
        uint16_t read2Bytes(int chip_addr, int offset);
};

#endif 

