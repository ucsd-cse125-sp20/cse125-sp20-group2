static void key_callback_wrapper (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ClientGame* clientGame = reinterpret_cast<ClientGame*> (glfwGetWindowUserPointer(window));
    if (clientGame)
    {
        clientGame->keyBindsHandler(window, key, scancode, action, mods);
    }
}

