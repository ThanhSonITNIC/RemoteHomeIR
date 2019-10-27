/*
 * Chế độ timer: hẹn giờ tắt. Thời gian tắt = interval * times of call
 * Chế độ oscillation: bật tắt theo chu kỳ. Thời gian bật = tắt = interval * times of call
 * Chế độ mosquito: bật một khoảng thời gian nhất định, tắt theo lần gọi. Thời gian tắt = interval * times of call
 *  
*/

/*
Fan Panasonic
Power: 14422590
Timer: 14541345
Speed: 14672385
Mode: 14475825
Oscillation: 14606865
*/

/*
Fan Airmate
Power: 2737486129
Timer: 3768077238
Speed: 338831067
Mode: 924466310
Oscillation: 970202566
*/

#include <IRremote.h> // thư viện hỗ trợ IR remote
#include "Device.h";

const int receiverPin = 2; // chân dùng để đọc tín hiệu
IRrecv irrecv(receiverPin); // tạo đối tượng IRrecv mới
decode_results results;// lưu giữ kết quả giải mã tín hiệu

#define pinNotify 3

Device device1(9, 5);
Device device2(8, 4, 1800000); // interval = 1800000 = 30'

void setup()
{
  irrecv.enableIRIn(); // start the IR receiver

  device1.init();
  device2.init();
}

void notify(){
  digitalWrite(pinNotify, HIGH);
  delay(100);
  digitalWrite(pinNotify, LOW);
}

void loop()
{
  device1.touchListener(); 
  device1.modeListener();

  device2.touchListener(); 
  device2.modeListener();
   
  if (irrecv.decode(&results)) // nếu nhận được tín hiệu
  {
    switch(results.value){
      
      // device 1
      case 14422590: //power
        device1.toggle();
        notify();
        break;
      case 14541345: //timer
        device1.timerTick();
        notify();
        break;
      case 14606865: //oscillation
        device1.oscillationTick();
        notify();
        break;
      case 14475825: //mosquito
        device1.mosquitoTick();
        notify();
        break;
        
      // device 2
      case 2737486129: //power
        device2.toggle();
        notify();
        break;
      case 3768077238: //timer
        device2.timerTick();
        notify();
        break;
      case 970202566: //oscillation
        device2.oscillationTick();
        notify();
        break;
      case 924466310: //mosquito
        device2.mosquitoTick();
        notify();
        break;
    }

    delay(200);
    irrecv.resume(); // nhận giá trị tiếp theo
  }
}
