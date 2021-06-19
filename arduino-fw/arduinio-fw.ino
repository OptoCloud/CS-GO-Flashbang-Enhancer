void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
}

int i = 0;
int value = 0;

void loop() {
  if (Serial.available() > 0)
  {
    uint8_t c = Serial.read();

      char msg[16]{0};
      
    switch (i++) {
    case 0:
      if (c != 0xAA) {
        i = 0;
      }
      break;
    case 1:
      value = c;
      break;
    case 2:
      if (c == 0xCC) {
        analogWrite(13, value);
      }
      i = 0;
      break;
    default:
      i = 0;
      break;
    }
  }
}