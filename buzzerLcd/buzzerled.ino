#define C 2093
#define E 2637
#define G 3136

#include <LiquidCrystal_I2C.h>

typedef struct _melody {
  int len;
  short freq[40];
  short duration[40];
} Melody;

Melody myMelody[7] = {
  {2,  {5587, 4186}, {200, 200}}, // index 0 알림음
  {5,  {6271, 5587, 5274, 4698, 8372}, {200, 300, 300, 500, 400}}, // 신나는 음악
  {8,  {164, 146, 164, 146, 164, 146, 164, 146}, {50, 50, 50, 50, 50, 50, 50, 50}}, // 급한 음악
  {7,  {1046, 1174, 1318, 1046, 1318, 1046, 1318}, {500, 300, 500, 300, 300, 300, 1000}}, // 도레미도미레미
  {9,  {2637, 2489, 2637, 2489, 2637, 1975, 2349, 2093, 1760}, {300, 300, 300, 300, 300, 300, 300, 300, 1000}},
  {38, {1568, 0, 1760, 1568, 0, 1760, 1975, 0, 2349, 2637, 0, 2349, 3136, 0, 2793, 3520, 0, 3136, 2793, 0, 3520, 3136, 0, 2637, 2349, 0, 2349, 2637, 0, 2349, 3136, 0, 2637, 2349, 0, 1975, 1760}, {300, 100, 150, 150, 100, 150, 150, 100, 150, 150, 100, 150, 150, 100, 150, 150, 100, 150, 150, 100, 150, 150, 100, 150, 150, 100, 150, 150, 100, 150, 150, 100, 150, 150, 100, 150, 1000}},
  {29, {G,E,0,E,0,E,0,E,0,E,G,E,C,E,G,E,C,E,G,E,0,E,C,0,C,0,C,0,C}, {300,450,50,150,50,150,50,350,50,200,200,200,400,200,200,200,200,400,200,150,50,200,150,50,150,50,150,50,1000}}
};

String input, substr;
String pic_name;
byte BUZZER = 10; // BUZZER 10번핀
byte note_n;
short frequency[100];
short duration[100];

int index;

LiquidCrystal_I2C lcd(0x3F, 16, 2); // channel, 16 x 2

void showAni() {
  Serial.println("ani");
  Serial.println(note_n);
  for(int i =0;i<note_n; i++){
    Serial.print(frequency[i]);
    Serial.print(' ');
  }
  Serial.println();
  for(int i =0;i<note_n; i++){
    Serial.print(duration[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println(pic_name);
  lcd.clear();
  lcd.print(pic_name);

  for (int noteIndex = 0; noteIndex < note_n; noteIndex++) {
    tone(BUZZER, frequency[noteIndex]);
    delay(duration[noteIndex]);
  }
  noTone(BUZZER);
}

void playSound(int set) {
  Serial.println("alarm");
  for (int i = 0; i < myMelody[set].len; i++) {
    tone(BUZZER, myMelody[set].freq[i]);
    delay(myMelody[set].duration[i]);
  }
  noTone(BUZZER);
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
  if (Serial.available() > 0) {
    input = Serial.readStringUntil('\n');
    delay(100);
    Serial.println(input);
    // 종류 받음
    substr = input.substring(0, input.indexOf('&'));
    input = input.substring(input.indexOf('&') + 1);
    if (substr.equals("animation")) {
      //note n
      note_n = input.substring(0, input.indexOf('&')).toInt();
      input = input.substring(input.indexOf('&') + 1);
      //frequency
      substr = input.substring(0, input.indexOf('&'));
      input = input.substring(input.indexOf('&') + 1);
      for (index = 0; index < note_n; index++) {
        frequency[index] = substr.substring(0, substr.indexOf(' ')).toInt();
        substr = substr.substring(substr.indexOf(' ') + 1);
      }
      //duration
      substr = input.substring(0, input.indexOf('&'));
      pic_name = input.substring(input.indexOf('&') + 1); // pic_name
      for (index = 0; index < note_n; index++) {
        duration[index] = substr.substring(0, substr.indexOf(' ')).toInt();
        substr = substr.substring(substr.indexOf(' ') + 1);
      }
      showAni();
    } else if(substr.equals("alarm")) {
      playSound(input.toInt());
    }
  }
}
