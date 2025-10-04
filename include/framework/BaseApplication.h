#ifndef _BASE_APPLICATION
#define _BASE_APPLICATION

#include <SDL3/SDL.h>
#include <imgui.h>

class BaseApplication
{
public:
    void Initialize(const char *title, int width, int height);
    void Run();
    virtual ~BaseApplication() = default;

protected:
    virtual void Start() = 0;
    virtual void Update(float delta_time) = 0;
    virtual void RenderUI() = 0;
    virtual void ProcessEvent(const SDL_Event &event) = 0;

private:
    SDL_Window *window_ = nullptr;
    SDL_GLContext gl_context_ = nullptr;
    ImGuiIO *io_ = nullptr;
    float scale_ = 1.0f;
    bool running_ = true;
    void SetupImgui();
    void NewFrame();
    void Render();
    void PollEvents();
    void Shutdown();
};
#endif
