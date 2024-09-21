int sw[6];       //鍵盤の数
int flag[6];     //フラグ

//MIDI
//#include <MIDI.h>
//MIDI_CREATE_DEFAULT_INSTANCE();

//これで音変わる？
#define MIDI_CH  1
int pn = 41;

void setup() {
  //  MIDI.begin();
  //  Serial.begin(31250);
  Serial.begin(115200);
  //  MIDI.turnThruOff();
  //  MIDI.sendProgramChange(pn, MIDI_CH);

  pinMode(2, INPUT);
  pinMode(3, INPUT);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
}

void loop() {
  digitalWrite(6, LOW);
  delay(10);
  digitalWrite(4, HIGH);
  sw[0] = digitalRead(2);
  sw[1] = digitalRead(3);

  digitalWrite(4, LOW);
  delay(10);
  digitalWrite(5, HIGH);
  sw[2] = digitalRead(2);
  sw[3] = digitalRead(3);

  digitalWrite(5, LOW);
  delay(10);
  digitalWrite(6, HIGH);
  sw[4] = digitalRead(2);
  sw[5] = digitalRead(3);

  note();

  //音色をランダムで変える
  if (sw[5] == 1) {
    if (flag[5] == 0) {
      flag[5] = 1;
      pn = random(0, 127);
    } else {
      if (flag[5] == 1) {
        flag[5] = 0;
      }
    }
  }

  for (int i = 0; i < sizeof(sw) / sizeof(sw[0]); i++) {
    Serial.print(sw[i]);
  }
  Serial.println("");
  delay(10);

  //    MIDI.sendProgramChange(pn, MIDI_CH);
}

void note() {
  //  MIDI.sendProgramChange(1, 1);
  int pitch[] = {60, 61, 62, 63, 64};
  for (int i = 0; i < sizeof(pitch) / sizeof(pitch[0]); i++) {
    if (sw[i] == 1) {
      if (flag[i] == 0) {
        flag[i] = 1;
        //        Serial.println(pitch[i]);
        //        MIDI.sendNoteOn(pitch[i], 127, 1);
      }
    } else {
      if (flag[i] == 1) {
        flag[i] = 0;
        //        MIDI.sendNoteOff(pitch[i], 0, 1);
      }
    }
  }
}
