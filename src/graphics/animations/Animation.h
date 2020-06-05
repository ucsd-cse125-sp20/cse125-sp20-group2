#pragma once
#include <graphics/render/Model.h>
#include <util/Config.h>
#include <objects/GameObject.h>

class Animation
{
public:

    /**
     * Determine the animation based on a string key in the config.
     * */
    Animation (std::string);

    /**
     * Given the gameobject, update the model to the next frame for the animation.
     * Loop back to the first frame if at the end.
     * */
    void playNextFrame(GameObject*);

    /**
     * Called by the constructor to set the model paths of the individual frames.
     * */
    void setFramePaths(std::string);

    /**
     * Stop playing the animation for this gameobject, setting it back to the static model.
     * */
    void stop(GameObject*);

    /**
     * Load the models of the animation. Ideally should be done early on to avoid latency issues.
     * */
    void loadFrames();

    /**
     * return whether or not this individual animation is loaded
     * */
    bool isLoaded();

    /**
     * Resets the frame back to 0
     * */
    void reset();

private:
    bool loaded;
    unsigned int frameCount;
    unsigned int currentFrameNumber;
    std::vector<std::string>    framePaths;
    std::vector<Model*>         frames;
};