#include <OneWire.h>

// LED Pin
#define LED 13

// OneWire Initialization
#define ONEWIREPIN 11
OneWire ibutton (ONEWIREPIN); // Пин D11 для подлючения iButton (Data)
byte addr[8];
int mode = 0; //0 - read, 1 - write

void setup_ibutton(){
  pinMode(LED, OUTPUT);
}

int writeByte(byte data) {
  int data_bit;
  for (data_bit = 0; data_bit < 8; data_bit++) {
    if (data & 1) {
      digitalWrite(ONEWIREPIN, LOW); pinMode(ONEWIREPIN, OUTPUT);
      delayMicroseconds(60);
      pinMode(ONEWIREPIN, INPUT); digitalWrite(ONEWIREPIN, HIGH);
      delay(10);
    } else {
      digitalWrite(ONEWIREPIN, LOW); pinMode(ONEWIREPIN, OUTPUT);
      pinMode(ONEWIREPIN, INPUT); digitalWrite(ONEWIREPIN, HIGH);
      delay(10);
    }
    data = data >> 1;
  }
  return 0;
}

void sendLogical(int val){
  int delayMicrosecondsVal = 10;
  if (val == 0){
    delayMicrosecondsVal = 60;
  }

  digitalWrite(ONEWIREPIN, LOW); pinMode(ONEWIREPIN, OUTPUT); delayMicroseconds(delayMicrosecondsVal);
  pinMode(ONEWIREPIN, INPUT); digitalWrite(ONEWIREPIN, HIGH); delay(10);
}

void writeiButton() {
    ibutton.skip();
    ibutton.reset();
    ibutton.write(0xD1);

    sendLogical(0);
    byte* currentItem = get_item(selectedMenuItem);
    byte newID[8] = { (currentItem[0]), (currentItem[1]), (currentItem[2]), (currentItem[3]), (currentItem[4]), (currentItem[5]), (currentItem[6]), (currentItem[7]) };

    ibutton.skip();
    ibutton.reset();
    ibutton.write(0xD5);
    
    for (byte x = 0; x < 8; x++) {
      writeByte(newID[x]);
      Serial.print('*');
    }

    ibutton.reset();
    // send 0xD1
    ibutton.write(0xD1);
    sendLogical(1);
}

void readiButton() {
  digitalWrite(LED, HIGH);
  delay(50);

  byte* currentItem = get_item(selectedMenuItem);
  bool isChanged = false;
  for (byte x = 0; x < 8; x++) {
    if (!isChanged && addr[x] != currentItem[x]){
      isChanged = true;
    }
    currentItem[x] = (addr[x]);
  }
  byte crc; // Проверка контрольной суммы
  crc = ibutton.crc8(addr, 7);
  if (crc != currentItem[7]){
    clear();
    display.println("CRC mismatch!");
    display.print(currentItem[7], HEX);
    display.print(" != ");
    display.print(crc, HEX);
    display.display();
    delay(2000);
    return;
  }
  if (isChanged){
    write_item(selectedMenuItem);
  }
  display_menu();

  digitalWrite(LED, LOW);
  delay(50);
}

void process_ibutton() {
  if (!ibutton.search (addr)) {
    ibutton.reset_search();
    return;
  }

  if (mode == 0){
    readiButton();
  }
  else if (mode == 1)
  {
    writeiButton();
    ibutton_read_mode();
    display_menu();
  }
}

void ibutton_write_mode()
{
  mode = 1;
}

void ibutton_read_mode()
{
  mode = 0;
}

int ibutton_get_mode()
{
  return mode;
}