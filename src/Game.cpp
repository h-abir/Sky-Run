#include "Game.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#include "stdio.h"
#include <string>
#include "coin.h"
#include <bits/stdc++.h>



const Uint32 Game::COLLISION_DELAY;
Game::Game() : 
    obstacle(1280, 500, 170, 170, 19), 
    coin1(1280, 500, 170, 170, 19),
    keyPressMusic(nullptr), 
    keyPressChunk(nullptr),
    gameOverSound(nullptr), 
    quit(false), 
    score(0), 
    startBackgroundTexture(nullptr),
    startButtonTexture(nullptr),
    win(nullptr), 
    rend(nullptr), 
    backgroundTexture(nullptr), 
    playerTexture(nullptr), 
    obstacleTexture(nullptr), 
    gameBGMusic(nullptr), 
    winWidth(0), 
    winHeight(0), 
    bgX(0), 
    bgSpeed(0), 
    resetThreshold(0), 
    playerX(0), 
    playerY(0), 
    playerWidth(0), 
    playerHeight(0), 
    playerSpeed(0),
    lastFrameTime(0),
    isUpKeyPressed(false),
    isDownKeyPressed(false),
    isStartButtonClicked(false),
    font(nullptr),
   scoreboardTimer(SDL_GetTicks()),
     collisionTime(0),
    collisionDetected(false),
    currentAppState(AppState::MainMenu)
      {
    // Your additional initialization code goes here
}



Game::~Game() {
    Mix_HaltMusic();             // Stop playing music
    Mix_FreeChunk(coinCollisionSound);
    Mix_FreeMusic(keyPressMusic); // Free the key press music
    Mix_FreeChunk(keyPressChunk); // Free the key press chunk
    Mix_FreeMusic(gameBGMusic);   // Free the game background music
    Mix_FreeChunk(gameOverSound); // Free the game over sound
    Mix_CloseAudio();
    SDL_DestroyTexture(startBackgroundTexture);
    SDL_DestroyTexture(startButtonTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(scoreboardTexture);
    SDL_DestroyTexture(obstacleTexture);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(menuBackgroundTexture);
    SDL_DestroyTexture(playButtonTexture);
    SDL_DestroyTexture(leaderBoardTexture);
    SDL_DestroyTexture(instructionsButtonTexture);
    SDL_DestroyTexture(quitButtonTexture);
    SDL_DestroyTexture(titleBoardTexture);
    SDL_DestroyTexture(titleTexture);
    SDL_DestroyTexture(playerNamePromptTexture);
    SDL_DestroyTexture(backButtonTexture);
    SDL_DestroyTexture(instructionBoardTexture);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    IMG_Quit();
    TTF_CloseFont(font);
    SDL_Quit();
}

bool Game::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "SDL Initialization Error: %s\n", SDL_GetError());
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG))) {
        fprintf(stderr, "SDL_image Initialization Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        SDL_Quit();
        return false;
    }
    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_Quit();
        return false;
    }

    printf("Initialization Complete\n");

    win = SDL_CreateWindow("Sky Run", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 960, 0);
    if (!win) {
        fprintf(stderr, "SDL Window Creation Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend) {
        fprintf(stderr, "SDL Renderer Creation Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return false;
    }
    // Load the start window textures in the initialize method
    startBackgroundTexture = loadTexture("res/safe Run.png", rend);
    startButtonTexture = loadTexture("res/start.png", rend);

    if (!startBackgroundTexture || !startButtonTexture) {
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return false;
    }

    backgroundTexture = loadTexture("res/Background.png", rend);
    playerTexture = loadTexture("res/Player.png", rend);
    obstacleTexture = loadTexture("res/Obstacle.png", rend);
    gameOverTexture = loadTexture("res/gameover.png", rend);
    scoreboardTexture = loadTexture("res/pics/board.png",rend);
    menuBackgroundTexture = loadTexture("res/pics/background2.png",rend);
    titleBoardTexture = loadTexture("res/pics/titleBoard.png",rend);
    instructionBoardTexture = loadTexture("res/pics/instructions.png",rend);
 coinTexture = loadTexture("res/pics/coin.png", rend);
    if (!coinTexture) {
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return false;
    }
    if (!backgroundTexture || !playerTexture || !obstacleTexture|| !gameOverTexture) {
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return false;
    }

    // Get the width and height of the window
    SDL_GetWindowSize(win, &winWidth, &winHeight);

    // Initialize variables for background position
    bgX = 0;
    bgSpeed = 20; // Adjust the speed of the scrolling
    resetThreshold = -winWidth; // Reset bgX when it goes beyond this threshold

    // Initialize variables for player position
    playerX = 50;
    playerY = winHeight / 2;
    playerWidth = 250;
    playerHeight = 180;
    playerSpeed = 30;

    // Load and play key press music
    keyPressMusic = Mix_LoadMUS("res/up_down.wav");
    if (!keyPressMusic) {
        fprintf(stderr, "Failed to load key press music! SDL_mixer Error: %s\n", Mix_GetError());
        SDL_Quit();
        return false;
    }
    coinCollisionSound = Mix_LoadWAV("res/pointgain.wav");
    if (!coinCollisionSound) {
    fprintf(stderr, "Failed to load coin collision sound! SDL_mixer Error: %s\n", Mix_GetError());
    SDL_Quit();
    // Handle the error as needed
}


    // Load key press sound as a chunk
    keyPressChunk = Mix_LoadWAV("res/up_down.wav");
    if (!keyPressChunk) {
        fprintf(stderr, "Failed to load key press chunk! SDL_mixer Error: %s\n", Mix_GetError());
        SDL_Quit();
        return false;
    }

    // Load and play game background music
    gameBGMusic = Mix_LoadMUS("res/bgm.mp3");
    if (!gameBGMusic) {
        fprintf(stderr, "Failed to load game background music! SDL_mixer Error: %s\n", Mix_GetError());
        SDL_Quit();
        return false;
    }
    gameOverSound = Mix_LoadWAV("res/die.wav");
    if (!gameOverSound) {
        fprintf(stderr, "Failed to load game over sound! SDL_mixer Error: %s\n", Mix_GetError());
        SDL_Quit();
        return false;
    }
    // Load a TrueType font
    font = TTF_OpenFont("res/Roboto-Black.ttf",200);
    font100 = TTF_OpenFont("res/Roboto-Black.ttf",100);
    font60 = TTF_OpenFont("res/Roboto-Black.ttf",60);
    // Check if the font was loaded successfully
    if (!font) {
        fprintf(stderr, "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    const int BGM_VOLUME = MIX_MAX_VOLUME;      // 100% volume for BGM
    const int KEYPRESS_VOLUME = MIX_MAX_VOLUME / 3;  // 50% volume for keypress sound

    // Set the volume for the background music
    Mix_VolumeMusic(BGM_VOLUME);

    // Set the volume for the keypress sound
    Mix_VolumeChunk(keyPressChunk, KEYPRESS_VOLUME);
    
    Mix_PlayMusic(gameBGMusic, -1); // -1 plays the music indefinitely for the entire game
    playButtonTexture = createTextureFromString("PLAY",{0,0,0,255});
    leaderBoardTexture = createTextureFromString("LEADERBOARD",{0,0,0,255});
    instructionsButtonTexture = createTextureFromString("INSTRUCTIONS",{0,0,0,255});
    quitButtonTexture = createTextureFromString("QUIT",{0,0,0,255});
    titleTexture = createTextureFromString("SAFE RUN",{0,0,0,255});
    playerNamePromptTexture = createTextureFromString("ENTER YOUR NAME: ",{255,255,255,255});
    backButtonTexture = createTextureFromString("< BACK",{0,0,0,255});
    return true;
}

// Inside the Game class in Game.cpp
bool Game::checkCollision(const SDL_Rect &rect1, const SDL_Rect &rect2) {
    return (
        rect1.x + rect1.w >= rect2.x &&
        rect2.x + rect2.w >= rect1.x &&
        rect1.y + rect1.h >= rect2.y &&
        rect2.y + rect2.h >= rect1.y);
}

void Game::run() {
    while (!quit) {
        handleEvents();

        if (currentAppState == AppState::MainMenu) {
            // Render the start window
            renderStartWindow();
            updateStartWindow();
        } 
        else if (currentAppState == AppState::InGame || currentAppState == AppState::GameOverWindow){
            // Render the game
            update();
            render();
        }
        else if(currentAppState == AppState::InstructionMenu)
        {
            // render Instruction Menu
            renderInstructionWindow();
        }
        else if(currentAppState == AppState::LeaderBoardMenu)
        {
            // render Leaderboard Menu
            renderLeaderboardWindow();
        }

        SDL_Delay(16); // Adjust as needed for your target frame rate
    }
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT){
            quit = true;
        }
        else if (e.type == SDL_KEYDOWN) {
            if (currentAppState == AppState::MainMenu) {
                // Handle key events specific to the start window
                switch (e.key.keysym.sym) {
                    // Add handling for key events in the start window if needed
                }
            } else if (currentAppState == AppState::InGame) {
                // Handle key events specific to the game
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        isUpKeyPressed = true;
                        Mix_PlayChannel(-1, keyPressChunk, 0);
                        break;
                    case SDLK_DOWN:
                        isDownKeyPressed = true;
                        Mix_PlayChannel(-1, keyPressChunk, 0);
                        break;
                    case SDLK_ESCAPE:
                        currentAppState = AppState::GameOverWindow;
                        break;
                }
            }
            else if(currentAppState == AppState::GameOverWindow)
            {
                if (e.type == SDL_KEYDOWN) {
                    // Handle backspace
                    if (e.key.keysym.sym == SDLK_BACKSPACE && playerName.size() > 0) {
                        playerName.pop_back();
                    }
                    // Handle other key presses (excluding non-printable characters)
                    else if (e.key.keysym.sym >= SDLK_SPACE && e.key.keysym.sym <= SDLK_z) {
                        char key = (char)e.key.keysym.sym;
                        playerName += key;
                    }
                    else if(e.key.keysym.sym == SDLK_RETURN)
                    {
                        FILE* scoreFile=fopen("data/Scores.dat","a");
                        fprintf(scoreFile,"%d\n%s\n",score,playerName.c_str());
                        fclose(scoreFile);
                        currentAppState = AppState::MainMenu;
                        playerName = "";
                    }
                }
            }
        } else if (e.type == SDL_KEYUP) {
            if (currentAppState == AppState::InGame) {
                // Handle key events specific to the game
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        isUpKeyPressed = false;
                        break;
                    case SDLK_DOWN:
                        isDownKeyPressed = false;
                        break;
                }
            }
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            // printf("%d %d\n",mouseX, mouseY);
            if(currentAppState == AppState::MainMenu)
            {
                    if( !playButtonClicked){
                        if(mouseX >= instructionsButtonRect.x && mouseX <= instructionsButtonRect.x+instructionsButtonRect.w && mouseY >= instructionsButtonRect.y && mouseY <= instructionsButtonRect.y+instructionsButtonRect.h) {
                            currentAppState = AppState::InstructionMenu;
                            
                        }
                        if (mouseX >= startButtonRect.x && mouseX <= startButtonRect.x+startButtonRect.w && mouseY >= startButtonRect.y && mouseY <= startButtonRect.y+startButtonRect.h) {
                            playButtonClicked=true;

                        }
                        if(mouseX >=quitButtonRect.x && mouseX<= quitButtonRect.x+quitButtonRect.w && mouseY >= quitButtonRect.y && mouseY <= quitButtonRect.y+quitButtonRect.h) {
                            quit =true;
                        }
                        if(mouseX >=leaderBoardButtonRect.x && mouseX<= leaderBoardButtonRect.x+leaderBoardButtonRect.w && mouseY >= leaderBoardButtonRect.y && mouseY <= leaderBoardButtonRect.y+leaderBoardButtonRect.h) {
                            currentAppState = AppState::LeaderBoardMenu;
                        }
                    } 

            }
            else if(currentAppState==AppState::InstructionMenu){
                 if(mouseX >= backButtonRect.x && mouseX <= backButtonRect.x+backButtonRect.w && mouseY >= backButtonRect.y && mouseY <= backButtonRect.y+backButtonRect.h) {
                            currentAppState= AppState::MainMenu;
                            
                        }
            }
            else if(currentAppState==AppState::LeaderBoardMenu){
                 if(mouseX >= backButtonRect.x && mouseX <= backButtonRect.x+backButtonRect.w && mouseY >= backButtonRect.y && mouseY <= backButtonRect.y+backButtonRect.h) {
                            currentAppState= AppState::MainMenu;            
                }
            }
        }
    }
    if(currentAppState == AppState::InGame)
    {
        // Update player position based on key states
        if (isUpKeyPressed) {
            playerY -= playerSpeed;
        }
        if (isDownKeyPressed) {
            playerY += playerSpeed;
        }
    }

    // Frame rate limiter
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastFrameTime < FRAME_DELAY) {
        SDL_Delay(static_cast<Uint32>(FRAME_DELAY - (currentTime - lastFrameTime)));
    }
    lastFrameTime = SDL_GetTicks();
}

void Game::updateStartWindow(){
    if(playButtonClicked){
        menuPlayerRect.x += 30;
    }
    if(menuPlayerRect.x>=winWidth){
        playButtonClicked=false;
        currentAppState = AppState::InGame;
        reset();
        menuPlayerRect.x=100;
    }
}

void Game::update() {
    obstacle.move();
 coin1.move();  // Update the coin
    bgX -= bgSpeed;

    if (bgX <= resetThreshold) {
        bgX = 0;
    }
    
    
    // Update the playerRect
    SDL_Rect playerRect = {playerX, playerY, playerWidth, playerHeight};

    // Check for collision with the obstacle
    SDL_Rect obstacleRect = {obstacle.x, obstacle.y, obstacle.width, obstacle.height};
    if (checkCollision(playerRect, obstacleRect)) {
        printf("Collision detected!\n");
        Mix_PlayChannel(-1, gameOverSound, 0);  // Play the game over sound
        // No delay and quit assignment here
        collisionDetected = true;
    }
    if (checkCollision(playerRect, {coin1.x, coin1.y, coin1.width, coin1.height})) {
    // Collision detected with the coin

    // Play the coin collision sound
    Mix_PlayChannel(-1, coinCollisionSound, 0);

    // Reset the coin position
    coin1.x = 1280;
    coin1.y = rand() % 700;

    // Increase the score
    score += 10;
}

    if(collisionDetected){
       // Wait for a brief period after collision
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - collisionTime >= COLLISION_DELAY) {
            // quit = true;  // Exit the game after the delay
            currentAppState = AppState::GameOverWindow;
        }
    }
    else {
        // No collision, update score
        ++score;
    }
}

void Game::renderStartWindow() {
    SDL_RenderClear(rend);

    // Render the start window background
    SDL_RenderCopy(rend, menuBackgroundTexture, NULL, NULL);

    // Render the "Start" button
    
    SDL_RenderCopy(rend, playButtonTexture, NULL, &startButtonRect);
    SDL_RenderCopy(rend, leaderBoardTexture, NULL, &leaderBoardButtonRect);
    SDL_RenderCopy(rend, instructionsButtonTexture, NULL, &instructionsButtonRect);
    SDL_RenderCopy(rend, quitButtonTexture, NULL, &quitButtonRect);
    SDL_RenderCopy(rend, titleBoardTexture, NULL, &titleBoardRect);
    SDL_RenderCopy(rend, titleTexture, NULL, &titleRect);
    SDL_RenderCopy(rend, playerTexture, NULL, &menuPlayerRect);
    SDL_RenderPresent(rend);
}

void Game::render() {
    SDL_RenderClear(rend);

    if (currentAppState == AppState::GameOverWindow) {
        // Render the game over window
        SDL_RenderCopy(rend, gameOverTexture, NULL, NULL);

        // Render the scoreboard
        renderScoreboard();
    }
    else if (currentAppState == AppState::InGame) {
        // Render the game background
        for (int i = 0; i < 2; ++i) {
            SDL_Rect bgRect = {bgX + i * winWidth, 0, winWidth, winHeight};
            SDL_RenderCopy(rend, backgroundTexture, NULL, &bgRect);
        }

        // Render the player
        SDL_Rect playerRect = {playerX, playerY, playerWidth, playerHeight};
        SDL_RenderCopy(rend, playerTexture, NULL, &playerRect);

        // Render the coin
        SDL_Rect coinRect = {coin1.x, coin1.y, coin1.width, coin1.height};
        SDL_RenderCopy(rend, coinTexture, NULL, &coinRect);

        // Render the obstacle
        SDL_Rect obstacleRect = {obstacle.x, obstacle.y, obstacle.width, obstacle.height};
        SDL_RenderCopy(rend, obstacleTexture, NULL, &obstacleRect);

        // Render the current score
        renderCurrentScore();
    }

    SDL_RenderPresent(rend);
}


void Game::renderScoreboard() {
    printf("Rendering Scoreboard...\n");

    // Ensure that the font is initialized
    if (!font) {
        fprintf(stderr, "Font not initialized!\n");
        return;
    }

    // Create a surface with the score text
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, ("Score: " + std::to_string(score)).c_str(), textColor);
    if (!scoreSurface) {
        fprintf(stderr, "Failed to render text! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    // Create a texture from the surface
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(rend, scoreSurface);
    SDL_FreeSurface(scoreSurface);

    // Ensure that the texture is created successfully
    if (!scoreTexture) {
        fprintf(stderr, "Failed to create texture from surface! SDL Error: %s\n", SDL_GetError());
        return;
    }

    // Set the position and size of the score text (adjust as needed)
    SDL_Rect scoreRect = {(winWidth-300)/2, winHeight - 350, 300, 60}; // Example position and size
    int promptHeight, promptWidth;
    TTF_SizeText(font60, "ENTER YOUR NAME: ", &promptWidth, &promptHeight);
    SDL_Rect playerNamePromptRect = {(winWidth-promptWidth)/2,winHeight-250,promptWidth,promptHeight};
    SDL_RenderCopy(rend, playerNamePromptTexture, NULL, &playerNamePromptRect);

    int playerNameHeight, playerNameWidth;
    TTF_SizeText(font60, playerName.c_str(), &playerNameWidth, &playerNameHeight);
    SDL_Rect playerNameRect = {(winWidth - playerNameWidth)/2, winHeight - 170, playerNameWidth, playerNameHeight};
    SDL_Texture* playerNameTexture = createTextureFromString(playerName, {255,255,255,255});
    SDL_RenderCopy(rend, playerNameTexture, NULL, &playerNameRect);
    SDL_DestroyTexture(playerNameTexture);
    
    // Render the score texture
    SDL_RenderCopy(rend, scoreTexture, NULL, &scoreRect);

    // Destroy the score texture when it's no longer needed
    SDL_DestroyTexture(scoreTexture);

    scoreboardTimer = SDL_GetTicks();
}
void Game::renderCurrentScore() {
    //printf("Rendering Scoreboard...\n");

    // Ensure that the font is initialized
    if (!font) {
        fprintf(stderr, "Font not initialized!\n");
        return;
    }

    // Create a surface with the score text
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, (std::to_string(score)).c_str(), textColor);
    if (!scoreSurface) {
        fprintf(stderr, "Failed to render text! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    // Create a texture from the surface
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(rend, scoreSurface);
    SDL_FreeSurface(scoreSurface);

    // Ensure that the texture is created successfully
    if (!scoreTexture) {
        fprintf(stderr, "Failed to create texture from surface! SDL Error: %s\n", SDL_GetError());
        return;
    }

    // Set the position and size of the score text (adjust as needed)
    SDL_Rect scoreRect = {900, 50, 200, 50}; // Example position and size
    SDL_Rect scoreboardRect = {850,10,300,130};
    // Render the score texture
    SDL_RenderCopy(rend, scoreboardTexture, NULL, &scoreboardRect);
    SDL_RenderCopy(rend, scoreTexture, NULL, &scoreRect);

    // Destroy the score texture when it's no longer needed
    SDL_DestroyTexture(scoreTexture);

    scoreboardTimer = SDL_GetTicks();
}


SDL_Texture* Game::loadTexture(const char* filePath, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        fprintf(stderr, "SDL Surface Creation Error: %s\n", SDL_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        fprintf(stderr, "SDL Texture Creation Error: %s\n", SDL_GetError());
        return nullptr;
    }

    return texture;
}

SDL_Texture* Game::createTextureFromString(std::string str,SDL_Color textColor){
    // Create a surface with the score text
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, str.c_str(), textColor);
    // TTF_SizeText(font, text, &width, &height)
    // Create a texture from the surface
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(rend, scoreSurface);
    SDL_FreeSurface(scoreSurface);
    return scoreTexture;
}

void Game::reset()
{
    obstacle = Obstacle(1280, 500, 170, 170, 19);
    score = 0;
    // Initialize variables for background position
    bgX = 0;
    bgSpeed = 20; // Adjust the speed of the scrolling
    resetThreshold = -winWidth; // Reset bgX when it goes beyond this threshold

    // Initialize variables for player position
    playerX = 50;
    playerY = winHeight / 2;
    playerWidth = 250;
    playerHeight = 180;
    playerSpeed = 15;
    collisionDetected = false;
}

void Game::renderInstructionWindow(){
    SDL_RenderClear(rend);
    SDL_RenderCopy(rend,menuBackgroundTexture,NULL,NULL);
    SDL_RenderCopy(rend,backButtonTexture,NULL,&backButtonRect);
    SDL_RenderCopy(rend,instructionBoardTexture,NULL,&instructionBoardRect);
    SDL_RenderPresent(rend);
    
}

void Game::renderLeaderboardWindow()
{
    SDL_RenderClear(rend);
    SDL_RenderCopy(rend,menuBackgroundTexture,NULL,NULL);
    SDL_RenderCopy(rend,backButtonTexture,NULL,&backButtonRect);
    std::ifstream in("data/Scores.dat");
    std::vector<std::pair<int,std::string>> v;
    std::string name;
    int point;
    while(in >> point)
    {
        in >> name;
        v.push_back({point,name});
    }
    sort(v.rbegin(), v.rend());

    for(int i = 0, height = 200; i < std::min((int)v.size(),5); i++, height += 120)
    {
        renderRow(v[i].second, v[i].first, height);
    }

    in.close();

    SDL_RenderPresent(rend);
}

void Game::renderRow(std::string name, int num, int height)
{
    int name_width, name_height;
    TTF_SizeText(font60, name.c_str(), &name_width, &name_height);
    SDL_Texture* nameTexture = createTextureFromString(name, {0,0,0,255});
    SDL_Rect rect = {100, height, name_width, name_height};
    SDL_RenderCopy(rend,nameTexture,NULL,&rect);

    int num_width, num_height;
    std::string num_str = std::to_string(num);
    TTF_SizeText(font60, num_str.c_str(), &num_width, &num_height);
    SDL_Texture* numTexture = createTextureFromString(num_str, {0,0,0,255});
    SDL_Rect rect2 = {1180-num_width, height, num_width, num_height};
    SDL_RenderCopy(rend,numTexture,NULL,&rect2);
}