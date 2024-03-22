-- Image object
Image = {
    height = 0,
    width = 0,
    colour_channels = 0,
    data = {}
}

-- Constructor
function Image:new(path)
    local img = {}
    setmetatable(img, self)
    self.__index = self
    img:set(0, 0, 0, {})
    if path then
        image_descriptor = toCppReadRGBImageFromFile(path)
        img.width = image_descriptor[1]
        img.height = image_descriptor[2]
        img.colour_channels = image_descriptor[3]
        img.data = image_descriptor[4]
    end
    return img
end

-- Setter
function Image:set(height, width, colour_channels, data)
    self.height = height
    self.width = width
    self.colour_channels = colour_channels
    self.data = data
end

-- Save to a file
function Image:saveToFile(path)
    toCppWriteRGBImageToFile(
        path,
        self.width,
        self.height,
        self.colour_channels,
        self.data
    )
end

-- Compare to another Image object
function Image:isSameAs(img)
    if self.height ~= img.height or
       self.width ~= img.width or
       self.colour_channels ~= img.colour_channels 
    then
        return false
    end

    for index, element in pairs(self.data) do
        if img.data[index] ~= element then
            return false
        end
    end

    return true
end

return Image