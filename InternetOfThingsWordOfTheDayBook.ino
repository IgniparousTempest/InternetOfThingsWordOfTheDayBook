/*
  A WiFi connected Word of the day display.

  Wiring:
   LCD pin 1 (VSS) -> Arduino Ground
   LCD pin 2 (VDD) -> Arduino 5V
   LCD pin 3 (VO)  -> Arduino Ground
   LCD pin 4 (RS)  -> Arduino digital pin 12
   LCD pin 5 (RW)  -> Arduino Ground
   LCD pin 6 (E)   -> Arduino digital pin 11
   LCD pin 11 (D4) -> Arduino digital pin 5
   LCD pin 12 (D5) -> Arduino digital pin 4
   LCD pin 13 (D6) -> Arduino digital pin 3
   LCD pin 14 (D7) -> Arduino digital pin 2

  For backlight:
   LCD pin 16 (K) -> Arduino Ground
   LCD pin 15 (A) -> 220 ohm resistor -> Arduino 5V

*/

#include <LiquidCrystal.h> // LCD library
#include <stdbool.h> // bool types
#include <string.h>  // strlen

#define LCD_WIDTH 20
#define LCD_HEIGHT 4
#define MAX(x, y) (((x) > (y)) ? (x) : (y)) // TODO: These are bad practice
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

typedef struct {
  const char* entry;
  const char* definition;
  char** definition_lines;
  int num_lines_definition;
} Word;

void print_Word(Word wrd, int time_in_seconds) {
  // Print the entry
  char str[LCD_WIDTH + 1];
  strncpy(str, wrd.entry, LCD_WIDTH + 1); //TODO: Words should be hyphenated and not truncated
  str[LCD_WIDTH] = '\0';
  lcd.setCursor(0, 0);
  lcd.print(str);

  // Print the definition
  print_definition_scrolling(wrd, time_in_seconds);
}

/**
 * Prints the definition begining on the second line, and scrols to the bottom.
 * 
 * @param wrd             The word containing the definition to print.
 * @param time_in_seconds The index to print.
 */
void print_definition_scrolling(Word wrd, int time_in_seconds) {
  int i;
  for (i = 1; i < MIN(wrd.num_lines_definition, LCD_HEIGHT - 1) + 1; i++) {
    time_in_seconds = time_in_seconds % MAX(wrd.num_lines_definition - (LCD_HEIGHT - 1) + 1, 1);
    lcd.setCursor(0, i);
    lcd.print(wrd.definition_lines[i - 1 + time_in_seconds]);
  }
}

Word Word_of_the_day;
int index = 0;
int index_old = index - 1;

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));

  Word_of_the_day = get_word_of_the_day();
  Serial.print("Number of Lines: ");
  Serial.println(Word_of_the_day.num_lines_definition);

  // set up the LCD's number of columns and rows:
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
}

void loop() {
  index = millis() / 5000;
  if (index != index_old)
    print_Word(Word_of_the_day, index);
  index_old = index;
}

