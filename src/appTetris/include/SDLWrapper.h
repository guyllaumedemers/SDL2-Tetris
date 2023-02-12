#ifndef INCLUDED_SDL_WRAPPER
#define INCLUDED_SDL_WRAPPER

// --- forward declaration
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
// ---

/// <summary>
/// Wrapper Class for smart pointer destructor override
/// </summary>
namespace SDLWrapper
{
	struct FreeSDLWindow final
	{
		void operator()(SDL_Window* Window) const;
	};

	struct FreeSDLRenderer final
	{
		void operator()(SDL_Renderer* Renderer) const;
	};

	struct FreeSDLTexture final
	{
		void operator()(SDL_Texture* Texture) const;
	};
}
#endif
