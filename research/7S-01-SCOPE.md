# 7S-01: SCOPE - simple_registry


**Date**: 2026-01-23

**BACKWASH DOCUMENT** - Generated: 2026-01-23
**Status**: Reverse-engineered from existing implementation

## 1. Problem Domain

### 1.1 What Problem Does This Library Solve?
SIMPLE_REGISTRY provides SCOOP-compatible Windows Registry access for Eiffel applications. It enables reading, writing, and managing registry keys and values through direct Win32 API calls.

### 1.2 Who Needs This?
- Windows application developers
- Configuration management systems
- Installation/setup programs
- System integration applications
- Applications needing persistent settings

### 1.3 What Exists Already?
- WEL (Windows Eiffel Library) has registry support
- No lightweight, SCOOP-compatible alternative

## 2. Scope Definition

### 2.1 IN Scope
- Read string values (REG_SZ)
- Read DWORD values (REG_DWORD)
- Write string values
- Write DWORD values
- Delete values
- Delete keys
- Check key/value existence
- Create keys
- Predefined root keys (HKEY_*)

### 2.2 OUT of Scope
- Multi-string values (REG_MULTI_SZ)
- Binary values (REG_BINARY)
- Expandable string values (REG_EXPAND_SZ)
- Registry enumeration
- Security descriptors
- Remote registry access
- Registry notifications

## 3. Success Criteria

- Clean compilation with EiffelStudio
- Successful read/write of string and DWORD values
- Proper error handling
- SCOOP compatibility
- No memory leaks from C wrapper
