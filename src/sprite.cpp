#include "sprite.hpp"
#include "boundary.hpp"

const int TILE_SIZE = 36;

void sprite::renderSprite(game& game) {
  int windowWidth, windowHeight;
  SDL_GetRendererOutputSize(game.mRenderer, &windowWidth, &windowHeight);

  float offsetX = game.mViewport.mTransform.position.x;
  float offsetY = game.mViewport.mTransform.position.y;
  for (auto entity : game.mWorld) {
    auto transform_val = entity->try_get<transform>();
    if (transform_val == nullptr) continue;
    auto sprite_val = entity->try_get<sprite>();
    if (sprite_val == nullptr) continue;
    auto sprite_tex = game.mTileResourceManager.get(sprite_val->tileset).value();
    if (!sprite_tex.has_value()) continue;
    auto boundary_val = entity->try_get<boundary>();
    SDL_Rect rect;
    rect.x = static_cast<int>(std::roundf((transform_val->position.x - offsetX) * 36.0));
    rect.y = static_cast<int>(std::roundf((transform_val->position.y - offsetY) * 36.0));
    rect.w = 36;
    rect.h = 36;
    if (boundary_val != nullptr) {
      rect.x -= boundary_val->rect.min.x * 36;
      rect.y -= boundary_val->rect.min.y * 36;
      rect.w = boundary_val->rect.max.x * 36;
      rect.h = boundary_val->rect.max.y * 36;
    }
    SDL_Rect srcRect;
    srcRect.x = (sprite_val->id % 20) * 18;
    srcRect.y = (sprite_val->id / 20) * 18;
    srcRect.w = 18;
    srcRect.h = 18;
    SDL_RenderCopy(game.mRenderer, sprite_tex.value(), &srcRect, &rect);
  }
}

