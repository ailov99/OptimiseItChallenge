require("image_utils")
require("matrix_utils")

-- main
function main()
    local img = Image:new("./test_data/inA.png")
    img:saveToFile("./test_data/luaOutA.png")
    -- TODO: Segment
end

main()