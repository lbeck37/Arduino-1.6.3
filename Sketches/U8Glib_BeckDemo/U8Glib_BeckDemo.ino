/* HelloWorld.pde  */

#include "U8glib.h"

// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The following list is incomplete. The complete list of supported
// devices with all constructor calls is here: http://code.google.com/p/u8glib/wiki/device
U8GLIB_DOGS102 u8g(13, 11, 10, 9, 8);     // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here
  //u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  //u8g.setFont(u8g_font_fub20n);
  //u8g.setFont(u8g_font_fub49n);
  //u8g.setFont(u8g_font_fur49n);

  u8g.setFont(u8g_font_fub35n);
  u8g.drawStr( 2, 40, "9");

  //u8g.setFont(u8g_font_fub42n);
  //u8g.drawStr( 32, 48, "9");

  //u8g.setFont(u8g_font_osb35n);
  u8g.setFont(u8g_font_fub11r);
  u8g.drawStr( 2, 63, "17.45 fub11r");

  u8g.setFont(u8g_font_micro);
  u8g.drawStr( 32, 8, "micro ABCabc12345");

  u8g.setFont(u8g_font_5x7);
  u8g.drawStr( 32, 18, "5x7 ABCabc1234");

  u8g.setFont(u8g_font_5x8);
  u8g.drawStr( 32, 28, "5x8 ABCabc1234");

  u8g.setFont(u8g_font_6x10);
  u8g.drawStr( 32, 38, "6x10 ABCabc");

  u8g.setFont(u8g_font_6x12);
  u8g.drawStr( 32, 48, "6x12 ABCabc");

}

void setup(void) {
  // flip screen, if required
  // u8g.setRot180();

  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}

void loop(void) {
  // picture loop
  u8g.firstPage();
  do {
    draw();
  } while( u8g.nextPage() );

  // rebuild the picture after some delay
  delay(50);
}

