void handleButtons() {
  unsigned long mls = millis();
  //button MENU debounce
  if( digitalRead(PIN_BTN1) == HIGH && (mls - btnMenuTimer) >= 1000 && btnMenuDown == false ) { //more than 1000 ms after last press
    buttonMenuAction();
    btnMenuTimer = mls;
    btnMenuDown = true;
  } else {
    btnMenuDown = false;
  }
  //btn Left debounce
  if( digitalRead(PIN_BTN2) == HIGH && (mls - btnLeftTimer) >= 500 && btnLeftDown == false ) { //more than 1000 ms after last press
    buttonLeftAction();
    btnLeftDown = true;
    btnLeftTimer = mls;
  } else {
    btnLeftDown = false;
  }
  //btn Right debounce
  if( digitalRead(PIN_BTN3) == HIGH && (mls - btnRightTimer) >= 500 && btnRightDown == false ) { //more than 500 ms after last press
       buttonRightAction();
       btnRightDown = true;
       btnRightTimer = mls;
  } else {
    btnRightDown = false;
  }
  
}

void buttonMenuAction(){
  switch(machineState) {
    case STATE_MAIN:
      //clock screen
      machineState = STATE_MENU_SELECT; //switch to the menu
      drawMenu();
    break;
    case STATE_MENU_SELECT:
      //menu select screen - enter the menu
      machineState = STATE_MENU_DISPLAY;//switch to display settings menu
      settingsState = 0; //reset the current setting cursor (first setting is selected everytime you enter a settings menu)
      drawMenu();
    break;
    case STATE_MENU_DISPLAY:
      machineState = STATE_MAIN; //return to the main screen (exit settings)
      drawClock();
    break;
    default:
      machineState = STATE_MAIN;
    break;
  }
}

void buttonLeftAction(){
  switch(machineState) {
    case STATE_MAIN:
      //clock screen - maybe switch clock faces or something
    break;
    case STATE_MENU_SELECT:
      //menu select screen - cycle menus
      if(menuState == 0 ) {
        menuState = totalMenuItems-1;
      } else {
        menuState--;
      }
      drawMenu();
    break;
    case STATE_MENU_DISPLAY:
      //in a menu - decrease the selected value
      if(settingsState == 0 ) {
        settingsState = totalSettingsItems-1;
      } else {
        settingsState--;
      }
      drawMenu();
    break;
    default:
    //who knows?
    break;
  }
}

void buttonRightAction(){
   switch(machineState) {
    case STATE_MAIN:
      //clock screen - maybe switch clock faces or something
    break;
    case STATE_MENU_SELECT:
      //menu select screen - cycle menus
      menuState++;
      if(menuState > totalMenuItems) {
        menuState = 0;
      }
      drawMenu();
    break;
    case STATE_MENU_DISPLAY:
      //in a menu - increase the selected value
      settingsState++;
      if(settingsState >= totalSettingsItems) {
        settingsState = 0;
      }
      drawMenu();
    break;
    default:
    //who knows?
    break;
  }
}
