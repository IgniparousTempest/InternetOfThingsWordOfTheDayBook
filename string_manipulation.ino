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
