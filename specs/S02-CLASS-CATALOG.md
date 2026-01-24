# S02: CLASS CATALOG - simple_registry

**BACKWASH DOCUMENT** - Generated: 2026-01-23
**Status**: Reverse-engineered from existing implementation

## 1. Class Overview

| Class | Type | Purpose |
|-------|------|---------|
| SIMPLE_REGISTRY | Effective | Windows Registry access |

## 2. Class Details

### 2.1 SIMPLE_REGISTRY

**Purpose**: SCOOP-compatible Windows Registry access.

**Creation**: Default create (no special initialization)

**Feature Groups**:

1. **Predefined Keys**
   - HKEY_CLASSES_ROOT
   - HKEY_CURRENT_USER
   - HKEY_LOCAL_MACHINE
   - HKEY_USERS
   - HKEY_CURRENT_CONFIG

2. **Read Operations**
   - read_string
   - read_integer

3. **Write Operations**
   - write_string
   - write_integer

4. **Delete Operations**
   - delete_value
   - delete_key

5. **Query Operations**
   - key_exists
   - value_exists
   - create_key

6. **Status**
   - last_operation_succeeded
   - last_read_succeeded
   - last_error

7. **Implementation**
   - pointer_to_string (private)
   - C externals (private)

## 3. C Wrapper Functions

| Function | Purpose |
|----------|---------|
| sr_read_string | Read REG_SZ value |
| sr_read_dword | Read REG_DWORD value |
| sr_write_string | Write REG_SZ value |
| sr_write_dword | Write REG_DWORD value |
| sr_delete_value | Delete registry value |
| sr_delete_key | Delete registry key |
| sr_key_exists | Check key existence |
| sr_value_exists | Check value existence |
| sr_create_key | Create registry key |
| sr_free_result | Free result structure |
| sr_result_* | Result field accessors |
