// --+-+-<ABT-Messgerät>-+-+--
//
//
//     --> Funktionen <--
//
// 1. Batteriemessung
//  |_ Ausgabe in "V"
// 
// 2. Automatische BAT Erkennung
//  |  Kann zwischen 2 Systemen 
//  |_ automatisch wächseln
//   |
//   |_ 1.5 V
//   |_ 3 V
//
// 3. Batterie Schutzt
//  |  Misst die Temperatur und 
//  |_ gibt alarm falls über 40°C
//
//     --> Benutztung <--
//
// 1. Batterie Anschliesen nur 
//  |_ 1,5 V und 5 V Batterien
//   |
//   |_ + zu A0
//   |_ - zu GND
//
// 2. ABT-M Starten
//  |_ Grundeinstellung 1,5 V BAT
//  |  ABE Erkennt zuferlässig 
//  |
//  |_ falls nicht entweder:
//  | |
//  | |_ Neu starten
//  | |  Auf den BAT Knopf
//  | |_ drücken
//  |
//  |  Nun wird ihnen Folgendes 
//  |_ angezeigt:
//   |
//   |_ Oben Links: V 
//   |_ Oben Rechts: Zustand
//   |_ Unten Links: Zustand
//   |_ Unten Rechts: BAT
//
// 
//     --> Wichtig <--
//
// Sobalt die Warnung angezeigt 
// wird, dass die Batterie zu 
// Warm ist (40°C) bitte Sofort
// aus der der Messung nehmen 
// un Warten bis die Batterie 
// Abgekühlt ist.
//
//
//     --> Sonstiges <--
//
// Copyright by Tom
//
// Schulprojekt RBG 9a 
// rbg-g.de
//
// Verwendet: Arduino UNO, L2C, 
//            3 LED's, Taster,
//            Piezo.
//
// ChatGPT und co. wurden 
// ausschlieslich zur erklärung 
// und debugging benutzt.
// Keine Zeile Code und Text 
// wurden mit einer KI Gemacht
//
// Zudem wurde das Schülerbuch 
// und die Offiezelle Arduino 
// benutzt.
//
// Viel Spaß beim Ausspribieren
// (TMP noch nicht Fertig)


#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int analogIn = A1;
int rawValue= 0;
double voltage = 0;
double tempC = 0;
int batterie=A0;
int messwert;
int rot = 5;
int gelb = 3;
int greun = 4;
int bs = 1; // 1 = 1.5 V und 3 = 3 V
int fehler = 0;
float spannung;
float geladen = 1.5;
float leer = 1.4;


void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("<ABT-Messgeraet>");
  lcd.setCursor(0, 1);
  lcd.print("");
  delay (1000);
  Serial.begin(9600);
  
  pinMode(rot,OUTPUT);
  pinMode(gelb,OUTPUT);
  pinMode(greun,OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(2), batterieart, RISING);
  lcd.setCursor(0, 0);
  lcd.print("Copyright by Tom");
  lcd.setCursor(0, 1);
  lcd.print("");
  delay (1000);
  lcd.setCursor(0, 0);
  lcd.print("Schulprojekt RBG");
  lcd.setCursor(0, 1);
  lcd.print("rbg-g.de      9a");
  delay(1000);
}

void loop() {
  lcd.clear();
  delay(200);
  
  rawValue = analogRead(analogIn);
  voltage = (rawValue / 1023.0) * 5000;
  tempC = (voltage-500) * 0.1;
  
  lcd.setCursor(0, 0);
  lcd.print("#####"); //1
  
  
  messwert=analogRead(batterie); //Berechnung V
  spannung= messwert * 5.0 / 1023;
  lcd.setCursor(0, 0);
  lcd.print("##########");//2
  
  Serial.println(spannung);
  
  if (bs == 1){ //Bat Erkennung 1
   if (spannung >= 2.5){ 
     Serial.println();
     Serial.println("-> ! Falsches Batterie-System    ! <-");
     Serial.println("-> ! Automatische waechslung zum ! <-");
     Serial.println("-> ! Batterie-System mit der     ! <-");
     Serial.println("-> ! Optiemalspannung von 3 V.   ! <-");
     Serial.println();
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("! Falsches Batt !");
     lcd.setCursor(0, 1);
     lcd.print("Wechsel zu 3.0V");
     bs = 2;
     geladen = 3;
     leer = 2.8;
     fehler = fehler + 1;
    
     for(int i = 0; i < 4; i++){
        tone(6,280);
        digitalWrite(rot, HIGH);
        digitalWrite(gelb, HIGH);
        digitalWrite(greun, HIGH);
        delay(250);
        digitalWrite(rot, LOW);
        digitalWrite(gelb, LOW);
        digitalWrite(greun, LOW);
        noTone(6);
        delay(250);
      }
     }
    }
  
  if (bs == 2){ //Bat Erkennung 2
   if (spannung <= 1.6){
     Serial.println();
     Serial.println("-> ! Falsches Batterie-System    ! <-");
     Serial.println("-> ! Automatische waechslung zum ! <-");
     Serial.println("-> ! Batterie-System mit der     ! <-");
     Serial.println("-> ! Optiemalspannung von 1.5 V. ! <-");
     Serial.println();
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("! Falsches Batt !");
     lcd.setCursor(0, 1);
     lcd.print("Wechsel zu 1.5V");
     bs = 1;
     geladen = 1.5;
     leer = 1.4;
     fehler = fehler + 1;
    
     for(int i = 0; i < 4; i++){
        tone(6,280);
        digitalWrite(rot, HIGH);
        digitalWrite(gelb, HIGH);
        digitalWrite(greun, HIGH);
        delay(250);
        digitalWrite(rot, LOW);
        digitalWrite(gelb, LOW);
        digitalWrite(greun, LOW);
        noTone(6);
        delay(250);
      }
     }
    }
  lcd.setCursor(0, 0);
  lcd.print("################");
  
  if (fehler == 2){ //Fehler erkennung 3
    Serial.println();
    Serial.println("=====================================================");
    Serial.println("!! Achtung: Ungeweuhnliche Batteriespannung.       !!");
    Serial.println("!! Wert liegt nicht im erwarteten Bereich.         !!");
    Serial.println("!! Meuglicherweise falsches Batteriesystem erkannt.!!");
    Serial.println("=====================================================");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("!! Bat Sy !!");
    lcd.setCursor(0, 1);
    lcd.print("!! Fehler !!");

      while (true) {
        digitalWrite(rot, HIGH);
        delay(500);
        digitalWrite(rot, LOW);
        delay(500);
     }
    }
  lcd.setCursor(0, 0);
  lcd.print("################");
  lcd.setCursor(0, 0);
  lcd.print("######");
  
  if (tempC >= 40){
    Serial.println();
    Serial.println("-> !  WARNUNG: Batterie zu heiß! ! <-");
    Serial.println("-> !  Temperatur euber 40 Grad.  ! <-");
    Serial.println("-> !  Gefahr von Euberhitzung!   ! <-");
    Serial.println("-> !  Bitte sofort trennen und   ! <-");
    Serial.println("-> !  Batterie abkeuhlen lassen. ! <-");
    Serial.println("-> !  Danach erneut messen.      ! <-");

     Serial.println();
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("! Alarm !");
     lcd.setCursor(0, 1);
     lcd.print("Batterie 40 C");
     bs = 1;
     geladen = 1.5;
     leer = 1.4;
     fehler = fehler + 1;
     
    }
  lcd.setCursor(0, 0);
  lcd.print("################");
  lcd.setCursor(0, 1);
  lcd.print("############100%");
   
  
  fehler = 0;
  
  Serial.println("===============================");
  Serial.print("Spannung: ");
  Serial.print(spannung,1);
  Serial.println(" V");
  
  
  if (spannung >= geladen) {//Geladen
      Serial.println("Zustand : VOLL");
      Serial.println("Anzeige : [############]");
      digitalWrite(greun, HIGH);
    
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(spannung);
      lcd.setCursor(5, 0);
      lcd.print("V");
      lcd.setCursor(0, 1);
      lcd.print("VOLL");
      lcd.setCursor(8, 0);
      lcd.print(tempC);
    }
    else if (spannung < geladen && spannung > leer) {//halb geladen
      Serial.print("Zustand : HALBLEER");
      Serial.println("Anzeige : [######------]");
      digitalWrite(gelb, HIGH);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(spannung);
      lcd.setCursor(5, 0);
      lcd.print("V");
      lcd.setCursor(0, 1);
      lcd.print("HALBLEER");
      lcd.setCursor(8, 0);
      lcd.print(tempC);
    }
    else if (spannung <= leer){//leer
      Serial.println("Zustand : LEER");
      Serial.println("Anzeige : [------------]");
      digitalWrite(rot, HIGH);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(spannung);
      lcd.setCursor(5, 0);
      lcd.print("V");
      lcd.setCursor(0, 1);
      lcd.print("LEER");
      lcd.setCursor(8, 0);
      lcd.print(tempC);
    }
  
  Serial.print("Batterie-System: ");
  
  if (bs == 1){ // Zweite Batteriesystem
      Serial.println("1.5 V");
      lcd.setCursor(8, 1);
      lcd.print("Bat:1.5V");
  }
  else if (bs == 2){ // Erste Batteriesystem
      Serial.println("3 V");
      lcd.setCursor(8, 1);
      lcd.print("Bat: 3 V");
  }
  
  Serial.println("===============================");
  noTone(6);
  
  delay(10000);
  lcd.clear();
  delay(200);
  digitalWrite(rot, LOW);
  digitalWrite(gelb, LOW);
  digitalWrite(greun, LOW);
  
}


void batterieart() {
  if (bs == 1){ // Zweite Batteriesystem 3v
    geladen = 3;
    leer = 2.8;
    Serial.println("zu 2");
    bs = 2;
    }
  else if (bs == 2){ // Erste Batteriesystem 1.5v
    geladen = 1.5;
    leer = 1.4;
    bs = 1;
    Serial.println("zu 1");
    }
  digitalWrite(rot, HIGH);
  digitalWrite(gelb, HIGH);
  digitalWrite(greun, HIGH);
 }