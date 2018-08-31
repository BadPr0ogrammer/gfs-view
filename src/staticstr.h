/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#pragma once

#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

#define CHAR_SET_NUM 25
extern const char* char_set_names[CHAR_SET_NUM];

#define DASH_LINE_NAME_NUM 3
extern const char* dash_line_names[DASH_LINE_NAME_NUM];

#define COLOR_NUM 17
extern const char *color_name[COLOR_NUM];

typedef struct {
  int no;
  char *name;
} codetable_4_230;

#define NUM_codetable_4_230 117
extern codetable_4_230 codetable_4_230_table[NUM_codetable_4_230];

typedef struct {
  char *name;
} level_table_192_t;

extern level_table_192_t level_table[192];

typedef struct tag_to_level_t {
  const char  *tag;
  const char  *lev;
  const char  *lval[TAG_TO_LEVEL_SIZE];
  int         idx;
} tag_to_level_t;

extern tag_to_level_t gfs_tag2level[TAG_TO_LEVEL_NUM];

int gfs_tag2level_idx(const string& tag, const string& lev = string(), const string& lval = string());
int grib_level_idx(const string& key);
int tag2level_idx(const string&role, const string& tag, const string& lev = string(), const string& lval = string());

