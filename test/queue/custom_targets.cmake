######################################################################
# PROJECT CUSTOM TARGETS
# Predefined targets for all team members
# Includes: unit tests, code complexity, static analysis, coverage, formatting
######################################################################

# -------------------------
# Unit Tests Target
# -------------------------
message(STATUS "To run Unit Tests, use target: run")
add_custom_target(run
    COMMAND QUEUE_test
    COMMENT "Running all unit tests"
)

# -------------------------
# Code Complexity Metrics (Lizard)
# -------------------------
find_program(LIZARD_PROGRAM lizard)
if(LIZARD_PROGRAM)
    message(STATUS "Lizard found. Targets: ccm (console), ccmr (report)")
else()
    message(STATUS "Lizard not found. Install Lizard to enable Code Complexity Metrics targets.")
endif()

# Print CCM in console
add_custom_target(ccm
    COMMAND ${LIZARD_PROGRAM} ../../../lib/queue
            --CCN 12 -Tnloc=30
            -a 4
            --languages cpp
            -V
            -i 1
    COMMENT "Code Complexity Metrics (CCM) printed to console"
)

# Generate CCM HTML report
add_custom_target(ccmr
    COMMAND ${CMAKE_COMMAND} -E make_directory ../../../reports/CCM/
    COMMAND ${LIZARD_PROGRAM} ../../../lib/queue
            --CCN 12
            -Tnloc=30
            -a 4
            --languages cpp
            -V
            -o ../../../reports/CCM/queue.html
    COMMENT "Generating Code Complexity Metrics HTML report"
)

# -------------------------
# Static Analysis (CppCheck)
# -------------------------
find_program(CPPCHECK_PROGRAM cppcheck)
if(CPPCHECK_PROGRAM)
    message(STATUS "CppCheck found. Targets: cppcheck, cppcheck_html_report")
else()
    message(STATUS "CppCheck not found. Install to enable static analysis targets.")
endif()

# Directory for cppcheck reports
set(CPPCHECK_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/cppcheck)
file(MAKE_DIRECTORY ${CPPCHECK_OUTPUT_DIR})

# Run cppcheck and generate XML
add_custom_target(cppcheck
    COMMAND ${CPPCHECK_PROGRAM} 
			../../../lib/queue 
			../../../test/queue
            -i../../../test/queue/out
            --enable=all
            --force
            --inconclusive
            --std=c99
            --suppress=missingIncludeSystem
            --suppress=missingInclude
			--checkers-report=checkers.xml
            --xml 2> ${CPPCHECK_OUTPUT_DIR}/cppcheck_report.xml
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running CppCheck and generating XML report"
    VERBATIM
)

# Generate HTML report from cppcheck XML
add_custom_target(cppcheck_html_report
    COMMAND cppcheck-htmlreport
            --file=${CPPCHECK_OUTPUT_DIR}/cppcheck_report.xml
            --report-dir=${CPPCHECK_OUTPUT_DIR}/html
            --source-dir=../../../lib/queue
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating CppCheck HTML report"
    VERBATIM
)
add_dependencies(cppcheck_html_report cppcheck)

# -------------------------
# Code Coverage (gcovr)
# -------------------------
find_program(GCOVR gcovr)
if(GCOVR)
    message(STATUS "gcovr found. Targets: ccc, ccr, ccca, ccra")
else()
    message(STATUS "gcovr not found. Install to enable code coverage targets.")
endif()

# Create report directories
add_custom_command(
    OUTPUT ../../../reports/CCR/ ../../../reports/CCR/JSON_ALL/
    COMMAND ${CMAKE_COMMAND} -E make_directory ../../../reports/CCR/
    COMMAND ${CMAKE_COMMAND} -E make_directory ../../../reports/CCR/JSON_ALL/
    COMMENT "Creating code coverage report directories"
)

# Unit test coverage HTML + JSON
add_custom_target(ccr
    COMMAND ${CMAKE_COMMAND} -E make_directory ../../../reports/CCR/
    COMMAND ${CMAKE_COMMAND} -E make_directory ../../../reports/CCR/JSON_ALL/
    COMMAND gcovr -r ../../../lib/queue
            --json ../../../reports/CCR/JSON_ALL/coverage_queue.json
            --json-base lib/queue
            --html-details ../../../reports/CCR/queue_report.html
            --html-theme github.dark-green
    COMMENT "Generating code coverage report for queue library"
)

# Unit test coverage check (fail-under-line)
add_custom_target(ccc
    COMMAND gcovr -r ../../../lib/queue --fail-under-line 90 .
    COMMENT "Checking code coverage (fail under 90%)"
)

# Whole project coverage JSON aggregation
add_custom_target(ccca
    COMMAND gcovr 
			-r ../../../ 
			--json-add-tracefile ../../../reports/CCR/JSON_ALL/coverage_*.json .
    COMMENT "Aggregating JSON coverage for whole project"
)
add_dependencies(ccca ccr)

# Whole project coverage HTML aggregation
add_custom_target(ccra
    COMMAND ${CMAKE_COMMAND} -E make_directory ../../../reports/CCR/
    COMMAND ${CMAKE_COMMAND} -E make_directory ../../../reports/CCR/JSON_ALL/HTML_OUT
    COMMAND gcovr 
			-r ../../../ 
			--json-add-tracefile ../../../reports/CCR/JSON_ALL/coverage_*.json
            --html-details -o ../../../reports/CCR/JSON_ALL/HTML_OUT/project_coverage.html
            --html-theme github.dark-green
    COMMENT "Aggregating HTML coverage for whole project"
)
add_dependencies(ccra ccr)

# -------------------------
# Code Formatting (clang-format)
# -------------------------
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    message(STATUS "clang-format found. Targets: format, format_test")
else()
    message(STATUS "clang-format not found. Install to enable code formatting targets.")
endif()

# Format library sources
add_custom_target(format
    COMMAND clang-format -i -style=file ../../../lib/queue/*.c ../../../lib/queue/*.h
    COMMENT "Formatting library source code"
)

# Format test sources
add_custom_target(format_test
    COMMAND clang-format -i -style=file ../*.c
    COMMENT "Formatting test source code"
)
