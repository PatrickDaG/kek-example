include_guard(GLOBAL)

include(CheckCXXCompilerFlag)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(CXXFLAGS_SANITIZE_FLAGS -fsanitize=address -fsanitize=undefined)

	set(CXXFLAGS_NORMAL_COMPILE_FLAGS -Wall -Wextra -Wold-style-cast -Wundef
		-Wcast-qual -Wredundant-decls -Wwrite-strings -Woverloaded-virtual
		-Wctor-dtor-privacy -Wnon-virtual-dtor -Wformat=2)
	set(CXXFLAGS_PEDANTIC_COMPILE_FLAGS ${CXXFLAGS_NORMAL_COMPILE_FLAGS}
		-pedantic-errors -pedantic -Weffc++
		-Wpointer-arith -Wmissing-include-dirs -Wcast-align
		-Wdisabled-optimization -Winvalid-pch)
	if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.6)
		set(CXXFLAGS_PEDANTIC_COMPILE_FLAGS ${CXXFLAGS_PEDANTIC_COMPILE_FLAGS} -Wnoexcept)
	endif()
	if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
		set(CXXFLAGS_PEDANTIC_COMPILE_FLAGS ${CXXFLAGS_PEDANTIC_COMPILE_FLAGS} -Wdouble-promotion
			-Wtrampolines -Wzero-as-null-pointer-constant -Wuseless-cast
			-Wvector-operation-performance -Wsized-deallocation)
	endif()
	if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.0)
		set(CXXFLAGS_PEDANTIC_COMPILE_FLAGS ${CXXFLAGS_PEDANTIC_COMPILE_FLAGS} -Wshift-overflow=2
			-Wnull-dereference -Wduplicated-cond)
	endif()

	set(CXXFLAGS_WERROR_FLAG -Werror)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CXXFLAGS_SANITIZE_FLAGS -fsanitize=address -fsanitize=undefined)

	set(CXXFLAGS_NORMAL_COMPILE_FLAGS -Wall -Wextra)
	set(CXXFLAGS_PEDANTIC_COMPILE_FLAGS -Weverything
		-Wno-unused-exception-parameter -Wno-missing-braces -Wno-padded
		-Wno-missing-noreturn -Wno-gnu-statement-expression -Wno-c++98-compat
		-Wno-weak-vtables -Wno-c++98-compat-pedantic -Wno-reserved-id-macro
		-Wno-sign-conversion -Wno-global-constructors -Wno-missing-prototypes
		-Wno-disabled-macro-expansion -Wno-documentation-unknown-command
		-Wno-unused-member-function -Wno-undefined-func-template -Wno-shadow
		-Wno-used-but-marked-unused -Wno-covered-switch-default
		-Wno-missing-variable-declarations -Wno-double-promotion
		-Wno-exit-time-destructors -Wno-ctad-maybe-unsupported)

	set(CXXFLAGS_WERROR_FLAG -Werror)

	check_cxx_compiler_flag(-Wzero-as-null-pointer-constant HAS_NULLPTR_WARNING)
	if(HAS_NULLPTR_WARNING)
		set(CXXFLAGS_PEDANTIC_COMPILE_FLAGS ${CXXFLAGS_PEDANTIC_COMPILE_FLAGS}
			-Wzero-as-null-pointer-constant)
	endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(CXXFLAGS_NORMAL_COMPILE_FLAGS /W3)
	set(CXXFLAGS_PEDANTIC_COMPILE_FLAGS /W3)
	set(CXXFLAGS_WERROR_FLAG /WX)
endif()
