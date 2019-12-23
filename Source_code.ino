#include <Adafruit_NeoPixel.h>  // Adafruit Library
int RGB_PIN = 3;  // RBG pin Number [D3] 
int NUMPIXELS = 8;  // Number of pixles
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);

#include <DHT.h> // DHT library
int DHT_PIN = 5;  // DHT pin Number [D5]
DHT dht(DHT_PIN, DHT11);


int TRIG_PIN = 10;  // Trig pin Number [D10][Ultra Sensor]
int TOUCH_PIN = 4;  // Touch Sensor pin Number [D4]
int ECHO_PIN = 12;  // EWcho pin Number [D12][Ultra Sensor]
int CDS_PIN = A0;  // Light sensor 
int POTEN_PIN = A5;  // 가변저항 핀번호 (A5)
int r, g, b, R, G, B;  // Varibles Decleration
int Mode;
int touch_count;
int Mode_EA = 5;  // mood Number [5]
int temp_1 = 23, temp_2 = 26;  // Stander Heat Number. [Temp sensor]
int cds_1 = 530, cds_2 = 1000;  // Stander Light Number. [Light sensor]
int dis_1 = 100, dis_2 = 300, dis_3 = 500;  // Distance Standers [Ultra sensor]
boolean touch = true;
boolean cds_RGB = true;  
double cds_cur, cds_pre = cds_1;

void setup() {
  
  pinMode(ECHO_PIN, INPUT); // echo input
  pinMode(TRIG_PIN, OUTPUT);  // Trig output
  pixels.begin();   // NeoPixle start
  pixels.show();   // NeoPixle init
}


void loop() {
  Mode_set(); // Mode_set
  int poten = analogRead(POTEN_PIN);  // Resistance Value read

  if ( Mode == 0 || Mode == 1 || Mode == 2 ) {  
    for ( int i = 0; i < NUMPIXELS; i++ ) { 
      pixels.setPixelColor(i, pixels.Color( map(poten, 0, 1023, 0, r) , map(poten, 0, 1023, 0, g) , map(poten, 0, 1023, 0, b) ) );  // 네오픽셀 색 지정
      pixels.show(); // 네오픽셀 ON
    }
  }

  // touch sensor
  if ( digitalRead(TOUCH_PIN) == 1 ) {  // touch sensor = on
    touch_count++;  // touch sensor counter
    delay(10);  // 

    if ( touch_count >= 100 ) {  // touch counter more than 100
      if ( r == 0 && g == 0 && b == 0 ) {  // if its off then turn on
        r = 1;
      }

      else if ( r != 0 || g != 0 || b != 0 ) {   // if its on then turn off
        r = 0;
        g = 0;
        b = 0;
        Mode = 0;
      }

      touch_count = 0;  // touch counter 
    }

    else if ( touch && ( r != 0 || g != 0 || b != 0 ) ) {  // if its on then change the mode
      Mode++;

      if ( Mode > Mode_EA ) {
        cds_RGB = true;
        Mode = 1;
      }

      touch = false;
    } else;                                                   // mode change
  }

  else {
    touch = true;
    touch_count = 0 ;
  }

}

// mode change function
void Mode_set() {
  if ( Mode == 1) {
    r = 210;
    g = 100;
    b = 10; 
  }

  else if ( Mode == 2 ) {
    temp_set();
  }

  else if ( Mode == 3 ) {
    cds_set();
  }

  else if ( Mode == 4 ) {
    ultra_set();
  }

  else if ( Mode == 5 ) {
    temp_set();
    cds_set();
  }
}


// Temp sensor 
void temp_set() {
  int celsius = dht.readTemperature();  // read celsius heat 

  if ( celsius < temp_1 ) {  // if the heat value < temp_1[23]
    r = 0;  
    g = 0; 
    b = 255; 
  }

  else if ( temp_1 <= celsius && celsius < temp_2 ) {  // if its bigger than temp_1 then temp_2
    r = 0;
    g = 255;
    b = 0;
  }

  else if ( celsius >= temp_2 ) { // if its bigger than temp_2
    r = 255;
    g = 0;
    b = 0;
  }
}


// LIGHT Sensor 
void cds_set() {

  if ( cds_RGB ) {
    r = random(255);
    g = random(255);
    b = random(255);
    cds_RGB = false;
  }

  int cds = analogRead(CDS_PIN);
  cds_cur = (0.99 * cds_pre) + (0.01 * cds);
  cds_pre = cds_cur;
  cds = cds_cur;

  if ( cds <= cds_1 )
    cds = cds_1;

  else if ( cds_2 < cds )
    cds = cds_2;

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color( map(cds, cds_1, cds_2, 0 , r) , map(cds, cds_1, cds_2, 0 , g) , map(cds, cds_1, cds_2, 0 , b) ) );
    pixels.show();
  }
}


// Ultra sonic sensor [Distance]
void ultra_set() {
  static long dis_pre;
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = ((float)(340 * duration) / 1000) / 2 ;
  delay(50);
  
  if ( distance > 2000 )
    distance = dis_pre;

  if ( distance < dis_1 )
    distance = dis_1;

  else if ( distance > dis_3 )
    distance = dis_3;

  dis_pre = distance;

  if ( dis_1 <= distance && distance <= dis_2 ) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color( 255 - map(distance, dis_1, dis_2, 0, 255) , map(distance, dis_1, dis_2, 0, 255) , 0) );
      pixels.show();
    }
  }
  else if ( dis_2 <= distance && distance <= dis_3 ) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color( 0 , 255 - map(distance, dis_2, dis_3, 0, 255) , map(distance, dis_2, dis_3, 0, 255) ) );
      pixels.show();
    }
  }
};
  
  
