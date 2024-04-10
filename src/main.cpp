

#include <Arduino.h>
#include <M5Atom.h>
#include <Adafruit_NeoPixel.h>

// ejemplo "strandtest" de la libreria Adafruit NeoPixel  

#define PIN       27  
#define NUMPIXELS 25 

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(
    NUMPIXELS, PIN,
    NEO_GRB + NEO_KHZ800);  // establecer el número de LED, número de pin, tipo de LED.
                        
uint32_t Wheel(byte WheelPos);

void rainbow(int wait);
void theaterChase(uint32_t color, int wait);
void theaterChaseRainbow(int wait);
void colorWipe(uint32_t color, int wait);

void setup() {
    M5.begin();      // inicializa M5Atom.
    pixels.begin();  // inicializa la libreria NeoPixel.
    pixels.setBrightness(50); // Establezca BRILLO en aproximadamente 1/5 (máx. = 255)  
}

void loop() {
    
  // Rellene a lo largo de los píxeles en varios colores....
  colorWipe(pixels.Color(255,   0,   0), 50); // Red
  colorWipe(pixels.Color(  0, 255,   0), 50); // Green
  colorWipe(pixels.Color(  0,   0, 255), 50); // Blue

  // Haz un efecto de marquesina de teatro en varios colores....
  theaterChase(pixels.Color(127, 127, 127), 50); // White, half brightness
  theaterChase(pixels.Color(127,   0,   0), 50); // Red, half brightness
  theaterChase(pixels.Color(  0,   0, 127), 50); // Blue, half brightness

  rainbow(10);             // Ciclo de arcoíris que fluye a lo largo de todos los píxeles.
  theaterChaseRainbow(50); // Teatro mejorado con arcoíris Variante Chase

}

// Algunas funciones propias para crear efectos animados -----------------

// Rellena píxeles uno tras otro con un color. los píxeles NO se borran
// primero; todo lo que haya allí quedará cubierto píxel a píxel. pasar en color
// (como un único valor 'empaquetado' de 32 bits, que puedes obtener llamando
// píxeles.Color(rojo, verde, azul) como se muestra en la función loop() anterior),
// y un tiempo de retraso (en milisegundos) entre píxeles.

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<pixels.numPixels(); i++) {     // Por cada pixel en pixels...
    pixels.setPixelColor(i, color);             // Establecer el color del píxel (en RAM)
    pixels.show();                              // Actualizar píxeles para que coincidan
    delay(wait);                                // Pausa por un momento 
  }
}

// Luces persiguiendo luces estilo marquesina de teatro. Pasar un color (valor de 32 bits,
// a la pixels.Color(r,g,b) como se mencionó anteriormente) y un tiempo de retardo (en ms)
// entre fotogramas.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {     // Repetir 10 veces...
    for(int b=0; b<3; b++) {    // 'b' cuenta de 0 a 2...
      pixels.clear();           // Establece todos los píxeles de la RAM en 0 (desactivado)
      // 'c' cuenta desde 'b' hasta el final de los píxeles en pasos de 3...
      for(int c=b; c<pixels.numPixels(); c += 3) {
        pixels.setPixelColor(c, color);     // Establece el píxel 'c' en el valor 'color'
      }
      pixels.show();    // Actualizar píxeles con nuevos contenidos
      delay(wait);      // Pausa por un momento 
    }
  }
}

// Ciclo del arco iris a lo largo de píxeles completos. Pasa el tiempo de retardo (en ms) entre fotogramas.
void rainbow(int wait) {
    // El tono del primer píxel recorre 5 bucles completos a través de la rueda de colores.
    // La rueda de colores tiene un rango de 65536 pero está bien si damos la vuelta, así que
    // solo cuenta de 0 a 5*65536. Agregando 256 a firstPixelHue cada vez
    // significa que haremos 5*65536/256 = 1280 pases a través de este bucle:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // pixels.rainbow() puede tomar un solo argumento (primer tono de píxel) o
    // opcionalmente algunos extras: número de repeticiones del arco iris (predeterminado 1),
    // saturación y valor (brillo) (ambos 0-255, similar al
    // Función ColorHSV(), por defecto 255), y un indicador verdadero/falso para saber si
    // para aplicar la corrección gamma para proporcionar colores "más reales" (verdadero por defecto).
    pixels.rainbow(firstPixelHue);
    // La línea anterior equivale a:
    // píxeles.rainbow(firstPixelHue, 1, 255, 255, verdadero);
    pixels.show();  // Actualizar píxeles con nuevos contenidos
    delay(wait);    // Pausa por un momento
  }
}

// Carpa de teatro mejorada con el arcoíris. Pasa el tiempo de retardo (en ms) entre fotogramas.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;        // El primer píxel comienza en rojo (tono 0)
  for(int a=0; a<30; a++) {     // Repetir 30 veces...
    for(int b=0; b<3; b++) {    // 'b' cuenta de 0 a 2...
      pixels.clear();           //  Establezca todos los píxeles de la RAM en 0 (desactivado)
      // 'c' cuenta desde 'b' hasta el final de los píxeles en incrementos de 3...
      for(int c=b; c<pixels.numPixels(); c += 3) {
        // el tono del píxel 'c' se compensa en una cantidad para completarlo
        // revolución de la rueda de colores (rango 65536) a lo largo
        // de los píxeles (pasos de pixels.numPixels()):
        int      hue   = firstPixelHue + c * 65536L / pixels.numPixels();
        uint32_t color = pixels.gamma32(pixels.ColorHSV(hue));      // hue -> RGB
        pixels.setPixelColor(c, color);     // Establezca el píxel 'c' en el valor 'color'
      }
      pixels.show();                // Actualizar píxeles con nuevos contenidos
      delay(wait);                  // Pausa por un momento
      firstPixelHue += 65536 / 90;  // Un ciclo de rueda de colores en más de 90 fotogramas.
    }
  }
}

/*
void rainbow(uint8_t wait) {
    uint16_t i, j;

    for (j = 0; j < 256; j++) {
        for (i = 0; i < pixels.numPixels(); i++) {
            pixels.setPixelColor(i, Wheel((i + j) & 255));
            while (M5.Btn.read() == 1);
        }
        pixels.show();
        delay(wait);
    }
}

// Ingrese un valor de 0 a 255 para obtener un valor de color.
// Los colores son una transición r - g - b - de regreso a r..
uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
*/