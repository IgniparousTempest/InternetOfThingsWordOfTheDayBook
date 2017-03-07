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
   LCD pin 11 (D5) -> Arduino digital pin 4
   LCD pin 11 (D6) -> Arduino digital pin 3
   LCD pin 11 (D7) -> Arduino digital pin 2

  For backlight:
   LCD pin 16 (K) -> Arduino Ground
   LCD pin 15 (A) -> 220 ohm resistor -> Arduino 5V

*/

#include <LiquidCrystal.h> // LCD library
#include <stdbool.h> // bool types
#include <string.h>  // strlen

#define LCD_WIDTH 20
#define LCD_HEIGHT 4
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

typedef struct {
  const char* entry;
  const char* definition;
  char** definition_lines;
  int num_lines_definition;
} Word;

Word get_Word_of_the_day() {
  Serial.println("Getting word of the day");
  int r = random(10);
  const char* entry;
  const char* definition;

  switch (r) {
    case 0:
      entry = "affable";
      definition = "Being pleasant and at ease in talking to others.";
      break;
    case 1:
      entry = "nugatory";
      definition = "Of little or no consequence : trifling, inconsequential.";
      break;
    case 2:
      entry = "cabotage";
      definition = "Trade or transport in coastal waters or airspace or between two points within a country.";
      break;
    case 3:
      entry = "laconic";
      definition = "Using or involving the use of a minimum of words.";
      break;
    case 4:
      entry = "genuflect";
      definition = "To bend the knee.";
      break;
    case 5:
      entry = "shunpike";
      definition = "A side road used to avoid the toll on or the speed and traffic of a superhighway.";
      break;
    case 6:
      entry = "indigenous";
      definition = "Having originated in and being produced, growing, living, or occurring naturally in a particular region or environment.";
      break;
    case 7:
      entry = "hoary";
      definition = "Gray or white with or as if with age.";
      break;
    case 8:
      entry = "kudos";
      definition = "Fame and renown resulting from an act or achievement : prestige.";
      break;
    case 9:
      entry = "finesse";
      definition = "Impressive delicacy and skill.";
      break;
  }

  Serial.print("Word of the day: ");
  Serial.println(entry);

  int num_lines_definition = 0;
  char** lines = prepare_definition_for_display(definition, &num_lines_definition);

  return (Word) {
    entry,
    definition,
    lines,
    num_lines_definition
  };
}

/**
 * Inserts '-', '\0' at a specific point in a new copy of of the specified string and returns the copy.
 * 
 * @param src     String to be inserted into.
 * @param len_src Length of string to be inserted into.
 * @param index   Position to insert characters.
 * 
 * @return The new string with the inserted hyphen and null character.
 */
char* insert_hyphen_null(char* src, int len_src, int index) {
  char* new_string = malloc(len_src + 2 + 1);
  memcpy(new_string, src, index);
  memcpy(new_string + index + 1, src + index - 1, len_src - index + 1);
  new_string[index - 1] = '-';
  new_string[index] = '\0';
  return new_string;
}

char** prepare_definition_for_display(const char* definition, int* num_lines) {
  char* formatted_string = malloc(strlen(definition) + 1);
  char* temp_str;
  strcpy(formatted_string, definition);

  int len = strlen(formatted_string);
  *num_lines = 0;
  int i = 0;
  int start = -1;

  while (true) {
    *num_lines += 1;
    i = start + LCD_WIDTH;
    if (i > len)
      break;

    while (true) {
      // i has reached the begining of its block
      if (i == start) {
        i = start + LCD_WIDTH;

        temp_str = formatted_string;
        formatted_string = insert_hyphen_null(formatted_string, len, i);
        free(temp_str);
        start = i + 1;
        len += 2;

        break;
      }
      else if (isspace(formatted_string[i])) {
        formatted_string[i] = '\0';
        start = i + 1;
        break;
      }
      i--;
    }
  }

  // Convert chain of strings to an array of strings
  char** definition_lines = malloc(*num_lines * sizeof(char*));
  int new_line_index;
  char* lines = formatted_string;
  for (i = 0; i < *num_lines; i++) {
    new_line_index = strchr(lines, '\0') - lines;
    definition_lines[i] = right_pad_string(lines, new_line_index + 1, LCD_WIDTH + 1, ' ');
    lines += new_line_index + 1;
  }

  free(formatted_string);
  return definition_lines;
}

/**
 * Creates a new string from an existing string, right padded with the specified character.
 * 
 * If the length of the new string is not greater than the source string, then a pointer to the source string is returned.
 * 
 * @param src     String to be padded.
 * @param len_src Length of string to be padded.
 * @param len_new Length of string to be returned.
 * @param pad     Character to pad with.
 * 
 * @return The right padded string.
 */
char* right_pad_string(char* src, int len_src, int len_new, char pad) {
  if (len_src >= len_new)
    return src;
    
  char* padded_str = malloc(len_new * sizeof(char));
  strncpy(padded_str, src, len_src);
  memset(padded_str + len_src - 1, pad, len_new - len_src);
  padded_str[len_new - 1] = '\0';
  return padded_str;
}

void print_Word(Word wrd, int index) {
  // Print the entry
  char str[LCD_WIDTH + 1];
  strncpy(str, wrd.entry, LCD_WIDTH + 1); //TODO: Words should be hyphenated and not truncated
  str[LCD_WIDTH] = '\0';
  lcd.setCursor(0, 0);
  lcd.print(str);

  // Print the definition
  int i;
  for (i = 1; i < MIN(wrd.num_lines_definition, LCD_HEIGHT - 1) + 1; i++) {
    Serial.println(i);
    index = index % wrd.num_lines_definition;
    lcd.setCursor(0, i);
    lcd.print(wrd.definition_lines[index]);
    index++;
  }
}

Word Word_of_the_day;
int index = 0;
int index_old = index - 1;

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));

  Word_of_the_day = get_Word_of_the_day();
  Serial.print("size: ");
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

