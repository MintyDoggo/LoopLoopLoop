/*
  ==============================================================================

    OpenGLComponent.h
    Created: 3 Jan 2024 12:52:26pm
    Author:  dog1

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <fstream>

//==============================================================================
/*
*/
class OpenGLComponent  : public juce::Component, public juce::OpenGLRenderer
{
public:
    OpenGLComponent();
    ~OpenGLComponent() override;

    void newOpenGLContextCreated() override;
    void renderOpenGL() override;
    void openGLContextClosing() override;
    void updateAnimation();

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::OpenGLContext openGLContext;

    struct Vertex
    {
        float position[2];
        float colour[4];
    };

    std::vector<Vertex> vertexBuffer;
    std::vector<unsigned int> indexBuffer;

    GLuint vbo; // Vertex buffer object.
    GLuint ibo; // Index buffer object.

    // shader code
    juce::String vertexShader;
    juce::String fragmentShader;

    std::unique_ptr<juce::OpenGLShaderProgram> shaderProgram;

    float xOffset = 0.0f;
    int test = 0;

    std::ofstream out;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLComponent)
};
