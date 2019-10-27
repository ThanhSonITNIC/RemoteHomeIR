class Device{
  private:
    byte pinRelay; // pin out of relay
    byte pinTouch; // pin in of touch switch
    long interval = 60000; // 60000 = 1' time of step
    int numTimerTimes = 0; // times of timer mode
    int numOscillationTimes = 0; // times of oscillation mode
    int numMosquitoTimes = 0; // times of mosquito mode
    int timeLiveMosquito = 30000; // time live on mode
    bool isRunning = false; // status on/off
    unsigned long millisWhenStart = 0; // = millis() when start mode
    
  public:
    Device(byte pinRelay, byte pinTouch){
      this->pinRelay = pinRelay;
      this->pinTouch = pinTouch;
    }

    Device(byte pinRelay, byte pinTouch, long interval){
      this->pinRelay = pinRelay;
      this->pinTouch = pinTouch;
      this->interval = interval;
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
      numMosquitoTimes = 0;
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
      numMosquitoTimes = 0;
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
      numMosquitoTimes = 0;
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

    // call mosquito mode
    void mosquitoTick(){
      on();
      numOscillationTimes = 0;
      numTimerTimes = 0;
      numMosquitoTimes++;
      millisWhenStart = millis();
    }

    void mosquitoEndStep(){
      if(millis() - millisWhenStart >= interval * numMosquitoTimes + timeLiveMosquito){
        on();
        millisWhenStart = millis();
      }else{
        off();
      }
    }

    void mosquitoMode(){
      if (millis() - millisWhenStart >= timeLiveMosquito) {
        mosquitoEndStep();
      }
    }

    // listen mode
    void modeListener(){
      if(numTimerTimes > 0){
        timerMode();
      }else if(numOscillationTimes > 0){
        oscillationMode();
      }else if(numMosquitoTimes > 0){
        mosquitoMode();
      }
    }
};
