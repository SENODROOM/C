#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <conio.h> // For Windows console input
#include <windows.h> // For Windows timing and console

// =============================================================================
// BASIC GAME ENGINE COMPONENTS
// =============================================================================

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define MAX_ENTITIES 100
#define FPS 60
#define FRAME_TIME (1000 / FPS)

// 2D Vector structure
typedef struct {
    float x, y;
} Vector2;

// 2D Rectangle structure
typedef struct {
    float x, y, width, height;
} Rectangle;

// Color structure
typedef struct {
    unsigned char r, g, b, a;
} Color;

// Entity structure
typedef struct {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    Rectangle bounds;
    Color color;
    int active;
    int type;
} Entity;

// Game state
typedef struct {
    Entity entities[MAX_ENTITIES];
    int entity_count;
    int score;
    int game_over;
    int paused;
    DWORD last_time;
} GameState;

GameState game;

// =============================================================================
// VECTOR MATH OPERATIONS
// =============================================================================

Vector2 vector2Create(float x, float y) {
    Vector2 v = {x, y};
    return v;
}

Vector2 vector2Add(Vector2 a, Vector2 b) {
    return vector2Create(a.x + b.x, a.y + b.y);
}

Vector2 vector2Subtract(Vector2 a, Vector2 b) {
    return vector2Create(a.x - b.x, a.y - b.y);
}

Vector2 vector2Multiply(Vector2 v, float scalar) {
    return vector2Create(v.x * scalar, v.y * scalar);
}

float vector2Magnitude(Vector2 v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

Vector2 vector2Normalize(Vector2 v) {
    float mag = vector2Magnitude(v);
    if (mag > 0) {
        return vector2Create(v.x / mag, v.y / mag);
    }
    return vector2Create(0, 0);
}

float vector2Dot(Vector2 a, Vector2 b) {
    return a.x * b.x + a.y * b.y;
}

float vector2Distance(Vector2 a, Vector2 b) {
    return vector2Magnitude(vector2Subtract(a, b));
}

// =============================================================================
// COLLISION DETECTION
// =============================================================================

int rectangleCollision(Rectangle a, Rectangle b) {
    return (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
}

int circleCollision(Vector2 center1, float radius1, Vector2 center2, float radius2) {
    float distance = vector2Distance(center1, center2);
    return distance <= (radius1 + radius2);
}

int pointInRectangle(Vector2 point, Rectangle rect) {
    return (point.x >= rect.x &&
            point.x <= rect.x + rect.width &&
            point.y >= rect.y &&
            point.y <= rect.y + rect.height);
}

// =============================================================================
// ENTITY MANAGEMENT
// =============================================================================

void initEntity(Entity* entity, Vector2 position, Vector2 size, Color color, int type) {
    entity->position = position;
    entity->velocity = vector2Create(0, 0);
    entity->acceleration = vector2Create(0, 0);
    entity->bounds.x = position.x;
    entity->bounds.y = position.y;
    entity->bounds.width = size.x;
    entity->bounds.height = size.y;
    entity->color = color;
    entity->active = 1;
    entity->type = type;
}

Entity* createEntity(Vector2 position, Vector2 size, Color color, int type) {
    if (game.entity_count >= MAX_ENTITIES) {
        return NULL;
    }
    
    Entity* entity = &game.entities[game.entity_count];
    initEntity(entity, position, size, color, type);
    game.entity_count++;
    
    return entity;
}

void updateEntity(Entity* entity, float dt) {
    if (!entity->active) return;
    
    // Update physics
    entity->velocity = vector2Add(entity->velocity, vector2Multiply(entity->acceleration, dt));
    entity->position = vector2Add(entity->position, vector2Multiply(entity->velocity, dt));
    
    // Update bounds
    entity->bounds.x = entity->position.x;
    entity->bounds.y = entity->position.y;
}

void destroyEntity(Entity* entity) {
    entity->active = 0;
}

// =============================================================================
// RENDERING SYSTEM
// =============================================================================

void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setColor(Color color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int color_code = (color.r > 128 ? 4 : 0) + 
                    (color.g > 128 ? 2 : 0) + 
                    (color.b > 128 ? 1 : 0);
    SetConsoleTextAttribute(hConsole, color_code);
}

void drawRectangle(Rectangle rect, Color color, char fill_char) {
    setColor(color);
    
    for (int y = (int)rect.y; y < (int)(rect.y + rect.height) && y < SCREEN_HEIGHT; y++) {
        for (int x = (int)rect.x; x < (int)(rect.x + rect.width) && x < SCREEN_WIDTH; x++) {
            setCursorPosition(x, y);
            printf("%c", fill_char);
        }
    }
}

void drawEntity(Entity* entity) {
    if (!entity->active) return;
    
    drawRectangle(entity->bounds, entity->color, '#');
}

void clearScreen() {
    system("cls");
}

// =============================================================================
// INPUT HANDLING
// =============================================================================

typedef struct {
    int up, down, left, right, space, escape;
} InputState;

InputState input;

void updateInput() {
    input.up = GetAsyncKeyState(VK_UP) & 0x8000;
    input.down = GetAsyncKeyState(VK_DOWN) & 0x8000;
    input.left = GetAsyncKeyState(VK_LEFT) & 0x8000;
    input.right = GetAsyncKeyState(VK_RIGHT) & 0x8000;
    input.space = GetAsyncKeyState(VK_SPACE) & 0x8000;
    input.escape = GetAsyncKeyState(VK_ESCAPE) & 0x8000;
}

// =============================================================================
// GAME SPECIFIC: PONG GAME
// =============================================================================

typedef struct {
    Entity* paddle1;
    Entity* paddle2;
    Entity* ball;
    int paddle1_score;
    int paddle2_score;
} PongGame;

PongGame pong;

void initPongGame() {
    // Create paddles
    Color white = {255, 255, 255, 255};
    
    pong.paddle1 = createEntity(vector2Create(2, SCREEN_HEIGHT/2 - 3), 
                              vector2Create(1, 6), white, 1);
    pong.paddle2 = createEntity(vector2Create(SCREEN_WIDTH - 3, SCREEN_HEIGHT/2 - 3), 
                              vector2Create(1, 6), white, 1);
    
    // Create ball
    pong.ball = createEntity(vector2Create(SCREEN_WIDTH/2, SCREEN_HEIGHT/2), 
                            vector2Create(1, 1), white, 2);
    
    // Set initial ball velocity
    pong.ball->velocity = vector2Create(5, 3);
    
    pong.paddle1_score = 0;
    pong.paddle2_score = 0;
}

void updatePongGame(float dt) {
    // Update paddle 1 (W/S keys)
    if (GetAsyncKeyState(87) & 0x8000) { // W key
        pong.paddle1->velocity.y = -10;
    } else if (GetAsyncKeyState(83) & 0x8000) { // S key
        pong.paddle1->velocity.y = 10;
    } else {
        pong.paddle1->velocity.y = 0;
    }
    
    // Update paddle 2 (Arrow keys)
    if (input.up) {
        pong.paddle2->velocity.y = -10;
    } else if (input.down) {
        pong.paddle2->velocity.y = 10;
    } else {
        pong.paddle2->velocity.y = 0;
    }
    
    // Update paddles
    updateEntity(pong.paddle1, dt);
    updateEntity(pong.paddle2, dt);
    
    // Keep paddles on screen
    if (pong.paddle1->position.y < 0) pong.paddle1->position.y = 0;
    if (pong.paddle1->position.y > SCREEN_HEIGHT - 6) pong.paddle1->position.y = SCREEN_HEIGHT - 6;
    if (pong.paddle2->position.y < 0) pong.paddle2->position.y = 0;
    if (pong.paddle2->position.y > SCREEN_HEIGHT - 6) pong.paddle2->position.y = SCREEN_HEIGHT - 6;
    
    // Update ball
    updateEntity(pong.ball, dt);
    
    // Ball collision with top/bottom walls
    if (pong.ball->position.y <= 0 || pong.ball->position.y >= SCREEN_HEIGHT - 1) {
        pong.ball->velocity.y = -pong.ball->velocity.y;
        if (pong.ball->position.y <= 0) pong.ball->position.y = 0;
        if (pong.ball->position.y >= SCREEN_HEIGHT - 1) pong.ball->position.y = SCREEN_HEIGHT - 1;
    }
    
    // Ball collision with paddles
    if (rectangleCollision(pong.ball->bounds, pong.paddle1->bounds)) {
        pong.ball->velocity.x = fabs(pong.ball->velocity.x);
        // Add some spin based on paddle hit position
        float hit_pos = (pong.ball->position.y - pong.paddle1->position.y) / pong.paddle1->bounds.height;
        pong.ball->velocity.y = 10 * (hit_pos - 0.5);
    }
    
    if (rectangleCollision(pong.ball->bounds, pong.paddle2->bounds)) {
        pong.ball->velocity.x = -fabs(pong.ball->velocity.x);
        // Add some spin based on paddle hit position
        float hit_pos = (pong.ball->position.y - pong.paddle2->position.y) / pong.paddle2->bounds.height;
        pong.ball->velocity.y = 10 * (hit_pos - 0.5);
    }
    
    // Ball out of bounds (scoring)
    if (pong.ball->position.x < 0) {
        pong.paddle2_score++;
        pong.ball->position = vector2Create(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        pong.ball->velocity = vector2Create(-5, (rand() % 10 - 5));
    }
    
    if (pong.ball->position.x > SCREEN_WIDTH) {
        pong.paddle1_score++;
        pong.ball->position = vector2Create(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        pong.ball->velocity = vector2Create(5, (rand() % 10 - 5));
    }
}

void drawPongGame() {
    // Draw center line
    Color white = {255, 255, 255, 255};
    for (int i = 0; i < SCREEN_HEIGHT; i += 2) {
        setCursorPosition(SCREEN_WIDTH/2, i);
        setColor(white);
        printf("|");
    }
    
    // Draw scores
    setCursorPosition(SCREEN_WIDTH/2 - 10, 0);
    printf("Player 1: %d", pong.paddle1_score);
    setCursorPosition(SCREEN_WIDTH/2 + 5, 0);
    printf("Player 2: %d", pong.paddle2_score);
    
    // Draw entities
    drawEntity(pong.paddle1);
    drawEntity(pong.paddle2);
    drawEntity(pong.ball);
}

// =============================================================================
// GAME SPECIFIC: SNAKE GAME
// =============================================================================

#define SNAKE_MAX_LENGTH 100
#define GRID_SIZE 20

typedef struct {
    Vector2 segments[SNAKE_MAX_LENGTH];
    int length;
    Vector2 direction;
    Vector2 food;
    int game_over;
} SnakeGame;

SnakeGame snake;

void initSnakeGame() {
    snake.length = 3;
    snake.segments[0] = vector2Create(10, 10);
    snake.segments[1] = vector2Create(9, 10);
    snake.segments[2] = vector2Create(8, 10);
    snake.direction = vector2Create(1, 0);
    snake.game_over = 0;
    
    // Place random food
    snake.food = vector2Create(rand() % GRID_SIZE, rand() % GRID_SIZE);
}

void updateSnakeGame(float dt) {
    if (snake.game_over) return;
    
    static float move_timer = 0;
    move_timer += dt;
    
    // Move snake every 0.2 seconds
    if (move_timer >= 0.2f) {
        move_timer = 0;
        
        // Move snake head
        Vector2 new_head = vector2Add(snake.segments[0], snake.direction);
        
        // Check wall collision
        if (new_head.x < 0 || new_head.x >= GRID_SIZE ||
            new_head.y < 0 || new_head.y >= GRID_SIZE) {
            snake.game_over = 1;
            return;
        }
        
        // Check self collision
        for (int i = 0; i < snake.length; i++) {
            if (vector2Distance(new_head, snake.segments[i]) < 0.1f) {
                snake.game_over = 1;
                return;
            }
        }
        
        // Move segments
        for (int i = snake.length - 1; i > 0; i--) {
            snake.segments[i] = snake.segments[i - 1];
        }
        snake.segments[0] = new_head;
        
        // Check food collision
        if (vector2Distance(snake.segments[0], snake.food) < 0.1f) {
            snake.length++;
            snake.food = vector2Create(rand() % GRID_SIZE, rand() % GRID_SIZE);
        }
    }
    
    // Handle input
    if (input.up && snake.direction.y == 0) {
        snake.direction = vector2Create(0, -1);
    } else if (input.down && snake.direction.y == 0) {
        snake.direction = vector2Create(0, 1);
    } else if (input.left && snake.direction.x == 0) {
        snake.direction = vector2Create(-1, 0);
    } else if (input.right && snake.direction.x == 0) {
        snake.direction = vector2Create(1, 0);
    }
}

void drawSnakeGame() {
    Color white = {255, 255, 255, 255};
    Color green = {0, 255, 0, 255};
    Color red = {255, 0, 0, 255};
    
    // Draw grid
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            setCursorPosition(x * 4 + 2, y * 2 + 3);
            
            // Check if this is snake segment
            int is_snake = 0;
            for (int i = 0; i < snake.length; i++) {
                if ((int)snake.segments[i].x == x && (int)snake.segments[i].y == y) {
                    setColor(green);
                    printf("██");
                    is_snake = 1;
                    break;
                }
            }
            
            // Check if this is food
            if (!is_snake && (int)snake.food.x == x && (int)snake.food.y == y) {
                setColor(red);
                printf("██");
            }
        }
    }
    
    // Draw score
    setCursorPosition(2, 1);
    setColor(white);
    printf("Score: %d", snake.length - 3);
    
    if (snake.game_over) {
        setCursorPosition(SCREEN_WIDTH/2 - 5, SCREEN_HEIGHT/2);
        setColor(red);
        printf("GAME OVER!");
        setCursorPosition(SCREEN_WIDTH/2 - 8, SCREEN_HEIGHT/2 + 1);
        printf("Press R to restart");
    }
}

// =============================================================================
// GAME SPECIFIC: BREAKOUT GAME
// =============================================================================

#define BRICK_ROWS 5
#define BRICK_COLS 10

typedef struct {
    Entity* paddle;
    Entity* ball;
    Entity* bricks[BRICK_ROWS][BRICK_COLS];
    int score;
    int lives;
    int ball_attached;
} BreakoutGame;

BreakoutGame breakout;

void initBreakoutGame() {
    Color white = {255, 255, 255, 255};
    Color red = {255, 0, 0, 255};
    Color orange = {255, 165, 0, 255};
    Color yellow = {255, 255, 0, 255};
    Color green = {0, 255, 0, 255};
    Color blue = {0, 0, 255, 255};
    
    // Create paddle
    breakout.paddle = createEntity(vector2Create(SCREEN_WIDTH/2 - 4, SCREEN_HEIGHT - 3), 
                                vector2Create(8, 1), white, 1);
    
    // Create ball
    breakout.ball = createEntity(vector2Create(SCREEN_WIDTH/2, SCREEN_HEIGHT - 5), 
                              vector2Create(1, 1), white, 2);
    breakout.ball->velocity = vector2Create(3, -3);
    breakout.ball_attached = 1;
    
    // Create bricks
    Color brick_colors[] = {red, orange, yellow, green, blue};
    for (int row = 0; row < BRICK_ROWS; row++) {
        for (int col = 0; col < BRICK_COLS; col++) {
            breakout.bricks[row][col] = createEntity(
                vector2Create(col * 7 + 5, row * 2 + 5),
                vector2Create(6, 1),
                brick_colors[row],
                3
            );
        }
    }
    
    breakout.score = 0;
    breakout.lives = 3;
}

void updateBreakoutGame(float dt) {
    // Update paddle
    if (input.left && breakout.paddle->position.x > 0) {
        breakout.paddle->velocity.x = -8;
    } else if (input.right && breakout.paddle->position.x < SCREEN_WIDTH - 8) {
        breakout.paddle->velocity.x = 8;
    } else {
        breakout.paddle->velocity.x = 0;
    }
    
    updateEntity(breakout.paddle, dt);
    
    // Update ball
    if (breakout.ball_attached) {
        // Ball follows paddle
        breakout.ball->position.x = breakout.paddle->position.x + 3;
        breakout.ball->position.y = breakout.paddle->position.y - 1;
        
        // Release ball with space
        if (input.space) {
            breakout.ball_attached = 0;
            breakout.ball->velocity = vector2Create(3, -3);
        }
    } else {
        updateEntity(breakout.ball, dt);
        
        // Ball collision with walls
        if (breakout.ball->position.x <= 0 || breakout.ball->position.x >= SCREEN_WIDTH - 1) {
            breakout.ball->velocity.x = -breakout.ball->velocity.x;
        }
        if (breakout.ball->position.y <= 0) {
            breakout.ball->velocity.y = -breakout.ball->velocity.y;
        }
        
        // Ball collision with paddle
        if (rectangleCollision(breakout.ball->bounds, breakout.paddle->bounds)) {
            breakout.ball->velocity.y = -fabs(breakout.ball->velocity.y);
            // Add spin based on hit position
            float hit_pos = (breakout.ball->position.x - breakout.paddle->position.x) / breakout.paddle->bounds.width;
            breakout.ball->velocity.x = 8 * (hit_pos - 0.5);
        }
        
        // Ball collision with bricks
        for (int row = 0; row < BRICK_ROWS; row++) {
            for (int col = 0; col < BRICK_COLS; col++) {
                if (breakout.bricks[row][col]->active &&
                    rectangleCollision(breakout.ball->bounds, breakout.bricks[row][col]->bounds)) {
                    
                    breakout.bricks[row][col]->active = 0;
                    breakout.score += 10;
                    
                    // Simple bounce direction
                    breakout.ball->velocity.y = -breakout.ball->velocity.y;
                }
            }
        }
        
        // Ball out of bounds
        if (breakout.ball->position.y >= SCREEN_HEIGHT) {
            breakout.lives--;
            if (breakout.lives > 0) {
                breakout.ball_attached = 1;
                breakout.ball->position = vector2Create(SCREEN_WIDTH/2, SCREEN_HEIGHT - 5);
                breakout.ball->velocity = vector2Create(0, 0);
            } else {
                game.game_over = 1;
            }
        }
    }
}

void drawBreakoutGame() {
    Color white = {255, 255, 255, 255};
    
    // Draw paddle and ball
    drawEntity(breakout.paddle);
    drawEntity(breakout.ball);
    
    // Draw bricks
    for (int row = 0; row < BRICK_ROWS; row++) {
        for (int col = 0; col < BRICK_COLS; col++) {
            if (breakout.bricks[row][col]->active) {
                drawEntity(breakout.bricks[row][col]);
            }
        }
    }
    
    // Draw UI
    setCursorPosition(2, 1);
    setColor(white);
    printf("Score: %d  Lives: %d", breakout.score, breakout.lives);
    
    if (breakout.ball_attached) {
        setCursorPosition(SCREEN_WIDTH/2 - 10, SCREEN_HEIGHT - 2);
        printf("Press SPACE to launch ball");
    }
}

// =============================================================================
// MAIN GAME LOOP
// =============================================================================

int current_game = 0; // 0=Pong, 1=Snake, 2=Breakout

void initGame() {
    game.entity_count = 0;
    game.score = 0;
    game.game_over = 0;
    game.paused = 0;
    game.last_time = GetTickCount();
    
    switch (current_game) {
        case 0:
            initPongGame();
            break;
        case 1:
            initSnakeGame();
            break;
        case 2:
            initBreakoutGame();
            break;
    }
}

void updateGame(float dt) {
    updateInput();
    
    if (input.escape) {
        game.game_over = 1;
        return;
    }
    
    if (input.space && game.game_over) {
        game.game_over = 0;
        initGame();
        return;
    }
    
    if (game.paused || game.game_over) return;
    
    switch (current_game) {
        case 0:
            updatePongGame(dt);
            break;
        case 1:
            updateSnakeGame(dt);
            break;
        case 2:
            updateBreakoutGame(dt);
            break;
    }
}

void drawGame() {
    clearScreen();
    
    switch (current_game) {
        case 0:
            drawPongGame();
            break;
        case 1:
            drawSnakeGame(dt);
            break;
        case 2:
            drawBreakoutGame();
            break;
    }
    
    // Draw instructions
    Color white = {255, 255, 255, 255};
    setColor(white);
    setCursorPosition(2, SCREEN_HEIGHT - 1);
    printf("ESC: Quit | SPACE: Action | Arrow Keys/WASD: Move");
    
    if (game.game_over) {
        setCursorPosition(SCREEN_WIDTH/2 - 10, SCREEN_HEIGHT/2);
        setColor({255, 0, 0, 255});
        printf("GAME OVER! Press SPACE to play again");
    }
}

void gameLoop() {
    initGame();
    
    while (!game.game_over) {
        DWORD current_time = GetTickCount();
        float dt = (current_time - game.last_time) / 1000.0f;
        game.last_time = current_time;
        
        updateGame(dt);
        drawGame();
        
        // Frame rate limiting
        DWORD frame_time = GetTickCount() - current_time;
        if (frame_time < FRAME_TIME) {
            Sleep(FRAME_TIME - frame_time);
        }
    }
}

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstratePong() {
    printf("=== PONG GAME ===\n");
    printf("Player 1: W/S keys\n");
    printf("Player 2: Arrow keys\n");
    printf("Press any key to start...\n");
    getchar();
    
    current_game = 0;
    gameLoop();
}

void demonstrateSnake() {
    printf("=== SNAKE GAME ===\n");
    printf("Arrow keys to move\n");
    printf("Press any key to start...\n");
    getchar();
    
    current_game = 1;
    gameLoop();
}

void demonstrateBreakout() {
    printf("=== BREAKOUT GAME ===\n");
    printf("Arrow keys to move paddle\n");
    printf("SPACE to launch ball\n");
    printf("Press any key to start...\n");
    getchar();
    
    current_game = 2;
    gameLoop();
}

int main() {
    printf("Game Programming Fundamentals\n");
    printf("=============================\n\n");
    
    // Initialize random seed
    srand(time(NULL));
    
    printf("Select a game to play:\n");
    printf("1. Pong\n");
    printf("2. Snake\n");
    printf("3. Breakout\n");
    printf("Enter choice (1-3): ");
    
    int choice;
    scanf("%d", &choice);
    getchar(); // Consume newline
    
    switch (choice) {
        case 1:
            demonstratePong();
            break;
        case 2:
            demonstrateSnake();
            break;
        case 3:
            demonstrateBreakout();
            break;
        default:
            printf("Invalid choice!\n");
            return 1;
    }
    
    printf("\nThanks for playing!\n");
    return 0;
}
