#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

constexpr int WINDOW_WIDTH = 1920;
constexpr int WINDOW_HEIGHT = 1080;

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 360;

constexpr unsigned char MENU_NORMAL_STATE = 0;
constexpr unsigned char MENU_HOVER_STATE = 1;
constexpr unsigned char MENU_DEFAULT_STATE = MENU_NORMAL_STATE;

struct Menu;
typedef void (*MenuItemClickedCallback)(struct Menu *menu, unsigned long index, void *userData);
struct MenuItemCallbacks
{
  MenuItemClickedCallback clicked;
};

struct Menu
{
  // array of label strings
  char **menuItemLabels;
  // array of state bitmasks
  unsigned char *menuItemStates;
  // array of x,y,w,h rects
  float *menuItemRects;
  // array of callbacks structure
  struct MenuItemCallbacks *menuItemCallbacks;
  // the number of menu items in the menu
  unsigned long size;
  // 0 - no selection, 1 - first selected (one-indexing logic)
  unsigned long selectedItem;
};

struct Menu *createMenu()
{
  struct Menu *menu = (struct Menu *)malloc(sizeof(struct Menu));
  menu->size = 0;
  menu->selectedItem = 0;
  menu->menuItemLabels = 0;
  menu->menuItemRects = 0;
  menu->menuItemStates = 0;
  menu->menuItemCallbacks = 0;
  return menu;
};

void addMenuItem(struct Menu *menu, const char *label, float width, float height)
{
  if (menu)
  {
    // add a label
    menu->menuItemLabels = (char **)realloc(menu->menuItemLabels, sizeof(char *) * menu->size + 1);
    menu->menuItemLabels[menu->size] = (char *)malloc(sizeof(char) * strlen(label));
    strcpy(menu->menuItemLabels[menu->size], label);
    // add a rect
    menu->menuItemRects = (float *)realloc(menu->menuItemRects, sizeof(float) * ((menu->size + 1) * 4));
    menu->menuItemRects[(menu->size * 4) + 0] = 0.0f;
    menu->menuItemRects[(menu->size * 4) + 1] = 0.0f;
    menu->menuItemRects[(menu->size * 4) + 2] = width;
    menu->menuItemRects[(menu->size * 4) + 3] = height;
    // add a state
    menu->menuItemStates = (unsigned char *)realloc(menu->menuItemStates, sizeof(unsigned char) * (menu->size + 1));
    menu->menuItemStates[menu->size] = MENU_DEFAULT_STATE;
    // add callbacks
    menu->menuItemCallbacks = (struct MenuItemCallbacks *)realloc(menu->menuItemCallbacks, sizeof(struct MenuItemCallbacks) * (menu->size + 1));
    menu->menuItemCallbacks[menu->size].clicked = 0;
    // increment the size
    menu->size++;
  }
}

void setMenuItemClickedCallback(struct Menu *menu, unsigned long index, MenuItemClickedCallback clicked)
{
  if (menu && index < menu->size && menu->menuItemCallbacks)
  {
    menu->menuItemCallbacks[index].clicked = clicked;
  }
}

void destroyMenu(struct Menu *menu)
{
  if (menu->size)
  {
    if (menu->menuItemLabels)
    {
      for (unsigned long i = 0; i < menu->size; i++)
      {
        if (menu->menuItemLabels[i])
        {
          free(menu->menuItemLabels[i]);
        }
      }
      free(menu->menuItemLabels);
      menu->menuItemLabels = 0;
    }
    if (menu->menuItemStates)
    {
      free(menu->menuItemStates);
      menu->menuItemStates = 0;
    }
    if (menu->menuItemRects)
    {
      free(menu->menuItemRects);
      menu->menuItemRects = 0;
    }
    if (menu->menuItemCallbacks)
    {
      free(menu->menuItemCallbacks);
      menu->menuItemCallbacks = 0;
    }
    menu->size = 0;
    menu->selectedItem = 0;
  }
}

struct GameCtx
{
  bool running;
};

void menuItemClicked(struct Menu *menu, unsigned long index, void *userData)
{
  const char *label = (const char *)menu->menuItemLabels[index];
  std::cout << "clicked on " << label << " menu item" << std::endl;
  switch (index)
  {
  case 0:
  {
    std::cout << "TODO: Start New Game" << std::endl;
  }
  break;
  case 1:
  {
    std::cout << "TODO: Load Saved Game" << std::endl;
  }
  break;
  case 2:
  {
    std::cout << "TODO: Push Options Screen" << std::endl;
  }
  break;
  case 3:
  {
    std::cout << "Quit Game" << std::endl;
    struct GameCtx *ctx = (struct GameCtx *)userData;
    if (ctx)
    {
      ctx->running = false;
    }
  }
  break;
  default:
    break;
  }
}

int main(int argc, char *argv[])
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    throw std::runtime_error("Unable to initialize SDL2: " + std::string(SDL_GetError()));
  }

  SDL_Window *window = SDL_CreateWindow("Menu Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
  if (!window)
  {
    throw std::runtime_error("Unable to create window: " + std::string(SDL_GetError()));
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer)
  {
    throw std::runtime_error("Unable to create renderer: " + std::string(SDL_GetError()));
  }

  if (TTF_Init() != 0)
  {
    throw std::runtime_error("Unable to init SDL TTF: " + std::string(TTF_GetError()));
  }

  TTF_Font *font = TTF_OpenFont("KenneyFuture.ttf", SCREEN_HEIGHT / 20);
  if (!font)
  {
    throw std::runtime_error("Unable to load font: " + std::string(TTF_GetError()));
  }

  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  struct GameCtx ctx;
  ctx.running = true;
  SDL_Event event;

  struct Menu *menu = createMenu();

  addMenuItem(menu, "New Game", SCREEN_WIDTH * 0.6, SCREEN_HEIGHT / 8);
  addMenuItem(menu, "Load Game", SCREEN_WIDTH * 0.6, SCREEN_HEIGHT / 8);
  addMenuItem(menu, "Options", SCREEN_WIDTH * 0.6, SCREEN_HEIGHT / 8);
  addMenuItem(menu, "Quit Game", SCREEN_WIDTH * 0.6, SCREEN_HEIGHT / 8);

  setMenuItemClickedCallback(menu, 0, menuItemClicked);
  setMenuItemClickedCallback(menu, 1, menuItemClicked);
  setMenuItemClickedCallback(menu, 2, menuItemClicked);
  setMenuItemClickedCallback(menu, 3, menuItemClicked);

  for (unsigned long i = 0; i < menu->size; i++)
  {
    // position each label in a vertical stack centered on the x axis
    menu->menuItemRects[i * 4 + 0] = (SCREEN_WIDTH - menu->menuItemRects[i * 4 + 2]) * 0.5f;
    menu->menuItemRects[i * 4 + 1] = i * menu->menuItemRects[i * 4 + 3];

    std::cout
        << "[" << i << "] "
        << menu->menuItemLabels[i] << " {"
        << menu->menuItemRects[i * 4 + 0] << ", "
        << menu->menuItemRects[i * 4 + 1] << ", "
        << menu->menuItemRects[i * 4 + 2] << ", "
        << menu->menuItemRects[i * 4 + 3] << "} ("
        << std::to_string(menu->menuItemStates[i]) << ")" << std::endl;
  }

  SDL_Color menuItemLabelColor{255, 255, 255, 255};
  SDL_Color hoverMenuItemLabelColor{255, 255, 0, 255};
  SDL_Color selectedMenuItemLabelColor{0, 255, 255, 255};

  bool wasMouseDown = false;

  while (ctx.running)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        ctx.running = false;
      }
      else if (event.type == SDL_KEYDOWN)
      {
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
          ctx.running = false;
        }
      }
    }

    // get the mouse state - note that the mouse coordinates are unscaled
    int unscaledMouseX = 0;
    int unscaledMouseY = 0;
    Uint32 mouseState = SDL_GetMouseState(&unscaledMouseX, &unscaledMouseY);

    // since we are using a logical size, we must transform the mouse coordinates
    float mouseX = 0.0f;
    float mouseY = 0.0f;
    SDL_RenderWindowToLogical(renderer, unscaledMouseX, unscaledMouseY, &mouseX, &mouseY);

    // if the left mouse button is pressed this frame
    bool isMouseDown = mouseState & SDL_BUTTON(SDL_BUTTON_LEFT);

    // didClick is our per-frame mouse-pressed-and-released event state
    bool didClick = false;
    if (isMouseDown && !wasMouseDown)
    {
      wasMouseDown = true;
    }
    else if (!isMouseDown && wasMouseDown)
    {
      wasMouseDown = false;
      didClick = true;
    }

    // update the menu state
    menu->selectedItem = 0;
    for (unsigned long i = 0; i < menu->size; i++)
    {
      SDL_Rect rect{
          (int)menu->menuItemRects[i * 4 + 0],
          (int)menu->menuItemRects[i * 4 + 1],
          (int)menu->menuItemRects[i * 4 + 2],
          (int)menu->menuItemRects[i * 4 + 3]};
      bool isHovered = (mouseX > rect.x && mouseY > rect.y && mouseX < rect.x + rect.w && mouseY < rect.y + rect.h);

      if (isHovered)
      {
        // when we hover, we "select" the menu item and set the hover state
        menu->menuItemStates[i] |= MENU_HOVER_STATE;
        // remember that the select item is the index plus one
        menu->selectedItem = i + 1;
      }
      else
      {
        // unset the menu hover state
        menu->menuItemStates[i] &= ~MENU_HOVER_STATE;
      }
    }

    // if we did click and there is a selected menu item
    if (didClick && menu->selectedItem != 0)
    {
      // remember that the select item is the index plus one
      // so we must subtract one to get the array index
      unsigned long index = menu->selectedItem - 1;
      // if there is a clicked callback set for the menu item
      if (menu->menuItemCallbacks[index].clicked)
      {
        // call the menu item clicked callback function
        // note that we pass a void pointer to the game context
        menu->menuItemCallbacks[index].clicked(menu, index, (void *)&ctx);
      }
    }

    SDL_SetRenderDrawColor(renderer, 0x30, 0x60, 0x90, 0xFF);
    SDL_RenderClear(renderer);

    // render the menu
    for (unsigned long i = 0; i < menu->size; i++)
    {
      bool isSelected = menu->selectedItem == i + 1;
      bool isHovered = menu->menuItemStates[i] & MENU_HOVER_STATE;

      // determine the colors by the selected and hover state
      SDL_Color *labelColor = &menuItemLabelColor;
      if (isHovered)
      {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x60, 0xFF);
        labelColor = &hoverMenuItemLabelColor;
        if (isMouseDown && isSelected)
        {
          SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x90, 0xFF);
          labelColor = &selectedMenuItemLabelColor;
        }
      }
      else
      {
        SDL_SetRenderDrawColor(renderer, 0x30, 0x30, 0x30, 0xFF);
      }

      const char *label = (const char *)menu->menuItemLabels[i];
      SDL_Surface *labelSurface = TTF_RenderUTF8_Blended(font, label, *labelColor);
      SDL_Texture *labelTexture = SDL_CreateTextureFromSurface(renderer, labelSurface);
      int labelTextureWidth = labelSurface->w;
      int labelTextureHeight = labelSurface->h;
      SDL_FreeSurface(labelSurface);
      SDL_FRect rect{
          menu->menuItemRects[i * 4 + 0],
          menu->menuItemRects[i * 4 + 1],
          menu->menuItemRects[i * 4 + 2],
          menu->menuItemRects[i * 4 + 3]};

      SDL_Rect labelRect{
          (int)(rect.x + (rect.w - labelTextureWidth) * 0.5f),
          (int)(rect.y + (rect.h - labelTextureHeight) * 0.5f),
          labelTextureWidth,
          labelTextureHeight};
      SDL_RenderFillRectF(renderer, &rect);
      SDL_RenderCopy(renderer, labelTexture, 0, &labelRect);
      SDL_DestroyTexture(labelTexture);
    }

    SDL_RenderPresent(renderer);
  }

  destroyMenu(menu);

  TTF_CloseFont(font);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
