//char t[32]; // used to sprintf for Serial output
    void buildPacket(uint8_t txBuffer[3])
    {
        txBuffer[0] = ( 11 ) & 0xFF;
        txBuffer[1] = ( 01 ) & 0xFF;        
        txBuffer[0] = ( 02 ) & 0xFF;
    }
