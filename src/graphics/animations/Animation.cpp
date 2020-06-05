#include <graphics/animations/Animation.h>

Animation::Animation(std::string animName)
{
    this->progress = 0;
    this->progressWait = Config::getInt("Animation_Frame_Rate");
    this->loaded = false;
    currentFrameNumber = 0;
    setFramePaths(animName);
}

void Animation::setFramePaths(std::string animName)
{
    // Get frame count
    frameCount = Config::getInt(animName + "_Frame_Count");
    //std::cout << "Frame count: " << frameCount << std::endl;

    // Get the paths to the individual frame model paths
    for (unsigned int i = 0; i < frameCount; i++)
    {
        framePaths.push_back(Config::get(animName + "_" + std::to_string(i)));
    }
}

void Animation::playNextFrame(GameObject* object)
{
    // Update progress
    progress++;

    // If progress is insuffcient, do not play next frame yet, in accordance with animation rate
    if (progress < progressWait)    return;
    else                            progress = 0;

    // Increment to next frame
    currentFrameNumber++;

    // Loop back to first frame
    if (currentFrameNumber == frameCount)
    {
        currentFrameNumber = 0;
    }

    /*std::cout << "Animation frame: " << currentFrameNumber << std::endl;
    std::cout << "Model: " << object->getModelPath() << std::endl;
    std::cout << "Frame: " << frames.at(currentFrameNumber)->path << std::endl;*/

    // Next frame
    object->model = frames.at(currentFrameNumber);
}

void Animation::loadFrames()
{
    // Iterate over frame paths
    for (std::string path : framePaths)
    {
        // Load individual frame models
        frames.push_back(new Model(path));
    }
    loaded = true;
}

bool Animation::isLoaded()
{
    return this->loaded;
}

void Animation::reset()
{
    currentFrameNumber = 0;
}