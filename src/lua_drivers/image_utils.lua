
-- Image object:
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
    if path then
        image_descriptor = toCppReadRGBImageFromFile(path)
        img.width = image_descriptor[1]
        img.height = image_descriptor[2]
        img.colour_channels = image_descriptor[3]
        img.data = image_descriptor[4]
    end
    return img
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

-- Clone
function Image:clone(height, width, colour_channels, data)
    -- TODO
end