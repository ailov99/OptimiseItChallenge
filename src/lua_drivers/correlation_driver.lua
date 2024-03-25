local Matrix = require "matrix_utils"
local socket = require "socket"

-- correlate_ilp
function correlate_ilp(m, times)
    toCppSetCorrelationModeILP()

    local timeBefore = socket.gettime()
    local correlated_table = toCppCorrelateMatrix(m.rows,m.cols,m.data)
    local timeAfter = socket.gettime()
    times[#times+1] = {"ILP", timeAfter - timeBefore}

    local correlated_matrix = Matrix:new()
    correlated_matrix:set(m.rows, m.cols, correlated_table)

    print("==== Input Matrix: ====")
    m:print()
    print("==== ILP Correlated (only top-right calculated): ====")
    correlated_matrix:print()
    print("")
end

-- correlate_basic
function correlate_basic(m, times)
    toCppSetCorrelationModeBasic()

    local timeBefore = socket.gettime()
    local correlated_table = toCppCorrelateMatrix(m.rows,m.cols,m.data)
    local timeAfter = socket.gettime()
    times[#times+1] = {"Basic", timeAfter - timeBefore}

    local correlated_matrix = Matrix:new()
    correlated_matrix:set(m.rows, m.cols, correlated_table)

    print("==== Input Matrix: ====")
    m:print()
    print("==== Basic Correlated (only top-right calculated): ====")
    correlated_matrix:print()
    print("")
end

--correlate_parallel
function correlate_parallel(m, times)
    toCppSetCorrelationModeParallel()

    local timeBefore = socket.gettime()
    local correlated_table = toCppCorrelateMatrix(m.rows,m.cols,m.data)
    local timeAfter = socket.gettime()
    times[#times+1] = {"Parallel", timeAfter - timeBefore}

    local correlated_matrix = Matrix:new()
    correlated_matrix:set(m.rows, m.cols, correlated_table)

    print("==== Input Matrix: ====")
    m:print()
    print("==== Parallel Correlated (only top-right calculated): ====")
    correlated_matrix:print()
    print("")
end

--correlate_vectorised
function correlate_vectorised(m, times)
    toCppSetCorrelationModeVectorised()

    local timeBefore = socket.gettime()
    local correlated_table = toCppCorrelateMatrix(m.rows,m.cols,m.data)
    local timeAfter = socket.gettime()
    times[#times+1] = {"Vectorised", timeAfter - timeBefore}

    local correlated_matrix = Matrix:new()
    correlated_matrix:set(m.rows, m.cols, correlated_table)

    print("==== Input Matrix: ====")
    m:print()
    print("==== Vectorised Correlated (only top-right calculated): ====")
    correlated_matrix:print()
    print("")
end

--correlate_max_opt_double_precision
function correlate_max_opt_double_precision(m, times)
    toCppSetCorrelationModeMaxOptDPrec()

    local timeBefore = socket.gettime()
    local correlated_table = toCppCorrelateMatrix(m.rows,m.cols,m.data)
    local timeAfter = socket.gettime()
    times[#times+1] = {"Max opt (doubles)", timeAfter - timeBefore}

    local correlated_matrix = Matrix:new()
    correlated_matrix:set(m.rows, m.cols, correlated_table)

    print("==== Input Matrix: ====")
    m:print()
    print("==== Max Optimised Double Precision Correlated (only top-right calculated): ====")
    correlated_matrix:print()
    print("")
end

--correlate_max_opt_single_precision
function correlate_max_opt_single_precision(m, times)
    toCppSetCorrelationModeMaxOptSPrec()

    local timeBefore = socket.gettime()
    local correlated_table = toCppCorrelateMatrix(m.rows,m.cols,m.data)
    local timeAfter = socket.gettime()
    times[#times+1] = {"Max opt (floats)", timeAfter - timeBefore}

    local correlated_matrix = Matrix:new()
    correlated_matrix:set(m.rows, m.cols, correlated_table)

    print("==== Input Matrix: ====")
    m:print()
    print("==== Max Optimised Single Precision Correlated (only top-right calculated): ====")
    correlated_matrix:print()
    print("")
end

-- main
function main()
    local m = Matrix:new("./test_data/m4x4.txt")
    local times = {}

    correlate_basic(m, times)
    correlate_ilp(m, times)
    correlate_parallel(m, times)
    correlate_vectorised(m, times)
    correlate_max_opt_double_precision(m, times)
    correlate_max_opt_single_precision(m, times)

    print("=== Runtimes: ===")
    for _, test_time in pairs(times) do
        local test_title = test_time[1]
        local test_seconds = test_time[2]
        print(string.format("Test %s: %.10fs", test_title, test_seconds))
    end
end

main()