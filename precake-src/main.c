#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "../src/basic.h"
#include "../src/mathstuff.h"
#include "../src/memorystuff.h"
#include "../src/stringstuff.h"

int main(int argc, char **argv) {
  if(argc < 2) {
    hc_puts(cake("Need at least one input file.\n"));
    return 1;
  }
  
  string_t tag = cake("cake");
  string_t filename_tag = cake(".caked");
  string_t replacement_format = cake("(string_t const){.data = \"%str\", .size = %u64}");
  
  for(u64 i = 0; i < argc - 1; ++i) {
    FILE *input = fopen(argv[i + 1], "r");
    if(!input) {
      hc_print(cake("Couldn't open %str.\n"), cake(strerror(errno)));
      return 2;
    }
    
    string_t filename = cake(argv[i + 1]);
    
    string_t source;
    fseek(input, 0, SEEK_END);
    source.size = ftell(input);
    fseek(input, 0, SEEK_SET);
    
    source.data = malloc(source.size);
    fread(source.data, source.size, 1, input);
    
    fclose(input);
    
    string_t precaked_filename = temp_concatenate(2, filename, filename_tag);
    hc_print(cake("%str -> %str\n"), filename, precaked_filename);
    
    char *precaked_filename_cstr = temp_cstr(precaked_filename);
    FILE *output = fopen(precaked_filename_cstr, "w");
    if(!output) {
      hc_print(cake("Couldn't open output file '%str'.\n"), precaked_filename);
      return 3;
    }
    
    u8 *end = source.data + source.size;
    u8 *it = source.data;
    u8 *copy_start = it;
    
    while(it < end) {
      while(it < end && *it != (u8)'"')
        ++it;
      
      if(it - tag.size < source.data) {
        ++it;
        continue;
      }
      
      if(memcmp(tag.data, it - tag.size, tag.size) == 0) {
        fwrite(copy_start, it - tag.size - copy_start, 1, output);
        
        string_t string;
        string.data = ++it;
        
        while(it < end && *it != (u8)'"')
          ++it;
        string.size = it - string.data;
        
        string_t replacement = hc_sprint(replacement_format, string, string.size);
        hc_fputs(output, replacement);
        
        copy_start = ++it;
      } else {
        ++it;
        while(it < end && *it != (u8)'"')
          ++it;
        ++it;
      }
    }
    fwrite(copy_start, end - copy_start, 1, output);
    
    free(source.data);
  }
  
  return 0;
}
