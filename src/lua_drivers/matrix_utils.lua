
-- Matrix object:
Matrix = {
    rows = 0,
    cols = 0,
    data = {}
}

-- Constructor:
function Matrix:new(path)
    local matrix = {}
    setmetatable(matrix, self)
    self.__index = self
    if path then
        matrix:load(path)
    end
    return matrix
end

-- Clone:
function Matrix:clone(rows, cols, data)
    local matrix = {}
    setmetatable(matrix, self)
    self.__index = self
    matrix.rows = rows
    matrix.cols = cols
    matrix.data = data
    return matrix
end

-- generate a random matrix of a given size, where elements are floats between 0 and 1
function Matrix:getRandomMatrix(rows, cols)
    local matrix = {}
    setmetatable(matrix, self)
    self.__index = self
    matrix.rows = rows
    matrix.cols = cols
    for i=1,rows*cols do
        matrix.data[#matrix.data+1] = math.random()
    end
    return matrix
end

-- get matrix size
function Matrix:size()
    return self.cols*self.rows
end

-- check if a file exists at path
function file_exists(file)
    local f = io.open(file, "rb")
    if f then f:close() end
    return f ~= nil
end

-- load a matrix from file
function Matrix:load(path)
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




