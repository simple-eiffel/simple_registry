/*
 * simple_registry.h - Windows Registry helper functions for Eiffel
 * 
 * This header provides a C interface to Windows Registry operations,
 * designed to be called from Eiffel via inline C externals.
 * 
 * Following Eric Bezault's recommended pattern: struct definitions
 * and helper functions in .h file, called from Eiffel inline C.
 */

#ifndef SIMPLE_REGISTRY_H
#define SIMPLE_REGISTRY_H

#include <windows.h>
#include <stdlib.h>
#include <string.h>

/* Result structure for registry operations */
typedef struct {
    int success;           /* Non-zero if operation succeeded */
    char* string_value;    /* String result (caller must free via sr_free_result) */
    DWORD dword_value;     /* DWORD result */
    char* error_message;   /* Error message if failed (caller must free) */
} sr_result;

/* Allocate and initialize a result structure */
static sr_result* sr_create_result(void) {
    sr_result* r = (sr_result*)malloc(sizeof(sr_result));
    if (r) {
        r->success = 0;
        r->string_value = NULL;
        r->dword_value = 0;
        r->error_message = NULL;
    }
    return r;
}

/* Set error message in result */
static void sr_set_error(sr_result* r, LONG error_code) {
    char buf[256];
    FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, error_code, 0, buf, sizeof(buf), NULL
    );
    r->error_message = _strdup(buf);
}

/* Free a result structure and its contents */
static void sr_free_result(sr_result* r) {
    if (r) {
        if (r->string_value) free(r->string_value);
        if (r->error_message) free(r->error_message);
        free(r);
    }
}

/* Read a string value from registry */
static sr_result* sr_read_string(HKEY root, const char* subkey, const char* name) {
    sr_result* r = sr_create_result();
    if (!r) return NULL;
    
    HKEY hKey;
    LONG result = RegOpenKeyExA(root, subkey, 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS) {
        sr_set_error(r, result);
        return r;
    }
    
    DWORD type, size = 0;
    result = RegQueryValueExA(hKey, name, NULL, &type, NULL, &size);
    if (result != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        sr_set_error(r, result);
        return r;
    }
    
    if (type != REG_SZ && type != REG_EXPAND_SZ) {
        RegCloseKey(hKey);
        r->error_message = _strdup("Value is not a string type");
        return r;
    }
    
    r->string_value = (char*)malloc(size + 1);
    if (!r->string_value) {
        RegCloseKey(hKey);
        r->error_message = _strdup("Memory allocation failed");
        return r;
    }
    
    result = RegQueryValueExA(hKey, name, NULL, NULL, (LPBYTE)r->string_value, &size);
    RegCloseKey(hKey);
    
    if (result == ERROR_SUCCESS) {
        r->string_value[size] = '\0';
        r->success = 1;
    } else {
        free(r->string_value);
        r->string_value = NULL;
        sr_set_error(r, result);
    }
    
    return r;
}

/* Read a DWORD value from registry */
static sr_result* sr_read_dword(HKEY root, const char* subkey, const char* name) {
    sr_result* r = sr_create_result();
    if (!r) return NULL;
    
    HKEY hKey;
    LONG result = RegOpenKeyExA(root, subkey, 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS) {
        sr_set_error(r, result);
        return r;
    }
    
    DWORD type, size = sizeof(DWORD);
    result = RegQueryValueExA(hKey, name, NULL, &type, (LPBYTE)&r->dword_value, &size);
    RegCloseKey(hKey);
    
    if (result == ERROR_SUCCESS && type == REG_DWORD) {
        r->success = 1;
    } else if (result == ERROR_SUCCESS) {
        r->error_message = _strdup("Value is not a DWORD type");
    } else {
        sr_set_error(r, result);
    }
    
    return r;
}

/* Write a string value to registry */
static int sr_write_string(HKEY root, const char* subkey, const char* name, const char* value) {
    HKEY hKey;
    LONG result = RegCreateKeyExA(root, subkey, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
    if (result != ERROR_SUCCESS) return 0;
    
    result = RegSetValueExA(hKey, name, 0, REG_SZ, (const BYTE*)value, (DWORD)strlen(value) + 1);
    RegCloseKey(hKey);
    
    return (result == ERROR_SUCCESS) ? 1 : 0;
}

/* Write a DWORD value to registry */
static int sr_write_dword(HKEY root, const char* subkey, const char* name, DWORD value) {
    HKEY hKey;
    LONG result = RegCreateKeyExA(root, subkey, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
    if (result != ERROR_SUCCESS) return 0;
    
    result = RegSetValueExA(hKey, name, 0, REG_DWORD, (const BYTE*)&value, sizeof(DWORD));
    RegCloseKey(hKey);
    
    return (result == ERROR_SUCCESS) ? 1 : 0;
}

/* Delete a registry value */
static int sr_delete_value(HKEY root, const char* subkey, const char* name) {
    HKEY hKey;
    LONG result = RegOpenKeyExA(root, subkey, 0, KEY_SET_VALUE, &hKey);
    if (result != ERROR_SUCCESS) return 0;
    
    result = RegDeleteValueA(hKey, name);
    RegCloseKey(hKey);
    
    return (result == ERROR_SUCCESS) ? 1 : 0;
}

/* Delete a registry key (must be empty) */
static int sr_delete_key(HKEY root, const char* subkey) {
    return (RegDeleteKeyA(root, subkey) == ERROR_SUCCESS) ? 1 : 0;
}

/* Check if a registry key exists */
static int sr_key_exists(HKEY root, const char* subkey) {
    HKEY hKey;
    LONG result = RegOpenKeyExA(root, subkey, 0, KEY_READ, &hKey);
    if (result == ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return 1;
    }
    return 0;
}

/* Check if a registry value exists */
static int sr_value_exists(HKEY root, const char* subkey, const char* name) {
    HKEY hKey;
    LONG result = RegOpenKeyExA(root, subkey, 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS) return 0;
    
    result = RegQueryValueExA(hKey, name, NULL, NULL, NULL, NULL);
    RegCloseKey(hKey);
    
    return (result == ERROR_SUCCESS) ? 1 : 0;
}

/* Create a registry key */
static int sr_create_key(HKEY root, const char* subkey) {
    HKEY hKey;
    LONG result = RegCreateKeyExA(root, subkey, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
    if (result == ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return 1;
    }
    return 0;
}

#endif /* SIMPLE_REGISTRY_H */
