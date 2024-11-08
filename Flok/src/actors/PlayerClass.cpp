﻿#include "PlayerClass.h"

#include "raylib.h"

#include "Constants.h"
#include "engine/Collisions.h"
#include "engine/Math.h"

namespace {

constexpr float k_SpeedLimit = 5.0F;
constexpr float k_FramesHop = 7.0F;
constexpr int k_Scale = 2;

PlayerClass::PlayerType Player;
}



void PlayerClass::Init() {
  Player.f_BoundingBox = {.x = 51.0F, .y = g_ScreenHeight / 2.0F, .width = 48.0F, .height = 48.0F};
  Player.f_Direction = {.x = 1, .y = 0};
  Player.f_Sprite = LoadTexture("res/ToxicFrog/BlueBlue/ToxicFrogBlueBlue_Hop.png");
  Player.f_FrameTimer = 0;
}



void PlayerClass::Unload() {
  if (IsTextureReady(Player.f_Sprite)) {
    UnloadTexture(Player.f_Sprite);
  }
}



void PlayerClass::Push(const Vector2& Direction, const float Force) {

  Vector2 Temp = Math::Multiply(Direction, Force * GetFrameTime());
  Temp = Math::Add(Player.f_Direction, Temp);

  // TODO
  //  Fix rotation so if it is aiming down, it remains looking forward
  //  Maybe use Player.f_Direction.x *= -1 if y is positive
  Player.f_Direction = Math::Normalize(Temp, Math::GetMag(Temp));

  if (Player.f_Speed < k_SpeedLimit) {
    Player.f_Speed += Force * GetFrameTime();
  } else {
    Player.f_Speed = k_SpeedLimit;
  }
}



void PlayerClass::Update() {
  Push({.x = 0, .y = 1}, g_Gravity);

  if (!Collisions::IsRectBorder(Player.f_BoundingBox)) {
    Player.f_BoundingBox.y += Player.f_Direction.y * Player.f_Speed;
  }

  Player.f_FrameTimer += GetFrameTime();
  if (Player.f_FrameTimer >= g_Second / k_FramesHop) {
    Player.f_FrameTimer = 0;
    Player.f_Frame++;

    if (Player.f_Frame > k_FramesHop) {
      Player.f_Frame = 0;
    }

  }
}



void PlayerClass::Draw() {
  DrawRectangleLinesEx(Player.f_BoundingBox, 2,GREEN);
  DrawTexturePro(Player.f_Sprite, {.x = 48 * (Player.f_Frame * 2 + 1), .y = 8, .width = 48, .height = 32},
                 {.x = Player.f_BoundingBox.x + Player.f_BoundingBox.width / 2.0F,
                  .y = Player.f_BoundingBox.y + Player.f_BoundingBox.height / 2.0F,
                  .width = Player.f_BoundingBox.width * k_Scale,
                  .height = Player.f_BoundingBox.height * k_Scale},
                 {.x = Player.f_BoundingBox.width * k_Scale / 2.0F, .y = Player.f_BoundingBox.height * k_Scale / 2.0F},
                 Math::GetRotation(Player.f_Direction) - 90.0F, WHITE);
}



Rectangle PlayerClass::GetBoundingBox() {
  return Player.f_BoundingBox;
}



void PlayerClass::MovePlayer(const float Height) {
  Player.f_BoundingBox.y += Height;
}
