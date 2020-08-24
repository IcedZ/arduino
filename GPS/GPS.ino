void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  float flat, flon;
  unsigned long fix_age; // returns +- latitude/longitude in degrees
  
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  
    /* For debugging. get rid of LAT and LON from LCD */
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("LAT=");  
    lcd.print(flat,6);
    lcd.setCursor(0,1);
    lcd.print("LON=");
    lcd.print(flon,6);
    delay(10000);
    
    

    // shut system off
    PowerOff();

  }
  
  /* Turn off after 5 minutes */
  if (millis() >= 300000)
    PowerOff();
}
