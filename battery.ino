//Battery related functions

void getBattery() {
  int batAn = analogRead(PIN_BAT);
  battery = map(batAn,0,2600,0,10);
  //send via BT
  cmdMessenger.sendCmdStart(iBat);
  cmdMessenger.sendCmdArg(battery);
  cmdMessenger.sendCmdArg(batAn);
  cmdMessenger.sendCmdEnd();
}
void drawBattery() {
  display.drawBitmap(2,2, spriteBatteryBg, 16, 8, BLACK);
  if(battery > 0) {
    display.fillRect(4,4,10,4,WHITE);
    display.fillRect(4,4,battery,4,BLACK);
  } else {
    display.fillRect(4,4,10,4,WHITE);
  }
}
