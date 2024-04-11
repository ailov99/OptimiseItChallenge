
-- sort_ints
function sort_ints(table)
    local write = io.write

    print("===Unsorted list:===")
    for _,i in pairs(table) do
        write(i)
        write' '
    end
    write'\n'

    local sorted_ints = toCppMergeSortInt(#table, table)

    print("===Sorted list:===")
    for _,i in pairs(sorted_ints) do
        write(i)
        write' '
    end
    write'\n'
end

-- main
function main()
    local unsorted = {}
    for i=100,1,-1 do
        unsorted[#unsorted+1] = math.random(1,100)
    end

    sort_ints(unsorted)
end


main()