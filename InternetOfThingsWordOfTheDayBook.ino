/*
  LiquidCrystal Library - Hello World

  Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
  library works with all LCD displays that are compatible with the
  Hitachi HD44780 driver. There are many of them out there, and you
  can usually tell them by the 16-pin interface.

  This sketch prints "Hello World!" to the LCD
  and shows the time.

  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

*/

#include <LiquidCrystal.h> // LCD library
#include <time.h>   // Random
#include <stdlib.h> // Random
#include <stdbool.h> // bool types
#include <string.h>  // strlen

#define 2 20
#define LCD_HEIGHT 4

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

word get_word_of_the_day() {
  int r = rand();
  switch (r % 10) {
    case 0:
      return (word) {
        "affable", "being pleasant and at ease in talking to others"
      };
    case 1:
      return (word) {
        "nugatory", "of little or no consequence : trifling, inconsequential"
      };
    case 2:
      return (word) {
        "cabotage", "trade or transport in coastal waters or airspace or between two points within a country"
      };
    case 3:
      return (word) {
        "laconic", "using or involving the use of a minimum of words"
      };
    case 4:
      return (word) {
        "genuflect", "to bend the knee"
      };
    case 5:
      return (word) {
        "shunpike", "a side road used to avoid the toll on or the speed and traffic of a superhighway"
      };
    case 6:
      return (word) {
        "indigenous", "having originated in and being produced, growing, living, or occurring naturally in a particular region or environment"
      };
    case 7:
      return (word) {
        "hoary", "gray or white with or as if with age"
      };
    case 8:
      return (word) {
        "kudos", "fame and renown resulting from an act or achievement : prestige"
      };
    case 9:
      return (word) {
        "finesse", "impressive delicacy and skill."
      };
  }
}

void print_word(word wrd) {
  lcd.setCursor(0, 0);
  char str[LCD_WIDTH + 1];
  sprintf(str, "%.*s", LCD_WIDTH, wrd.word); //TODO: Words should be hyphenated and not truncated
  str[LCD_WIDTH] = '\0';
  lcd.print(str);

  int len = strlen(wrd.definition);
  int start = 0;
  int offset;
  int available_lines;
  for (available_lines = LCD_HEIGHT - 1; available_lines > 0; available_lines--) {
    if (len - start < LCD_WIDTH) {
      strncpy(str, wrd.definition + start, len - start);
      lcd.print(str);
      break;
    }
    else {
      offset = 0;
      while (true) {
        if (offset == LCD_WIDTH) {
          strncpy(str, wrd.definition + start, LCD_WIDTH);
          str[LCD_WIDTH] = '\0';
          lcd.print(str);
          start += LCD_WIDTH;
          break;
        }
        else if (wrd.definition[start + LCD_WIDTH - offset] == ' ') {
          strncpy(str, wrd.definition + start, LCD_WIDTH - offset);
          str[LCD_WIDTH - offset] = '\0';
          lcd.print(str);
          start += LCD_WIDTH - offset + 1;
          break;
        }
        offset += 1;
      }
    }
  }
}

void setup() {
  srand(time(NULL)); // Init random function

  // set up the LCD's number of columns and rows:
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}

