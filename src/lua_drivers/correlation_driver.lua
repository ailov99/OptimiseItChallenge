require("matrix_utils")

-- correlate_ilp
function correlate_ilp(m)
    toCppSetCorrelationModeILP()
    local correlated_table = toCppCorrelateMatrix(m.rows,m.cols,m.data)
    local correlated_matrix = Matrix:clone(m.rows, m.cols, correlated_table)

    print("==== Input Matrix: ====")
    m:print()
    print("==== ILP Correlated (only top-right calculated): ====")
    correlated_matrix:print()
    print("")
end

-- correlate_basic
function correlate_basic(m)
    toCppSetCorrelationModeBasic()
    local correlated_table = toCppCorrelateMatrix(m.rows,m.cols,m.data)
    local correlated_matrix = Matrix:clone(m.rows, m.cols, correlated_table)

    print("==== Input Matrix: ====")
    m:print()
    print("==== Basic Correlated (only top-right calculated): ====")
    correlated_matrix:print()
    print("")
end

--correlate_parallel
function correlate_parallel(m)
    toCppSetCorrelationModeParallel()
    local correlated_table = toCppCorrelateMatrix(m.rows,m.cols,m.data)
    local correlated_matrix = Matrix:clone(m.rows, m.cols, correlated_table)

    print("==== Input Matrix: ====")
    m:print()
    print("==== Parallel Correlated (only top-right calculated): ====")
    correlated_matrix:print()
    print("")
end

--correlate_vectorised
function correlate_vectorised(m)
    toCppSetCorrelationModeVectorised()
    local correlated_table = toCppCorrelateMatrix(m.rows,m.cols,m.data)
    local correlated_matrix = Matrix:clone(m.rows, m.cols, correlated_table)

    print("==== Input Matrix: ====")
    m:print()
    print("==== Vectorised Correlated (only top-right calculated): ====")
    correlated_matrix:print()
    print("")
end

--correlate_max_opt_double_precision
function correlate_max_opt_double_precision(m)
    toCppSetCorrelationModeMaxOptDPrec()
    local correlated_table = toCppCorrelateMatrix(m.rows,m.cols,m.data)
    local correlated_matrix = Matrix:clone(m.rows, m.cols, correlated_table)

    print("==== Input Matrix: ====")
    m:print()
    print("==== Max Optimised Double Precision Correlated (only top-right calculated): ====")
    correlated_matrix:print()
    print("")
end

--correlate_max_opt_single_precision
function correlate_max_opt_single_precision(m)
    toCppSetCorrelationModeMaxOptSPrec()
    local correlated_table = toCppCorrelateMatrix(m.rows,m.cols,m.data)
    local correlated_matrix = Matrix:clone(m.rows, m.cols, correlated_table)

    print("==== Input Matrix: ====")
    m:print()
    print("==== Max Optimised Single Precision Correlated (only top-right calculated): ====")
    correlated_matrix:print()
    print("")
end

-- main
function main()
    local m = Matrix:new("./test_data/m4x4.txt")

    correlate_basic(m)
    correlate_ilp(m)
    correlate_parallel(m)
    correlate_vectorised(m)
    correlate_max_opt_double_precision(m)
    correlate_max_opt_single_precision(m)
end

main()