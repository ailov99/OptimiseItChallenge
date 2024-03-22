local Image = require "image_utils"

function read_write_image_to_file_test()
    print("=== TEST read_write_image_to_file_test ===")
    local input_images_filenames = {
        "./test_data/inA.png",
        "./test_data/inB.png",
        "./test_data/inC.png",
        "./test_data/inD.png",
        "./test_data/inE.png",
    }

    -- Read all images in
    local input_images = {}
    for index, filename in pairs(input_images_filenames) do
        input_images[index] = Image:new(filename)
    end

    -- Write all images back and read them back in
    local output_images_filenames = {
        "./test_data/outAtest.png",
        "./test_data/outBtest.png",
        "./test_data/outCtest.png",
        "./test_data/outDtest.png",
        "./test_data/outEtest.png",
    }
    for index, filename in pairs(output_images_filenames) do
        local read_image = input_images[index]
        read_image:saveToFile(filename)
        local readback_image = Image:new(filename)
        assert(
            read_image:isSameAs(readback_image), 
            string.format("Image %s ~= Image %s", 
                input_images_filenames[index], 
                output_images_filenames[index]
            )
        )
    end

    -- Cleanup
    for _, filename in pairs(output_images_filenames) do
        os.remove(filename)
    end

    print("=== PASS ===")
end

function run_all_segmentation_tests()
    read_write_image_to_file_test()
end