local Matrix = require "matrix_utils"
local socket = require "socket"


function correlate_basic_test(test_times_4x4, test_times_30x30)
    print("=== TEST correlate_basic_test ===")
    local m_4x4 = Matrix:new("./test_data/m4x4.txt")
    local m_4x4_correlated = Matrix:new("./test_data/m4x4_correlated.txt")
    local m_30x30 = Matrix:new("./test_data/m30x30.txt")
    local m_30x30_correlated = Matrix:new("./test_data/m30x30_correlated.txt")

    toCppSetCorrelationModeBasic()

    local timeBefore = socket.gettime()
    local cpp_4x4_correlated_data = toCppCorrelateMatrix(m_4x4.rows, m_4x4.cols, m_4x4.data)
    local timeAfter = socket.gettime()
    test_times_4x4[#test_times_4x4+1] = {"Basic", timeAfter - timeBefore}

    local cpp_4x4_correlated_matrix = Matrix:new()
    cpp_4x4_correlated_matrix:set(m_4x4.rows, m_4x4.cols, cpp_4x4_correlated_data)

    timeBefore = socket.gettime()
    local cpp_30x30_correlated_data = toCppCorrelateMatrix(m_30x30.rows, m_30x30.cols, m_30x30.data)
    timeAfter = socket.gettime()
    test_times_30x30[#test_times_30x30+1] = {"Basic", timeAfter - timeBefore}

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

function correlate_ilp_test(test_times_4x4, test_times_30x30)
    print("=== TEST correlate_ilp_test ===")
    local m_4x4 = Matrix:new("./test_data/m4x4.txt")
    local m_4x4_correlated = Matrix:new("./test_data/m4x4_correlated.txt")
    local m_30x30 = Matrix:new("./test_data/m30x30.txt")
    local m_30x30_correlated = Matrix:new("./test_data/m30x30_correlated.txt")

    toCppSetCorrelationModeILP()

    local timeBefore = socket.gettime()
    local cpp_4x4_correlated_data = toCppCorrelateMatrix(m_4x4.rows, m_4x4.cols, m_4x4.data)
    local timeAfter = socket.gettime()
    test_times_4x4[#test_times_4x4+1] = {"ILP", timeAfter - timeBefore}

    local cpp_4x4_correlated_matrix = Matrix:new()
    cpp_4x4_correlated_matrix:set(m_4x4.rows, m_4x4.cols, cpp_4x4_correlated_data)

    timeBefore = socket.gettime()
    local cpp_30x30_correlated_data = toCppCorrelateMatrix(m_30x30.rows, m_30x30.cols, m_30x30.data)
    timeAfter = socket.gettime()
    test_times_30x30[#test_times_30x30+1] = {"ILP", timeAfter - timeBefore}

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

function correlate_parallel_test(test_times_4x4, test_times_30x30)
    print("=== TEST correlate_parallel_test ===")
    local m_4x4 = Matrix:new("./test_data/m4x4.txt")
    local m_4x4_correlated = Matrix:new("./test_data/m4x4_correlated.txt")
    local m_30x30 = Matrix:new("./test_data/m30x30.txt")
    local m_30x30_correlated = Matrix:new("./test_data/m30x30_correlated.txt")

    toCppSetCorrelationModeParallel()


    local timeBefore = socket.gettime()
    local cpp_4x4_correlated_data = toCppCorrelateMatrix(m_4x4.rows, m_4x4.cols, m_4x4.data)
    local timeAfter = socket.gettime()
    test_times_4x4[#test_times_4x4+1] = {"OMP", timeAfter - timeBefore}

    local cpp_4x4_correlated_matrix = Matrix:new()
    cpp_4x4_correlated_matrix:set(m_4x4.rows, m_4x4.cols, cpp_4x4_correlated_data)

    timeBefore = socket.gettime()
    local cpp_30x30_correlated_data = toCppCorrelateMatrix(m_30x30.rows, m_30x30.cols, m_30x30.data)
    timeAfter = socket.gettime()
    test_times_30x30[#test_times_30x30+1] = {"OMP", timeAfter - timeBefore}

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

function correlate_vectorised_test(test_times_4x4, test_times_30x30)
    print("=== TEST correlate_vectorised_test ===")
    local m_4x4 = Matrix:new("./test_data/m4x4.txt")
    local m_4x4_correlated = Matrix:new("./test_data/m4x4_correlated.txt")
    local m_30x30 = Matrix:new("./test_data/m30x30.txt")
    local m_30x30_correlated = Matrix:new("./test_data/m30x30_correlated.txt")

    toCppSetCorrelationModeVectorised()

    local timeBefore = socket.gettime()
    local cpp_4x4_correlated_data = toCppCorrelateMatrix(m_4x4.rows, m_4x4.cols, m_4x4.data)
    local timeAfter = socket.gettime()
    test_times_4x4[#test_times_4x4+1] = {"Vectorised", timeAfter - timeBefore}

    local cpp_4x4_correlated_matrix = Matrix:new()
    cpp_4x4_correlated_matrix:set(m_4x4.rows, m_4x4.cols, cpp_4x4_correlated_data)

    timeBefore = socket.gettime()
    local cpp_30x30_correlated_data = toCppCorrelateMatrix(m_30x30.rows, m_30x30.cols, m_30x30.data)
    timeAfter = socket.gettime()
    test_times_30x30[#test_times_30x30+1] = {"Vectorised", timeAfter - timeBefore}

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

function correlate_max_opt_double_precision_test(test_times_4x4, test_times_30x30)
    print("=== TEST correlate_max_opt_double_precision_test ===")
    local m_4x4 = Matrix:new("./test_data/m4x4.txt")
    local m_4x4_correlated = Matrix:new("./test_data/m4x4_correlated.txt")
    local m_30x30 = Matrix:new("./test_data/m30x30.txt")
    local m_30x30_correlated = Matrix:new("./test_data/m30x30_correlated.txt")

    toCppSetCorrelationModeMaxOptDPrec()

    local timeBefore = socket.gettime()
    local cpp_4x4_correlated_data = toCppCorrelateMatrix(m_4x4.rows, m_4x4.cols, m_4x4.data)
    local timeAfter = socket.gettime()
    test_times_4x4[#test_times_4x4+1] = {"Max opt (doubles)", timeAfter - timeBefore}

    local cpp_4x4_correlated_matrix = Matrix:new()
    cpp_4x4_correlated_matrix:set(m_4x4.rows, m_4x4.cols, cpp_4x4_correlated_data)

    timeBefore = socket.gettime()
    local cpp_30x30_correlated_data = toCppCorrelateMatrix(m_30x30.rows, m_30x30.cols, m_30x30.data)
    timeAfter = socket.gettime()
    test_times_30x30[#test_times_30x30+1] = {"Max opt (doubles)", timeAfter - timeBefore}

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

function correlate_max_opt_single_precision_test(test_times_4x4, test_times_30x30)
    print("=== TEST correlate_max_opt_single_precision_test ===")
    local m_4x4 = Matrix:new("./test_data/m4x4.txt")
    local m_4x4_correlated = Matrix:new("./test_data/m4x4_correlated_floats.txt")
    local m_30x30 = Matrix:new("./test_data/m30x30.txt")
    local m_30x30_correlated = Matrix:new("./test_data/m30x30_correlated_floats.txt")

    toCppSetCorrelationModeMaxOptSPrec()

    local timeBefore = socket.gettime()
    local cpp_4x4_correlated_data = toCppCorrelateMatrix(m_4x4.rows, m_4x4.cols, m_4x4.data)
    local timeAfter = socket.gettime()
    test_times_4x4[#test_times_4x4+1] = {"Max opt (floats)", timeAfter - timeBefore}

    local cpp_4x4_correlated_matrix = Matrix:new()
    cpp_4x4_correlated_matrix:set(m_4x4.rows, m_4x4.cols, cpp_4x4_correlated_data)

    timeBefore = socket.gettime()
    local cpp_30x30_correlated_data = toCppCorrelateMatrix(m_30x30.rows, m_30x30.cols, m_30x30.data)
    timeAfter = socket.gettime()
    test_times_30x30[#test_times_30x30+1] = {"Max opt (floats)", timeAfter - timeBefore}

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
    local test_times_4x4 = {}
    local test_times_30x30 = {}
    correlate_basic_test(test_times_4x4, test_times_30x30)
    correlate_ilp_test(test_times_4x4, test_times_30x30)
    correlate_parallel_test(test_times_4x4, test_times_30x30)
    correlate_vectorised_test(test_times_4x4, test_times_30x30)
    correlate_max_opt_double_precision_test(test_times_4x4, test_times_30x30)
    correlate_max_opt_single_precision_test(test_times_4x4, test_times_30x30)

    print("=== Test times (4x4 matrix): ===")
    for _, test_time in pairs(test_times_4x4) do
        local test_title = test_time[1]
        local test_seconds = test_time[2]
        print(string.format("Test %s: %.10fs", test_title, test_seconds))
    end
    print("=== Test times (30x30 matrix): ===")
    for _, test_time in pairs(test_times_30x30) do
        local test_title = test_time[1]
        local test_seconds = test_time[2]
        print(string.format("Test %s: %.10fs", test_title, test_seconds))
    end
end