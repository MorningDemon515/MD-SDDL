#ifndef SHADER_H
#define SHADER_H

class Shader
{
private:
    unsigned int program;

public:
    Shader(const char* vShader, const char* fShader);
    ~Shader();

    void Use();

    unsigned int GetLoc(const char* name);
};

#endif