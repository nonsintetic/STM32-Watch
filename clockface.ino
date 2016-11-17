void drawClock() {
    display.clearDisplay();
    //draw hours and minutes
    display.setTextSize(3);
    display.setCursor(5,30);
    if(currentHours < 10) { display.print(0); }
    display.print( currentHours );
    if(colon == 1) {
      display.print( ":" );
      colon = 0;
    } else {
      display.print( " " );
      colon = 1;
    }
    if(currentMinutes < 10) { display.print(0); }
    display.print( currentMinutes );
    
    //draw seconds
    display.setCursor(82,20);
    display.setTextSize(1);
    if(currentSeconds < 10) { display.print(0); }
    display.println( currentSeconds );
}

