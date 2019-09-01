char t[32]; // used to sprintf for Serial output

//enter the length of the payload after the txbuffer and match this with the length in the main module
    void buildPacket(uint8_t txBuffer[1])
    {
        txBuffer[0] = ( 11 ) & 0xFF;
    }
