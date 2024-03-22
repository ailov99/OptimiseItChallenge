-- Matrix object:
local Matrix = {
    rows = 0,
    cols = 0,
    data = {}
}

-- Constructor:
function Matrix:new(path)
    local o = {}
    setmetatable(o, self)
    self.__index = self
    o:set(0, 0, {})
    if path then
        o:loadMatrix(path)
    end
    return o
end

-- Setter:
function Matrix:set(rows, cols, data)
    self.rows = rows
    self.cols = cols
    self.data = data
end

-- get matrix size
function Matrix:size()
    return self.cols*self.rows
end

-- print out a matrix
function Matrix:print()
    local write = io.write
    for y=0,self.cols-1,1 do
        for x=1,self.rows,1 do
            write(self.data[x + y*self.rows])
            write' '
        end
        write'\n'
    end
end

-- check if a file exists at path
function file_exists(file)
    local f = io.open(file, "rb")
    if f then f:close() end
    return f ~= nil
end

-- load a matrix from file
function Matrix:loadMatrix(path)
    if not file_exists(path) then return {} end
    local first_line = true
    for line in io.lines(path) do
        local line_split_nums = {}
        for token in line:gmatch("%S+") do table.insert(line_split_nums, tonumber(token)) end
        if first_line then
            self.rows = line_split_nums[1]
            self.cols = line_split_nums[2]
            first_line = false
        else
            for _,n_str in pairs(line_split_nums) do table.insert(self.data, tonumber(n_str)) end
        end
    end
end

-- matrix to matrix epsilon equality
function Matrix:approxEq(m, eps)
    if m.rows ~= self.rows or m.cols ~= self.cols then
        return false
    end

    local math = math
    local eps_diff = 10^(-eps)

    for y=0,m.cols-1,1 do
        for x=1,m.rows,1 do
            local data_index = x + y*m.rows
            local elements_diff = math.abs(m.data[data_index] - self.data[data_index])
            if elements_diff > eps_diff then
                print(data_index)
                print(m.data[data_index])
                print(self.data[data_index])
                return false
            end
        end
    end

    return true
end


return Matrix