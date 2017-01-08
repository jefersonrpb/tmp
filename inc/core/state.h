#ifndef STATE_H
#define STATE_H

typedef struct {
    void (*update)(Game* game);
    void (*draw)(Game* game);
} State; 

#endif
