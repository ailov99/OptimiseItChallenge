local Matrix = require "matrix_utils"

function correlate_basic_test()
    print("=== TEST correlate_basic_test ===")
    local m_4x4 = Matrix:new("./test_data/m4x4.txt")
    local m_4x4_correlated = Matrix:new("./test_data/m4x4_correlated.txt")
    local m_30x30 = Matrix:new("./test_data/m30x30.txt")
    local m_30x30_correlated = Matrix:new("./test_data/m30x30_correlated.txt")

    toCppSetCorrelationModeBasic()
    local cpp_4x4_correlated_data = toCppCorrelateMatrix(m_4x4.rows, m_4x4.cols, m_4x4.data)
    local cpp_4x4_correlated_matrix = Matrix:new()
    cpp_4x4_correlated_matrix:set(m_4x4.rows, m_4x4.cols, cpp_4x4_correlated_data)
    local cpp_30x30_correlated_data = toCppCorrelateMatrix(m_30x30.rows, m_30x30.cols, m_30x30.data)
    local cpp_30x30_correlated_matrix = Matrix:new()
    cpp_30x30_correlated_matrix:set(m_30x30.rows, m_30x30.cols, cpp_30x30_correlated_data)

    assert(
        cpp_4x4_correlated_matrix:approxEq(m_4x4_correlated, 6), 
        "correlate_basic 4x4 failed"
    )
    print("=== PASS ===")
    assert(
        cpp_30x30_correlated_matrix:approxEq(m_30x30_correlated, 6),
        "correlate_basic 30x30 failed"
    )
    print("=== PASS ===")
end

function correlate_ilp_test()
    print("=== TEST correlate_ilp_test ===")
    local m_4x4 = Matrix:new("./test_data/m4x4.txt")
    local m_4x4_correlated = Matrix:new("./test_data/m4x4_correlated.txt")
    local m_30x30 = Matrix:new("./test_data/m30x30.txt")
    local m_30x30_correlated = Matrix:new("./test_data/m30x30_correlated.txt")

    toCppSetCorrelationModeILP()
    local cpp_4x4_correlated_data = toCppCorrelateMatrix(m_4x4.rows, m_4x4.cols, m_4x4.data)
    local cpp_4x4_correlated_matrix = Matrix:new()
    cpp_4x4_correlated_matrix:set(m_4x4.rows, m_4x4.cols, cpp_4x4_correlated_data)
    local cpp_30x30_correlated_data = toCppCorrelateMatrix(m_30x30.rows, m_30x30.cols, m_30x30.data)
    local cpp_30x30_correlated_matrix = Matrix:new()
    cpp_30x30_correlated_matrix:set(m_30x30.rows, m_30x30.cols, cpp_30x30_correlated_data)

    assert(
        cpp_4x4_correlated_matrix:approxEq(m_4x4_correlated, 6), 
        "correlate_ilp 4x4 failed"
    )
    print("=== PASS ===")
    assert(
        cpp_30x30_correlated_matrix:approxEq(m_30x30_correlated, 6),
        "correlate_ilp 30x30 failed"
    )
    print("=== PASS ===")
end

function correlate_parallel_test()
    print("=== TEST correlate_parallel_test ===")
    local m_4x4 = Matrix:new("./test_data/m4x4.txt")
    local m_4x4_correlated = Matrix:new("./test_data/m4x4_correlated.txt")
    local m_30x30 = Matrix:new("./test_data/m30x30.txt")
    local m_30x30_correlated = Matrix:new("./test_data/m30x30_correlated.txt")

    toCppSetCorrelationModeParallel()
    local cpp_4x4_correlated_data = toCppCorrelateMatrix(m_4x4.rows, m_4x4.cols, m_4x4.data)
    local cpp_4x4_correlated_matrix = Matrix:new()
    cpp_4x4_correlated_matrix:set(m_4x4.rows, m_4x4.cols, cpp_4x4_correlated_data)
    local cpp_30x30_correlated_data = toCppCorrelateMatrix(m_30x30.rows, m_30x30.cols, m_30x30.data)
    local cpp_30x30_correlated_matrix = Matrix:new()
    cpp_30x30_correlated_matrix:set(m_30x30.rows, m_30x30.cols, cpp_30x30_correlated_data)

    assert(
        cpp_4x4_correlated_matrix:approxEq(m_4x4_correlated, 6), 
        "correlate_parallel 4x4 failed"
    )
    print("=== PASS ===")
    assert(
        cpp_30x30_correlated_matrix:approxEq(m_30x30_correlated, 6),
        "correlate_parallel 30x30 failed"
    )
    print("=== PASS ===")
end

function correlate_vectorised_test()
    print("=== TEST correlate_vectorised_test ===")
    local m_4x4 = Matrix:new("./test_data/m4x4.txt")
    local m_4x4_correlated = Matrix:new("./test_data/m4x4_correlated.txt")
    local m_30x30 = Matrix:new("./test_data/m30x30.txt")
    local m_30x30_correlated = Matrix:new("./test_data/m30x30_correlated.txt")

    toCppSetCorrelationModeVectorised()
    local cpp_4x4_correlated_data = toCppCorrelateMatrix(m_4x4.rows, m_4x4.cols, m_4x4.data)
    local cpp_4x4_correlated_matrix = Matrix:new()
    cpp_4x4_correlated_matrix:set(m_4x4.rows, m_4x4.cols, cpp_4x4_correlated_data)
    local cpp_30x30_correlated_data = toCppCorrelateMatrix(m_30x30.rows, m_30x30.cols, m_30x30.data)
    local cpp_30x30_correlated_matrix = Matrix:new()
    cpp_30x30_correlated_matrix:set(m_30x30.rows, m_30x30.cols, cpp_30x30_correlated_data)

    assert(
        cpp_4x4_correlated_matrix:approxEq(m_4x4_correlated, 6), 
        "correlate_vectorised 4x4 failed"
    )
    print("=== PASS ===")
    assert(
        cpp_30x30_correlated_matrix:approxEq(m_30x30_correlated, 6),
        "correlate_vectorised 30x30 failed"
    )
    print("=== PASS ===")
end

function correlate_max_opt_double_precision_test()
    print("=== TEST correlate_max_opt_double_precision_test ===")
    local m_4x4 = Matrix:new("./test_data/m4x4.txt")
    local m_4x4_correlated = Matrix:new("./test_data/m4x4_correlated.txt")
    local m_30x30 = Matrix:new("./test_data/m30x30.txt")
    local m_30x30_correlated = Matrix:new("./test_data/m30x30_correlated.txt")

    toCppSetCorrelationModeMaxOptDPrec()
    local cpp_4x4_correlated_data = toCppCorrelateMatrix(m_4x4.rows, m_4x4.cols, m_4x4.data)
    local cpp_4x4_correlated_matrix = Matrix:new()
    cpp_4x4_correlated_matrix:set(m_4x4.rows, m_4x4.cols, cpp_4x4_correlated_data)
    local cpp_30x30_correlated_data = toCppCorrelateMatrix(m_30x30.rows, m_30x30.cols, m_30x30.data)
    local cpp_30x30_correlated_matrix = Matrix:new()
    cpp_30x30_correlated_matrix:set(m_30x30.rows, m_30x30.cols, cpp_30x30_correlated_data)

    assert(
        cpp_4x4_correlated_matrix:approxEq(m_4x4_correlated, 6), 
        "correlate_max_opt_double_precision 4x4 failed"
    )
    print("=== PASS ===")
    assert(
        cpp_30x30_correlated_matrix:approxEq(m_30x30_correlated, 6),
        "correlate_max_opt_double_precision 30x30 failed"
    )
    print("=== PASS ===")
end

function correlate_max_opt_single_precision_test()
    print("=== TEST correlate_max_opt_single_precision_test ===")
    local m_4x4 = Matrix:new("./test_data/m4x4.txt")
    local m_4x4_correlated = Matrix:new("./test_data/m4x4_correlated_floats.txt")
    local m_30x30 = Matrix:new("./test_data/m30x30.txt")
    local m_30x30_correlated = Matrix:new("./test_data/m30x30_correlated_floats.txt")

    toCppSetCorrelationModeMaxOptSPrec()
    local cpp_4x4_correlated_data = toCppCorrelateMatrix(m_4x4.rows, m_4x4.cols, m_4x4.data)
    local cpp_4x4_correlated_matrix = Matrix:new()
    cpp_4x4_correlated_matrix:set(m_4x4.rows, m_4x4.cols, cpp_4x4_correlated_data)
    local cpp_30x30_correlated_data = toCppCorrelateMatrix(m_30x30.rows, m_30x30.cols, m_30x30.data)
    local cpp_30x30_correlated_matrix = Matrix:new()
    cpp_30x30_correlated_matrix:set(m_30x30.rows, m_30x30.cols, cpp_30x30_correlated_data)

    assert(
        cpp_4x4_correlated_matrix:approxEq(m_4x4_correlated, 6), 
        "correlate_max_opt_single_precision 4x4 failed"
    )
    print("=== PASS ===")
    assert(
        cpp_30x30_correlated_matrix:approxEq(m_30x30_correlated, 6),
        "correlate_max_opt_single_precision 30x30 failed"
    )
    print("=== PASS ===")
end

function run_all_correlation_tests()
    correlate_basic_test()
    correlate_ilp_test()
    correlate_parallel_test()
    correlate_vectorised_test()
    correlate_max_opt_double_precision_test()
    correlate_max_opt_single_precision_test()
end