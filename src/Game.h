#ifndef GAME_H
#define GAME_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"  // Include SDL_image for IMG_Load
#include "Obstacle.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#include <string>
#include "coin.h" 

class Game {
public:
    Game();
    ~Game();

    bool initialize();
    void run();
    void renderScoreboard();
    void renderCurrentScore();

private:
    enum class AppState {
        MainMenu,
        InGame,
        LeaderBoardMenu,
        InstructionMenu,
        GameOverWindow,
    };

    AppState currentAppState; 
    TTF_Font* font, *font100, *font60;        // Font for rendering text
    SDL_Texture* startBackgroundTexture;
    SDL_Texture* startButtonTexture;
    SDL_Window* win;
    SDL_Renderer* rend;
    SDL_Texture* backgroundTexture;
    SDL_Texture* playerTexture;
    SDL_Texture* obstacleTexture;
    SDL_Texture* gameOverTexture;
    SDL_Texture* scoreTexture;
    SDL_Texture* scoreboardTexture;
    SDL_Texture* menuBackgroundTexture;
    SDL_Texture* playButtonTexture;
    SDL_Texture* leaderBoardTexture;
    SDL_Texture* instructionsButtonTexture;
    SDL_Texture* quitButtonTexture;
    SDL_Texture* titleBoardTexture;
    SDL_Texture* instructionBoardTexture;
    SDL_Texture* titleTexture;
    SDL_Texture* playerNamePromptTexture;
    SDL_Texture* backButtonTexture;
     SDL_Texture* coinTexture;

    Obstacle obstacle;
    Mix_Chunk* coinCollisionSound;
    Mix_Music* keyPressMusic; // Background music
    Mix_Chunk* keyPressChunk; // Sound effect for key press
    Mix_Music* gameBGMusic;   // Game background music
    Mix_Chunk* gameOverSound;
    SDL_Color textColor = {255, 255, 255, 255};  // White color for text (adjust as needed)
    SDL_Rect startButtonRect ={1010,200,170,100};
    SDL_Rect leaderBoardButtonRect ={730,300,450,100};
    SDL_Rect instructionsButtonRect ={730,400,450,100};
    SDL_Rect quitButtonRect = {1010,500,170,100};
    SDL_Rect titleBoardRect = {330,0,600,200};
    SDL_Rect titleRect = {430,40,400,130};
    SDL_Rect menuPlayerRect = {100,600,300,200};
    SDL_Rect backButtonRect = {50,50,200,100};
    SDL_Rect instructionBoardRect = {100,200,1080,750};
    int winWidth;
    int winHeight;
    int bgX;
    int bgSpeed;
    int resetThreshold;
    int playerX;
    int playerY;
    int playerWidth;
    int playerHeight;
    int playerSpeed;
    int score;

    bool quit;
    bool isUpKeyPressed;
    bool isDownKeyPressed;
    bool isStartButtonClicked;
    bool collisionDetected;
    bool playButtonClicked =false;

    const Uint32 FRAME_DELAY = 16; // Set the desired frame delay in milliseconds
    Uint32 lastFrameTime = 0;
    Uint32 scoreboardTimer;
    const Uint32 SCOREBOARD_DISPLAY_TIME = 7000;
    Uint32 collisionTime;
    static const Uint32 COLLISION_DELAY = 1000; 

    std::string playerName = "";
    Coin coin1;  // Example coin instance
    void renderStartWindow();
    void updateStartWindow();
    void handleEvents();
    void update();
    void render();
    void renderInstructionWindow();
    void renderLeaderboardWindow();
    void reset();
    void renderRow(std::string name, int num, int height); 
    

    // Declare the collision check function
    bool checkCollision(const SDL_Rect& rect1, const SDL_Rect& rect2);

    // Declare the loadTexture function
    SDL_Texture* loadTexture(const char* filePath, SDL_Renderer* renderer);
    SDL_Texture* createTextureFromString(std::string str, SDL_Color textColor);

};

#endif // GAME_H
