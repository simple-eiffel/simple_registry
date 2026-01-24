# S03: CONTRACTS - simple_registry

**BACKWASH DOCUMENT** - Generated: 2026-01-23
**Status**: Reverse-engineered from existing implementation

## 1. SIMPLE_REGISTRY Contracts

### 1.1 Read Operations

#### read_string
```eiffel
require
    subkey_not_empty: not a_subkey.is_empty
```
Returns: detachable STRING_32 (Void on failure)

#### read_integer
```eiffel
require
    subkey_not_empty: not a_subkey.is_empty
```
Sets: last_read_succeeded, last_error

### 1.2 Write Operations

#### write_string
```eiffel
require
    subkey_not_empty: not a_subkey.is_empty
```
Sets: last_operation_succeeded

#### write_integer
```eiffel
require
    subkey_not_empty: not a_subkey.is_empty
```
Sets: last_operation_succeeded

### 1.3 Delete Operations

#### delete_value
```eiffel
require
    subkey_not_empty: not a_subkey.is_empty
```
Sets: last_operation_succeeded

#### delete_key
```eiffel
require
    subkey_not_empty: not a_subkey.is_empty
```
Sets: last_operation_succeeded

### 1.4 Query Operations

#### key_exists
```eiffel
require
    subkey_not_empty: not a_subkey.is_empty
```
Returns: BOOLEAN

#### value_exists
```eiffel
require
    subkey_not_empty: not a_subkey.is_empty
```
Returns: BOOLEAN

#### create_key
```eiffel
require
    subkey_not_empty: not a_subkey.is_empty
```
Sets: last_operation_succeeded

## 2. Status Attributes

| Attribute | Type | Description |
|-----------|------|-------------|
| last_operation_succeeded | BOOLEAN | Write/delete success |
| last_read_succeeded | BOOLEAN | Read success |
| last_error | detachable STRING_32 | Error message |
