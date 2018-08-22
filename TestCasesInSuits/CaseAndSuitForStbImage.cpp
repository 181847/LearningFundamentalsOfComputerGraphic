#include "CaseAndSuitForStbImage.h"
#define STB_IMAGE_IMPLEMENTATION
extern "C"
{
#include <stb_image.h>
}


void CASE_NAME_IN_STB_IMG(BasicTest)::Run()
{
    int x, y, n = 4;
    //std::string imagePath = LOAD_IMAGE_PATH + "InsideBox29.png";
    std::string imagePath = LOAD_IMAGE_PATH + "OldImgs/RenderReflect/ThisIsForSceneRayColor06.png";
    unsigned char *data = stbi_load(imagePath.c_str(), &x, &y, &n, 0);

    if (!data)
    {
        printf("Error!, cannot load image: %s\n", imagePath.c_str());
        return;
    }

    Image restoreImage(x, y);
    unsigned char * restoreAddr = restoreImage.GetRawData();

    memcpy(restoreAddr, data, x * y * 4);
    
    BlockShowImg(&restoreImage, L"restored image");

    stbi_image_free(data);
}