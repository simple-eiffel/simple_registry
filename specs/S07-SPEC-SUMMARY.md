# S07: SPEC SUMMARY - simple_registry

**BACKWASH DOCUMENT** - Generated: 2026-01-23
**Status**: Reverse-engineered from existing implementation

## 1. Library Overview

**Name**: simple_registry
**Purpose**: Windows Registry access for Eiffel
**Status**: Production ready (basic functionality)

## 2. Quick Reference

### 2.1 Reading Values
```eiffel
registry: SIMPLE_REGISTRY
create registry

-- Read string
if attached registry.read_string (registry.HKEY_CURRENT_USER,
    "Software\MyApp", "Setting") as value
then
    print (value)
end

-- Read integer
count := registry.read_integer (registry.HKEY_CURRENT_USER,
    "Software\MyApp", "Count")
if registry.last_read_succeeded then
    print (count)
end
```

### 2.2 Writing Values
```eiffel
registry.write_string (registry.HKEY_CURRENT_USER,
    "Software\MyApp", "Setting", "Value")
if registry.last_operation_succeeded then
    print ("Written")
end

registry.write_integer (registry.HKEY_CURRENT_USER,
    "Software\MyApp", "Count", 42)
```

### 2.3 Checking Existence
```eiffel
if registry.key_exists (registry.HKEY_CURRENT_USER, "Software\MyApp") then
    ...
end

if registry.value_exists (registry.HKEY_CURRENT_USER,
    "Software\MyApp", "Setting") then
    ...
end
```

### 2.4 Creating/Deleting
```eiffel
registry.create_key (registry.HKEY_CURRENT_USER, "Software\MyApp")
registry.delete_value (registry.HKEY_CURRENT_USER, "Software\MyApp", "Old")
registry.delete_key (registry.HKEY_CURRENT_USER, "Software\MyApp\Temp")
```

## 3. Key Specifications

| Aspect | Specification |
|--------|---------------|
| Classes | 1 |
| Platform | Windows only |
| Value Types | String, DWORD |
| Dependencies | Windows SDK |

## 4. Warnings

1. **Windows only** - Not portable
2. **Admin rights** for HKLM writes
3. **ASCII conversion** - Unicode may be truncated
4. **delete_key** requires empty key
