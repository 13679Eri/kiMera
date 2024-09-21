// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPinA = 13;    // the number of the LED pin
const int ledPinB = 3;

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status
int data = 0;

int pre_button = 0;
int flag = 0;

void setup() {
  Serial.begin(115200);   // USBシリアル
  Serial1.begin(115200);  // ピンのシリアル通信
  // initialize the LED pin as an output:
  pinMode(ledPinA, OUTPUT);
  pinMode(ledPinB, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:

  if (buttonState != pre_button) {
    flag = 1;
  }

  if (buttonState == HIGH && flag == 1) {
    // turn LED on:
    digitalWrite(ledPinA, HIGH);
    data = 1;
    flag = 0;
    Serial.println(data);
    Serial1.write(data);
  } else if (buttonState == LOW && flag == 1) {
    // turn LED off:
    digitalWrite(ledPinA, LOW);
    data = 0;
    flag = 0;
    Serial.println(data);
    Serial1.write(data);
  }
  pre_button = buttonState;

  if (Serial1.available()) {          // 受信データを確認する
    byte Signal = (byte)Serial1.read();  // 受信データを読み取る
    if (Signal == 60 ) {  // 受信データが自分の番号なのか確認
      digitalWrite(ledPinB, HIGH);
    } else if (Signal == 10) {
      digitalWrite(ledPinB, LOW);
    }
  }
}
