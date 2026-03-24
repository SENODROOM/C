#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

// =============================================================================
// GRAPHICS PRIMITIVES
// =============================================================================

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_VERTICES 1000
#define MAX_SHAPES 100

// Color structure
typedef struct {
    unsigned char r, g, b, a;
} Color;

// Point structure
typedef struct {
    float x, y;
} Point;

// 2D Vector structure
typedef struct {
    float x, y;
} Vector2;

// Rectangle structure
typedef struct {
    float x, y, width, height;
} Rectangle;

// Circle structure
typedef struct {
    float x, y, radius;
} Circle;

// Triangle structure
typedef struct {
    Point vertices[3];
    Color color;
} Triangle;

// Line structure
typedef struct {
    Point start, end;
    Color color;
    float thickness;
} Line;

// =============================================================================
// CONSOLE GRAPHICS ENGINE
// =============================================================================

typedef struct {
    HANDLE console;
    CHAR_INFO* buffer;
    COORD bufferSize;
    COORD bufferCoord;
    SMALL_RECT writeRegion;
} ConsoleGraphics;

ConsoleGraphics cg;

// Color palette for console
const Color consolePalette[16] = {
    {0, 0, 0, 255},       // Black
    {128, 0, 0, 255},     // Dark Red
    {0, 128, 0, 255},     // Dark Green
    {128, 128, 0, 255},   // Dark Yellow
    {0, 0, 128, 255},     // Dark Blue
    {128, 0, 128, 255},   // Dark Magenta
    {0, 128, 128, 255},   // Dark Cyan
    {192, 192, 192, 255}, // Light Gray
    {128, 128, 128, 255}, // Dark Gray
    {255, 0, 0, 255},     // Red
    {0, 255, 0, 255},     // Green
    {255, 255, 0, 255},   // Yellow
    {0, 0, 255, 255},     // Blue
    {255, 0, 255, 255},   // Magenta
    {0, 255, 255, 255},   // Cyan
    {255, 255, 255, 255}  // White
};

// Initialize console graphics
void initConsoleGraphics() {
    cg.console = GetStdHandle(STD_OUTPUT_HANDLE);
    cg.bufferSize.X = SCREEN_WIDTH;
    cg.bufferSize.Y = SCREEN_HEIGHT;
    cg.bufferCoord.X = 0;
    cg.bufferCoord.Y = 0;
    cg.writeRegion.Left = 0;
    cg.writeRegion.Top = 0;
    cg.writeRegion.Right = SCREEN_WIDTH - 1;
    cg.writeRegion.Bottom = SCREEN_HEIGHT - 1;
    
    cg.buffer = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * SCREEN_WIDTH * SCREEN_HEIGHT);
    
    // Hide cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(cg.console, &cursorInfo);
    
    // Set console size
    COORD size = {SCREEN_WIDTH, SCREEN_HEIGHT};
    SetConsoleScreenBufferSize(cg.console, size);
    
    SMALL_RECT windowSize = {0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1};
    SetConsoleWindowInfo(cg.console, TRUE, &windowSize);
}

// Clear console buffer
void clearConsoleBuffer(Color color) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int index = y * SCREEN_WIDTH + x;
            cg.buffer[index].Char.AsciiChar = ' ';
            cg.buffer[index].Attributes = 
                (color.r > 128 ? 4 : 0) | 
                (color.g > 128 ? 2 : 0) | 
                (color.b > 128 ? 1 : 0);
        }
    }
}

// Set pixel in console buffer
void setPixel(int x, int y, Color color) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
    
    int index = y * SCREEN_WIDTH + x;
    cg.buffer[index].Char.AsciiChar = 219; // Full block character
    cg.buffer[index].Attributes = 
        (color.r > 128 ? 4 : 0) | 
        (color.g > 128 ? 2 : 0) | 
        (color.b > 128 ? 1 : 0);
}

// Draw line using Bresenham's algorithm
void drawLine(int x1, int y1, int x2, int y2, Color color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    while (1) {
        setPixel(x1, y1, color);
        
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Draw rectangle
void drawRectangle(Rectangle rect, Color color, int filled) {
    if (filled) {
        for (int y = (int)rect.y; y < (int)(rect.y + rect.height); y++) {
            for (int x = (int)rect.x; x < (int)(rect.x + rect.width); x++) {
                setPixel(x, y, color);
            }
        }
    } else {
        // Draw outline
        drawLine((int)rect.x, (int)rect.y, 
                (int)(rect.x + rect.width), (int)rect.y, color);
        drawLine((int)(rect.x + rect.width), (int)rect.y, 
                (int)(rect.x + rect.width), (int)(rect.y + rect.height), color);
        drawLine((int)(rect.x + rect.width), (int)(rect.y + rect.height), 
                (int)rect.x, (int)(rect.y + rect.height), color);
        drawLine((int)rect.x, (int)(rect.y + rect.height), 
                (int)rect.x, (int)rect.y, color);
    }
}

// Draw circle using midpoint algorithm
void drawCircle(Circle circle, Color color, int filled) {
    int x = (int)circle.radius;
    int y = 0;
    int err = 0;
    
    while (x >= y) {
        if (filled) {
            // Draw filled circle by drawing horizontal lines
            for (int i = -x; i <= x; i++) {
                setPixel((int)circle.x + i, (int)circle.y + y, color);
                setPixel((int)circle.x + i, (int)circle.y - y, color);
                setPixel((int)circle.x + y, (int)circle.y + i, color);
                setPixel((int)circle.x - y, (int)circle.y + i, color);
            }
        } else {
            // Draw outline
            setPixel((int)circle.x + x, (int)circle.y + y, color);
            setPixel((int)circle.x - x, (int)circle.y + y, color);
            setPixel((int)circle.x + x, (int)circle.y - y, color);
            setPixel((int)circle.x - x, (int)circle.y - y, color);
            setPixel((int)circle.x + y, (int)circle.y + x, color);
            setPixel((int)circle.x - y, (int)circle.y + x, color);
            setPixel((int)circle.x + y, (int)circle.y - x, color);
            setPixel((int)circle.x - y, (int)circle.y - x, color);
        }
        
        if (err <= 0) {
            y += 1;
            err += 2 * y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

// Draw triangle
void drawTriangle(Triangle triangle, int filled) {
    if (filled) {
        // Use scanline algorithm for filled triangle
        int min_y = (int)fmin(fmin(triangle.vertices[0].y, triangle.vertices[1].y), triangle.vertices[2].y);
        int max_y = (int)fmax(fmax(triangle.vertices[0].y, triangle.vertices[1].y), triangle.vertices[2].y);
        
        for (int y = min_y; y <= max_y; y++) {
            float x1 = -1, x2 = -1;
            
            // Find intersections with scanline
            for (int i = 0; i < 3; i++) {
                int j = (i + 1) % 3;
                if ((triangle.vertices[i].y <= y && triangle.vertices[j].y > y) ||
                    (triangle.vertices[j].y <= y && triangle.vertices[i].y > y)) {
                    float x = triangle.vertices[i].x + 
                             (y - triangle.vertices[i].y) * 
                             (triangle.vertices[j].x - triangle.vertices[i].x) / 
                             (triangle.vertices[j].y - triangle.vertices[i].y);
                    
                    if (x1 == -1) x1 = x;
                    else if (x2 == -1) x2 = x;
                }
            }
            
            if (x1 != -1 && x2 != -1) {
                int start_x = (int)fmin(x1, x2);
                int end_x = (int)fmax(x1, x2);
                
                for (int x = start_x; x <= end_x; x++) {
                    setPixel(x, y, triangle.color);
                }
            }
        }
    } else {
        // Draw outline
        drawLine((int)triangle.vertices[0].x, (int)triangle.vertices[0].y,
                (int)triangle.vertices[1].x, (int)triangle.vertices[1].y, triangle.color);
        drawLine((int)triangle.vertices[1].x, (int)triangle.vertices[1].y,
                (int)triangle.vertices[2].x, (int)triangle.vertices[2].y, triangle.color);
        drawLine((int)triangle.vertices[2].x, (int)triangle.vertices[2].y,
                (int)triangle.vertices[0].x, (int)triangle.vertices[0].y, triangle.color);
    }
}

// Render console buffer
void renderConsole() {
    WriteConsoleOutput(cg.console, cg.buffer, cg.bufferSize, cg.bufferCoord, &cg.writeRegion);
}

// Cleanup console graphics
void cleanupConsoleGraphics() {
    if (cg.buffer) {
        free(cg.buffer);
        cg.buffer = NULL;
    }
}

// =============================================================================
// GUI WIDGETS
// =============================================================================

typedef struct {
    Rectangle bounds;
    char text[256];
    Color bg_color;
    Color text_color;
    int is_visible;
    int is_enabled;
} Button;

typedef struct {
    Rectangle bounds;
    char text[256];
    Color bg_color;
    Color text_color;
    int max_length;
    int is_visible;
    int is_enabled;
} TextBox;

typedef struct {
    Rectangle bounds;
    char* items[50];
    int item_count;
    int selected_index;
    Color bg_color;
    Color text_color;
    Color selected_color;
    int is_visible;
    int is_enabled;
} ListBox;

typedef struct {
    Rectangle bounds;
    int min_value;
    int max_value;
    int current_value;
    Color bg_color;
    Color thumb_color;
    int is_visible;
    int is_enabled;
} ScrollBar;

// =============================================================================
// BUTTON IMPLEMENTATION
// =============================================================================

Button* createButton(float x, float y, float width, float height, const char* text) {
    Button* button = (Button*)malloc(sizeof(Button));
    button->bounds.x = x;
    button->bounds.y = y;
    button->bounds.width = width;
    button->bounds.height = height;
    strcpy(button->text, text);
    button->bg_color = (Color){100, 100, 100, 255};
    button->text_color = (Color){255, 255, 255, 255};
    button->is_visible = 1;
    button->is_enabled = 1;
    return button;
}

void drawButton(Button* button) {
    if (!button->is_visible) return;
    
    // Draw button background
    drawRectangle(button->bounds, button->bg_color, 1);
    
    // Draw button border
    Color border_color = button->is_enabled ? 
                        (Color){200, 200, 200, 255} : 
                        (Color){64, 64, 64, 255};
    drawRectangle(button->bounds, border_color, 0);
    
    // Draw text (simplified - just show first character for demo)
    if (button->text[0]) {
        Color text_color = button->is_enabled ? button->text_color : (Color){128, 128, 128, 255};
        int text_x = (int)(button->bounds.x + button->bounds.width / 2 - 4);
        int text_y = (int)(button->bounds.y + button->bounds.height / 2 - 1);
        
        // Draw text as pixels (simplified)
        for (int i = 0; button->text[i] && i < 10; i++) {
            setPixel(text_x + i * 8, text_y, text_color);
        }
    }
}

int isButtonClicked(Button* button, int mouse_x, int mouse_y) {
    if (!button->is_visible || !button->is_enabled) return 0;
    
    return (mouse_x >= button->bounds.x && 
            mouse_x < button->bounds.x + button->bounds.width &&
            mouse_y >= button->bounds.y && 
            mouse_y < button->bounds.y + button->bounds.height);
}

void destroyButton(Button* button) {
    if (button) {
        free(button);
    }
}

// =============================================================================
// TEXT BOX IMPLEMENTATION
// =============================================================================

TextBox* createTextBox(float x, float y, float width, float height, int max_length) {
    TextBox* textbox = (TextBox*)malloc(sizeof(TextBox));
    textbox->bounds.x = x;
    textbox->bounds.y = y;
    textbox->bounds.width = width;
    textbox->bounds.height = height;
    textbox->max_length = max_length;
    strcpy(textbox->text, "");
    textbox->bg_color = (Color){240, 240, 240, 255};
    textbox->text_color = (Color){0, 0, 0, 255};
    textbox->is_visible = 1;
    textbox->is_enabled = 1;
    return textbox;
}

void drawTextBox(TextBox* textbox) {
    if (!textbox->is_visible) return;
    
    // Draw background
    drawRectangle(textbox->bounds, textbox->bg_color, 1);
    
    // Draw border
    Color border_color = textbox->is_enabled ? 
                        (Color){128, 128, 128, 255} : 
                        (Color){64, 64, 64, 255};
    drawRectangle(textbox->bounds, border_color, 0);
    
    // Draw text (simplified)
    if (textbox->text[0]) {
        int text_x = (int)textbox->bounds.x + 2;
        int text_y = (int)textbox->bounds.y + 1;
        
        for (int i = 0; textbox->text[i] && i < textbox->max_length; i++) {
            setPixel(text_x + i * 8, text_y, textbox->text_color);
        }
    }
}

void appendToTextBox(TextBox* textbox, char ch) {
    if (!textbox->is_enabled) return;
    
    int len = strlen(textbox->text);
    if (len < textbox->max_length - 1) {
        textbox->text[len] = ch;
        textbox->text[len + 1] = '\0';
    }
}

void backspaceTextBox(TextBox* textbox) {
    if (!textbox->is_enabled) return;
    
    int len = strlen(textbox->text);
    if (len > 0) {
        textbox->text[len - 1] = '\0';
    }
}

void clearTextBox(TextBox* textbox) {
    strcpy(textbox->text, "");
}

void destroyTextBox(TextBox* textbox) {
    if (textbox) {
        free(textbox);
    }
}

// =============================================================================
// LIST BOX IMPLEMENTATION
// =============================================================================

ListBox* createListBox(float x, float y, float width, float height) {
    ListBox* listbox = (ListBox*)malloc(sizeof(ListBox));
    listbox->bounds.x = x;
    listbox->bounds.y = y;
    listbox->bounds.width = width;
    listbox->bounds.height = height;
    listbox->item_count = 0;
    listbox->selected_index = -1;
    listbox->bg_color = (Color){255, 255, 255, 255};
    listbox->text_color = (Color){0, 0, 0, 255};
    listbox->selected_color = (Color){0, 100, 200, 255};
    listbox->is_visible = 1;
    listbox->is_enabled = 1;
    
    // Initialize items array
    for (int i = 0; i < 50; i++) {
        listbox->items[i] = NULL;
    }
    
    return listbox;
}

void addListBoxItem(ListBox* listbox, const char* item) {
    if (listbox->item_count < 50) {
        listbox->items[listbox->item_count] = strdup(item);
        listbox->item_count++;
    }
}

void drawListBox(ListBox* listbox) {
    if (!listbox->is_visible) return;
    
    // Draw background
    drawRectangle(listbox->bounds, listbox->bg_color, 1);
    
    // Draw border
    Color border_color = listbox->is_enabled ? 
                        (Color){128, 128, 128, 255} : 
                        (Color){64, 64, 64, 255};
    drawRectangle(listbox->bounds, border_color, 0);
    
    // Draw items
    int item_height = 20;
    int visible_items = (int)(listbox->bounds.height / item_height);
    
    for (int i = 0; i < listbox->item_count && i < visible_items; i++) {
        int item_y = (int)(listbox->bounds.y + i * item_height);
        
        // Draw selection
        if (i == listbox->selected_index) {
            Rectangle selection = {
                listbox->bounds.x, item_y,
                listbox->bounds.width, item_height
            };
            drawRectangle(selection, listbox->selected_color, 1);
        }
        
        // Draw text (simplified)
        if (listbox->items[i]) {
            int text_x = (int)listbox->bounds.x + 2;
            int text_y = item_y + 8;
            
            Color text_color = (i == listbox->selected_index) ? 
                            (Color){255, 255, 255, 255} : 
                            listbox->text_color;
            
            for (int j = 0; listbox->items[i][j] && j < 20; j++) {
                setPixel(text_x + j * 8, text_y, text_color);
            }
        }
    }
}

void selectListBoxItem(ListBox* listbox, int index) {
    if (index >= 0 && index < listbox->item_count) {
        listbox->selected_index = index;
    }
}

const char* getSelectedListBoxItem(ListBox* listbox) {
    if (listbox->selected_index >= 0 && listbox->selected_index < listbox->item_count) {
        return listbox->items[listbox->selected_index];
    }
    return NULL;
}

void destroyListBox(ListBox* listbox) {
    if (listbox) {
        for (int i = 0; i < listbox->item_count; i++) {
            if (listbox->items[i]) {
                free(listbox->items[i]);
            }
        }
        free(listbox);
    }
}

// =============================================================================
// SCROLL BAR IMPLEMENTATION
// =============================================================================

ScrollBar* createScrollBar(float x, float y, float width, float height, int min_val, int max_val) {
    ScrollBar* scrollbar = (ScrollBar*)malloc(sizeof(ScrollBar));
    scrollbar->bounds.x = x;
    scrollbar->bounds.y = y;
    scrollbar->bounds.width = width;
    scrollbar->bounds.height = height;
    scrollbar->min_value = min_val;
    scrollbar->max_value = max_val;
    scrollbar->current_value = min_val;
    scrollbar->bg_color = (Color){200, 200, 200, 255};
    scrollbar->thumb_color = (Color){100, 100, 100, 255};
    scrollbar->is_visible = 1;
    scrollbar->is_enabled = 1;
    return scrollbar;
}

void drawScrollBar(ScrollBar* scrollbar) {
    if (!scrollbar->is_visible) return;
    
    // Draw background
    drawRectangle(scrollbar->bounds, scrollbar->bg_color, 1);
    
    // Draw thumb
    float range = scrollbar->max_value - scrollbar->min_value;
    float thumb_pos = (scrollbar->current_value - scrollbar->min_value) / range;
    float thumb_height = scrollbar->bounds.height / 4;
    float thumb_y = scrollbar->bounds.y + thumb_pos * (scrollbar->bounds.height - thumb_height);
    
    Rectangle thumb = {
        scrollbar->bounds.x, thumb_y,
        scrollbar->bounds.width, thumb_height
    };
    drawRectangle(thumb, scrollbar->thumb_color, 1);
}

void setScrollBarValue(ScrollBar* scrollbar, int value) {
    if (value >= scrollbar->min_value && value <= scrollbar->max_value) {
        scrollbar->current_value = value;
    }
}

int getScrollBarValue(ScrollBar* scrollbar) {
    return scrollbar->current_value;
}

void destroyScrollBar(ScrollBar* scrollbar) {
    if (scrollbar) {
        free(scrollbar);
    }
}

// =============================================================================
// ADVANCED GRAPHICS FUNCTIONS
// =============================================================================

// Draw bezier curve
void drawBezierCurve(Point p0, Point p1, Point p2, Point p3, Color color) {
    for (float t = 0; t <= 1; t += 0.01f) {
        float u = 1 - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;
        
        Point point;
        point.x = uuu * p0.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * p3.x;
        point.y = uuu * p0.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * p3.y;
        
        setPixel((int)point.x, (int)point.y, color);
    }
}

// Draw gradient rectangle
void drawGradientRectangle(Rectangle rect, Color color1, Color color2) {
    for (int y = (int)rect.y; y < (int)(rect.y + rect.height); y++) {
        float ratio = (y - rect.y) / rect.height;
        
        Color color;
        color.r = (unsigned char)(color1.r + (color2.r - color1.r) * ratio);
        color.g = (unsigned char)(color1.g + (color2.g - color1.g) * ratio);
        color.b = (unsigned char)(color1.b + (color2.b - color1.b) * ratio);
        color.a = 255;
        
        for (int x = (int)rect.x; x < (int)(rect.x + rect.width); x++) {
            setPixel(x, y, color);
        }
    }
}

// Draw polygon
void drawPolygon(Point* vertices, int vertex_count, Color color, int filled) {
    if (vertex_count < 3) return;
    
    if (filled) {
        // Use scanline algorithm for filled polygon
        int min_y = vertices[0].y;
        int max_y = vertices[0].y;
        
        for (int i = 1; i < vertex_count; i++) {
            if (vertices[i].y < min_y) min_y = vertices[i].y;
            if (vertices[i].y > max_y) max_y = vertices[i].y;
        }
        
        for (int y = min_y; y <= max_y; y++) {
            float intersections[100];
            int intersection_count = 0;
            
            for (int i = 0; i < vertex_count; i++) {
                int j = (i + 1) % vertex_count;
                if ((vertices[i].y <= y && vertices[j].y > y) ||
                    (vertices[j].y <= y && vertices[i].y > y)) {
                    float x = vertices[i].x + 
                             (y - vertices[i].y) * 
                             (vertices[j].x - vertices[i].x) / 
                             (vertices[j].y - vertices[i].y);
                    intersections[intersection_count++] = x;
                }
            }
            
            // Sort intersections
            for (int i = 0; i < intersection_count - 1; i++) {
                for (int j = i + 1; j < intersection_count; j++) {
                    if (intersections[i] > intersections[j]) {
                        float temp = intersections[i];
                        intersections[i] = intersections[j];
                        intersections[j] = temp;
                    }
                }
            }
            
            // Fill between pairs of intersections
            for (int i = 0; i < intersection_count; i += 2) {
                int start_x = (int)intersections[i];
                int end_x = (int)intersections[i + 1];
                
                for (int x = start_x; x <= end_x; x++) {
                    setPixel(x, y, color);
                }
            }
        }
    } else {
        // Draw outline
        for (int i = 0; i < vertex_count; i++) {
            int j = (i + 1) % vertex_count;
            drawLine((int)vertices[i].x, (int)vertices[i].y,
                    (int)vertices[j].x, (int)vertices[j].y, color);
        }
    }
}

// =============================================================================
// ANIMATION SYSTEM
// =============================================================================

typedef struct {
    float x, y;
    float vx, vy;
    float radius;
    Color color;
} Ball;

typedef struct {
    Ball balls[50];
    int ball_count;
    float gravity;
    float damping;
} AnimationSystem;

AnimationSystem anim_system;

void initAnimationSystem() {
    anim_system.ball_count = 10;
    anim_system.gravity = 0.5f;
    anim_system.damping = 0.99f;
    
    for (int i = 0; i < anim_system.ball_count; i++) {
        anim_system.balls[i].x = rand() % SCREEN_WIDTH;
        anim_system.balls[i].y = rand() % (SCREEN_HEIGHT / 2);
        anim_system.balls[i].vx = (rand() % 10 - 5) / 10.0f;
        anim_system.balls[i].vy = 0;
        anim_system.balls[i].radius = 5 + rand() % 10;
        anim_system.balls[i].color.r = rand() % 256;
        anim_system.balls[i].color.g = rand() % 256;
        anim_system.balls[i].color.b = rand() % 256;
        anim_system.balls[i].color.a = 255;
    }
}

void updateAnimationSystem() {
    for (int i = 0; i < anim_system.ball_count; i++) {
        Ball* ball = &anim_system.balls[i];
        
        // Apply gravity
        ball->vy += anim_system.gravity;
        
        // Apply damping
        ball->vx *= anim_system.damping;
        ball->vy *= anim_system.damping;
        
        // Update position
        ball->x += ball->vx;
        ball->y += ball->vy;
        
        // Bounce off walls
        if (ball->x - ball->radius < 0 || ball->x + ball->radius > SCREEN_WIDTH) {
            ball->vx = -ball->vx * 0.8f;
            ball->x = (ball->x - ball->radius < 0) ? ball->radius : SCREEN_WIDTH - ball->radius;
        }
        
        if (ball->y - ball->radius < 0 || ball->y + ball->radius > SCREEN_HEIGHT) {
            ball->vy = -ball->vy * 0.8f;
            ball->y = (ball->y - ball->radius < 0) ? ball->radius : SCREEN_HEIGHT - ball->radius;
        }
    }
}

void drawAnimationSystem() {
    for (int i = 0; i < anim_system.ball_count; i++) {
        Ball* ball = &anim_system.balls[i];
        Circle circle = {ball->x, ball->y, ball->radius};
        drawCircle(circle, ball->color, 1);
    }
}

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstrateBasicShapes() {
    printf("=== BASIC SHAPES DEMO ===\n");
    
    // Clear screen
    clearConsoleBuffer((Color){0, 0, 0, 255});
    
    // Draw various shapes
    Rectangle rect1 = {100, 100, 200, 100};
    drawRectangle(rect1, (Color){255, 0, 0, 255}, 0);
    
    Rectangle rect2 = {350, 100, 200, 100};
    drawRectangle(rect2, (Color){0, 255, 0, 255}, 1);
    
    Circle circle1 = {200, 300, 50};
    drawCircle(circle1, (Color){0, 0, 255, 255}, 0);
    
    Circle circle2 = {500, 300, 50};
    drawCircle(circle2, (Color){255, 255, 0, 255}, 1);
    
    Triangle triangle = {
        {{100, 450}, {200, 450}, {150, 400}},
        {255, 0, 255, 255}
    };
    drawTriangle(triangle, 0);
    
    renderConsole();
    Sleep(2000);
    
    printf("Basic shapes displayed for 2 seconds...\n");
}

void demonstrateLines() {
    printf("=== LINES DEMO ===\n");
    
    clearConsoleBuffer((Color){0, 0, 0, 255});
    
    // Draw various lines
    drawLine(50, 50, 750, 50, (Color){255, 255, 255, 255});
    drawLine(50, 50, 50, 550, (Color){255, 255, 255, 255});
    drawLine(750, 50, 750, 550, (Color){255, 255, 255, 255});
    drawLine(50, 550, 750, 550, (Color){255, 255, 255, 255});
    
    // Diagonal lines
    drawLine(50, 50, 750, 550, (Color){255, 0, 0, 255});
    drawLine(750, 50, 50, 550, (Color){0, 255, 0, 255});
    
    // Random lines
    for (int i = 0; i < 20; i++) {
        int x1 = rand() % SCREEN_WIDTH;
        int y1 = rand() % SCREEN_HEIGHT;
        int x2 = rand() % SCREEN_WIDTH;
        int y2 = rand() % SCREEN_HEIGHT;
        
        Color color = {
            rand() % 256,
            rand() % 256,
            rand() % 256,
            255
        };
        
        drawLine(x1, y1, x2, y2, color);
    }
    
    renderConsole();
    Sleep(2000);
    
    printf("Lines displayed for 2 seconds...\n");
}

void demonstrateCurves() {
    printf("=== CURVES DEMO ===\n");
    
    clearConsoleBuffer((Color){0, 0, 0, 255});
    
    // Draw bezier curves
    Point p0 = {100, 100};
    Point p1 = {200, 50};
    Point p2 = {600, 50};
    Point p3 = {700, 100};
    
    drawBezierCurve(p0, p1, p2, p3, (Color){255, 0, 0, 255});
    
    Point p4 = {100, 200};
    Point p5 = {200, 300};
    Point p6 = {600, 300};
    Point p7 = {700, 200};
    
    drawBezierCurve(p4, p5, p6, p7, (Color){0, 255, 0, 255});
    
    Point p8 = {100, 350};
    Point p9 = {200, 500};
    Point p10 = {600, 500};
    Point p11 = {700, 350};
    
    drawBezierCurve(p8, p9, p10, p11, (Color){0, 0, 255, 255});
    
    renderConsole();
    Sleep(2000);
    
    printf("Curves displayed for 2 seconds...\n");
}

void demonstrateGradients() {
    printf("=== GRADIENTS DEMO ===\n");
    
    clearConsoleBuffer((Color){0, 0, 0, 255});
    
    // Draw gradient rectangles
    Rectangle grad1 = {100, 100, 200, 100};
    drawGradientRectangle(grad1, (Color){255, 0, 0, 255}, (Color){0, 255, 0, 255});
    
    Rectangle grad2 = {350, 100, 200, 100};
    drawGradientRectangle(grad2, (Color){0, 0, 255, 255}, (Color){255, 255, 0, 255});
    
    Rectangle grad3 = {100, 250, 200, 100};
    drawGradientRectangle(grad3, (Color){255, 255, 255, 255}, (Color){0, 0, 0, 255});
    
    Rectangle grad4 = {350, 250, 200, 100};
    drawGradientRectangle(grad4, (Color){128, 0, 128, 255}, (Color){255, 128, 0, 255});
    
    renderConsole();
    Sleep(2000);
    
    printf("Gradients displayed for 2 seconds...\n");
}

void demonstrateGUI() {
    printf("=== GUI DEMO ===\n");
    
    clearConsoleBuffer((Color){240, 240, 240, 255});
    
    // Create GUI elements
    Button* button1 = createButton(100, 100, 150, 30, "Click Me");
    Button* button2 = createButton(300, 100, 150, 30, "Cancel");
    TextBox* textbox = createTextBox(100, 200, 300, 25, 50);
    ListBox* listbox = createListBox(500, 100, 200, 150);
    ScrollBar* scrollbar = createScrollBar(750, 100, 20, 150, 0, 100);
    
    // Add items to listbox
    addListBoxItem(listbox, "Item 1");
    addListBoxItem(listbox, "Item 2");
    addListBoxItem(listbox, "Item 3");
    addListBoxItem(listbox, "Item 4");
    addListBoxItem(listbox, "Item 5");
    
    // Draw GUI elements
    drawButton(button1);
    drawButton(button2);
    drawTextBox(textbox);
    drawListBox(listbox);
    drawScrollBar(scrollbar);
    
    renderConsole();
    Sleep(2000);
    
    // Cleanup
    destroyButton(button1);
    destroyButton(button2);
    destroyTextBox(textbox);
    destroyListBox(listbox);
    destroyScrollBar(scrollbar);
    
    printf("GUI elements displayed for 2 seconds...\n");
}

void demonstrateAnimation() {
    printf("=== ANIMATION DEMO ===\n");
    
    initAnimationSystem();
    
    for (int frame = 0; frame < 100; frame++) {
        clearConsoleBuffer((Color){0, 0, 0, 255});
        
        updateAnimationSystem();
        drawAnimationSystem();
        
        // Draw frame counter
        char frame_text[50];
        sprintf(frame_text, "Frame: %d", frame);
        for (int i = 0; frame_text[i]; i++) {
            setPixel(10 + i * 8, 10, (Color){255, 255, 255, 255});
        }
        
        renderConsole();
        Sleep(50); // 20 FPS
    }
    
    printf("Animation completed (100 frames)...\n");
}

void demonstratePolygon() {
    printf("=== POLYGON DEMO ===\n");
    
    clearConsoleBuffer((Color){0, 0, 0, 255});
    
    // Draw pentagon
    Point pentagon[5];
    float center_x = 200, center_y = 300, radius = 80;
    for (int i = 0; i < 5; i++) {
        float angle = (2 * 3.14159 * i) / 5 - 3.14159 / 2;
        pentagon[i].x = center_x + radius * cos(angle);
        pentagon[i].y = center_y + radius * sin(angle);
    }
    drawPolygon(pentagon, 5, (Color){255, 0, 0, 255}, 0);
    
    // Draw hexagon
    Point hexagon[6];
    center_x = 600;
    for (int i = 0; i < 6; i++) {
        float angle = (2 * 3.14159 * i) / 6;
        hexagon[i].x = center_x + radius * cos(angle);
        hexagon[i].y = center_y + radius * sin(angle);
    }
    drawPolygon(hexagon, 6, (Color){0, 255, 0, 255}, 1);
    
    // Draw star
    Point star[10];
    center_x = 400;
    center_y = 450;
    for (int i = 0; i < 10; i++) {
        float angle = (2 * 3.14159 * i) / 10 - 3.14159 / 2;
        float r = (i % 2 == 0) ? 60 : 25;
        star[i].x = center_x + r * cos(angle);
        star[i].y = center_y + r * sin(angle);
    }
    drawPolygon(star, 10, (Color){255, 255, 0, 255}, 0);
    
    renderConsole();
    Sleep(2000);
    
    printf("Polygons displayed for 2 seconds...\n");
}

// =============================================================================
// MAIN FUNCTION
// =============================================================================

int main() {
    printf("Graphics and GUI Programming\n");
    printf("============================\n\n");
    
    printf("Initializing console graphics...\n");
    initConsoleGraphics();
    
    // Seed random number generator
    srand(time(NULL));
    
    // Run demonstrations
    demonstrateBasicShapes();
    demonstrateLines();
    demonstrateCurves();
    demonstrateGradients();
    demonstratePolygon();
    demonstrateGUI();
    demonstrateAnimation();
    
    // Cleanup
    cleanupConsoleGraphics();
    
    printf("\nAll graphics and GUI examples demonstrated!\n");
    printf("Note: Console graphics are limited but demonstrate core concepts.\n");
    printf("For advanced graphics, consider libraries like OpenGL, DirectX, or SDL.\n");
    
    return 0;
}
