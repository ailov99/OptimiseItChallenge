-- print out a matrix
function print_matrix(n_columns, n_rows, data_table)
    local write = io.write
    for y=0,n_columns-1,1 do
        for x=1,n_rows,1 do
            write(data_table[x + y*n_rows])
            write' '
        end
        write'\n'
    end
end

-- correlate_ilp
function correlate_ilp()
    toCppSetCorrelationModeBasic()
    local ny = 4
    local nx = 4
    local data = {
        0.12, 0.14, 0.04, 0.01,
        0.11, 0.99, 0.95, 0.35,
        0.45, 0.67, 0.56, 0.11,
        0.76, 0.15, 0.52, 0.17
    }
    local correlated_table = toCppCorrelateMatrix(nx,ny,data)

    print_matrix(ny,nx,data)
    print("==== ILP Correlated (only top-right calculated): ====")
    print_matrix(ny,nx,correlated_table)
end

-- correlate_basic
function correlate_basic()
    toCppSetCorrelationModeBasic()
    local ny = 4
    local nx = 4
    local data = {
        0.12, 0.14, 0.04, 0.01,
        0.11, 0.99, 0.95, 0.35,
        0.45, 0.67, 0.56, 0.11,
        0.76, 0.15, 0.52, 0.17
    }
    local correlated_table = toCppCorrelateMatrix(nx,ny,data)

    print_matrix(ny,nx,data)
    print("==== Basic Correlated (only top-right calculated): ====")
    print_matrix(ny,nx,correlated_table)
end

-- main
correlate_basic()
correlate_ilp()