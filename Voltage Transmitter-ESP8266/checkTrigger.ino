void checkTrigger() {
  pinMode(EXTWAKEpin, INPUT);// need to immediately check this pin
  if (digitalRead(EXTWAKEpin) == LOW) {//flag if HIGH, then the external wake circuit woke the board up
    externalWake = true;
  }
  pinMode(DONEpin, OUTPUT);
  pinMode(LEDpin, OUTPUT);// LED on Board
  digitalWrite(LEDpin, LOW); //LOW to turn on
  
  //Wear Leveling stuff for timer wake tracking
  EEPROM.begin(20);
  int wearLevelingIndex;
  byte wakeCount;
  for (int i = 0; i < 20; i++) {
    wakeCount = EEPROM.read(i);
    if (wakeCount > 0) {
      wearLevelingIndex = i;
      break;
    }
    if (i == 19)
      wakeCount = 1;
  }
  EEPROM.write(wearLevelingIndex, 0);
  wearLevelingIndex++;
  if (wearLevelingIndex >= 20)
    wearLevelingIndex = 0;
  Serial.println("");
  Serial.print(wakeCount);
  Serial.print(" ");
  Serial.println(wearLevelingIndex);
  if (wakeCount < sleepHours) {
    wakeCount++;
    EEPROM.write(wearLevelingIndex, wakeCount);
    EEPROM.end();
    while (1) {
      digitalWrite(DONEpin, HIGH);
      delay(100);
      digitalWrite(DONEpin, LOW);
      delay(100);
    }
  }
  else {
    wakeCount = 1;
    EEPROM.write(wearLevelingIndex, wakeCount);
    EEPROM.end();
  }

  batteryVoltage = 0.00342 * analogRead(A0) + .823;
  Serial.println("");
  if (externalWake)
    Serial.println("external wake");
  else {
    Serial.println("timer wake");
    //if (batteryVoltage >= 3.3)
    //digitalWrite(DONEpin, HIGH);// standard wake, so just go back to sleep, who cares...
  }
  Serial.print("Battery Voltage = ");
  Serial.println(batteryVoltage);
  dtostrf(batteryVoltage, 3, 1, batteryBuf);//converts battery voltage float to char array
}
