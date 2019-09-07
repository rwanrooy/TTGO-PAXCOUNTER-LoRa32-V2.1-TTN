// Building payload from sensordata
    void buildPacket(uint8_t txBuffer[3])
    {
  if (relais_on == true)
  {
    txBuffer[0] = ( 01 ) & 0xFF;
  }
  else
  {
    txBuffer[0] = ( 00 ) & 0xFF;
  }
    }
