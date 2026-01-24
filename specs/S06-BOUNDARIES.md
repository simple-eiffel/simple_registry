# S06: BOUNDARIES - simple_registry

**BACKWASH DOCUMENT** - Generated: 2026-01-23
**Status**: Reverse-engineered from existing implementation

## 1. API Boundaries

### 1.1 Public Interface
- Predefined keys (5 POINTERs)
- Read operations (2)
- Write operations (2)
- Delete operations (2)
- Query operations (3)
- Status attributes (3)

### 1.2 Private Features
| Feature | Visibility | Purpose |
|---------|------------|---------|
| pointer_to_string | {NONE} | C string conversion |
| c_hkey_* | {NONE} | Windows macros |
| c_sr_* | {NONE} | C wrapper calls |

## 2. Input Boundaries

### 2.1 Root Key
| Constraint | Validation |
|------------|------------|
| Must be valid HKEY | Use predefined constants |

### 2.2 Subkey Path
| Constraint | Validation |
|------------|------------|
| Not empty | Precondition |
| Valid path | Registry validates |
| Max length | ~255 characters |

### 2.3 Value Name
| Constraint | Validation |
|------------|------------|
| Can be empty | Default value |
| Valid name | Registry validates |

### 2.4 Values
| Type | Range |
|------|-------|
| String | Any (ASCII converted) |
| Integer | INTEGER range |

## 3. Output Boundaries

### 3.1 Read Results
| Operation | Success | Failure |
|-----------|---------|---------|
| read_string | STRING_32 | Void |
| read_integer | INTEGER | 0 + last_read_succeeded=False |

### 3.2 Status
- last_operation_succeeded: BOOLEAN
- last_read_succeeded: BOOLEAN
- last_error: STRING_32 or Void

## 4. Error Boundaries

### 4.1 Common Errors
- Key not found
- Value not found
- Access denied
- Invalid parameter
