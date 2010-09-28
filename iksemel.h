/* iksemel (XML parser for Jabber)
** Copyright (C) 2000-2007 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#ifndef IKSEMEL_H
#define IKSEMEL_H 1

#include <stddef.h>	/* size_t for C */

/* minimum sax buffer size */
#define SAX_BUFFER_MIN_SIZE 128

/* sax parser structure plus extra data of dom parser */
#define DEFAULT_DOM_CHUNK_SIZE 256

/* sax parser structure plus extra data of stream parser */
#define DEFAULT_STREAM_CHUNK_SIZE 256

/* iks structure, its data, child iks structures, for stream parsing */
#define DEFAULT_IKS_CHUNK_SIZE 1024

/* iks structure, its data, child iks structures, for file parsing */
#define DEFAULT_DOM_IKS_CHUNK_SIZE 2048

/* file is read by blocks with this size */
#define FILE_IO_BUF_SIZE 4096

/*****  object stack  *****/

struct ikstack_struct;
typedef struct ikstack_struct ikstack;

ikstack *iks_stack_new (size_t meta_chunk, size_t data_chunk);
void *iks_stack_alloc (ikstack *s, size_t size);
char *iks_stack_strdup (ikstack *s, const char *src, size_t len);
char *iks_stack_strcat (ikstack *s, char *old, size_t old_len, const char *src, size_t src_len);
void iks_stack_stat (ikstack *s, size_t *allocated, size_t *used);
void iks_stack_delete (ikstack *s);

/*****  utilities  *****/

void *iks_malloc (size_t size);
void iks_free (void *ptr);
void iks_set_mem_funcs (void *(*malloc_func)(size_t size), void (*free_func)(void *ptr));

char *iks_strdup (const char *src);
char *iks_strcat (char *dest, const char *src);
int iks_strcmp (const char *a, const char *b);
int iks_strcasecmp (const char *a, const char *b);
int iks_strncmp (const char *a, const char *b, size_t n);
int iks_strncasecmp (const char *a, const char *b, size_t n);
size_t iks_strlen (const char *src);
char *iks_escape (ikstack *s, char *src, size_t len);
char *iks_unescape (ikstack *s, char *src, size_t len);

/*****  dom tree  *****/

enum ikstype {
	IKS_NONE = 0,
	IKS_TAG,
	IKS_ATTRIBUTE,
	IKS_CDATA
};

struct iks_struct;
typedef struct iks_struct iks;

iks *iks_new (const char *name);
iks *iks_new_within (const char *name, ikstack *s);
iks *iks_insert (iks *x, const char *name);
iks *iks_insert_cdata (iks *x, const char *data, size_t len);
iks *iks_insert_attrib (iks *x, const char *name, const char *value);
iks *iks_insert_node (iks *x, iks *y);
iks *iks_append (iks *x, const char *name);
iks *iks_prepend (iks *x, const char *name);
iks *iks_append_cdata (iks *x, const char *data, size_t len);
iks *iks_prepend_cdata (iks *x, const char *data, size_t len);
void iks_hide (iks *x);
void iks_delete (iks *x);
iks *iks_next (iks *x);
iks *iks_next_tag (iks *x);
iks *iks_prev (iks *x);
iks *iks_prev_tag (iks *x);
iks *iks_parent (iks *x);
iks *iks_root (iks *x);
iks *iks_child (iks *x);
iks *iks_first_tag (iks *x);
iks *iks_attrib (iks *x);
iks *iks_find (iks *x, const char *name);
char *iks_find_cdata (iks *x, const char *name);
char *iks_find_attrib (iks *x, const char *name);
iks *iks_find_with_attrib (iks *x, const char *tagname, const char *attrname, const char *value);
ikstack *iks_stack (iks *x);
enum ikstype iks_type (iks *x);
char *iks_name (iks *x);
char *iks_cdata (iks *x);
size_t iks_cdata_size (iks *x);
int iks_has_children (iks *x);
int iks_has_attribs (iks *x);
char *iks_string (ikstack *s, iks *x);
iks *iks_copy (iks *x);
iks *iks_copy_within (iks *x, ikstack *s);

/*****  sax parser  *****/

enum ikserror {
	IKS_OK = 0,
	IKS_NOMEM,
	IKS_BADXML,
	IKS_HOOK
};

enum ikstagtype {
	IKS_OPEN,
	IKS_CLOSE,
	IKS_SINGLE
};

typedef int (iksTagHook)(void *user_data, char *name, char **atts, int type);
typedef int (iksCDataHook)(void *user_data, char *data, size_t len);
typedef void (iksDeleteHook)(void *user_data);

struct iksparser_struct;
typedef struct iksparser_struct  iksparser;

iksparser *iks_sax_new (void *user_data, iksTagHook *tagHook, iksCDataHook *cdataHook);
iksparser *iks_sax_extend (ikstack *s, void *user_data, iksTagHook *tagHook, iksCDataHook *cdataHook, iksDeleteHook *deleteHook);
ikstack *iks_parser_stack (iksparser *prs);
void *iks_user_data (iksparser *prs);
unsigned long iks_nr_bytes (iksparser *prs);
unsigned long iks_nr_lines (iksparser *prs);
int iks_parse (iksparser *prs, const char *data, size_t len, int finish);
void iks_parser_reset (iksparser *prs);
void iks_parser_delete (iksparser *prs);

/*****  dom parser  *****/

enum iksfileerror {
	IKS_FILE_NOFILE = 4,
	IKS_FILE_NOACCESS,
	IKS_FILE_RWERR
};

iksparser *iks_dom_new (iks **iksptr);
void iks_set_size_hint (iksparser *prs, size_t approx_size);
iks *iks_tree (const char *xml_str, size_t len, int *err);
int iks_load (const char *fname, iks **xptr);
int iks_save (const char *fname, iks *x);

#ifdef __cplusplus
}
#endif

#endif  /* IKSEMEL_H */
