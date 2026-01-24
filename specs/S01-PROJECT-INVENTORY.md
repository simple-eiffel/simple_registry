# S01: PROJECT INVENTORY - simple_registry

**BACKWASH DOCUMENT** - Generated: 2026-01-23
**Status**: Reverse-engineered from existing implementation

## 1. Project Structure

```
simple_registry/
  src/
    simple_registry.e            # Main registry class
  c/
    simple_registry.h            # C wrapper implementation
  testing/
    lib_tests.e                  # Test suite
    test_app.e                   # Test runner
  research/
  specs/
  simple_registry.ecf
```

## 2. Source Files

| File | Purpose | Lines |
|------|---------|-------|
| simple_registry.e | Registry access class | ~330 |
| simple_registry.h | C wrapper for Win32 API | ~200 |

## 3. Test Files

| File | Purpose |
|------|---------|
| lib_tests.e | Test suite |
| test_app.e | Test runner |

## 4. Dependencies

### 4.1 External Libraries
| Library | Purpose |
|---------|---------|
| Windows SDK | windows.h for Win32 API |

### 4.2 EiffelBase Dependencies
- C_STRING: String marshalling
- POINTER: Handle management

## 5. Build Requirements

- Windows operating system
- Windows SDK in include path
- simple_registry.h accessible
- C compiler for inline externals
