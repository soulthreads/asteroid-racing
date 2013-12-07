#ifndef ASSETS_H
#define ASSETS_H

#include <string>

#include <GLES2/gl2.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <png.h>

using std::string;

struct FileData {
    const size_t length;
    const char *data;
    AAsset *asset;
};

struct ImageData {
    unsigned char *data;
    uint32_t width, height;
};

void initAssetManager (AAssetManager *am);
FileData getAssetData (const string &path);
void releaseAssetData (const FileData *fileData);

GLuint buildProgramFromAssets (const string &vertexShaderPath, const string &fragmentShaderPath);

ImageData loadImageFromPngAsset (const string &pngPath);

GLuint loadTextureFromAsset (const string &path);
GLuint loadCubeMapFromAssets (const string paths[]);

GLuint loadObjFromAssets (const string &pathToObj, const string &pathToMtl, GLuint &nvertices);

#endif // ASSETS_H
