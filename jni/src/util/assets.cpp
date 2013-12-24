#include "assets.h"
#include "shader.h"

#include <cstdlib>
#include <cassert>

#include <map>
#include <vector>
#include <sstream>

#define LOG_TAG "AR_assets"
#include "logs.h"

using namespace std;

static AAssetManager *assetManager;

FileData getAssetData (const string &path) {
    auto asset = AAssetManager_open (assetManager, path.c_str (), AASSET_MODE_BUFFER);
    assert (asset != nullptr);

    return (FileData) {AAsset_getLength (asset), (const char *)AAsset_getBuffer (asset), asset};
}

void initAssetManager (AAssetManager *am) {
    assetManager = am;
}

void releaseAssetData (const FileData *fileData) {
    assert (fileData != nullptr);
    assert (fileData->asset != nullptr);

    AAsset_close (fileData->asset);
}

GLuint buildProgramFromAssets (const string &vertexShaderPath, const string &fragmentShaderPath) {
    auto vertexShader = getAssetData (vertexShaderPath);
    auto fragmentShader = getAssetData (fragmentShaderPath);

    GLuint program = buildProgram (vertexShader.data, vertexShader.length,
                                   fragmentShader.data, fragmentShader.length);

    releaseAssetData (&vertexShader);
    releaseAssetData (&fragmentShader);

    return program;
}

struct PngDataHandle {
    const png_bytep data;
    png_size_t offset;
};

static void readDataFromMemory (png_structp pngPtr, png_bytep out, png_size_t length) {
    PngDataHandle *handle = (PngDataHandle *)png_get_io_ptr (pngPtr);
    const png_bytep src = handle->data + handle->offset;
    memcpy (out, src, length);
    handle->offset += length;
}

ImageData loadImageFromPngAsset (const string &pngPath) {
    auto pngAsset = getAssetData (pngPath);

    assert (pngAsset.data != nullptr && pngAsset.length > 8);
    assert (png_check_sig (pngAsset.data, 8));

    png_structp pngPtr = png_create_read_struct (PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    assert (pngPtr != 0);
    png_infop infoPtr = png_create_info_struct (pngPtr);

    auto pngDataHandle = (PngDataHandle) {(png_bytep)pngAsset.data, 0};
    png_set_read_fn (pngPtr, &pngDataHandle, readDataFromMemory);

    png_uint_32 width = 0, height = 0;
    int bitDepth = 0, colorType = -1;
    png_read_info (pngPtr, infoPtr);
    png_get_IHDR (pngPtr, infoPtr, &width, &height, &bitDepth, &colorType, nullptr, nullptr, nullptr);

    // Convert transparency to full alpha
    if (png_get_valid (pngPtr, infoPtr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha (pngPtr);

    // Convert grayscale, if needed
    if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
        png_set_expand_gray_1_2_4_to_8 (pngPtr);

    // Convert paletted images, if needed
    if (colorType == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb (pngPtr);

    // Add alpha channel, if there is none
    if (colorType == PNG_COLOR_TYPE_PALETTE || colorType == PNG_COLOR_TYPE_RGB)
        png_set_add_alpha (pngPtr, 0xff, PNG_FILLER_AFTER);

    if (bitDepth < 8)
        png_set_packing (pngPtr);
    else if (bitDepth == 16)
        png_set_scale_16 (pngPtr);

    png_read_update_info (pngPtr, infoPtr);
    colorType = png_get_color_type (pngPtr, infoPtr);

    assert (colorType == PNG_COLOR_TYPE_RGBA);

    const png_size_t rowSize = png_get_rowbytes (pngPtr, infoPtr);
    const int dataLength = rowSize * height;
    assert (rowSize > 0);

    const auto rawImage = new png_byte [dataLength];
    assert (rawImage != NULL);

    png_bytep rowPtrs [height];

    for (png_uint_32 i = 0; i < height; ++i) {
        rowPtrs[i] = rawImage + i * rowSize;
    }

    png_read_image (pngPtr, rowPtrs);

    png_read_end (pngPtr, infoPtr);
    png_destroy_read_struct (&pngPtr, &infoPtr, nullptr);
    releaseAssetData (&pngAsset);

    return (ImageData) {rawImage, width, height};
}

GLuint loadTextureFromAsset (const string &path) {
    GLuint texture;
    glGenTextures (1, &texture);
    assert (texture != 0);

    glBindTexture (GL_TEXTURE_2D, texture);

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto png = loadImageFromPngAsset (path);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA,
                  png.width, png.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, png.data);
    glGenerateMipmap (GL_TEXTURE_2D);

    glBindTexture (GL_TEXTURE_2D, 0);

    delete [] png.data;

    return texture;
}

GLuint loadCubeMapFromAssets (const vector<string> &paths) {
    GLuint texture;
    glGenTextures (1, &texture);
    assert (texture != 0);

    glBindTexture (GL_TEXTURE_CUBE_MAP, texture);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ImageData pngs[6];
    for (int i = 0; i < 6; ++i) {
        pngs[i] = loadImageFromPngAsset (paths[i]);
        glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA,
                      pngs[0].width, pngs[0].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pngs[i].data);
    }

    glBindTexture (GL_TEXTURE_CUBE_MAP, 0);

    for (int i = 0; i < 6; ++i) {
        delete [] pngs[i].data;
    }

    return texture;
}

struct Kd {
    GLfloat r, g, b;
};

GLuint loadObjFromAssets (const string &pathToObj, const string &pathToMtl, GLuint &nvertices)
{
    auto objAsset = getAssetData (pathToObj);
    auto mtlAsset = getAssetData (pathToMtl);

    map<string, Kd> materials;
    istringstream mtlStream (mtlAsset.data);
    string line, key;

    string currentMtl;
    while (getline (mtlStream, line)) {
        istringstream lstr (line);
        lstr >> key;
        if (key == "newmtl") {
            lstr >> currentMtl;
        } else if (key == "Kd") {
            Kd kd;
            lstr >> kd.r >> kd.g >> kd.b;
            materials[currentMtl] = kd;
        }
    }

    releaseAssetData (&mtlAsset);

    vector<GLfloat> vertices;
    vector<GLfloat> normals;
    vector<GLfloat> out;
    istringstream objStream (objAsset.data);

    while (getline (objStream, line)) {
        istringstream lstr (line);
        lstr >> key;
        if (key == "v") {
            GLfloat x, y, z;
            lstr >> x >> y >> z;
            vertices.push_back (x);
            vertices.push_back (y);
            vertices.push_back (z);
        } else if (key == "vn") {
            GLfloat x, y, z;
            lstr >> x >> y >> z;
            normals.push_back (x);
            normals.push_back (y);
            normals.push_back (z);
        } else if (key == "f") {
            string word;
            int v, t, n;
            for (int i = 0; i < 3; ++i) {
                lstr >> word;
                if (word.find ("//")) {
                    sscanf (word.c_str (), "%d//%d", &v, &n);
                } else {
                    sscanf (word.c_str (), "%d/%d/%d", &v, &t, &n);
                }

                out.push_back (vertices[(v-1)*3]);
                out.push_back (vertices[(v-1)*3+1]);
                out.push_back (vertices[(v-1)*3+2]);

                out.push_back (normals[(n-1)*3]);
                out.push_back (normals[(n-1)*3+1]);
                out.push_back (normals[(n-1)*3+2]);

                out.push_back (materials[currentMtl].r);
                out.push_back (materials[currentMtl].g);
                out.push_back (materials[currentMtl].b);
            }
        } else if (key == "usemtl") {
            lstr >> currentMtl;
        }
    }

    releaseAssetData (&objAsset);

    nvertices = out.size () / 3;
    LOGI ("Number of Vertices: %d\n", nvertices);


    GLuint vbo;
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, out.size() * sizeof(GLfloat), out.data (), GL_STATIC_DRAW);
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    return vbo;
}


vector<string> listFilesInDir(const string &path)
{
    vector<string> ret;
    AAssetDir *assetDir = AAssetManager_openDir (assetManager, path.c_str ());
    const char *filename = nullptr;
    while ((filename = AAssetDir_getNextFileName (assetDir)) != nullptr) {
        ret.push_back (filename);
    }
    AAssetDir_close (assetDir);
    return ret;
}
