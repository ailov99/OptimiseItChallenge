local Image = require "image_utils"

-- main
local function main()
    local img = Image:new("./test_data/inA.png")
    img:saveToFile("./test_data/luaOutA.png")
end

main()