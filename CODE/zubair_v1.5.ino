/*---------------------------Deklarasi LCD -----------------------------------------------------*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
/*---------------------------Deklarasi Pin Trigger----------------------------------------------*/
#define triggerA  18
#define triggerB  16
#define triggerC  11
#define triggerD  7
#define triggerE  3
#define triggerF  49
#define triggerG  43
#define triggerH  39
#define triggerI  35
/*---------------------------Deklarasi Pin Echo-------------------------------------------------*/
#define echoA  17
#define echoB  12
#define echoC  9
#define echoD  5
#define echoE  51
#define echoF  45
#define echoG  41
#define echoH  37
#define echoI  33
/*---------------------------Deklarasi Pin Lampu Jalur 1----------------------------------------*/
#define merah1  30
#define kuning1  53
#define hijau1  52
/*---------------------------Deklarasi Pin Lampu Jalur 2----------------------------------------*/
#define merah2  50
#define kuning2  48
#define hijau2  47
/*---------------------------Deklarasi Pin Lampu Jalur 3----------------------------------------*/
#define merah3  46
#define kuning3  44
#define hijau3  42
/*---------------------------Deklarasi Pin Sensor Suara-----------------------------------------*/
#define suara1 31
#define suara2 34
#define suara3 38

long duration, A, B, C, D, E, F, G, H, I; //variabel hasil baca sensor ultrasonik
unsigned long waktuSebelumnya = 0; //variabel untuk fungsi millis

int batas = 14; //batas pembacaan sensor ultrasonik
int x; //menyimpan 3 nilai hasil pembacaan sensor ultrasonik
int detik; //counter up detik
int detikShow; //counter down detik
int cursorY = 1; //perpindahan kursor
int nyalaMerah = 0; //variabel menyimpan waktu nyala lampu merah
int nyalaHijau = 5; //variabel menyimpan waktu nyala lampu hijau
int nyalaKuning = 2; //variabel menyimpan waktu nyala lampu kuning
int allRed = 3; //semua warna merah
int saatKosong = 5; //variabel menyimpan waktu nyala lampu saat kosong
int saatSepi = 10; //variabel menyimpan waktu nyala lampu saat sepi
int saatSedang = 20; //variabel menyimpan waktu nyala lampu saat sedang
int saatPadat = 30; //variabel menyimpan waktu nyala lampu saat padat

bool sA, sB, sC; //variabel menyimpan hasil pembacaan sensor suara
boolean nyalaJalur1 = true; //variabel penanda jalur 1
boolean nyalaJalur2 = false; //variabel penanda jalur 2
boolean nyalaJalur3 = false; //variabel penanda jalur 3

String statusJalur = ""; //variabel kondisi jalur
String statusSuara = ""; //variabel kondisi suara
String kondisi = ""; //variabel kondisi jalur + suara
String lampu = ""; //variabel kondisi lampu
String data = ""; //variabel penampil data

void setup() {
  Serial.begin(9600); //delkarasi komunikasi serial
  //deklarasi lcd
  lcd.init();
  lcd.backlight();
  /*---------------------------Konfigurasi Pin Sensor Jalur 1------------------------------------*/
  pinMode(triggerA, OUTPUT);
  pinMode(echoA, INPUT);
  pinMode(triggerB, OUTPUT);
  pinMode(echoB, INPUT);
  pinMode(triggerC, OUTPUT);
  pinMode(echoC, INPUT);
  /*---------------------------Konfigurasi Pin Sensor Jalur 2------------------------------------*/
  pinMode(triggerD, OUTPUT);
  pinMode(echoD, INPUT);
  pinMode(triggerE, OUTPUT);
  pinMode(echoE, INPUT);
  pinMode(triggerF, OUTPUT);
  pinMode(echoF, INPUT);
  /*---------------------------Konfigurasi Pin Sensor Jalur 3------------------------------------*/
  pinMode(triggerG, OUTPUT);
  pinMode(echoG, INPUT);
  pinMode(triggerH, OUTPUT);
  pinMode(echoH, INPUT);
  pinMode(triggerI, OUTPUT);
  pinMode(echoI, INPUT);
  /*---------------------------Konfigurasi Pin Lampu Jalur 1-------------------------------------*/
  pinMode(merah1, OUTPUT);
  pinMode(kuning1, OUTPUT);
  pinMode(hijau1, OUTPUT);
  /*---------------------------Konfigurasi Pin Lampu Jalur 2-------------------------------------*/
  pinMode(merah2, OUTPUT);
  pinMode(kuning2, OUTPUT);
  pinMode(hijau2, OUTPUT);
  /*---------------------------Konfigurasi Pin Lampu Jalur 3-------------------------------------*/
  pinMode(merah3, OUTPUT);
  pinMode(kuning3, OUTPUT);
  pinMode(hijau3, OUTPUT);
  /*---------------------------Konfigurasi Sensor Suara------------------------------------------*/
  pinMode(suara1, INPUT);
  pinMode(suara2, INPUT);
  pinMode(suara3, INPUT);
  /*---------------------------Tampilan Awal-----------------------------------------------------*/
  lcd.setCursor(2, 0);
  lcd.print("SKRIPSI  ELEKTRO");
  lcd.setCursor(2, 1);
  lcd.print("Azzubair Ibrahim");
  lcd.setCursor(5, 2);
  lcd.print("1710501078");
  lcd.setCursor(1, 3);
  lcd.print("Universitas  Tidar");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SMART TRAFFIC LIGHT");
  lcd.setCursor(6, 1);
  lcd.print("BASED ON");
  lcd.setCursor(4, 2);
  lcd.print("FUZZY SYSTEM");
  lcd.setCursor(4, 3);
  lcd.print("VERSION  1.5");

  digitalWrite(merah1, HIGH);
  digitalWrite(kuning1, LOW);
  digitalWrite(hijau1, LOW);

  digitalWrite(merah2, HIGH);
  digitalWrite(kuning2, LOW);
  digitalWrite(hijau2, LOW);

  digitalWrite(merah3, HIGH);
  digitalWrite(kuning3, LOW);
  digitalWrite(hijau3, LOW);
  delay(3000);

  jalur1();
  lampuJalur1();
  detik = nyalaMerah;
  detikShow = nyalaHijau + 1 ;
  lcd.clear();
}

void loop() {
  unsigned long waktuSekarang = millis();
  if (waktuSekarang - waktuSebelumnya > 1000 ) {
    waktuSebelumnya = waktuSekarang;
    detik++;// detik +1
    detikShow--;
    if (detikShow <= 0) {
      detikShow = 0;
    }
  }

  lampuJalur1();
  lampuJalur2();
  lampuJalur3();

  lcd.setCursor(4, 0);
  lcd.print("KONDISI JALUR");
  lcd.setCursor(1, 1);
  lcd.print("J1");
  lcd.setCursor(1, 2);
  lcd.print("J2");
  lcd.setCursor(1, 3);
  lcd.print("J3");
  data = kondisi + " " + lampu;
  lcd.setCursor(0, cursorY);
  lcd.print(">");
  lcd.setCursor(4, cursorY);
  lcd.print(data);
  lcd.setCursor(18, 0);
  lcd.print(konvertke2digit(detikShow));
}

void sensorA() {
  digitalWrite(triggerA, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerA, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerA, LOW);

  duration = pulseIn(echoA, HIGH);

  A = duration / 29.41 / 2 ;
  duration = 0;
}

void sensorB() {
  digitalWrite(triggerB, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerB, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerB, LOW);

  duration = pulseIn(echoB, HIGH);

  B = duration / 29.41 / 2 ;
  duration = 0;
}

void sensorC() {
  digitalWrite(triggerC, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerC, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerC, LOW);

  duration = pulseIn(echoC, HIGH);

  C = duration / 29.41 / 2 ;
  duration = 0;
}

void sensorD() {
  digitalWrite(triggerD, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerD, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerD, LOW);

  duration = pulseIn(echoD, HIGH);

  D = duration / 29.41 / 2 ;
  duration = 0;
}

void sensorE() {
  digitalWrite(triggerE, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerE, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerE, LOW);

  duration = pulseIn(echoE, HIGH);

  E = duration / 29.41 / 2 ;
  duration = 0;
}

void sensorF() {
  digitalWrite(triggerF, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerF, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerF, LOW);

  duration = pulseIn(echoF, HIGH);

  F = duration / 29.41 / 2 ;
  duration = 0;
}

void sensorG() {
  digitalWrite(triggerG, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerG, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerG, LOW);

  duration = pulseIn(echoG, HIGH);

  G = duration / 29.41 / 2 ;
  duration = 0;
}

void sensorH() {
  digitalWrite(triggerH, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerH, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerH, LOW);

  duration = pulseIn(echoH, HIGH);

  H = duration / 29.41 / 2 ;
  duration = 0;
}

void sensorI() {
  digitalWrite(triggerI, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerI, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerI, LOW);

  duration = pulseIn(echoI, HIGH);

  I = duration / 29.41 / 2 ;
  duration = 0;
}

void suaraA() {
  sA = digitalRead(suara1);
}

void suaraB() {
  sB = digitalRead(suara2);
}

void suaraC() {
  sC = digitalRead(suara3);
}

void jalur1() {
  sensorA();
  if (A <= batas) {
    A = 1;
  } else {
    A = 0;
  }

  sensorB();
  if (B <= batas) {
    B = 1;
  } else {
    B = 0;
  }

  sensorC();
  if (C <= batas) {
    C = 1;
  } else {
    C = 0;
  }

  x = (x << 1) + A;
  x = (x << 1) + B;
  x = (x << 1) + C;

  suaraA();

  if (x == 0b000 and sA == 0) {
    statusJalur = "KOSONG";
    statusSuara = "SENYAP";
    nyalaHijau = saatKosong;//
  } else if (x == 0b000 and sA == 1) {
    statusJalur = "KOSONG";
    statusSuara = "BISING";
    nyalaHijau = saatKosong;//
  } else if (x == 0b100 and sA == 0) {
    statusJalur = "SEPI  ";
    statusSuara = "SENYAP";
    nyalaHijau = saatSepi;//10
  } else if (x == 0b100 and sA == 1) {
    statusJalur = "SEPI  ";
    statusSuara = "BISING";
    nyalaHijau = saatSedang;//
  } else if (x == 0b110 and sA == 0) {
    statusJalur = "NORMAL";
    statusSuara = "SENYAP";
    nyalaHijau = saatSepi;//10
  } else if (x == 0b110 and sA == 1) {
    statusJalur = "NORMAL";
    statusSuara = "BISING";
    nyalaHijau = saatSedang;//
  } else if (x == 0b111 and sA == 0) {
    statusJalur = "PADAT ";
    statusSuara = "SENYAP";
    nyalaHijau = saatSedang;//
  } else if (x == 0b111 and sA == 1) {
    statusJalur = "PADAT ";
    statusSuara = "BISING";
    nyalaHijau = saatPadat;//
  } else {
    statusJalur = "SEPI  ";
    statusSuara = "SENYAP";
    nyalaHijau = saatSepi;
  }

  x = 0; //
  kondisi = statusJalur + " " + statusSuara;
}

void jalur2() {
  sensorD();
  if (D <= batas) {
    D = 1;
  } else {
    D = 0;
  }

  sensorE();
  if (E <= batas) {
    E = 1;
  } else {
    E = 0;
  }

  sensorF();
  if (F <= batas) {
    F = 1;
  } else {
    F = 0;
  }

  x = (x << 1) + D;
  x = (x << 1) + E;
  x = (x << 1) + F;

  suaraB();

  if (x == 0b000 and sB == 0) {
    statusJalur = "KOSONG";
    statusSuara = "SENYAP";
    nyalaHijau = saatKosong;//
  } else if (x == 0b000 and sB == 1) {
    statusJalur = "KOSONG";
    statusSuara = "BISING";
    nyalaHijau = saatKosong;
  } else if (x == 0b100 and sB == 0) {
    statusJalur = "SEPI  ";
    statusSuara = "SENYAP";
    nyalaHijau = saatSepi;
  } else if (x == 0b100 and sB == 1) {
    statusJalur = "SEPI  ";
    statusSuara = "BISING";
    nyalaHijau = saatSedang;
  } else if (x == 0b110 and sB == 0) {
    statusJalur = "NORMAL";
    statusSuara = "SENYAP";
    nyalaHijau = saatSepi;
  } else if (x == 0b110 and sB == 1) {
    statusJalur = "NORMAL";
    statusSuara = "BISING";
    nyalaHijau = saatSedang;
  } else if (x == 0b111 and sB == 0) {
    statusJalur = "PADAT ";
    statusSuara = "SENYAP";
    nyalaHijau = saatSedang;
  } else if (x == 0b111 and sB == 1) {
    statusJalur = "PADAT ";
    statusSuara = "BISING";
    nyalaHijau = saatPadat;
  } else {
    statusJalur = "SEPI  ";
    statusSuara = "SENYAP";
    nyalaHijau = saatSepi;
  }

  x = 0; //
  kondisi = statusJalur + " " + statusSuara;
}

void jalur3() {
  sensorG();
  if (G <= batas) {
    G = 1;
  } else {
    G = 0;
  }

  sensorH();
  if (H <= batas) {
    H = 1;
  } else {
    H = 0;
  }

  sensorI();
  if (I <= batas) {
    I = 1;
  } else {
    I = 0;
  }

  x = (x << 1) + G;
  x = (x << 1) + H;
  x = (x << 1) + I;

  suaraC();

  if (x == 0b000 and sC == 0) {
    statusJalur = "KOSONG";
    statusSuara = "SENYAP";
    nyalaHijau = saatKosong;
  } else if (x == 0b000 and sC == 1) {
    statusJalur = "KOSONG";
    statusSuara = "BISING";
    nyalaHijau = saatKosong;
  } else if (x == 0b100 and sC == 0) {
    statusJalur = "SEPI  ";
    statusSuara = "SENYAP";
    nyalaHijau = saatSepi;
  } else if (x == 0b100 and sC == 1) {
    statusJalur = "SEPI  ";
    statusSuara = "BISING";
    nyalaHijau = saatSedang;
  } else if (x == 0b110 and sC == 0) {
    statusJalur = "NORMAL";
    statusSuara = "SENYAP";
    nyalaHijau = saatSepi;
  } else if (x == 0b110 and sC == 1) {
    statusJalur = "NORMAL";
    statusSuara = "BISING";
    nyalaHijau = saatSedang;
  } else if (x == 0b111 and sC == 0) {
    statusJalur = "PADAT ";
    statusSuara = "SENYAP";
    nyalaHijau = saatSedang;
  } else if (x == 0b111 and sC == 1) {
    statusJalur = "PADAT ";
    statusSuara = "BISING";
    nyalaHijau = saatPadat;
  } else {
    statusJalur = "SEPI  ";
    statusSuara = "SENYAP";
    nyalaHijau = saatSepi;
  }

  x = 0;
  kondisi = statusJalur + " " + statusSuara;
}

void lampuJalur1()
{
  if (nyalaJalur1) {
    if (detik < nyalaMerah) {
      lampu = "M";
      digitalWrite(merah1, HIGH);
    } else if ((detik > nyalaMerah) and (detik < nyalaMerah + nyalaKuning)) {
      lampu = "K";
      digitalWrite(merah1, LOW);
      digitalWrite(kuning1, HIGH);
    } else if ((detik >= nyalaMerah + nyalaKuning) and (detik <= nyalaMerah + nyalaKuning + nyalaHijau)) {
      lampu = "H";
      digitalWrite(hijau1, HIGH);
      digitalWrite(kuning1, LOW);
    } else if ((detik > nyalaMerah + nyalaKuning + nyalaHijau) and (detik < nyalaMerah + (2 * nyalaKuning) + nyalaHijau)) {
      lampu = "K";
      digitalWrite(hijau1, LOW);
      digitalWrite(kuning1, HIGH);
    } else if (detik == nyalaMerah + nyalaHijau + (2 * nyalaKuning)) {
      lcd.setCursor(4, 1);
      lcd.print("              M");
      digitalWrite(kuning1, LOW);
      digitalWrite(merah1, HIGH);
      jalur2();
      detikShow = nyalaHijau + 1;
      delay(allRed * 1000);
      nyalaJalur1 = false;
      nyalaJalur2 = true;
      lcd.clear();
      cursorY = 2;
      detik = nyalaMerah;
    }
  }
}

void lampuJalur2()
{
  if (nyalaJalur2) {
    if (detik < nyalaMerah) {
      lampu = "M";
      digitalWrite(merah2, HIGH);
    } else if ((detik > nyalaMerah) and (detik < nyalaMerah + nyalaKuning)) {
      lampu = "K";
      digitalWrite(merah2, LOW);
      digitalWrite(kuning2, HIGH);
    } else if ((detik >= nyalaMerah + nyalaKuning) and (detik <= nyalaMerah + nyalaKuning + nyalaHijau)) {
      lampu = "H";
      digitalWrite(hijau2, HIGH);
      digitalWrite(kuning2, LOW);
    } else if ((detik > nyalaMerah + nyalaKuning + nyalaHijau) and (detik < nyalaMerah + (2 * nyalaKuning) + nyalaHijau)) {
      lampu = "K";
      digitalWrite(hijau2, LOW);
      digitalWrite(kuning2, HIGH);
    } else if (detik == nyalaMerah + nyalaHijau + (2 * nyalaKuning)) {
      lcd.setCursor(4, 2);
      lcd.print("              M");
      digitalWrite(kuning2, LOW);
      digitalWrite(merah2, HIGH);
      jalur3();
      detikShow = nyalaHijau + 1;
      delay(allRed * 1000);
      lcd.clear();
      cursorY = 3;
      nyalaJalur3 = true;
      nyalaJalur2 = false;
      detik = nyalaMerah;
    }
  }
}

void lampuJalur3()
{
  if (nyalaJalur3) {
    if (detik < nyalaMerah) {
      lampu = "M";
      digitalWrite(merah3, HIGH);
    } else if ((detik > nyalaMerah) and (detik < nyalaMerah + nyalaKuning)) {
      lampu = "K";
      digitalWrite(merah3, LOW);
      digitalWrite(kuning3, HIGH);
    } else if ((detik >= nyalaMerah + nyalaKuning) and (detik <= nyalaMerah + nyalaKuning + nyalaHijau)) {
      lampu = "H";
      digitalWrite(hijau3, HIGH);
      digitalWrite(kuning3, LOW);
    } else if ((detik > nyalaMerah + nyalaKuning + nyalaHijau) and (detik < nyalaMerah + (2 * nyalaKuning) + nyalaHijau)) {
      lampu = "K";
      digitalWrite(hijau3, LOW);
      digitalWrite(kuning3, HIGH);
    } else if (detik == nyalaMerah + nyalaHijau + (2 * nyalaKuning)) {
      lcd.setCursor(4, 3);
      lcd.print("              M");
      digitalWrite(kuning3, LOW);
      digitalWrite(merah3, HIGH);
      jalur1();
      detikShow = nyalaHijau + 1;
      delay(allRed * 1000);
      lcd.clear();
      cursorY = 1;
      nyalaJalur3 = false;
      nyalaJalur1 = true;
      detik = nyalaMerah;
    }
  }
}

String konvertke2digit(int angkanya) {
  if (angkanya < 10) {
    return "0" + String(angkanya);
  }
  else {
    return String(angkanya);
  }
}
