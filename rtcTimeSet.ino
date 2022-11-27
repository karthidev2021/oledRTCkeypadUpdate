//The following code is for updating the rtc module using keypad and oled itself instead of setting the time by uploading the code
//This code uses MatrixKeypad.h library. It conatins two method for getting data
//one is blocking the program until the key pressed.
//We used the second one which is non-blocking method. we will scan for a certain time frequently
//so current time will keep on updating on the screen which makes us easier to enter the new data correctly.

//'A'-back
//'B'-next
//'F'-terminate 


//custom model for handling DateTime object
class DateTimeModel {
public:
  int year, month, day, hour, minute, second;

  DateTime getData() {
    return DateTime(year, month, day, hour, minute, second);
  }


  //This function will update the model frequently..As it has two arguments both has default value
  //if data is -1---> update model with current date & time
  //or else it will update with incoming data value for the specific field
  void updateModel(int field = -1, int data = -1) {
    if (data == -1) {
      year = rtc.now().year();
      month = rtc.now().month();
      day = rtc.now().day();
      hour = rtc.now().hour();
      minute = rtc.now().minute();
      second = rtc.now().second();
    } else {
      //for updating the specific field alone
      switch (field) {
        case 0:
          year = data;
          break;
        case 1:
          month = data;
          break;
        case 2:
          day = data;
          break;
        case 3:
          hour = data;
          break;
        case 4:
          minute = data;
          break;
        case 5:
          second = data;
          break;
        default: break;
      }
    }
  }
};

//object for our custom DateTime model
DateTimeModel dateTimeModel;

//This is the function which can be called anywhere in the program
// to set the RTC in real time by using keypad & with oled display.
void rtcSet() {

  int fieldCount = 0;
  int lengthCount = 0;
  String data = "";
  long lastScan;
  int dateTimeLength[6] = { 4, 2, 2, 2, 2, 2 };   //defines the length of each field like year,month....(4 digits for 'year', remaining for all fields 2 digit is sufficient)
  char fields[6][6] = {
    { "year" }, { "Mon" }, { "Day" }, { "Hr" }, { "Min" }, { "Sec" }
  };//displaying the input field message

  dateTimeModel.updateModel();
  while (fieldCount < 6) {
    dateTime = rtc.now();
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setCursor(0, 0);
    oled.setTextColor(WHITE);
    oled.println(dateTime.timestamp());
    oled.print(fields[fieldCount]);
    oled.print(":");
    oled.print(data);
    oled.display();

    //Non-blocking method for getting inputs...as per the documentation minimum 100ms is enough to scan if any key is pressed or not..
    if ((millis() - lastScan) > 100) {
      MatrixKeypad_scan(keypad);
      lastScan = millis();
    }
    if (MatrixKeypad_hasKey(keypad)) {
      key = MatrixKeypad_getKey(keypad);
      Serial.println(key);
      if (key == 'A') {
        if (fieldCount != 0) {
          fieldCount -= 1;
          data = "";
          lengthCount = 0;
        }
      } else if (key == 'B') {
        if (fieldCount != 5) {
          fieldCount += 1;
          data = "";
          lengthCount = 0;
        }
      }
      else if(key=='F'){
        break;
      } 
      else {

        oled.print(key);
        data += key;
        lengthCount += 1;
        if (dateTimeLength[fieldCount] == lengthCount) {
          lengthCount = 0;
          Serial.println(data);
          dateTimeModel.updateModel(fieldCount, data.toInt());
          data = "";
          fieldCount += 1;
          rtc.adjust(dateTimeModel.getData());
        }
      }
      dateTimeModel.updateModel();
      delay(300);
    }
  }
}
