#ifndef TYPES
#define TYPES
    
typedef enum {
    UNTYPED = 0,
    INCORRECT,
    CORRECT,
} character_state;

typedef enum {
    EASY,
    HARD,
    WU,
    TAYLOR,
    CHRIS,
} gamemode;

typedef struct {
    int mx;
    int my;
} window_size;

#endif // TYPES
