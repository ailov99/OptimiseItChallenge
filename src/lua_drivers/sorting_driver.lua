

function sort_ints(table)
    local sorted_ints = toCppMergeSortInt(4, table)
    for _,i in pairs(sorted_ints) do
        print(i)
    end
end

-- main
function main()
    print("__Sorting_driver_called__")

    local unsorted = {1, 4, 3, 2}

    sort_ints(unsorted)
end


main()