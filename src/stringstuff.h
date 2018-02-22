#ifndef STRINGSTUFF_H
#define STRINGSTUFF_H

#include "basic.h"

u8 is_whitespace(u8 x) {
  return x == ' ' || x == '\t' || x == '\n';
}
u8 is_number(u8 x) {
  return x >= (u8)'0' && x <= (u8)'9';
}
u8 is_letter(u8 x) {
  return (x >= (u8)'a' && x <= (u8)'z') || (x >= (u8)'A' && x <= (u8)'Z');
}

typedef struct {
  u64 size;
  u8 *data;
} string_t;

#define BOOLEAN_STRING(x) (x ? cake"true" : cake"false")
#define NULL_STRING cake"(null)"
#define EMPTY_STRING cake("");


// saltycake(HACK): This is for Windows. I haven't compiled the preprocessor for Windows yet.
string_t cake(char const *str) {
  string_t s;
  s.size = strlen(str);
  s.data = (u8*)str;
  
  return s;
}

string_t allocate_string(u64 size) {
  string_t s;
  s.size = size;
  s.data = malloc(size);
  
  return s;
}
void free_string(string_t *s) {
  assert(s);
  
  if(s->data)
    free(s->data);
  
  memset(s, 0, sizeof(string_t));
}

string_t temp_concatenate(u64 n, ...) {
  if(!n)
    return EMPTY_STRING;
  
  string_t s = {};
  
  va_list args;
  va_start(args, n);
  for(u64 i = 0; i < n; ++i)
    s.size += va_arg(args, string_t).size;
  va_end(args);
  
  s.data = temp_allocate(s.size);
  
  u8 *cursor = s.data;
  va_start(args, n);
  for(u64 i = 0; i < n; ++i) {
    string_t arg = va_arg(args, string_t);
    memcpy(cursor, arg.data, arg.size);
    cursor += arg.size;
  }
  va_end(args);
  
  return s;
}

static inline string_t copy_string(string_t s) {
  string_t copy;
  copy.size = s.size;
  copy.data = malloc(s.size);
  memcpy(copy.data, s.data, s.size);
  
  return copy;
}

char* temp_cstr(string_t s) {
  assert(s.data && s.size);
  
  if(s.data[s.size - 1] == 0)
    return (char*)s.data;
  
  char* cstr = temp_allocate(s.size + 1);
  memcpy(cstr, s.data, s.size);
  cstr[s.size] = 0;
  
  return cstr;
}

string_t temp_string_from_cstr(char *chars) {
  string_t s;
  s.size = strlen(chars);
  s.data = (u8*)chars;
  
  return s;
}

string_t string_from_u8(u8 *data, u64 size) {
  string_t s = {};
  
  s.data = malloc(size);
  s.size = size;
  memcpy(s.data, data, size);
  
  return s;
}

u8 are_strings_same(string_t a, string_t b) {
  if(a.size != b.size)
    return FALSE;
  
  for(u64 i = 0; i < a.size; ++i) {
    if(a.data[i] != b.data[i])
      return FALSE;
  }
  
  return TRUE;
}

void fill_u64_number_string(u64 value, u8 length, u8 **start_, u8 *end_of_buffer) {
  assert(length);
  assert(*start_ && end_of_buffer);
  assert(*start_ + length < end_of_buffer);
  
  u8 *start = *start_;
  *start_ += length;
  
  do {
    *(start + --length) = (u8)'0' + (value % 10);
    value /= 10;
  } while(length);
}

void fill_s64_number_string(s64 value, u8 length, u8 **start_, u8 *end_of_buffer) {
  assert(length);
  assert(*start_ && end_of_buffer);
  assert(*start_ + length < end_of_buffer);
  
  u8 *start = *start_;
  *start_ += length;
  
  if(value < 0) {
    value = -value;
    *(start++) = (u8)'-';
    --length;
  }
  
  do {
    *(start + --length) = (u8)'0' + (value % 10);
    value /= 10;
  } while(length);
}

void fill_f64_number_string(
  f64 value, u8 length, u8 f_length, u8 **start_, u8 *end_of_buffer
) {
  assert(length);
  assert(*start_ && end_of_buffer);
  assert(*start_ + length < end_of_buffer);
  
  u8 *start = *start_;
  *start_ += length;
  
  //printf("length = %hhu\nf_length = %hhu\n", length, f_length);
  
  u8 i_length = length - f_length;
  if(value < 0) {
    value = -value;
    *(start++) = (u8)'-';
    --i_length;
  }
  
  u32 i = (u32)value;
  fill_u64_number_string(i, i_length, &start, end_of_buffer);
  
  if(f_length) {
    *(start++) = (u8)'.';
    --f_length;
    u32 f = (value - i)*exp10_u64(f_length);
    fill_u64_number_string(f, f_length, &start, end_of_buffer);
  }
  
  *start_ = start;
}

void hc_fputs(FILE *output, string_t s) {
  fwrite(s.data, s.size, 1, output);
}
void hc_puts(string_t s) {
  hc_fputs(stdout, s);
}

u8 is_print_arg(u8 *c, u64 x) {
  assert(c);
  
  if(!x) return 0;
  
  u8 is_valid = TRUE;
  
  switch(*c) {
    case (u8)'s':
      is_valid = (x >= 2 && c[1] == (u8)'t' && c[2] == (u8)'r');
      if(is_valid)
        break;
    case (u8)'u':
      is_valid =
        ((x >= 2 && c[1] == (u8)'1' && c[2] == (u8)'6') ||
        (x >= 1 && c[1] == (u8)'8'));
      if(is_valid)
        break;
    case (u8)'f':
      is_valid =
        x >= 2 && 
        ((c[1] == (u8)'3' && c[2] == (u8)'2') ||
        (c[1] == (u8)'6' && c[2] == (u8)'4'));
      if(is_valid)
        break;
      break;
    case (u8)'c':
      is_valid = TRUE;
      break;
    default:
      is_valid = FALSE;
  }
  
  return is_valid;
}

#define PRINT_ARG_GET_TYPE(x) ((x) & 0x3)
#define PRINT_ARG_GET_SIZE(x) (((x) >> 2) & 0x3)

#define PRINT_ARG_MAKE_SIZE(x) (((x) & 0x3) << 2)

#define PRINT_ARG_TYPE_UNSIGNED 0x00
#define PRINT_ARG_TYPE_SIGNED   0x01
#define PRINT_ARG_TYPE_FLOAT    0x02
#define PRINT_ARG_TYPE_CHAR     0x03
#define PRINT_ARG_TYPE_STRING   0x13

typedef struct {
  u64 length;
  u8 type;
  u8 f_length;
} print_arg_info_t;

u8 extract_print_arg_info(
  print_arg_info_t *info, va_list list, u8 *start, u64 left, u8 **end
) {
  assert(info && start && end);
  
  if(left == 0)
    return FALSE;
  switch(*start) {
    case (u8)'u': {
      info->type |= PRINT_ARG_TYPE_UNSIGNED;
      
      if(left >= 1 && start[1] == (u8)'8') {
        info->type |= PRINT_ARG_MAKE_SIZE(0);
        u8 value = va_arg(list, u32);
        info->length = log10_u8(value) + 1;
        *end = start + 2;
      } else if(left >= 2) {
        if(start[1] == (u8)'1' && start[2] == (u8)'6') {
          info->type |= PRINT_ARG_MAKE_SIZE(1);
          u16 value = va_arg(list, u32);
          info->length = log10_u16(value) + 1;
        } else if(start[1] == (u8)'3' && start[2] == (u8)'2') {
          info->type |= PRINT_ARG_MAKE_SIZE(2);
          u32 value = va_arg(list, u32);
          info->length = log10_u32(value) + 1;
        } else if(start[1] == (u8)'6' && start[2] == (u8)'4') {
          info->type |= PRINT_ARG_MAKE_SIZE(3);
          info->length = log10_u64(va_arg(list, u64)) + 1;
        } else
          return FALSE;
        
        *end = start + 3;
      } else
        return FALSE;
    } break;
    
    case (u8)'s': {
      if(left >= 1 && start[1] == (u8)'8') {
        info->type |= PRINT_ARG_TYPE_SIGNED | PRINT_ARG_MAKE_SIZE(0);
        s8 val = va_arg(list, s32);
        info->length = val < 0 ? log10_u8(-val) + 2 : log10_u8(val) + 1;
        *end = start + 2;
      } else if(left >= 2) {
        if(start[1] == (u8)'1' && start[2] == (u8)'6') {
          info->type |= PRINT_ARG_TYPE_SIGNED | PRINT_ARG_MAKE_SIZE(1);
          s16 val = va_arg(list, s32);
          info->length = val < 0 ? log10_u16(-val) + 2 : log10_u16(val) + 1;
        } else if(start[1] == (u8)'3' && start[2] == (u8)'2') {
          info->type |= PRINT_ARG_TYPE_SIGNED | PRINT_ARG_MAKE_SIZE(2);
          s32 val = va_arg(list, s32);
          info->length = val < 0 ? log10_u32(-val) + 2 : log10_u32(val) + 1;
        } else if(start[1] == (u8)'6' && start[2] == (u8)'4') {
          info->type |= PRINT_ARG_TYPE_SIGNED | PRINT_ARG_MAKE_SIZE(3);
          s64 val = va_arg(list, s64);
          info->length = val < 0 ? log10_u64(-val) + 2 : log10_u64(val) + 1;
        } else if(start[1] == (u8)'t' && start[2] == (u8)'r') {
          info->type |= PRINT_ARG_TYPE_STRING;
          info->length = (va_arg(list, string_t)).size;
        } else
          return FALSE;
        
        *end = start + 3;
      } else
        return FALSE;
    } break;
    
    case (u8)'f': {
      if(left < 2)
        return FALSE;
      
      if(start[1] == (u8)'3' && start[2] == (u8)'2')
        info->type |= PRINT_ARG_MAKE_SIZE(2);
      else if(start[1] == (u8)'6' && start[2] == (u8)'4')
        info->type |= PRINT_ARG_MAKE_SIZE(3);
      else
        return FALSE;
      
      info->type |= PRINT_ARG_TYPE_FLOAT;
      
      f64 val = va_arg(list, f64);
      if(val < 0) {
        val = -val;
        info->length = 1;
      } else 
        info->length = 0;
      
      u32 i = val;
      info->length += log10_u32(i) + 1;
      
      u8 x = 7;
      u32 f = (val - i)*exp10_u64(x);
      while(x && f % 10 == 0)
        f /= 10, --x;
      
      if(f) {
        info->f_length = log10_u32(f) + 2;
        info->length += info->f_length;
      } else
        info->f_length = 0;
      
      *end = start + 3;
    } break;
    
    case (u8)'c': {
      info->type = PRINT_ARG_TYPE_CHAR;
      info->length = 1;
      *end = start + 1;
    } break;
    default:
      return FALSE;
  }
  
  return TRUE;
}

/*u64 hc_vsprint_string_length(string_t format, va_list list) {
  u8 *format_end = format.data + format.size;
  u64 arg_count = 0;
  u8 *it = format.data;
  while(it + 1 < format_end) {
    if(*it == (u8)'%' && is_print_arg(it + 1, format_end)) {
      ++arg_count;
    }
    ++it;
  }
  
  if(!arg_count)
    return copy_string(format);
  
  u64 *arg_lengths = temp_allocate(arg_count*sizeof(u64));
  
  u64 *cut_length = temp_allocate((arg_count + 1)*sizeof(u64));
  
  it = format.data;
  u8 *previous_cut_end = it;
  u64 arg_it = 0;
  u64 final_length = 0;
  va_list list1;
  va_copy(list1, list);
  while(arg_it < arg_count) {
    while(it < format_end && *it != (u8)'%')
      ++it;
    
    assert(it >= format_end);
    
    u8 *specifier_end;
    u8 could_extract_info = extract_print_arg_info(
      infos + arg_it, list1, it, &specifier_end, format_end
    );
    
    if(could_extract_info) {
      cuts[arg_it].size = it - previous_cut_end;
      final_length += infos[arg_it].length + cuts[arg_it].size;
      previous_cut_end = specifier_end;
      ++arg_it;
    }
    
    ++it;
  }
  va_end(list1);
  
  final_length += format_end - previous_cut_end;
  
  return final_length;
}*/

string_t hc_vsprint(memory_allocate_t ma, string_t format, va_list list) {
  u64 arg_count = 0;
  u8 *format_end = format.data + format.size;
  
  for(u8 *it = format.data; it < format_end; ++it)
    arg_count += (*it == (u8)'%' && is_print_arg(it + 1, format_end - it - 1));
  
  //printf("printing '%s'\n", format.data);
  
  if(!arg_count)
    return copy_string(format);
  
  print_arg_info_t *infos = temp_allocate(arg_count*sizeof(print_arg_info_t));
  string_t *cuts = temp_allocate((arg_count + 1)*sizeof(string_t));
  
  string_t s = {};
  
  u8 *it = format.data;
  cuts[0].data = format.data;
  
  va_list list1;
  va_copy(list1, list);
  for(u64 i = 0; i < arg_count; ++i) {
    print_arg_info_t *info = infos + i;
    u8 could_extract_info;
    do {
      while(it + 1 < format_end && *it != (u8)'%')
        ++it;
      
      assert(it < format_end);
      
      could_extract_info = extract_print_arg_info(
        info, list1, it + 1, format_end - it - 1, &cuts[i + 1].data
      );
    } while(!could_extract_info);
    
    //printf("pai[%lu].length = %lu\n", i, info->length);
    
    cuts[i].size = it - cuts[i].data;
    s.size += info->length + cuts[i].size;
    
    it = cuts[i + 1].data;
  }
  va_end(list1);
  
  cuts[arg_count].size = format_end - cuts[arg_count].data;
  s.size += cuts[arg_count].size;
  
  s.data = ma(s.size);
  memset(s.data, 0, s.size);
  
  u8 *s_end = s.data + s.size;
  u8 *cursor = s.data;
  for(u64 i = 0; i <= arg_count; ++i) {
    memcpy(cursor, cuts[i].data, cuts[i].size);
    cursor += cuts[i].size;
    
    if(i == arg_count)
      break;
    
    print_arg_info_t info = infos[i];
    switch(info.type) {
      case PRINT_ARG_TYPE_UNSIGNED | PRINT_ARG_MAKE_SIZE(0) :
      case PRINT_ARG_TYPE_UNSIGNED | PRINT_ARG_MAKE_SIZE(1) :
      case PRINT_ARG_TYPE_UNSIGNED | PRINT_ARG_MAKE_SIZE(2) :
        fill_u64_number_string(va_arg(list, u32), info.length, &cursor, s_end);
        break;
      case PRINT_ARG_TYPE_UNSIGNED | PRINT_ARG_MAKE_SIZE(3) :
        fill_u64_number_string(va_arg(list, u64), info.length, &cursor, s_end);
        break;
      
      case PRINT_ARG_TYPE_SIGNED | PRINT_ARG_MAKE_SIZE(0) :
      case PRINT_ARG_TYPE_SIGNED | PRINT_ARG_MAKE_SIZE(1) :
      case PRINT_ARG_TYPE_SIGNED | PRINT_ARG_MAKE_SIZE(2) :
        fill_s64_number_string(va_arg(list, s32), info.length, &cursor, s_end);
        break;
      case PRINT_ARG_TYPE_SIGNED | PRINT_ARG_MAKE_SIZE(3) :
        fill_s64_number_string(va_arg(list, s64), info.length, &cursor, s_end);
        break;
      
      case PRINT_ARG_TYPE_FLOAT | PRINT_ARG_MAKE_SIZE(2) :
      case PRINT_ARG_TYPE_FLOAT | PRINT_ARG_MAKE_SIZE(3) : {
        fill_f64_number_string(
          va_arg(list, f64), info.length, info.f_length, &cursor, s_end
        );
      } break;
      
      case PRINT_ARG_TYPE_CHAR: {
        *(cursor++) = va_arg(list, u32);
      } break;
      
      case PRINT_ARG_TYPE_STRING : {
        string_t x = va_arg(list, string_t);
        memcpy(cursor, x.data, x.size);
        cursor += x.size;
      } break;
    }
  }
  
  return s;
}

string_t hc_sprint(string_t format, ...) {
  va_list list;
  va_start(list, format);
  string_t s = hc_vsprint(malloc, format, list);
  va_end(list);
  
  return s;
}

void hc_print(string_t format, ...) {
  va_list list;
  va_start(list, format);
  string_t s = hc_vsprint(temp_allocate, format, list);
  va_end(list);
  
  hc_puts(s);
}
void hc_fprint(FILE *output, string_t format, ...) {
  va_list list;
  va_start(list, format);
  string_t s = hc_vsprint(temp_allocate, format, list);
  va_end(list);
  
  hc_fputs(output, s);
}

u8 can_numerize_u64_string(u8 *start) {
  return is_number(*start);
}
u64 numerize_u64_string(u8 *start, u8 *end, u8 **end_of_value_) {
  assert(start && end);
  
  u8 *end_of_value = start;
  while(end_of_value < end && is_number(*end_of_value))
    ++end_of_value;
  
  u64 value = 0;
  for(u8 *it = start; it < end_of_value; ++it) {
    value += *it - (u8)'0';
    if(it != end_of_value - 1)
      value *= 10;
  }
  
  if(end_of_value_)
    *end_of_value_ = end_of_value;
  
  return value;
}

u8 can_numerize_s64_string(u8 *start, u8 *end) {
  if(!is_number(*start) && *start == (u8)'-' && start + 1 >= end && is_number(start[1]))
    return TRUE;
  
  return TRUE;
}
s64 numerize_s64_string(u8 *start, u8 *end, u8 **end_of_value_) {
  assert(start && end);
  
  s8 is_negative = (*start == (u8)'-');
  if(is_negative)
    ++start;
  
  u8 *end_of_value = start;
  
  while(end_of_value < end && is_number(*end_of_value))
    ++end_of_value;
  
  if(end_of_value_)
    *end_of_value_ = end_of_value;
  
  s64 value = 0;
  for(u8 *it = start; it < end_of_value; ++it) {
    value += *it - (u8)'0';
    if(it < end_of_value - 1)
      value *= 10;
  }
  
  return (value != 0 && is_negative) ? -value : value;
}

u8 can_numerize_f32_string(u8 *start, u8 *end_of_buffer) {
  if(*start == (u8)'-')
    ++start;
  
  u8 dots_encoutered = 0, has_number = 0;
  while(start < end_of_buffer) {
    if(!is_number(*start))
      break;
    
    if(*start == (u8)'.' && ++dots_encoutered > 1) {
      return FALSE;
    } else if(is_number(*start))
      has_number = TRUE;
      
    ++start;
  }
  
  return has_number;
}
f32 numerize_f32_string(u8 *start, u8 *end_of_buffer, u8 **end_of_value_) {
  assert(start && end_of_buffer);
  
  u8 is_negative = (*start == (u8)'-');
  if(is_negative)
    ++start;
  
  u8 *end = start;
  while(end < end_of_buffer && (is_number(*end) || *end == '.'))
    ++end;
  
  if(end_of_value_)
    *end_of_value_ = end;
  
  u8 *dot_place = start;
  while(dot_place < end && *dot_place != (u8)'.')
    ++dot_place;
  
  f32 value = 0;
  
  for(; start < dot_place; ++start) {
    value += *start - (u8)'0';
    if(start < dot_place - 1)
      value *= 10;
  }
  ++start;
  
  u64 x = 10;
  for(; start < end; ++start) {
    value += (f32)(*start - (u8)'0') / x;
    x *= 10;
  }
  
  return (value != 0.0f && is_negative) ? -value : value;
}

#endif // STRINGSTUFF_H
