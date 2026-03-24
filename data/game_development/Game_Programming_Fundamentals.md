# Game Programming Fundamentals

This file contains comprehensive game programming examples in C, including basic game engine components, collision detection, entity management, and three complete games: Pong, Snake, and Breakout.

## 📚 Game Programming Overview

### 🎮 Game Components
- **Game Loop**: Core game execution cycle
- **Entity Management**: Object-oriented game entities
- **Physics**: Movement, collision, and response
- **Rendering**: Visual representation
- **Input**: User interaction handling

### 🔧 Core Systems
- **Vector Math**: 2D mathematical operations
- **Collision Detection**: Object intersection testing
- **State Management**: Game state tracking
- **Timing**: Frame rate and animation control

## 🎮 Game Engine Architecture

### Game Loop Structure
```c
void gameLoop() {
    initGame();
    
    while (!game.game_over) {
        float dt = calculateDeltaTime();
        
        updateGame(dt);
        renderGame();
        
        limitFrameRate();
    }
}
```

**Components**:
- **Initialization**: Set up game state
- **Update**: Process game logic
- **Render**: Draw game elements
- **Timing**: Control frame rate

### Delta Time Calculation
```c
DWORD current_time = GetTickCount();
float dt = (current_time - game.last_time) / 1000.0f;
game.last_time = current_time;
```

**Benefits**:
- Frame-rate independent physics
- Consistent game speed
- Smooth animations

## 🔢 Vector Mathematics

### 2D Vector Structure
```c
typedef struct {
    float x, y;
} Vector2;
```

### Vector Operations
```c
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
```

**Applications**:
- **Position**: Object location
- **Velocity**: Movement speed and direction
- **Acceleration**: Change in velocity
- **Force**: Physics calculations

## 🎯 Entity Management

### Entity Structure
```c
typedef struct {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    Rectangle bounds;
    Color color;
    int active;
    int type;
} Entity;
```

### Entity Operations
```c
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

void updateEntity(Entity* entity, float dt) {
    if (!entity->active) return;
    
    // Update physics
    entity->velocity = vector2Add(entity->velocity, vector2Multiply(entity->acceleration, dt));
    entity->position = vector2Add(entity->position, vector2Multiply(entity->velocity, dt));
    
    // Update bounds
    entity->bounds.x = entity->position.x;
    entity->bounds.y = entity->position.y;
}
```

### Entity Pool Management
```c
#define MAX_ENTITIES 100

Entity entities[MAX_ENTITIES];
int entity_count = 0;

Entity* createEntity(Vector2 position, Vector2 size, Color color, int type) {
    if (entity_count >= MAX_ENTITIES) {
        return NULL;
    }
    
    Entity* entity = &entities[entity_count];
    initEntity(entity, position, size, color, type);
    entity_count++;
    
    return entity;
}

void destroyEntity(Entity* entity) {
    entity->active = 0;
}
```

## 💥 Collision Detection

### Rectangle Collision
```c
typedef struct {
    float x, y, width, height;
} Rectangle;

int rectangleCollision(Rectangle a, Rectangle b) {
    return (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
}
```

### Circle Collision
```c
int circleCollision(Vector2 center1, float radius1, Vector2 center2, float radius2) {
    float distance = vector2Distance(center1, center2);
    return distance <= (radius1 + radius2);
}
```

### Point in Rectangle
```c
int pointInRectangle(Vector2 point, Rectangle rect) {
    return (point.x >= rect.x &&
            point.x <= rect.x + rect.width &&
            point.y >= rect.y &&
            point.y <= rect.y + rect.height);
}
```

### Collision Response
```c
void handleBallPaddleCollision(Entity* ball, Entity* paddle) {
    if (rectangleCollision(ball->bounds, paddle->bounds)) {
        // Reverse ball X velocity
        ball->velocity.x = -ball->velocity.x;
        
        // Add spin based on hit position
        float hit_pos = (ball->position.y - paddle->position.y) / paddle->bounds.height;
        ball->velocity.y = 10 * (hit_pos - 0.5);
    }
}
```

## 🎨 Rendering System

### Console Rendering
```c
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
    
    for (int y = (int)rect.y; y < (int)(rect.y + rect.height); y++) {
        for (int x = (int)rect.x; x < (int)(rect.x + rect.width); x++) {
            setCursorPosition(x, y);
            printf("%c", fill_char);
        }
    }
}
```

### Entity Rendering
```c
void drawEntity(Entity* entity) {
    if (!entity->active) return;
    
    drawRectangle(entity->bounds, entity->color, '#');
}
```

### Screen Management
```c
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

void clearScreen() {
    system("cls");
}

void drawBorder() {
    Color white = {255, 255, 255, 255};
    setColor(white);
    
    // Draw top and bottom borders
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        setCursorPosition(x, 0);
        printf("#");
        setCursorPosition(x, SCREEN_HEIGHT - 1);
        printf("#");
    }
    
    // Draw left and right borders
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        setCursorPosition(0, y);
        printf("#");
        setCursorPosition(SCREEN_WIDTH - 1, y);
        printf("#");
    }
}
```

## ⌨️ Input Handling

### Input State Structure
```c
typedef struct {
    int up, down, left, right, space, escape;
} InputState;

InputState input;
```

### Input Update
```c
void updateInput() {
    input.up = GetAsyncKeyState(VK_UP) & 0x8000;
    input.down = GetAsyncKeyState(VK_DOWN) & 0x8000;
    input.left = GetAsyncKeyState(VK_LEFT) & 0x8000;
    input.right = GetAsyncKeyState(VK_RIGHT) & 0x8000;
    input.space = GetAsyncKeyState(VK_SPACE) & 0x8000;
    input.escape = GetAsyncKeyState(VK_ESCAPE) & 0x8000;
}
```

### Input Processing
```c
void processInput() {
    if (input.up) {
        player->velocity.y = -5;
    } else if (input.down) {
        player->velocity.y = 5;
    } else {
        player->velocity.y = 0;
    }
    
    if (input.left) {
        player->velocity.x = -5;
    } else if (input.right) {
        player->velocity.x = 5;
    } else {
        player->velocity.x = 0;
    }
}
```

## 🎮 Game Implementations

### Pong Game
```c
typedef struct {
    Entity* paddle1;
    Entity* paddle2;
    Entity* ball;
    int paddle1_score;
    int paddle2_score;
} PongGame;

void updatePongGame(float dt) {
    // Update paddles with input
    if (GetAsyncKeyState(87) & 0x8000) { // W key
        pong.paddle1->velocity.y = -10;
    } else if (GetAsyncKeyState(83) & 0x8000) { // S key
        pong.paddle1->velocity.y = 10;
    }
    
    // Update ball physics
    updateEntity(pong.ball, dt);
    
    // Ball collision with walls
    if (pong.ball->position.y <= 0 || pong.ball->position.y >= SCREEN_HEIGHT - 1) {
        pong.ball->velocity.y = -pong.ball->velocity.y;
    }
    
    // Ball collision with paddles
    handleBallPaddleCollision(pong.ball, pong.paddle1);
    handleBallPaddleCollision(pong.ball, pong.paddle2);
    
    // Scoring
    if (pong.ball->position.x < 0) {
        pong.paddle2_score++;
        resetBall();
    } else if (pong.ball->position.x > SCREEN_WIDTH) {
        pong.paddle1_score++;
        resetBall();
    }
}
```

### Snake Game
```c
#define SNAKE_MAX_LENGTH 100

typedef struct {
    Vector2 segments[SNAKE_MAX_LENGTH];
    int length;
    Vector2 direction;
    Vector2 food;
    int game_over;
} SnakeGame;

void updateSnakeGame(float dt) {
    static float move_timer = 0;
    move_timer += dt;
    
    // Move snake every 0.2 seconds
    if (move_timer >= 0.2f) {
        move_timer = 0;
        
        // Calculate new head position
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
            placeFood();
        }
    }
    
    // Handle direction input
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
```

### Breakout Game
```c
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
        
        if (input.space) {
            breakout.ball_attached = 0;
            breakout.ball->velocity = vector2Create(3, -3);
        }
    } else {
        updateEntity(breakout.ball, dt);
        
        // Ball collision with bricks
        for (int row = 0; row < BRICK_ROWS; row++) {
            for (int col = 0; col < BRICK_COLS; col++) {
                if (breakout.bricks[row][col]->active &&
                    rectangleCollision(breakout.ball->bounds, breakout.bricks[row][col]->bounds)) {
                    
                    breakout.bricks[row][col]->active = 0;
                    breakout.score += 10;
                    breakout.ball->velocity.y = -breakout.ball->velocity.y;
                }
            }
        }
    }
}
```

## 🕐 Timing and Frame Rate

### Frame Rate Control
```c
#define FPS 60
#define FRAME_TIME (1000 / FPS)

void limitFrameRate() {
    DWORD frame_time = GetTickCount() - current_time;
    if (frame_time < FRAME_TIME) {
        Sleep(FRAME_TIME - frame_time);
    }
}
```

### Animation Timing
```c
typedef struct {
    float current_time;
    float duration;
    int playing;
} Animation;

void updateAnimation(Animation* anim, float dt) {
    if (anim->playing) {
        anim->current_time += dt;
        if (anim->current_time >= anim->duration) {
            anim->current_time = 0;
            // Animation complete
        }
    }
}
```

## 💡 Advanced Topics

### 1. Component System
```c
typedef enum {
    COMPONENT_POSITION,
    COMPONENT_VELOCITY,
    COMPONENT_RENDER,
    COMPONENT_COLLISION
} ComponentType;

typedef struct {
    ComponentType type;
    void* data;
} Component;

typedef struct {
    Component components[16];
    int component_count;
} GameObject;
```

### 2. Scene Management
```c
typedef struct {
    GameObject* objects[MAX_OBJECTS];
    int object_count;
    void (*update)(float dt);
    void (*render)(void);
} Scene;

void changeScene(Scene* new_scene) {
    current_scene = new_scene;
    new_scene->update(0); // Initialize
}
```

### 3. Resource Management
```c
typedef struct {
    char* data;
    int width, height;
} Texture;

Texture* loadTexture(const char* filename) {
    // Load image file
    // Create texture structure
    return texture;
}
```

### 4. Audio System
```c
typedef struct {
    char* filename;
    int playing;
    int volume;
} Sound;

void playSound(Sound* sound) {
    // Play audio file
    sound->playing = 1;
}
```

## 📊 Performance Optimization

### 1. Spatial Partitioning
```c
typedef struct {
    Rectangle bounds;
    GameObject* objects[MAX_OBJECTS_PER_CELL];
    int object_count;
} GridCell;

GridCell grid[GRID_WIDTH][GRID_HEIGHT];

void updateSpatialGrid() {
    // Clear grid
    // Assign objects to cells based on position
}
```

### 2. Object Pooling
```c
typedef struct {
    Entity pool[MAX_ENTITIES];
    int available[MAX_ENTITIES];
    int available_count;
} EntityPool;

Entity* getEntityFromPool(EntityPool* pool) {
    if (pool->available_count > 0) {
        int index = pool->available[--pool->available_count];
        return &pool->pool[index];
    }
    return NULL;
}
```

### 3. Culling
```c
void renderVisibleObjects() {
    for (int i = 0; i < entity_count; i++) {
        if (isEntityVisible(&entities[i])) {
            drawEntity(&entities[i]);
        }
    }
}
```

## ⚠️ Common Pitfalls

### 1. Frame Rate Dependency
```c
// Wrong - Movement depends on frame rate
player->position.x += 5;

// Right - Frame rate independent movement
player->position.x += 5 * dt;
```

### 2. Memory Leaks
```c
// Wrong - Forgetting to free entities
Entity* entity = createEntity(position, size, color, type);
// Use entity but forget to clean up

// Right - Proper cleanup
Entity* entity = createEntity(position, size, color, type);
// Use entity...
destroyEntity(entity);
```

### 3. Collision Order
```c
// Wrong - Collision depends on update order
updatePlayer();
updateBalls();
checkCollisions(); // Player might miss collision

// Right - Separate physics and collision phases
updatePhysics();
checkCollisions();
```

### 4. Input Lag
```c
// Wrong - Input processed after movement
updateMovement();
processInput();

// Right - Input processed before movement
processInput();
updateMovement();
```

## 🔧 Real-World Applications

### 1. Platform Games
```c
void updatePlatformPhysics(Entity* player, float dt) {
    // Apply gravity
    player->velocity.y += GRAVITY * dt;
    
    // Check platform collisions
    for (int i = 0; i < platform_count; i++) {
        if (checkPlatformCollision(player, platforms[i])) {
            resolvePlatformCollision(player, platforms[i]);
        }
    }
}
```

### 2. Puzzle Games
```c
int checkWinCondition() {
    for (int i = 0; i < puzzle_pieces; i++) {
        if (!isPieceInCorrectPosition(&pieces[i])) {
            return 0;
        }
    }
    return 1;
}
```

### 3. RPG Systems
```c
typedef struct {
    int health, mana, level;
    int strength, defense, magic;
    char name[50];
} Character;

void updateCharacter(Character* char, float dt) {
    // Regenerate mana
    char->mana += char->magic * dt;
    if (char->mana > char->max_mana) {
        char->mana = char->max_mana;
    }
}
```

### 4. Strategy Games
```c
typedef struct {
    Vector2 position;
    int health;
    int attack_power;
    int team;
    int selected;
} Unit;

void updateUnitAI(Unit* unit, float dt) {
    if (unit->team == ENEMY_TEAM) {
        // Simple AI behavior
        Vector2 player_pos = findNearestEnemy(unit);
        moveTowards(unit, player_pos);
        
        if (inAttackRange(unit, player_pos)) {
            attackTarget(unit, player_pos);
        }
    }
}
```

## 🎓 Best Practices

### 1. Game Loop Design
```c
// Keep game loop simple and focused
while (running) {
    processInput();
    update(dt);
    render();
    waitForNextFrame();
}
```

### 2. Data Organization
```c
// Group related data
typedef struct {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
} PhysicsComponent;
```

### 3. Error Handling
```c
// Always check return values
Entity* entity = createEntity(position, size, color, type);
if (entity == NULL) {
    printf("Failed to create entity\n");
    return;
}
```

### 4. Performance Monitoring
```c
// Track frame rate
static int frame_count = 0;
static float fps_timer = 0;

fps_timer += dt;
frame_count++;

if (fps_timer >= 1.0f) {
    printf("FPS: %d\n", frame_count);
    frame_count = 0;
    fps_timer = 0;
}
```

### 5. Modular Design
```c
// Separate concerns into modules
// physics.c - Physics calculations
// rendering.c - Drawing functions
// input.c - Input handling
// audio.c - Sound management
```

Game programming in C provides the foundation for creating engaging interactive experiences. Master these fundamentals to build your own games and game engines!
