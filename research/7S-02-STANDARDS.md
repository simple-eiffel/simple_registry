# 7S-02: STANDARDS - simple_registry

**BACKWASH DOCUMENT** - Generated: 2026-01-23
**Status**: Reverse-engineered from existing implementation

## 1. Applicable Standards

### 1.1 Windows Registry API
- Win32 API for registry operations
- Uses RegOpenKeyEx, RegQueryValueEx, RegSetValueEx, etc.
- Standard Windows error handling

### 1.2 Registry Key Hierarchy
| Root Key | Constant | Purpose |
|----------|----------|---------|
| HKEY_CLASSES_ROOT | HKCR | File associations |
| HKEY_CURRENT_USER | HKCU | User settings |
| HKEY_LOCAL_MACHINE | HKLM | Machine settings |
| HKEY_USERS | HKU | All user profiles |
| HKEY_CURRENT_CONFIG | HKCC | Hardware config |

### 1.3 Value Types
| Type | Code | Supported |
|------|------|-----------|
| REG_SZ | 1 | YES |
| REG_DWORD | 4 | YES |
| REG_BINARY | 3 | NO |
| REG_MULTI_SZ | 7 | NO |
| REG_EXPAND_SZ | 2 | NO |

## 2. Eiffel Standards

### 2.1 Design by Contract
- Preconditions on subkey/value names
- Status flags for operation results
- Error message capture

### 2.2 Inline C Pattern
- Uses C header file (simple_registry.h)
- Inline C for API calls
- Proper memory management

## 3. Simple Ecosystem Standards

### 3.1 C Integration
- External C wrapper for complex operations
- Result structures for multi-value returns
- Memory cleanup (sr_free_result)
