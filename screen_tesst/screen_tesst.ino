#include <M5Core2.h>
//弦木金打の順にする？

//センサ
//ヴァイオリン
#include <Unit_Sonic.h>　　//超音波
#include "IMU_6886.h"　　　//加速度

//トランペット
#include <driver/i2s.h>
#include "fft.h"
#include <math.h>

#define PIN_CLK  19
#define PIN_DATA 27
#define MODE_MIC 0

//ヴァイオリン
IMU_6886 imu6886;
SONIC_I2C sensor;
#define JOY_ADDR 0x52     //ジョイスティック


//violinのグローバル変数
float accX = 0;
float accY = 0;
float accZ = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;

float distan = 0;
float angle = 0;
uint8_t x_data, y_data, button_data;

bool chord = false;
bool isConnected = false;
bool isSounding = false;

//MIDI
#include "M5UnitSynth.h"
M5UnitSynth synth;
int note_color[4] = {1 , 41, 57, 74};
int nc = 0;

// 受信データ格納
int ch = 0;
int note = 0;
bool ring = 0;
int velo = 0;

int patern = 0;
int prev_note = -1;
float mainbody_ag = 0;

//送信データ
int mode = 0;
int previousMode = 0;

//ボタン
ButtonColors cl_on  = {0x7BEF, 0x7f7f7f, 0x7f7f7f}; // タップした時の色 (背景, 文字列, ボーダー)
ButtonColors cl_p = {BLACK, WHITE, BLACK}; // 指を離した時の色 (背景, 文字列, ボーダー)
ButtonColors cl_v = {0xCB8D4A, WHITE, 0xCB8D4A}; // 指を離した時の色 (背景, 文字列, ボーダー)
ButtonColors cl_t = {0xFFDD31, WHITE, 0xFFDD31}; // 指を離した時の色 (背景, 文字列, ボーダー)
ButtonColors cl_f = {0xDEDCD7, WHITE, 0xDEDCD7}; // 指を離した時の色 (背景, 文字列, ボーダー)

Button btn_p(200, 20, 90, 35, false , "piano", cl_p, cl_on);
Button btn_v(200, 75, 90, 35, false , "violin", cl_v, cl_on);
Button btn_t(200, 130, 90, 35, false , "trumpet", cl_t, cl_on);
Button btn_f(200, 185, 90, 35, false , "flute", cl_f, cl_on);
//
//Button btn_v(200, 75, 90, 35, false , "violin", cl_v, cl_on);
//Button btn_f(200, 185, 90, 35, false , "flute", cl_f, cl_on);
//Button btn_t(200, 130, 90, 35, false , "trumpet", cl_t, cl_on);
//Button btn_p(200, 20, 90, 35, false , "piano", cl_p, cl_on);

void setup() {
  M5.begin();

  //シリアル
  //  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 13, 14); //nanoから受け取る
  Serial2.begin(115200, SERIAL_8N1, 3, 1); //MIDI

  //MIDI
  synth.begin(&Serial2, UNIT_SYNTH_BAUD, 33, 32);
  //いきなり全部チャンネル設定するとダメみたい
  //送られたchによって変えることにした
  for (int i = 0; i < 4; i++) {
    if (ch == i + 1) {
      synth.setInstrument(0, i + 1, note_color[i]);
    }
  }

  //画面 ボタン
  M5.Lcd.setTextSize(1);  //Set the font size.
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.fillScreen(0xffb9dc);
  M5.Lcd.fillRect(30, 14, 150, 200, 0xff0d);

  //センサ Wire: SDA SCL
  Wire.begin(21, 22); //joy
  sensor.begin(&Wire, 0x57, 21, 22, 400000L); //超音波
  imu6886.Init(21, 22); //加速度
  InitI2SMic(); //PDM

  btn_p.addHandler(event_btn_p, E_TOUCH + E_RELEASE); // ボタンのイベント関数を指定
  btn_v.addHandler(event_btn_v, E_TOUCH + E_RELEASE);
  btn_t.addHandler(event_btn_t, E_TOUCH + E_RELEASE);
  btn_f.addHandler(event_btn_f, E_TOUCH + E_RELEASE);
  M5.Buttons.draw(); // 全てのボタンを描画
}

void loop() {
  M5.update();

  // モードが変更された場合のみセット関数を呼び出す
  if (mode != previousMode) {
    Serial1.write(mode);
    for (int i = 0; i < 4; i++) {
      synth.setAllNotesOff(i);
      //      M5.Lcd.setCursor(35, 100);
      //      M5.Lcd.print("AllNoteoff");
    }
    if (mode == 1) {
      set_piano();
      //      Serial.println("piano");
    } else if (mode == 2) {
      set_violin();
      //      Serial.println("violin");
    } else if (mode == 3) {
      set_trumpet();
      //      Serial.println("trumpet");
    } else if (mode == 4) {
      set_flute();
      //      Serial.println("flute");
    }
    previousMode = mode; // 前回のモードを更新
  }

  // 現在のモードに基づいて関数を呼び出す
  switch (mode) {
    case 1:
      piano();
      break;
    case 2:
      violin();
      break;
    case 3:
      trumpet();
      break;
    case 4:
      flute();
      break;
  }
  delay(10); // 少し待機してから再度ループ
}

void event_btn_p(Event & e) {
  if (e == E_TOUCH) {
    mode = 1;
  }
}

void event_btn_v(Event & e) {
  if (e == E_TOUCH) {
    mode = 2;
  }
}

void event_btn_t(Event & e) {
  if (e == E_TOUCH) {
    mode = 3;
  }
}

void event_btn_f(Event & e) {
  if (e == E_TOUCH) {
    mode = 4;
  }
}

void set_piano() {
  //描画
  M5.Lcd.fillRect(30, 14, 150, 200, 0xff0d);
  M5.Lcd.fillRect(60, 120, 90, 30, BLACK);  //Draw a 50x50 blue rectangle wireframe at (x,y).
  M5.Lcd.fillRect(84, 90, 66, 30, BLACK);
  M5.Lcd.fillCircle(117, 90, 32, BLACK);
  M5.Lcd.fillRect(70, 135, 70, 15, WHITE);
  M5.Lcd.drawLine(80, 140, 80, 150, BLACK);
  M5.Lcd.drawLine(90, 140, 90, 150, BLACK);
  M5.Lcd.drawLine(110, 135, 110, 150, BLACK);
  M5.Lcd.drawLine(120, 140, 120, 150, BLACK);
  M5.Lcd.drawLine(130, 140, 130, 150, BLACK);
  M5.Lcd.fillRect(80, 135, 4, 8, BLACK);
  M5.Lcd.fillRect(90, 135, 4, 8, BLACK);
  M5.Lcd.fillRect(110, 135, 4, 8, BLACK);
  M5.Lcd.fillRect(120, 135, 4, 8, BLACK);
  M5.Lcd.fillRect(130, 135, 4, 8, BLACK);
}

void piano() {
  M5.update();
  ch = 1;
  serial_receve();
  //  M5.Lcd.setCursor(35, 215);
  //  M5.Lcd.printf("mode=%d", mode);
}

void set_violin() {
  M5.Lcd.fillRect(30, 14, 150, 200, 0xff0d);
  M5.Lcd.fillCircle(110, 90, 30, 0xcb24);
  M5.Lcd.fillCircle(110, 135, 30, 0xcb24);
  M5.Lcd.fillRect(105, 40, 10, 70, BLACK);
  M5.Lcd.drawLine(95, 100, 85, 130, BLACK);
  M5.Lcd.drawLine(120, 100, 130, 130, BLACK);
  M5.Lcd.fillTriangle(105, 135, 115, 135, 110, 165, BLACK);
}

void violin() {
  M5.update();
  ch = 2;
  synth.setInstrument(0, 2, note_color[1]);
  //  M5.Lcd.setCursor(35, 215);
  //  M5.Lcd.printf("mode=%d", mode);

  //IMU
  imu6886.getGyroData(&gyroX, &gyroY, &gyroZ);
  imu6886.getAccelData(&accX, &accY, &accZ);
  //角度
  angle = atan2(accX, accZ) * 180.0 / PI;
  //  M5.Lcd.setCursor(35, 125);
  //  M5.Lcd.printf("angle=%f", angle);

  //joystick
  Wire.requestFrom(JOY_ADDR, 3);  //Request 3 bytes from the slave device.  向从设备请求3个字节
  if (Wire.available()) {  //If data is received.  如果接收到数据
    x_data = Wire.read();
    y_data = Wire.read();
    button_data = Wire.read();
  }

  if (120 <= x_data && x_data <= 130) {
    //    flag = 0;
    note = -1;
    //止める
    synth.setAllNotesOff(2);
    synth.setInstrument(0, 2, note_color[1]);
    prev_note = note;
  } else if (130 < x_data) {
    Serial1.write(mode);
    //本体の角度
    if (Serial1.available()) {
      String receivedData = Serial1.readString();  // シリアルから文字列としてデータを受け取る
      mainbody_ag = receivedData.toFloat();
    }
    angle += mainbody_ag;
    distan = sensor.getDistance() / 10;

    note = violin_Pitch(angle, distan);
    if (prev_note != note) {
      //      flag = 1;
      synth.setNoteOff(ch, prev_note, 0);
      synth.setNoteOn(ch, note, 127);
      prev_note = note;
    }
  } else if (x_data < 120) {
    Serial1.write(mode);
    //本体の角度
    if (Serial1.available()) {
      String receivedData = Serial1.readString();  // シリアルから文字列としてデータを受け取る
      mainbody_ag = receivedData.toFloat();
    }
    angle += mainbody_ag;
    distan = sensor.getDistance() / 10;

    note = violin_Pitch(angle, distan);
    if (prev_note != note) {
      //      flag = 2;
      synth.setNoteOff(ch, prev_note, 0);
      synth.setNoteOn(ch, note, 127);
      prev_note = note;
    }
  }

  //描画
  //  M5.Lcd.setCursor(35, 145);
  //  M5.Lcd.printf("ch=%d", ch);
  //  M5.Lcd.setCursor(35, 160);
  //  M5.Lcd.printf("dis=%f", distan);
  //  M5.Lcd.setCursor(35, 175);
  //  M5.Lcd.printf("body_ag=%f", mainbody_ag);
  //  M5.Lcd.setCursor(35, 190);
  //  M5.Lcd.printf("joy=%d", x_data);
  //  M5.Lcd.setCursor(35, 215);
  //  M5.Lcd.printf("mode=%d", mode);
}

void set_trumpet() {
  M5.Lcd.fillRect(30, 14, 150, 200, 0xff0d);
  M5.Lcd.fillTriangle(60, 75, 90, 90, 60, 105, YELLOW);
  M5.Lcd.fillRect(85, 88, 80, 4, YELLOW);
  M5.Lcd.fillRect(110, 88, 4, 30, YELLOW);
  M5.Lcd.fillRect(130, 88, 4, 30, YELLOW);
  M5.Lcd.fillRect(150, 88, 4, 30, YELLOW);
  M5.Lcd.drawLine(88, 90, 90, 118, YELLOW);
  M5.Lcd.drawLine(90, 118, 154, 118, YELLOW);
  M5.Lcd.fillTriangle(173, 83, 163, 88, 173, 93, YELLOW);
}

void trumpet() {
  M5.update();
  ch = 3;
  synth.setInstrument(0, ch, note_color[2]);
  //  M5.Lcd.setCursor(35, 215);
  //  M5.Lcd.printf("mode=%d", mode);

  uint8_t* microRawData = (uint8_t*)calloc(2048, sizeof(uint8_t));
  size_t bytesread;
  i2s_read(I2S_NUM_0, (char*)microRawData, 2048, &bytesread, portMAX_DELAY);

  fft_config_t* real_fft_plan = fft_init(1024, FFT_REAL, FFT_FORWARD, NULL, NULL);
  int16_t* buffptr = (int16_t*)microRawData;

  for (int i = 0; i < real_fft_plan->size; i++) {
    float adc_data = (float)map(buffptr[i], INT16_MIN, INT16_MAX, -2000, 2000);
    real_fft_plan->input[i] = adc_data;
  }

  //周波数
  fft_execute(real_fft_plan);
  float pitch = calculatePitch(real_fft_plan->output, real_fft_plan->size, 44100);

  // 音量
  float volume = calculateVolume(buffptr, 1024);
  //  int trVelo = toVelocity(volume);

  fft_destroy(real_fft_plan);
  free(microRawData);

  if (volume >= 100 && 270 <= pitch && pitch <= 1100) {
    synth.setInstrument(0, 3, 57);
    Serial1.write(mode);
    serial_receve(); //paternが送られてくる
    note = frequencyToMidiNoteNumber(pitch); //基準音決める
    note = convertNote(patern); //基準音とpaternから鳴る音を決める
    if (prev_note != note) {
      synth.setNoteOff(3, prev_note, 0); //前の音消す
      synth.setNoteOn(3, note, 127); //鳴らす
      prev_note = note;
    }
  } else {
      if (prev_note != -1) {
        note = -1;
        synth.setNoteOn(3, note, 0);
        prev_note = note;
      }
  }

  //  M5.Lcd.setCursor(35, 120);
  //  M5.Lcd.printf("ch=%d", ch);
  //  M5.Lcd.setCursor(35, 135);
  //  M5.Lcd.printf("volume=%d", volume);
  //  M5.Lcd.setCursor(35, 150);
  //  M5.Lcd.printf("patern=%d", patern);
  //  M5.Lcd.setCursor(35, 175);
  //  M5.Lcd.printf("note=%d", note);
}

void set_flute() {
  M5.Lcd.fillRect(30, 14, 150, 200, 0xff0d);
  M5.Lcd.fillRect(43, 98, 100, 15,  0xd69a);
  M5.Lcd.fillCircle(63, 108, 5, 0x9492);
  M5.Lcd.fillCircle(83, 108, 5, 0x9492);
  M5.Lcd.fillCircle(102, 108, 5, 0x9492);
  M5.Lcd.fillCircle(132, 108, 5, 0x9492);
}

void flute() {
  M5.update();
  ch = 4;
  //  M5.Lcd.setCursor(35, 215);
  //  M5.Lcd.printf("mode=%d", mode);
  serial_receve();
}

int split(String data, char delimiter, String * dst) {
  int index = 0;
  int datalength = data.length();
  for (int i = 0; i < datalength; i++) {
    char tmp = data.charAt(i);
    if (tmp == delimiter) {
      index++;
    } else {
      dst[index] += tmp;
    }
  }
  return (index + 1);
}


void serial_receve() {
  if (Serial1.available()) {      // 受信データを確認する
    String cmds[4] = {"\n"}; //グローバル変数にすると桁が増えていっちゃう
    String myString = Serial1.readStringUntil('\n');  //改行されるところまで読む

    // 分割数 = 分割処理(文字列, 区切り文字, 配列)
    int index = split(myString, ',', cmds);

    ch = cmds[0].toInt();
    note = cmds[1].toInt();
    ring = cmds[2].toInt();
    velo = cmds[3].toInt();

    //いきなり全部チャンネル設定するとダメみたい
    //送られたchによって変えることにした
    for (int i = 0; i < 4; i++) {
      if (ch == i + 1) {
        synth.setInstrument(0, i + 1, note_color[i]);
      }
    }

    if (ch == 3) {
      patern = note; //0~5
    } else {
      //鳴らす
      if (ring == 1) {
        synth.setNoteOn(ch, note, velo);
      } else {
        synth.setNoteOff(ch, note, velo);
      }
    }

    if (note == -1) {
      for (int i = 0; i < 4; i++) {
        synth.setAllNotesOff(i);
      }
    }

    //    M5.Lcd.setCursor(35, 20);
    //    M5.Lcd.printf("ch=%d,note=%d,ring=%d,velo=%d", ch, note, ring, velo);
  }
}


int violin_Pitch(float angle, float newvalue) {
  double dis[] = {28, 26.5, 24.5, 22.5, 20.5, 19.5, 18.5};
  int size = sizeof(dis) / sizeof(dis[0]);

  if (35 <= angle && angle <= 45) { // G弦
    chord = false;
    int notes[] = {55, 56, 57, 58, 59, 60, 61};
    return determineNote(dis, notes, size, newvalue);
  }

  if (15 <= angle && angle <= 25) { // D弦
    chord = false;
    int notes[] = {62, 63, 64, 65, 66, 67, 68};
    return determineNote(dis, notes, size, newvalue);
  }

  if (-25 <= angle && angle <= -15) { // A弦
    chord = false;
    int notes[] = {69, 70, 71, 72, 73, 74, 75};
    return determineNote(dis, notes, size, newvalue);
  }

  if (-45 <= angle && angle <= -35) { // E弦
    chord = false;
    int notes[] = {76, 77, 78, 79, 80, 81, 82};
    return determineNote(dis, notes, size, newvalue);
  }

  return -2; // エラーハンドリング
}


int determineNote(double dis[], int notes[], int size, float newvalue) {
  for (int i = 0; i < size; i++) {
    if (i == 0) {
      if (dis[i] <= newvalue) {
        return notes[i];
      }
    } else {
      if (dis[i] <= newvalue && newvalue <= dis[i - 1]) {
        return notes[i];
      }
    }
  }
  return notes[size - 1]; // デフォルトで最後の音符を返す
}

// PDM初期化
bool InitI2SMic() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,
    .dma_buf_len = 128,
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_PIN_NO_CHANGE,
    .ws_io_num = PIN_CLK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = PIN_DATA
  };
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);

  return true;
}

//fft
float calculatePitch(float * fftOutput, int fftSize, float sampleRate) {
  float maxVal = 0;
  int maxIndex = 0;

  for (int i = 1; i < fftSize / 2; i++) {
    double magnitude = sqrt(fftOutput[2 * i] * fftOutput[2 * i] + fftOutput[2 * i + 1] * fftOutput[2 * i + 1]);
    if (magnitude > maxVal) {
      maxVal = magnitude;
      maxIndex = i;
    }
  }

  float peakFrequency = (float)maxIndex * sampleRate / fftSize;
  return peakFrequency;
}


// 周波数をMIDIノート番号に変換
int frequencyToMidiNoteNumber(float frequency) {
  if (frequency <= 0) return 0;
  if (frequency <= 277.2) {
    return 58;
  } else if (277.2 < frequency && frequency <= 392) {
    return 65;
  } else if (392 < frequency && frequency <= 554.4) {
    return 70;
  } else if (554.4 < frequency && frequency <= 784) {
    return 77;
  } else if (784 < frequency && frequency <= 1108.7) {
    return 82;
  } else if (1108.7 < frequency) {
    return 89;
  }
}

// 音量を計算する関数
float calculateVolume(int16_t* buffer, int bufferSize) {
  int32_t sum = 0;
  for (int i = 0; i < bufferSize; i++) {
    sum += abs(buffer[i]);
  }
  return (float)sum / bufferSize;
}

int toVelocity(float vl) {
  // 圧力変化を音量にマップ（適宜調整）
  return map(vl, 100, 500, 0, 127);
}

int convertNote(int patern) {
  switch (patern) {
    case 0:  return note; // 押さえてない
    case 1:  return note - 2; // flag1 だけが true
    case 2:  return note - 1; // flag4 だけが true
    case 3:  return note - 3; // flag5 だけが true
    case 4:  return note - 3; // flag1 と flag4 が true
    case 5:  return note - 4; // flag4 と flag5 が true
    case 6:  return note - 5; // flag1 と flag5 が true
    case 7:  return note - 6; // flag1, flag4, flag5 が全て true
    default: return -1;    // 上記以外の場合
  }
}
