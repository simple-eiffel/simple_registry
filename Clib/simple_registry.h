/*
 * simple_registry.h - Windows Registry access for Eiffel
 * Copyright (c) 2025 Larry Rix - MIT License
 */

#ifndef SIMPLE_REGISTRY_H
#define SIMPLE_REGISTRY_H

#include <windows.h>

/* Predefined key constants */
#define SR_HKEY_CLASSES_ROOT     ((HKEY)(ULONG_PTR)0x80000000)
#define SR_HKEY_CURRENT_USER     ((HKEY)(ULONG_PTR)0x80000001)
#define SR_HKEY_LOCAL_MACHINE    ((HKEY)(ULONG_PTR)0x80000002)
#define SR_HKEY_USERS            ((HKEY)(ULONG_PTR)0x80000003)
#define SR_HKEY_CURRENT_CONFIG   ((HKEY)(ULONG_PTR)0x80000005)

/* Value type constants */
#define SR_REG_SZ        1
#define SR_REG_EXPAND_SZ 2
#define SR_REG_BINARY    3
#define SR_REG_DWORD     4
#define SR_REG_MULTI_SZ  7

/* Result structure for registry operations */
typedef struct {
    int success;
    int error_code;
    int value_type;
    char* string_value;      /* For REG_SZ, REG_EXPAND_SZ */
    unsigned long dword_value;  /* For REG_DWORD */
    unsigned char* binary_value; /* For REG_BINARY */
    int binary_length;
    char* error_message;
} sr_result;

/* Open a registry key. Returns handle or NULL on failure. */
HKEY sr_open_key(HKEY root, const char* subkey, int write_access);

/* Close a registry key. */
void sr_close_key(HKEY key);

/* Read a string value. Caller must free result. */
sr_result* sr_read_string(HKEY root, const char* subkey, const char* value_name);

/* Read a DWORD value. */
sr_result* sr_read_dword(HKEY root, const char* subkey, const char* value_name);

/* Read any value (auto-detect type). Caller must free result. */
sr_result* sr_read_value(HKEY root, const char* subkey, const char* value_name);

/* Write a string value. */
int sr_write_string(HKEY root, const char* subkey, const char* value_name, const char* data);

/* Write a DWORD value. */
int sr_write_dword(HKEY root, const char* subkey, const char* value_name, unsigned long data);

/* Delete a value. */
int sr_delete_value(HKEY root, const char* subkey, const char* value_name);

/* Delete a key (must be empty). */
int sr_delete_key(HKEY root, const char* subkey);

/* Check if a key exists. */
int sr_key_exists(HKEY root, const char* subkey);

/* Check if a value exists. */
int sr_value_exists(HKEY root, const char* subkey, const char* value_name);

/* Create a key. */
int sr_create_key(HKEY root, const char* subkey);

/* Free result structure. */
void sr_free_result(sr_result* result);

/* Get last error message. */
const char* sr_get_last_error(void);

#endif /* SIMPLE_REGISTRY_H */
