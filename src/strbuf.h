#ifndef CARP_STRBUF_H
#define CARP_STRBUF_H

#include <string.h>
#include <stdio.h>

#define STRBUF_DEFAULT_CAP 256

typedef struct {
  char* data;
  int len;
  int cap;
} StringBuf;

/* Ensure at least `extra` bytes of free space */
static void strbuf_grow(StringBuf* sb, int extra) {
  if (sb->len + extra > sb->cap) {
    int new_cap = (sb->len + extra) * 2;
    if (new_cap < STRBUF_DEFAULT_CAP) new_cap = STRBUF_DEFAULT_CAP;
    sb->data = CARP_REALLOC(sb->data, new_cap);
    sb->cap = new_cap;
  }
}

StringBuf StringBuf_create() {
  StringBuf sb;
  sb.data = CARP_MALLOC(STRBUF_DEFAULT_CAP);
  sb.len = 0;
  sb.cap = STRBUF_DEFAULT_CAP;
  return sb;
}

StringBuf StringBuf_with_MINUS_capacity(int cap) {
  StringBuf sb;
  sb.data = CARP_MALLOC(cap > 0 ? cap : STRBUF_DEFAULT_CAP);
  sb.len = 0;
  sb.cap = cap > 0 ? cap : STRBUF_DEFAULT_CAP;
  return sb;
}

void StringBuf_append_MINUS_str(StringBuf* sb, String* s) {
  int slen = strlen(*s);
  strbuf_grow(sb, slen);
  memcpy(sb->data + sb->len, *s, slen);
  sb->len += slen;
}

void StringBuf_append_MINUS_char(StringBuf* sb, char c) {
  strbuf_grow(sb, 1);
  sb->data[sb->len++] = c;
}

void StringBuf_append_MINUS_bytes(StringBuf* sb, Array* bytes) {
  strbuf_grow(sb, bytes->len);
  memcpy(sb->data + sb->len, bytes->data, bytes->len);
  sb->len += bytes->len;
}

void StringBuf_append_MINUS_int(StringBuf* sb, int n) {
  char tmp[24];
  int slen = snprintf(tmp, sizeof(tmp), "%d", n);
  strbuf_grow(sb, slen);
  memcpy(sb->data + sb->len, tmp, slen);
  sb->len += slen;
}

void StringBuf_append_MINUS_double(StringBuf* sb, double n) {
  char tmp[64];
  int slen = snprintf(tmp, sizeof(tmp), "%g", n);
  strbuf_grow(sb, slen);
  memcpy(sb->data + sb->len, tmp, slen);
  sb->len += slen;
}

void StringBuf_append_MINUS_crlf(StringBuf* sb) {
  strbuf_grow(sb, 2);
  sb->data[sb->len++] = '\r';
  sb->data[sb->len++] = '\n';
}

int StringBuf_length(StringBuf* sb) {
  return sb->len;
}

/* Returns an owned String, consuming the buffer contents.
   The StringBuf is reset to empty but keeps its allocation. */
String StringBuf_to_MINUS_string(StringBuf* sb) {
  String s = CARP_MALLOC(sb->len + 1);
  memcpy(s, sb->data, sb->len);
  s[sb->len] = '\0';
  sb->len = 0;
  return s;
}

/* Returns a copy as String without modifying the buffer. */
String StringBuf_str(StringBuf* sb) {
  String s = CARP_MALLOC(sb->len + 1);
  memcpy(s, sb->data, sb->len);
  s[sb->len] = '\0';
  return s;
}

void StringBuf_clear(StringBuf* sb) {
  sb->len = 0;
}

void StringBuf_delete(StringBuf sb) {
  if (sb.data) CARP_FREE(sb.data);
}

StringBuf StringBuf_copy(StringBuf* sb) {
  StringBuf c;
  c.cap = sb->cap;
  c.len = sb->len;
  c.data = CARP_MALLOC(c.cap);
  memcpy(c.data, sb->data, c.len);
  return c;
}

#endif
