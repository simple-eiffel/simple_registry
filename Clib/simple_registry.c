/*
 * simple_registry.c - Windows Registry access for Eiffel
 * Copyright (c) 2025 Larry Rix - MIT License
 */

#include "simple_registry.h"
#include <stdlib.h>
#include <string.h>

static char last_error_msg[512] = {0};

static void store_last_error(void) {
    DWORD err = GetLastError();
    FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        last_error_msg, sizeof(last_error_msg) - 1, NULL
    );
}

const char* sr_get_last_error(void) {
    return last_error_msg;
}

HKEY sr_open_key(HKEY root, const char* subkey, int write_access) {
    HKEY result = NULL;
    REGSAM access = KEY_READ;

    if (write_access) {
        access |= KEY_WRITE;
    }

    if (RegOpenKeyExA(root, subkey, 0, access, &result) != ERROR_SUCCESS) {
        store_last_error();
        return NULL;
    }

    return result;
}

void sr_close_key(HKEY key) {
    if (key != NULL) {
        RegCloseKey(key);
    }
}

sr_result* sr_read_string(HKEY root, const char* subkey, const char* value_name) {
    sr_result* result;
    HKEY key;
    DWORD type, size = 0;
    char* buffer;
    LONG err;

    result = (sr_result*)malloc(sizeof(sr_result));
    if (!result) return NULL;
    memset(result, 0, sizeof(sr_result));

    key = sr_open_key(root, subkey, 0);
    if (!key) {
        result->error_message = _strdup(last_error_msg);
        return result;
    }

    /* Get size first */
    err = RegQueryValueExA(key, value_name, NULL, &type, NULL, &size);
    if (err != ERROR_SUCCESS) {
        SetLastError(err);
        store_last_error();
        result->error_message = _strdup(last_error_msg);
        sr_close_key(key);
        return result;
    }

    if (type != REG_SZ && type != REG_EXPAND_SZ) {
        result->error_message = _strdup("Value is not a string type");
        sr_close_key(key);
        return result;
    }

    buffer = (char*)malloc(size + 1);
    if (!buffer) {
        result->error_message = _strdup("Memory allocation failed");
        sr_close_key(key);
        return result;
    }

    err = RegQueryValueExA(key, value_name, NULL, &type, (LPBYTE)buffer, &size);
    sr_close_key(key);

    if (err != ERROR_SUCCESS) {
        free(buffer);
        SetLastError(err);
        store_last_error();
        result->error_message = _strdup(last_error_msg);
        return result;
    }

    buffer[size] = '\0';
    result->success = 1;
    result->value_type = type;
    result->string_value = buffer;

    return result;
}

sr_result* sr_read_dword(HKEY root, const char* subkey, const char* value_name) {
    sr_result* result;
    HKEY key;
    DWORD type, size = sizeof(DWORD), value;
    LONG err;

    result = (sr_result*)malloc(sizeof(sr_result));
    if (!result) return NULL;
    memset(result, 0, sizeof(sr_result));

    key = sr_open_key(root, subkey, 0);
    if (!key) {
        result->error_message = _strdup(last_error_msg);
        return result;
    }

    err = RegQueryValueExA(key, value_name, NULL, &type, (LPBYTE)&value, &size);
    sr_close_key(key);

    if (err != ERROR_SUCCESS) {
        SetLastError(err);
        store_last_error();
        result->error_message = _strdup(last_error_msg);
        return result;
    }

    if (type != REG_DWORD) {
        result->error_message = _strdup("Value is not a DWORD type");
        return result;
    }

    result->success = 1;
    result->value_type = type;
    result->dword_value = value;

    return result;
}

sr_result* sr_read_value(HKEY root, const char* subkey, const char* value_name) {
    sr_result* result;
    HKEY key;
    DWORD type, size = 0;
    LONG err;

    result = (sr_result*)malloc(sizeof(sr_result));
    if (!result) return NULL;
    memset(result, 0, sizeof(sr_result));

    key = sr_open_key(root, subkey, 0);
    if (!key) {
        result->error_message = _strdup(last_error_msg);
        return result;
    }

    /* Get type and size */
    err = RegQueryValueExA(key, value_name, NULL, &type, NULL, &size);
    if (err != ERROR_SUCCESS) {
        SetLastError(err);
        store_last_error();
        result->error_message = _strdup(last_error_msg);
        sr_close_key(key);
        return result;
    }

    result->value_type = type;

    if (type == REG_SZ || type == REG_EXPAND_SZ) {
        char* buffer = (char*)malloc(size + 1);
        if (!buffer) {
            result->error_message = _strdup("Memory allocation failed");
            sr_close_key(key);
            return result;
        }

        err = RegQueryValueExA(key, value_name, NULL, &type, (LPBYTE)buffer, &size);
        if (err == ERROR_SUCCESS) {
            buffer[size] = '\0';
            result->success = 1;
            result->string_value = buffer;
        } else {
            free(buffer);
            SetLastError(err);
            store_last_error();
            result->error_message = _strdup(last_error_msg);
        }
    } else if (type == REG_DWORD) {
        DWORD value;
        size = sizeof(DWORD);
        err = RegQueryValueExA(key, value_name, NULL, &type, (LPBYTE)&value, &size);
        if (err == ERROR_SUCCESS) {
            result->success = 1;
            result->dword_value = value;
        } else {
            SetLastError(err);
            store_last_error();
            result->error_message = _strdup(last_error_msg);
        }
    } else if (type == REG_BINARY) {
        unsigned char* buffer = (unsigned char*)malloc(size);
        if (!buffer) {
            result->error_message = _strdup("Memory allocation failed");
            sr_close_key(key);
            return result;
        }

        err = RegQueryValueExA(key, value_name, NULL, &type, buffer, &size);
        if (err == ERROR_SUCCESS) {
            result->success = 1;
            result->binary_value = buffer;
            result->binary_length = size;
        } else {
            free(buffer);
            SetLastError(err);
            store_last_error();
            result->error_message = _strdup(last_error_msg);
        }
    } else {
        result->error_message = _strdup("Unsupported value type");
    }

    sr_close_key(key);
    return result;
}

int sr_write_string(HKEY root, const char* subkey, const char* value_name, const char* data) {
    HKEY key;
    LONG err;

    /* Create or open key with write access */
    err = RegCreateKeyExA(root, subkey, 0, NULL, 0, KEY_WRITE, NULL, &key, NULL);
    if (err != ERROR_SUCCESS) {
        SetLastError(err);
        store_last_error();
        return 0;
    }

    err = RegSetValueExA(key, value_name, 0, REG_SZ, (const BYTE*)data, (DWORD)(strlen(data) + 1));
    RegCloseKey(key);

    if (err != ERROR_SUCCESS) {
        SetLastError(err);
        store_last_error();
        return 0;
    }

    return 1;
}

int sr_write_dword(HKEY root, const char* subkey, const char* value_name, unsigned long data) {
    HKEY key;
    LONG err;

    err = RegCreateKeyExA(root, subkey, 0, NULL, 0, KEY_WRITE, NULL, &key, NULL);
    if (err != ERROR_SUCCESS) {
        SetLastError(err);
        store_last_error();
        return 0;
    }

    err = RegSetValueExA(key, value_name, 0, REG_DWORD, (const BYTE*)&data, sizeof(DWORD));
    RegCloseKey(key);

    if (err != ERROR_SUCCESS) {
        SetLastError(err);
        store_last_error();
        return 0;
    }

    return 1;
}

int sr_delete_value(HKEY root, const char* subkey, const char* value_name) {
    HKEY key;
    LONG err;

    key = sr_open_key(root, subkey, 1);
    if (!key) return 0;

    err = RegDeleteValueA(key, value_name);
    RegCloseKey(key);

    if (err != ERROR_SUCCESS) {
        SetLastError(err);
        store_last_error();
        return 0;
    }

    return 1;
}

int sr_delete_key(HKEY root, const char* subkey) {
    LONG err = RegDeleteKeyA(root, subkey);

    if (err != ERROR_SUCCESS) {
        SetLastError(err);
        store_last_error();
        return 0;
    }

    return 1;
}

int sr_key_exists(HKEY root, const char* subkey) {
    HKEY key = sr_open_key(root, subkey, 0);
    if (key) {
        sr_close_key(key);
        return 1;
    }
    return 0;
}

int sr_value_exists(HKEY root, const char* subkey, const char* value_name) {
    HKEY key;
    DWORD type, size = 0;
    LONG err;

    key = sr_open_key(root, subkey, 0);
    if (!key) return 0;

    err = RegQueryValueExA(key, value_name, NULL, &type, NULL, &size);
    sr_close_key(key);

    return (err == ERROR_SUCCESS) ? 1 : 0;
}

int sr_create_key(HKEY root, const char* subkey) {
    HKEY key;
    LONG err;

    err = RegCreateKeyExA(root, subkey, 0, NULL, 0, KEY_READ, NULL, &key, NULL);
    if (err != ERROR_SUCCESS) {
        SetLastError(err);
        store_last_error();
        return 0;
    }

    RegCloseKey(key);
    return 1;
}

void sr_free_result(sr_result* result) {
    if (result) {
        if (result->string_value) free(result->string_value);
        if (result->binary_value) free(result->binary_value);
        if (result->error_message) free(result->error_message);
        free(result);
    }
}
