#include <avr/pgmspace.h>

// Entries stored in flash memory
const char entry_0[] PROGMEM = "affable";
const char entry_1[] PROGMEM = "nugatory";
const char entry_2[] PROGMEM = "cabotage";
const char entry_3[] PROGMEM = "laconic";
const char entry_4[] PROGMEM = "genuflect";
const char entry_5[] PROGMEM = "shunpike";
const char entry_6[] PROGMEM = "indigenous";
const char entry_7[] PROGMEM = "hoary";
const char entry_8[] PROGMEM = "kudos";
const char entry_9[] PROGMEM = "finesse";

// Definitions stored in flash memory
const char definition_0[] PROGMEM = "Being pleasant and at ease in talking to others.";
const char definition_1[] PROGMEM = "Of little or no consequence : trifling, inconsequential.";
const char definition_2[] PROGMEM = "Trade or transport in coastal waters or airspace or between two points within a country.";
const char definition_3[] PROGMEM = "Using or involving the use of a minimum of words.";
const char definition_4[] PROGMEM = "To bend the knee.";
const char definition_5[] PROGMEM = "A side road used to avoid the toll on or the speed and traffic of a superhighway.";
const char definition_6[] PROGMEM = "Having originated in and being produced, growing, living, or occurring naturally in a particular region or environment.";
const char definition_7[] PROGMEM = "Gray or white with or as if with age.";
const char definition_8[] PROGMEM = "Fame and renown resulting from an act or achievement : prestige.";
const char definition_9[] PROGMEM = "Impressive delicacy and skill.";

// Pointers to flash memory
const char* const string_table[] PROGMEM = {entry_0, entry_1, entry_2, entry_3, entry_4, entry_5, entry_6, entry_7, entry_8, entry_9, definition_0, definition_1, definition_2, definition_3, definition_4, definition_5, definition_6, definition_7, definition_8, definition_9};


Word get_word_of_the_day() {
  Serial.println("Getting word of the day");
  const char* entry;
  const char* definition;

  get_word_from_flash(&entry, &definition);

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
 * Gets word from flash memory. This function is for testing purposes and does not require WiFi.
 * 
 * @param entry      String to store entry.
 * @param definition String to store definition.
 */
static void get_word_from_flash(char** entry, char** definition) {
  int r = random(10);

  // strlen_P requires a compile time constant, so strlen_P(string_table[r]); would not work.
  int len;
  len = strlen_P((char*)pgm_read_word(&(string_table[r])));
  *entry = malloc(len + 1);
  
  len = strlen_P((char*)pgm_read_word(&(string_table[r + 10])));
  *definition = malloc(len + 1);

  strcpy_P(*entry, (char*)pgm_read_word(&(string_table[r])));
  strcpy_P(*definition, (char*)pgm_read_word(&(string_table[r + 10])));
}

