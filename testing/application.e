note
	description: "Test application for simple_registry"
	date: "$Date$"
	revision: "$Revision$"

class
	APPLICATION

create
	make

feature -- Initialization

	make
			-- Run tests.
		local
			l_tests: TEST_SIMPLE_REGISTRY
			l_passed, l_failed: INTEGER
		do
			print ("Testing SIMPLE_REGISTRY...%N%N")

			create l_tests

			-- Test: HKEY constants
			print ("  test_hkey_constants_not_null: ")
			l_tests.on_prepare
			l_tests.test_hkey_constants_not_null
			l_tests.on_clean
			print ("PASSED%N")
			l_passed := l_passed + 1

			-- Test: Known key exists
			print ("  test_known_key_exists: ")
			l_tests.on_prepare
			l_tests.test_known_key_exists
			l_tests.on_clean
			print ("PASSED%N")
			l_passed := l_passed + 1

			-- Test: Nonexistent key
			print ("  test_nonexistent_key: ")
			l_tests.on_prepare
			l_tests.test_nonexistent_key
			l_tests.on_clean
			print ("PASSED%N")
			l_passed := l_passed + 1

			-- Test: Create and delete key
			print ("  test_create_and_delete_key: ")
			l_tests.on_prepare
			l_tests.test_create_and_delete_key
			l_tests.on_clean
			print ("PASSED%N")
			l_passed := l_passed + 1

			-- Test: Write and read string
			print ("  test_write_and_read_string: ")
			l_tests.on_prepare
			l_tests.test_write_and_read_string
			l_tests.on_clean
			print ("PASSED%N")
			l_passed := l_passed + 1

			-- Test: Write and read integer
			print ("  test_write_and_read_integer: ")
			l_tests.on_prepare
			l_tests.test_write_and_read_integer
			l_tests.on_clean
			print ("PASSED%N")
			l_passed := l_passed + 1

			-- Test: Value exists
			print ("  test_value_exists: ")
			l_tests.on_prepare
			l_tests.test_value_exists
			l_tests.on_clean
			print ("PASSED%N")
			l_passed := l_passed + 1

			-- Test: Delete value
			print ("  test_delete_value: ")
			l_tests.on_prepare
			l_tests.test_delete_value
			l_tests.on_clean
			print ("PASSED%N")
			l_passed := l_passed + 1

			-- Test: Read system string
			print ("  test_read_system_string: ")
			l_tests.on_prepare
			l_tests.test_read_system_string
			l_tests.on_clean
			print ("PASSED%N")
			l_passed := l_passed + 1

			print ("%N======================================%N")
			print ("Results: " + l_passed.out + " passed, " + l_failed.out + " failed%N")
		rescue
			print ("FAILED%N")
			l_failed := l_failed + 1
			retry
		end

end
