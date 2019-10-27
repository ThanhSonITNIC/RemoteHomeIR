class Device{
  private:
    byte pinRelay; // pin out of relay
    byte pinTouch; // pin in of touch switch
    long interval = 300000; // 300000 = 5' time of step
    int numTimerTimes = 0; // times of timer mode
    int numOscillationTimes = 0; // times of oscillation mode
    bool isRunning = false; // status on/off
    unsigned long millisWhenStart = 0; // = millis() when start mode
    
  public:
    Device(byte pinRelay, byte pinTouch){
      this->pinRelay = pinRelay;
      this->pinTouch = pinTouch;
    }
    
    void init(){
      pinMode(pinRelay, OUTPUT);
      pinMode(pinTouch, INPUT); 
      digitalWrite(pinRelay, LOW);  
    }

    // turn on
    void on(){
      digitalWrite(pinRelay, HIGH);
      isRunning = true;
    }

    // turn off
    void off(){
      digitalWrite(pinRelay, LOW);
      isRunning = false;	  
    }
    
    // switch toggle and off all mode
    void toggle(){
      isRunning ? off() : on();
      numTimerTimes = 0;
      numOscillationTimes = 0;
    }

    // if touch (hardware) => toggle
    void touchListener(){
      if(digitalRead(pinTouch) == HIGH){
        toggle();
        delay(500);
      }
    }

    // call timer mode
    void timerTick(){
      on();
      numTimerTimes++;
      numOscillationTimes = 0;
      millisWhenStart = millis();
    }

    // on timer mode
    void timerMode(){
      if (millisWhenStart + interval * numTimerTimes <= millis()) {
        off();
        numTimerTimes = 0;
      }
    }

    // call oscillation mode
    void oscillationTick(){
      on();
      numOscillationTimes++;
      numTimerTimes = 0;
      millisWhenStart = millis();
    }

    // on oscillation mode
    void oscillationMode(){
      if (millis() - millisWhenStart >= interval * numOscillationTimes) {
        oscillationEndStep();
      }
    }

    // step oscillation mode
    void oscillationEndStep(){
      if(millis() - millisWhenStart >= (interval * numOscillationTimes)*2){
        on();
        millisWhenStart = millis();
      }else{
        off();
      }
    }

    // listen mode
    void modeListener(){
      if(numTimerTimes > 0){
        timerMode();
      }else if(numOscillationTimes > 0){
        oscillationMode();  
      }
    }
};
