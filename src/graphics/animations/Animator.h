#pragma once
#include <graphics/animations/Animation.h>
#include <unordered_map>
#include <objects/Player.h>

class Animator
{
public:
    
    /**
     * Construct an animator with no initial animations.
     * */
    Animator();

    /**
     * Return whether or not the animations are all loaded.
     * */
    bool isAnimsLoadedFor(GameObject*);

    /**
     * Load the frames of each individual animation.
     * */
    void loadAnimations(GameObject*);

    /**
     * Play the current animation
     * TODO: in the future, this should be based on a state machine.
     * */
    void play(GameObject*);

    /**
     * Add a new animation to the Animator. Will set animsLoaded to false.
     * */
    void addAnimation(GameObject*, std::string, Animation*);

    /**
     * Sets the current animation state for this object. Resets progress on current animation.
     * */
    void setCurrentAnimation(GameObject*, std::string);

    /**
     * Returns whether or not the current animation is set.
     * */
    bool isCurrentAnimationSet();

    std::unordered_map<GameObject*, bool> animsLoadedFor;

private:
    std::unordered_map<GameObject*, std::string> prevState;
    std::unordered_map<GameObject*, std::string> currentState;
    std::unordered_map<GameObject*, Animation*> currentAnimation;
    std::unordered_map<GameObject*, std::unordered_map<std::string, Animation*>> animations;
};