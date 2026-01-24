# S04: FEATURE SPECS - simple_registry

**BACKWASH DOCUMENT** - Generated: 2026-01-23
**Status**: Reverse-engineered from existing implementation

## 1. SIMPLE_REGISTRY Features

### 1.1 Predefined Keys

| Feature | Type | Windows Constant |
|---------|------|------------------|
| HKEY_CLASSES_ROOT | POINTER | HKCR |
| HKEY_CURRENT_USER | POINTER | HKCU |
| HKEY_LOCAL_MACHINE | POINTER | HKLM |
| HKEY_USERS | POINTER | HKU |
| HKEY_CURRENT_CONFIG | POINTER | HKCC |

### 1.2 Read Operations

| Feature | Signature | Description |
|---------|-----------|-------------|
| read_string | `(a_root: POINTER; a_subkey, a_value_name: READABLE_STRING_GENERAL): detachable STRING_32` | Read REG_SZ value |
| read_integer | `(a_root: POINTER; a_subkey, a_value_name: READABLE_STRING_GENERAL): INTEGER` | Read REG_DWORD value |

### 1.3 Write Operations

| Feature | Signature | Description |
|---------|-----------|-------------|
| write_string | `(a_root: POINTER; a_subkey, a_value_name, a_value: READABLE_STRING_GENERAL)` | Write REG_SZ value |
| write_integer | `(a_root: POINTER; a_subkey, a_value_name: READABLE_STRING_GENERAL; a_value: INTEGER)` | Write REG_DWORD value |

### 1.4 Delete Operations

| Feature | Signature | Description |
|---------|-----------|-------------|
| delete_value | `(a_root: POINTER; a_subkey, a_value_name: READABLE_STRING_GENERAL)` | Delete registry value |
| delete_key | `(a_root: POINTER; a_subkey: READABLE_STRING_GENERAL)` | Delete registry key (must be empty) |

### 1.5 Query Operations

| Feature | Signature | Description |
|---------|-----------|-------------|
| key_exists | `(a_root: POINTER; a_subkey: READABLE_STRING_GENERAL): BOOLEAN` | Check if key exists |
| value_exists | `(a_root: POINTER; a_subkey, a_value_name: READABLE_STRING_GENERAL): BOOLEAN` | Check if value exists |
| create_key | `(a_root: POINTER; a_subkey: READABLE_STRING_GENERAL)` | Create registry key |

### 1.6 Status

| Feature | Type | Description |
|---------|------|-------------|
| last_operation_succeeded | BOOLEAN | Last write/delete success |
| last_read_succeeded | BOOLEAN | Last read success |
| last_error | detachable STRING_32 | Error message |
