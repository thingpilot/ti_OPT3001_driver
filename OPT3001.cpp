/**
  * @file    OPT3001.cpp
  * @version 0.1.0
  * @author  
  * @brief   OPT3001 driver module. Measures light intensity in lux.
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

/** Includes
 */
#include "OPT3001.h"


const float OPT_CHIP_ERROR = -255;
char data[5];

/** Constructor.
 * 
 * @param sda I2C data line pin
 * @param scl I2C clock line pin
 */
OPT3001::OPT3001(PinName sda, PinName slc) : I2C(sda,slc) 
{
    memset(data,'\0',5);
    data[0] = OPT3001_REG_CONFIG;
    int res = this->write(OPT3001_DEFAULT_ADDRESS, data, 2);
}
/* Initialisation. 
*  Reads The Manufacturer ID on register address 0x7E.
*  @return Manufacturer_ID 
*/
int OPT3001::ReadSignature(void)
{ 
    uint16_t  Manufacturer_ID = read2Bytes(OPT3001_DEFAULT_ADDRESS,OPT3001_REG_MAN_ID);
    if (Manufacturer_ID == 0)
    {
        return (int) OPT_CHIP_ERROR;
    } 
    else 
    {  
        return Manufacturer_ID;
    }    
}  

unsigned long OPT3001::ReadDeviceId(void)
{    
    uint16_t  Device_ID = read2Bytes(OPT3001_DEFAULT_ADDRESS, OPT3001_REG_DEV_ID);
    if (Device_ID == 0) 
    {
        return (int) OPT_CHIP_ERROR;
    } 
    else 
    {     
        return Device_ID;
    } 
}
/** Ambient light measurement TI OPT3001
 */   
float OPT3001::Readlight()
{   
    /* start measurement
    OPT3001 needs this extra initialisation before acquiring the results- 
    (zero to full-scale) */
    data[0] =OPT3001_REG_CONFIG;
    data[1] =OPT3001_CFG_DEFAULT_1;
    data[2]= OPT3001_CFG_DEFAULT;

    this->write(OPT3001_DEFAULT_ADDRESS, data, 3);
    wait_us(1500000); //15ms

    uint16_t  rawT = read2Bytes(OPT3001_DEFAULT_ADDRESS, OPT3001_REG_RESULT);

    if (rawT==OPT_CHIP_ERROR) 
    {
        return (int) OPT_CHIP_ERROR;
    }
    else
    {
        /* swap bytes, as OPT3001 sends MSB first and I2C driver expects LSB first */
        rawT = (rawT << 8) | (rawT >> 8);

        /* calculate lux per LSB */
        uint8_t exp = (rawT >> 12) & 0x0F;
        uint32_t lsb_size_x100 = (1 << exp);

        /* remove 4-bit exponent and leave 12-bit mantissa only */
        rawT &= 0x0FFF;
        float lght_lux= (((int)rawT * lsb_size_x100) / 100);

        return lght_lux; 
    }
}   
      
uint16_t OPT3001::read2Bytes(int chip_addr, int offset)
{
    memset(data,0,3);
    data[0] = offset;
    int res =this->write(chip_addr, data, 1);
    if (res != 0) 
    {     
        return OPT_CHIP_ERROR;
    }
    // read data from chip
    wait_us(15000); 
    memset(data,0,3);
    res = this->read(chip_addr, data,2);
    if (res != 0)
    {
        return OPT_CHIP_ERROR;
    }
        
    return  data[0] << 8 | data[1];
}


