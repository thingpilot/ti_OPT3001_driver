
#include "OPT3001.h"
#include "mbed.h"

 //Shift by one bit to get 7 bit I2C Addrress
//char HDC_COMMN = HDC_MANID_OFF;
const float OPT_CHIP_ERROR = -255;
const unsigned long OPT_CHIP_SER_ERROR = 0;
char Buffer1[5];
Serial pc(PC_10, PC_11);

OPT3001::OPT3001(PinName sda, PinName slc) : I2C(sda,slc) 
{
    memset(Buffer1,'\0',5);
    Buffer1[0] = OPT3001_REG_CONFIG;
    this->frequency(I2C_FREQ);
    int res = this->write(OPT3001_DEFAULT_ADDRESS, Buffer1, 2);
    //pc.printf("OPT Constructor Initialization  : Res =%d\r\n", res);
}

int OPT3001::ReadSignature(void)
{ 
    
uint16_t  Manufacturer_ID = read2Bytes(OPT3001_DEFAULT_ADDRESS,OPT3001_REG_MAN_ID);
    if (Manufacturer_ID == 0) {
     
      
        pc.printf("Error  reading HDC Manufacturer ID\r\n");
        return (int) OPT_CHIP_ERROR;
    } else {  
           
     
        pc.printf("Manufacturer_ID  :%x\r\n", (int) Manufacturer_ID);
        return Manufacturer_ID;
    }    
}  
    
 float OPT3001::Readlight()
{  
    uint16_t  rawT = read2Bytes(OPT3001_DEFAULT_ADDRESS, OPT3001_REG_RESULT);
    // if (rawT == 0) {
    //      pc.printf("Error reading");
    //     return OPT_CHIP_ERROR;
    // } else {
        //     /* swap bytes, as OPT3001 sends MSB first and I2C driver expects LSB first */
       
        rawT = (rawT << 8) | (rawT >> 8);

        /* calculate lux per LSB */
        uint8_t exp = (rawT >> 12) & 0x0F;
        uint32_t lsb_size_x100 = (1 << exp);
        
        /* remove 4-bit exponent and leave 12-bit mantissa only */
        rawT &= 0x0FFF;
        float lght_lux= (((int)rawT * lsb_size_x100) / 100);

       
  
        return lght_lux;
    //}
}   
    

    unsigned long OPT3001::ReadSerialNumber(void)
{    
    wait_us(15000); //15ms
    memset(Buffer1,0,4);
    Buffer1[0] = OPT3001_REG_MAN_ID;
    int res = this->write(OPT3001_DEFAULT_ADDRESS, Buffer1, 1);
    if (res != 0) {      
     
    
      return (unsigned long) OPT_CHIP_SER_ERROR;
    }      
 
    wait_us(15000); //0.015
    memset(Buffer1,0,4);
    res = this->read(OPT3001_DEFAULT_ADDRESS, Buffer1,4);
    if (res != 0) {
     
    
     
      return (unsigned long) OPT_CHIP_SER_ERROR;
    }
      
//    unsigned long rawser = Buffer[0] << 16 | Buffer[1] << 8 | Buffer[0];
      unsigned long rawser = Buffer1[2] << 16 | Buffer1[1] << 8 | Buffer1[0];
  
   
   
    return rawser;
}
    
    
    //Private Member functions 
    
    uint16_t OPT3001::read2Bytes(int chip_addr, int offset)
{
    memset(Buffer1,0,3);
    // send chip address onto buss
    Buffer1[0] = offset;
    int res =this->write(chip_addr, Buffer1, 1);
    if (res != 0) {
       
    pc.printf("HERE");
       
        return 0;
    }
    // read data from chip
    wait_us(15000); //0.015
    memset(Buffer1,0,3);
    res = this->read(chip_addr, Buffer1,2);
    if (res != 0) {
    pc.printf("THERE");
        return 0;
    }
    

    uint16_t result = ((((uint16_t)Buffer1[0]) << 8) | Buffer1[1]) & 0x1FFF;
    pc.printf("Buffer Result   : %d\r\n", result);

    return  Buffer1[0] << 8 | Buffer1[1];
}


