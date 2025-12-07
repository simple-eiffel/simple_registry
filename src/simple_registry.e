note
	description: "[
		SCOOP-compatible Windows Registry access.
		Uses direct Win32 API calls via C wrapper.
	]"
	author: "Larry Rix"
	date: "$Date$"
	revision: "$Revision$"

class
	SIMPLE_REGISTRY

feature -- Predefined Keys

	HKEY_CLASSES_ROOT: POINTER
		once
			Result := c_hkey_classes_root
		end

	HKEY_CURRENT_USER: POINTER
		once
			Result := c_hkey_current_user
		end

	HKEY_LOCAL_MACHINE: POINTER
		once
			Result := c_hkey_local_machine
		end

	HKEY_USERS: POINTER
		once
			Result := c_hkey_users
		end

	HKEY_CURRENT_CONFIG: POINTER
		once
			Result := c_hkey_current_config
		end

feature -- Read Operations

	read_string (a_root: POINTER; a_subkey, a_value_name: READABLE_STRING_GENERAL): detachable STRING_32
			-- Read string value from registry.
		require
			subkey_not_empty: not a_subkey.is_empty
		local
			l_subkey, l_name: C_STRING
			l_result: POINTER
		do
			create l_subkey.make (a_subkey.to_string_8)
			create l_name.make (a_value_name.to_string_8)
			l_result := c_sr_read_string (a_root, l_subkey.item, l_name.item)
			if l_result /= default_pointer then
				if c_sr_result_success (l_result) /= 0 then
					Result := pointer_to_string (c_sr_result_string (l_result))
				else
					last_error := pointer_to_string (c_sr_result_error (l_result))
				end
				c_sr_free_result (l_result)
			end
		end

	read_integer (a_root: POINTER; a_subkey, a_value_name: READABLE_STRING_GENERAL): INTEGER
			-- Read DWORD value from registry.
		require
			subkey_not_empty: not a_subkey.is_empty
		local
			l_subkey, l_name: C_STRING
			l_result: POINTER
		do
			create l_subkey.make (a_subkey.to_string_8)
			create l_name.make (a_value_name.to_string_8)
			l_result := c_sr_read_dword (a_root, l_subkey.item, l_name.item)
			if l_result /= default_pointer then
				if c_sr_result_success (l_result) /= 0 then
					Result := c_sr_result_dword (l_result).to_integer_32
					last_read_succeeded := True
				else
					last_error := pointer_to_string (c_sr_result_error (l_result))
					last_read_succeeded := False
				end
				c_sr_free_result (l_result)
			end
		end

feature -- Write Operations

	write_string (a_root: POINTER; a_subkey, a_value_name, a_value: READABLE_STRING_GENERAL)
			-- Write string value to registry.
		require
			subkey_not_empty: not a_subkey.is_empty
		local
			l_subkey, l_name, l_value: C_STRING
		do
			create l_subkey.make (a_subkey.to_string_8)
			create l_name.make (a_value_name.to_string_8)
			create l_value.make (a_value.to_string_8)
			last_operation_succeeded := c_sr_write_string (a_root, l_subkey.item, l_name.item, l_value.item) /= 0
		end

	write_integer (a_root: POINTER; a_subkey, a_value_name: READABLE_STRING_GENERAL; a_value: INTEGER)
			-- Write DWORD value to registry.
		require
			subkey_not_empty: not a_subkey.is_empty
		local
			l_subkey, l_name: C_STRING
		do
			create l_subkey.make (a_subkey.to_string_8)
			create l_name.make (a_value_name.to_string_8)
			last_operation_succeeded := c_sr_write_dword (a_root, l_subkey.item, l_name.item, a_value.to_natural_32) /= 0
		end

feature -- Delete Operations

	delete_value (a_root: POINTER; a_subkey, a_value_name: READABLE_STRING_GENERAL)
			-- Delete a registry value.
		require
			subkey_not_empty: not a_subkey.is_empty
		local
			l_subkey, l_name: C_STRING
		do
			create l_subkey.make (a_subkey.to_string_8)
			create l_name.make (a_value_name.to_string_8)
			last_operation_succeeded := c_sr_delete_value (a_root, l_subkey.item, l_name.item) /= 0
		end

	delete_key (a_root: POINTER; a_subkey: READABLE_STRING_GENERAL)
			-- Delete a registry key (must be empty).
		require
			subkey_not_empty: not a_subkey.is_empty
		local
			l_subkey: C_STRING
		do
			create l_subkey.make (a_subkey.to_string_8)
			last_operation_succeeded := c_sr_delete_key (a_root, l_subkey.item) /= 0
		end

feature -- Query Operations

	key_exists (a_root: POINTER; a_subkey: READABLE_STRING_GENERAL): BOOLEAN
			-- Does the registry key exist?
		require
			subkey_not_empty: not a_subkey.is_empty
		local
			l_subkey: C_STRING
		do
			create l_subkey.make (a_subkey.to_string_8)
			Result := c_sr_key_exists (a_root, l_subkey.item) /= 0
		end

	value_exists (a_root: POINTER; a_subkey, a_value_name: READABLE_STRING_GENERAL): BOOLEAN
			-- Does the registry value exist?
		require
			subkey_not_empty: not a_subkey.is_empty
		local
			l_subkey, l_name: C_STRING
		do
			create l_subkey.make (a_subkey.to_string_8)
			create l_name.make (a_value_name.to_string_8)
			Result := c_sr_value_exists (a_root, l_subkey.item, l_name.item) /= 0
		end

	create_key (a_root: POINTER; a_subkey: READABLE_STRING_GENERAL)
			-- Create a registry key.
		require
			subkey_not_empty: not a_subkey.is_empty
		local
			l_subkey: C_STRING
		do
			create l_subkey.make (a_subkey.to_string_8)
			last_operation_succeeded := c_sr_create_key (a_root, l_subkey.item) /= 0
		end

feature -- Status

	last_operation_succeeded: BOOLEAN
			-- Did the last write/delete operation succeed?

	last_read_succeeded: BOOLEAN
			-- Did the last read operation succeed?

	last_error: detachable STRING_32
			-- Error message from last failed operation.

feature {NONE} -- Implementation

	pointer_to_string (a_ptr: POINTER): STRING_32
			-- Convert C string pointer to STRING_32.
		local
			l_c_string: C_STRING
		do
			if a_ptr /= default_pointer then
				create l_c_string.make_by_pointer (a_ptr)
				Result := l_c_string.string.to_string_32
			else
				create Result.make_empty
			end
		end

feature {NONE} -- C externals: Predefined keys

	c_hkey_classes_root: POINTER
		external "C macro use <windows.h>"
		alias "HKEY_CLASSES_ROOT"
		end

	c_hkey_current_user: POINTER
		external "C macro use <windows.h>"
		alias "HKEY_CURRENT_USER"
		end

	c_hkey_local_machine: POINTER
		external "C macro use <windows.h>"
		alias "HKEY_LOCAL_MACHINE"
		end

	c_hkey_users: POINTER
		external "C macro use <windows.h>"
		alias "HKEY_USERS"
		end

	c_hkey_current_config: POINTER
		external "C macro use <windows.h>"
		alias "HKEY_CURRENT_CONFIG"
		end

feature {NONE} -- C externals: Operations

	c_sr_read_string (a_root: POINTER; a_subkey, a_name: POINTER): POINTER
		external
			"C inline use %"simple_registry.h%""
		alias
			"return sr_read_string((HKEY)$a_root, (const char*)$a_subkey, (const char*)$a_name);"
		end

	c_sr_read_dword (a_root: POINTER; a_subkey, a_name: POINTER): POINTER
		external
			"C inline use %"simple_registry.h%""
		alias
			"return sr_read_dword((HKEY)$a_root, (const char*)$a_subkey, (const char*)$a_name);"
		end

	c_sr_write_string (a_root: POINTER; a_subkey, a_name, a_value: POINTER): INTEGER
		external
			"C inline use %"simple_registry.h%""
		alias
			"return sr_write_string((HKEY)$a_root, (const char*)$a_subkey, (const char*)$a_name, (const char*)$a_value);"
		end

	c_sr_write_dword (a_root: POINTER; a_subkey, a_name: POINTER; a_value: NATURAL_32): INTEGER
		external
			"C inline use %"simple_registry.h%""
		alias
			"return sr_write_dword((HKEY)$a_root, (const char*)$a_subkey, (const char*)$a_name, (unsigned long)$a_value);"
		end

	c_sr_delete_value (a_root: POINTER; a_subkey, a_name: POINTER): INTEGER
		external
			"C inline use %"simple_registry.h%""
		alias
			"return sr_delete_value((HKEY)$a_root, (const char*)$a_subkey, (const char*)$a_name);"
		end

	c_sr_delete_key (a_root: POINTER; a_subkey: POINTER): INTEGER
		external
			"C inline use %"simple_registry.h%""
		alias
			"return sr_delete_key((HKEY)$a_root, (const char*)$a_subkey);"
		end

	c_sr_key_exists (a_root: POINTER; a_subkey: POINTER): INTEGER
		external
			"C inline use %"simple_registry.h%""
		alias
			"return sr_key_exists((HKEY)$a_root, (const char*)$a_subkey);"
		end

	c_sr_value_exists (a_root: POINTER; a_subkey, a_name: POINTER): INTEGER
		external
			"C inline use %"simple_registry.h%""
		alias
			"return sr_value_exists((HKEY)$a_root, (const char*)$a_subkey, (const char*)$a_name);"
		end

	c_sr_create_key (a_root: POINTER; a_subkey: POINTER): INTEGER
		external
			"C inline use %"simple_registry.h%""
		alias
			"return sr_create_key((HKEY)$a_root, (const char*)$a_subkey);"
		end

	c_sr_free_result (a_result: POINTER)
		external
			"C inline use %"simple_registry.h%""
		alias
			"sr_free_result((sr_result*)$a_result);"
		end

	c_sr_result_success (a_result: POINTER): INTEGER
		external
			"C inline use %"simple_registry.h%""
		alias
			"return ((sr_result*)$a_result)->success;"
		end

	c_sr_result_string (a_result: POINTER): POINTER
		external
			"C inline use %"simple_registry.h%""
		alias
			"return ((sr_result*)$a_result)->string_value;"
		end

	c_sr_result_dword (a_result: POINTER): NATURAL_32
		external
			"C inline use %"simple_registry.h%""
		alias
			"return ((sr_result*)$a_result)->dword_value;"
		end

	c_sr_result_error (a_result: POINTER): POINTER
		external
			"C inline use %"simple_registry.h%""
		alias
			"return ((sr_result*)$a_result)->error_message;"
		end

end
