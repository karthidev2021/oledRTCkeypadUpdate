class DateTimeModel {
public:
  int year, month, day, hour, minute, second;

  DateTime getData() {
    return DateTime(year, month, day, hour, minute, second);
  }

  void updateModel(int field = -1, int data = -1) {
    if (data == -1) {
      year = rtc.now().year();
      month = rtc.now().month();
      day = rtc.now().day();
      hour = rtc.now().hour();
      minute = rtc.now().minute();
      second = rtc.now().second();
    } else {
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

DateTimeModel dateTimeModel;


void rtcSet() {

  int fieldCount = 0;
  int lengthCount = 0;
  String data = "";
  long lastScan;
  int dateTimeLength[6] = { 4, 2, 2, 2, 2, 2 };
  char fields[6][6] = {
    { "year" }, { "Mon" }, { "Day" }, { "Hr" }, { "Min" }, { "Sec" }
  };
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
