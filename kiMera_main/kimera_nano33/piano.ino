void set_piano(){ 
  //ピアノ
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);

  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
}

void piano() {
  digitalWrite(12, LOW);
  delay(10);
  digitalWrite(7, HIGH);
  sw[0] = digitalRead(2);
  sw[1] = digitalRead(3);
  sw[2] = digitalRead(4);
  sw[3] = digitalRead(5);
  sw[4] = digitalRead(6);

  digitalWrite(7, LOW);
  delay(10);
  digitalWrite(8, HIGH);
  sw[5] = digitalRead(2);
  sw[6] = digitalRead(3);
  sw[7] = digitalRead(4);
  sw[8] = digitalRead(5);
  sw[9] = digitalRead(6);

  digitalWrite(8, LOW);
  delay(10);
  digitalWrite(9, HIGH);
  sw[10] = digitalRead(2);
  sw[11] = digitalRead(3);
  sw[12] = digitalRead(4);
  sw[13] = digitalRead(5);
  sw[14] = digitalRead(6);

  digitalWrite(9, LOW);
  delay(10);
  digitalWrite(10, HIGH);
  sw[15] = digitalRead(2);
  sw[16] = digitalRead(3);
  sw[17] = digitalRead(4);
  sw[18] = digitalRead(5);
  sw[19] = digitalRead(6);

  digitalWrite(10, LOW);
  delay(10);
  digitalWrite(11, HIGH);
  sw[20] = digitalRead(2);
  sw[21] = digitalRead(3);
  sw[22] = digitalRead(4);
  sw[23] = digitalRead(5);
  sw[24] = digitalRead(6);

  digitalWrite(11, LOW);
  delay(10);
  digitalWrite(12, HIGH);
  sw[25] = digitalRead(2);
  sw[26] = digitalRead(3);
  sw[27] = digitalRead(4);
  sw[28] = digitalRead(5);
  sw[29] = digitalRead(6);

  piano_note();
}

void piano_note() {
  ch = 1;
  int pitch[] = { 48, 49, 50, 51, 52,
                  53, 54, 55, 56, 57,
                  58, 59, 60, 61, 62,
                  63, 64, 65, 66, 67,
                  68, 69, 70, 71, 72,
                  73, 74, 75, 76, 77
                };
  for (int i = 0; i < 30; i++) {
    if (sw[i] == 1) {
      if (flag[i] == 0) {
        flag[i] = 1;
        data_send(1, pitch[i], flag[i], 127);
      }
    } else {
      if (flag[i] == 1) {
        flag[i] = 0;
        data_send(1, pitch[i], flag[i], 0);
      }
    }
  }
}
