//the main menu, where you select settings to change
void drawMenu(){
  display.clearDisplay();
  if(machineState == STATE_MENU_SELECT) { //draw the icon and text for the menu
     switch(menuState){
        case 0:
          drawMenuIcon(menuClock);
        break;
        case 1:
          drawMenuIcon(menuBluetooth);
        break;
        case 2:
          drawMenuIcon(menuGpio);
        break;
        case 3:
          drawMenuIcon(menuLed);
        break;
     }
  } else if (machineState == STATE_MENU_DISPLAY || machineState == STATE_MENU_EDIT) { //draw the actual settings within a menu
     switch(menuState){
        case 0:
          drawMenuContents(menuClock);
        break;
        case 1:
          drawMenuContents(menuBluetooth);
        break;
        case 2:
          drawMenuContents(menuGpio);
        break;
        case 3:
          drawMenuContents(menuLed);
        break;
     }
  }
}

void drawMenuIcon(Menuitem menu) {
  display.setCursor(40,40);
  display.setTextSize(1);
  display.println(menu.title);
  display.refresh();
}

// Draws the settings screen inside a menu
// @param menu - a struct detailing the menu and its settings
void drawMenuContents(Menuitem menu) {
  //page title
  int16_t pos = 30;
  display.setCursor(40,16);
  display.setTextSize(1);
  display.println(menu.title);
  //loop through menu's settings and draw
  byte i = 0;
  display.setTextSize(1);
  while(i < totalSettingsItems) {
    //is this the current selected item?
    if(settingsState == i) {
      display.drawFastVLine(1,pos,7,BLACK);
    }
    //draw the setting according to its type
    switch(menu.settings[i].type){
      case 0:
        //empty setting field
        display.setCursor(13,pos);
        display.println("----");
      break;
      case 1:
        {
        //number field
        display.setCursor(3,pos);
        byte j = 0;
        if(menu.settings[i].value < 10) {
          display.print(0);
        }
        display.print(menu.settings[i].value);
        display.print(" ");
        display.println(menu.settings[i].title);
        }
      break;
      case 2:
        //checkbox field
        display.drawRect(3,pos,7,7,BLACK);
        if(menu.settings[i].value > 0) {
          //stored value is 'true'
          display.fillRect(5,pos+2,3,3,BLACK);
        }
        display.setCursor(21,pos);
        display.println(menu.settings[i].title);
      break;
      case 3:
        //select field
        display.setCursor(23,pos);
        display.println(menu.settings[i].title);
      break;
      case 4:
        //exit
        display.setCursor(3,pos);
        display.print("< ");
        display.println(menu.settings[i].title);
      break;
      default:
        //default?
      break;
    }

    pos += 9; //increase vertical position for next line
    i++; //next setting
  }
  display.refresh();
}

