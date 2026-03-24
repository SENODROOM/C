# Graphics and GUI Programming

This file contains comprehensive graphics and GUI programming examples in C, including console graphics primitives, GUI widgets, animation systems, and advanced rendering techniques.

## 📚 Graphics Programming Overview

### 🎨 Graphics Concepts
- **Pixels**: Basic building blocks of graphics
- **Primitives**: Basic shapes (lines, rectangles, circles, triangles)
- **Transformations**: Translation, rotation, scaling
- **Color Theory**: RGB color model and color blending
- **Animation**: Frame-based movement and interpolation

### 🖼️ Rendering Pipeline
- **Scene Setup**: Define objects and properties
- **Transform**: Apply transformations to objects
- **Rasterization**: Convert vectors to pixels
- **Display**: Show final image on screen

## 🖥️ Console Graphics Engine

### Graphics Context Structure
```c
typedef struct {
    HANDLE console;
    CHAR_INFO* buffer;
    COORD bufferSize;
    COORD bufferCoord;
    SMALL_RECT writeRegion;
} ConsoleGraphics;
```

### Color Management
```c
typedef struct {
    unsigned char r, g, b, a;
} Color;

const Color consolePalette[16] = {
    {0, 0, 0, 255},       // Black
    {128, 0, 0, 255},     // Dark Red
    {0, 128, 0, 255},     // Dark Green
    {128, 128, 0, 255},   // Dark Yellow
    {0, 0, 128, 255},     // Dark Blue
    {128, 0, 128, 255},   // Dark Magenta
    {0, 128, 128, 255},   // Dark Cyan
    {192, 192, 192, 255}, // Light Gray
    // ... more colors
};
```

### Console Initialization
```c
void initConsoleGraphics() {
    cg.console = GetStdHandle(STD_OUTPUT_HANDLE);
    cg.bufferSize.X = SCREEN_WIDTH;
    cg.bufferSize.Y = SCREEN_HEIGHT;
    cg.bufferCoord.X = 0;
    cg.bufferCoord.Y = 0;
    
    cg.buffer = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * SCREEN_WIDTH * SCREEN_HEIGHT);
    
    // Hide cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(cg.console, &cursorInfo);
    
    // Set console size
    COORD size = {SCREEN_WIDTH, SCREEN_HEIGHT};
    SetConsoleScreenBufferSize(cg.console, size);
}
```

## 🎯 Graphics Primitives

### Point Structure
```c
typedef struct {
    float x, y;
} Point;
```

### Rectangle Structure
```c
typedef struct {
    float x, y, width, height;
} Rectangle;
```

### Circle Structure
```c
typedef struct {
    float x, y, radius;
} Circle;
```

### Triangle Structure
```c
typedef struct {
    Point vertices[3];
    Color color;
} Triangle;
```

## 🖌️ Drawing Functions

### Pixel Manipulation
```c
void setPixel(int x, int y, Color color) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
    
    int index = y * SCREEN_WIDTH + x;
    cg.buffer[index].Char.AsciiChar = 219; // Full block character
    cg.buffer[index].Attributes = 
        (color.r > 128 ? 4 : 0) | 
        (color.g > 128 ? 2 : 0) | 
        (color.b > 128 ? 1 : 0);
}
```

### Line Drawing (Bresenham's Algorithm)
```c
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
```

### Rectangle Drawing
```c
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
```

### Circle Drawing (Midpoint Algorithm)
```c
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
```

### Triangle Drawing
```c
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
```

## 🎨 GUI Widgets

### Button Widget
```c
typedef struct {
    Rectangle bounds;
    char text[256];
    Color bg_color;
    Color text_color;
    int is_visible;
    int is_enabled;
} Button;

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
    
    // Draw text (simplified)
    if (button->text[0]) {
        Color text_color = button->is_enabled ? button->text_color : (Color){128, 128, 128, 255};
        int text_x = (int)(button->bounds.x + button->bounds.width / 2 - 4);
        int text_y = (int)(button->bounds.y + button->bounds.height / 2 - 1);
        
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
```

### Text Box Widget
```c
typedef struct {
    Rectangle bounds;
    char text[256];
    Color bg_color;
    Color text_color;
    int max_length;
    int is_visible;
    int is_enabled;
} TextBox;

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
```

### List Box Widget
```c
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
    
    return listbox;
}

void addListBoxItem(ListBox* listbox, const char* item) {
    if (listbox->item_count < 50) {
        listbox->items[listbox->item_count] = strdup(item);
        listbox->item_count++;
    }
}
```

### Scroll Bar Widget
```c
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
```

## 🎭 Advanced Graphics

### Bézier Curves
```c
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
```

### Gradient Rectangle
```c
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
```

### Polygon Drawing
```c
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
            
            // Sort intersections and fill between pairs
            for (int i = 0; i < intersection_count - 1; i++) {
                for (int j = i + 1; j < intersection_count; j++) {
                    if (intersections[i] > intersections[j]) {
                        float temp = intersections[i];
                        intersections[i] = intersections[j];
                        intersections[j] = temp;
                    }
                }
            }
            
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
```

## 🎬 Animation System

### Ball Structure
```c
typedef struct {
    float x, y;
    float vx, vy;
    float radius;
    Color color;
} Ball;
```

### Animation System
```c
typedef struct {
    Ball balls[50];
    int ball_count;
    float gravity;
    float damping;
} AnimationSystem;
```

### Physics Update
```c
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
```

## 🎯 Transformations

### 2D Vector Operations
```c
typedef struct {
    float x, y;
} Vector2;

Vector2 vector2Create(float x, float y) {
    Vector2 v = {x, y};
    return v;
}

Vector2 vector2Add(Vector2 a, Vector2 b) {
    return vector2Create(a.x + b.x, a.y + b.y);
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
```

### Translation
```c
Point translatePoint(Point p, float dx, float dy) {
    Point result;
    result.x = p.x + dx;
    result.y = p.y + dy;
    return result;
}
```

### Rotation
```c
Point rotatePoint(Point p, float angle, Point center) {
    // Translate to origin
    float x = p.x - center.x;
    float y = p.y - center.y;
    
    // Rotate
    float cos_a = cos(angle);
    float sin_a = sin(angle);
    float new_x = x * cos_a - y * sin_a;
    float new_y = x * sin_a + y * cos_a;
    
    // Translate back
    Point result;
    result.x = new_x + center.x;
    result.y = new_y + center.y;
    return result;
}
```

### Scaling
```c
Point scalePoint(Point p, float scale_x, float scale_y, Point center) {
    Point result;
    result.x = center.x + (p.x - center.x) * scale_x;
    result.y = center.y + (p.y - center.y) * scale_y;
    return result;
}
```

## 🌈 Color Theory

### Color Blending
```c
Color blendColors(Color c1, Color c2, float ratio) {
    Color result;
    result.r = (unsigned char)(c1.r * (1 - ratio) + c2.r * ratio);
    result.g = (unsigned char)(c1.g * (1 - ratio) + c2.g * ratio);
    result.b = (unsigned char)(c1.b * (1 - ratio) + c2.b * ratio);
    result.a = 255;
    return result;
}
```

### RGB to HSL Conversion
```c
typedef struct {
    float h, s, l;
} HSLColor;

HSLColor rgbToHsl(Color rgb) {
    float r = rgb.r / 255.0f;
    float g = rgb.g / 255.0f;
    float b = rgb.b / 255.0f;
    
    float max_val = fmax(fmax(r, g), b);
    float min_val = fmin(fmin(r, g), b);
    float delta = max_val - min_val;
    
    HSLColor hsl;
    hsl.l = (max_val + min_val) / 2.0f;
    
    if (delta == 0) {
        hsl.h = 0;
        hsl.s = 0;
    } else {
        hsl.s = (hsl.l > 0.5f) ? delta / (2.0f - max_val - min_val) : delta / (max_val + min_val);
        
        if (max_val == r) {
            hsl.h = (g - b) / delta + (g < b ? 6 : 0);
        } else if (max_val == g) {
            hsl.h = (b - r) / delta + 2;
        } else {
            hsl.h = (r - g) / delta + 4;
        }
        
        hsl.h /= 6.0f;
    }
    
    return hsl;
}
```

## 💡 Performance Optimization

### Dirty Rectangle Optimization
```c
typedef struct {
    Rectangle bounds;
    int is_dirty;
} DirtyRegion;

DirtyRegion dirty_regions[100];
int dirty_region_count = 0;

void markDirtyRegion(Rectangle rect) {
    if (dirty_region_count < 100) {
        dirty_regions[dirty_region_count].bounds = rect;
        dirty_regions[dirty_region_count].is_dirty = 1;
        dirty_region_count++;
    }
}

void renderDirtyRegions() {
    for (int i = 0; i < dirty_region_count; i++) {
        if (dirty_regions[i].is_dirty) {
            // Render only the dirty region
            renderRegion(dirty_regions[i].bounds);
            dirty_regions[i].is_dirty = 0;
        }
    }
    dirty_region_count = 0;
}
```

### Spatial Partitioning
```c
typedef struct {
    Rectangle bounds;
    int object_indices[100];
    int object_count;
} GridCell;

GridCell grid[10][10];

void updateSpatialGrid() {
    // Clear grid
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            grid[y][x].object_count = 0;
        }
    }
    
    // Assign objects to grid cells
    for (int i = 0; i < object_count; i++) {
        int grid_x = (int)(objects[i].x / (SCREEN_WIDTH / 10));
        int grid_y = (int)(objects[i].y / (SCREEN_HEIGHT / 10));
        
        if (grid_x >= 0 && grid_x < 10 && grid_y >= 0 && grid_y < 10) {
            grid[grid_y][grid_x].object_indices[grid[grid_y][grid_x].object_count++] = i;
        }
    }
}
```

## 🎮 Input Handling

### Mouse Input
```c
typedef struct {
    int x, y;
    int left_button;
    int right_button;
    int middle_button;
} MouseState;

MouseState getMouseState() {
    MouseState mouse;
    
    // Get mouse position
    POINT cursor_pos;
    GetCursorPos(&cursor_pos);
    mouse.x = cursor_pos.x;
    mouse.y = cursor_pos.y;
    
    // Get button states
    mouse.left_button = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
    mouse.right_button = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
    mouse.middle_button = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
    
    return mouse;
}
```

### Keyboard Input
```c
typedef struct {
    int keys[256];
} KeyboardState;

KeyboardState getKeyboardState() {
    KeyboardState keyboard;
    
    for (int i = 0; i < 256; i++) {
        keyboard.keys[i] = GetAsyncKeyState(i) & 0x8000;
    }
    
    return keyboard;
}
```

## 🖼️ Image Processing

### Basic Image Structure
```c
typedef struct {
    int width, height;
    Color* pixels;
} Image;

Image* createImage(int width, int height) {
    Image* image = (Image*)malloc(sizeof(Image));
    image->width = width;
    image->height = height;
    image->pixels = (Color*)malloc(width * height * sizeof(Color));
    return image;
}

void setPixelImage(Image* image, int x, int y, Color color) {
    if (x >= 0 && x < image->width && y >= 0 && y < image->height) {
        image->pixels[y * image->width + x] = color;
    }
}

Color getPixelImage(Image* image, int x, int y) {
    if (x >= 0 && x < image->width && y >= 0 && y < image->height) {
        return image->pixels[y * image->width + x];
    }
    return (Color){0, 0, 0, 255};
}
```

### Image Filters
```c
void applyGrayscaleFilter(Image* image) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            Color pixel = getPixelImage(image, x, y);
            int gray = (int)(pixel.r * 0.299 + pixel.g * 0.587 + pixel.b * 0.114);
            setPixelImage(image, x, y, (Color){gray, gray, gray, pixel.a});
        }
    }
}

void applyInvertFilter(Image* image) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            Color pixel = getPixelImage(image, x, y);
            setPixelImage(image, x, y, (Color){255 - pixel.r, 255 - pixel.g, 255 - pixel.b, pixel.a});
        }
    }
}
```

## ⚠️ Common Pitfalls

### 1. Memory Management
```c
// Wrong - Forgetting to free memory
Image* image = createImage(100, 100);
// Use image but forget to free

// Right - Always free allocated memory
Image* image = createImage(100, 100);
// Use image...
free(image->pixels);
free(image);
```

### 2. Bounds Checking
```c
// Wrong - No bounds checking
void setPixelUnsafe(int x, int y, Color color) {
    buffer[y * width + x] = color; // May crash
}

// Right - Always check bounds
void setPixelSafe(int x, int y, Color color) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        buffer[y * width + x] = color;
    }
}
```

### 3. Color Format Mismatch
```c
// Wrong - Mixing color formats
unsigned int color = 0x00FF00; // ARGB format
buffer[y * width + x] = color; // Expects RGB format

// Right - Consistent color format
Color color = {0, 255, 0, 255}; // RGBA struct
buffer[y * width + x] = color;
```

### 4. Performance Issues
```c
// Wrong - Redundant calculations
void slowRendering() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float distance = sqrt(x * x + y * y); // Expensive
            if (distance < radius) {
                setPixel(x, y, color);
            }
        }
    }
}

// Right - Optimize calculations
void fastRendering() {
    int radius_squared = radius * radius;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int dx = x - center_x;
            int dy = y - center_y;
            if (dx * dx + dy * dy < radius_squared) {
                setPixel(x, y, color);
            }
        }
    }
}
```

## 🔧 Real-World Applications

### 1. Game Engine
```c
void renderGame(GameState* game) {
    // Clear background
    clearScreen(game->background_color);
    
    // Render game objects
    for (int i = 0; i < game->object_count; i++) {
        renderObject(&game->objects[i]);
    }
    
    // Render UI
    renderUI(game->ui_elements);
    
    // Present frame
    swapBuffers();
}
```

### 2. Image Editor
```c
void handleImageEditor(Image* image, EditorState* editor) {
    if (editor->tool == TOOL_BRUSH) {
        drawBrush(image, editor->mouse_x, editor->mouse_y, editor->brush_size);
    } else if (editor->tool == TOOL_ERASER) {
        drawEraser(image, editor->mouse_x, editor->mouse_y, editor->eraser_size);
    } else if (editor->tool == TOOL_FILL) {
        floodFill(image, editor->mouse_x, editor->mouse_y, editor->fill_color);
    }
}
```

### 3. Data Visualization
```c
void renderChart(Chart* chart) {
    // Draw axes
    drawLine(chart->x, chart->y + chart->height, 
            chart->x + chart->width, chart->y + chart->height, chart->axis_color);
    drawLine(chart->x, chart->y, 
            chart->x, chart->y + chart->height, chart->axis_color);
    
    // Draw data points
    for (int i = 0; i < chart->data_count; i++) {
        float x = chart->x + (i / (float)chart->data_count) * chart->width;
        float y = chart->y + chart->height - (chart->data[i] / chart->max_value) * chart->height;
        drawCircle((Circle){x, y, 3}, chart->data_color, 1);
    }
    
    // Draw lines between points
    for (int i = 0; i < chart->data_count - 1; i++) {
        float x1 = chart->x + (i / (float)chart->data_count) * chart->width;
        float y1 = chart->y + chart->height - (chart->data[i] / chart->max_value) * chart->height;
        float x2 = chart->x + ((i + 1) / (float)chart->data_count) * chart->width;
        float y2 = chart->y + chart->height - (chart->data[i + 1] / chart->max_value) * chart->height;
        
        drawLine((int)x1, (int)y1, (int)x2, (int)y2, chart->line_color);
    }
}
```

### 4. GUI Framework
```c
void handleGUIEvent(GUIEvent* event) {
    switch (event->type) {
        case EVENT_MOUSE_CLICK:
            for (int i = 0; i < widget_count; i++) {
                if (widgets[i]->handle_click) {
                    widgets[i]->handle_click(widgets[i], event->mouse_x, event->mouse_y);
                }
            }
            break;
            
        case EVENT_KEY_PRESS:
            for (int i = 0; i < widget_count; i++) {
                if (widgets[i]->handle_key) {
                    widgets[i]->handle_key(widgets[i], event->key_code);
                }
            }
            break;
    }
}
```

## 🎓 Best Practices

### 1. Resource Management
```c
// Always initialize and cleanup
void initGraphics() {
    initConsoleGraphics();
    loadTextures();
    initShaders();
}

void cleanupGraphics() {
    cleanupShaders();
    freeTextures();
    cleanupConsoleGraphics();
}
```

### 2. Error Handling
```c
// Check return values
if (!initConsoleGraphics()) {
    printf("Failed to initialize graphics\n");
    return 1;
}
```

### 3. Modular Design
```c
// Separate concerns
void renderBackground();
void renderObjects();
void renderUI();
void renderEffects();
```

### 4. Performance Monitoring
```c
// Track frame rate
static int frame_count = 0;
static float fps_timer = 0;

void updateFPS(float dt) {
    fps_timer += dt;
    frame_count++;
    
    if (fps_timer >= 1.0f) {
        printf("FPS: %d\n", frame_count);
        frame_count = 0;
        fps_timer = 0;
    }
}
```

### 5. Cross-Platform Considerations
```c
// Use platform abstraction
#ifdef _WIN32
    // Windows-specific code
#elif __linux__
    // Linux-specific code
#elif __APPLE__
    // macOS-specific code
#endif
```

Graphics and GUI programming in C provides the foundation for creating visually rich applications. Master these concepts to build engaging user interfaces and visual applications!
