# S05: CONSTRAINTS - simple_registry

**BACKWASH DOCUMENT** - Generated: 2026-01-23
**Status**: Reverse-engineered from existing implementation

## 1. Platform Constraints

### 1.1 Windows Only
- Requires Windows operating system
- Uses Win32 API (windows.h)
- Not portable to other platforms

### 1.2 Build Requirements
- Windows SDK
- C compiler for inline externals
- simple_registry.h in include path

## 2. Functional Constraints

### 2.1 Value Type Limitations
| Type | Status |
|------|--------|
| REG_SZ (string) | Supported |
| REG_DWORD (integer) | Supported |
| REG_BINARY | Not supported |
| REG_MULTI_SZ | Not supported |
| REG_EXPAND_SZ | Not supported |
| REG_QWORD | Not supported |

### 2.2 Operation Limitations
- No enumeration of keys/values
- No security descriptor access
- No remote registry support
- delete_key requires empty key

## 3. Security Constraints

### 3.1 Permission Requirements
| Root Key | Read | Write |
|----------|------|-------|
| HKEY_CURRENT_USER | User | User |
| HKEY_LOCAL_MACHINE | User | Admin |
| HKEY_CLASSES_ROOT | User | Varies |
| HKEY_USERS | Admin | Admin |

### 3.2 Error Handling
- Access denied returns failure status
- Error message captured in last_error
- Operations should check success status

## 4. String Constraints

### 4.1 Encoding
- Strings converted to ASCII (to_string_8)
- Unicode values may be truncated
- Consider Unicode-aware version for Phase 2
