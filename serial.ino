void attachSerialCallbacks() {
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(iBat, onGetBatteryStatus);
  cmdMessenger.attach(iTime, onGetTime);
}


//COMMANDS FROM SERIAL

void OnUnknownCommand() {
  Serial.println("BT: Unknown command");
  cmdMessenger.sendCmd(kError,"Command without attached callback");
}

void onGetBatteryStatus() {
  Serial.println("got battery status");
}

// get the unix timestamp from phone
// @param int timeGot - the current timestamp
// TODO: add some basic validation maybe to counter bad signal issues. maybe add a flag when the user forces a time update, 
// and just check if the new time is massively different from current one for routine updates (maybe issue an android notice for it)
void onGetTime(){
  time_t timeGot = cmdMessenger.readInt32Arg();
  Serial.print("Updating Time to ");
  Serial.println(timeGot);
  rtc.setTime(timeGot+3600*TZ_OFFSET);
}

