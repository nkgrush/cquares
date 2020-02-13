#include "logic.h"
#include <curses.h>
#include <math.h>

rectangle_t *rectangle_create() {
    rectangle_t *rect; rect = (rectangle_t*)calloc(1, sizeof(rectangle_t));
    return rect;
}

void rectangle_destroy(rectangle_t *rect) {
    free(rect);
}


void rectangle_resize_x(rectangle_t *rect, float ratio) { 
    //if ( !ratio ) return;
    rect->width *= ratio;
}

void rectangle_resize_y(rectangle_t *rect, float ratio) { 
    //if ( !ratio ) return;
    rect->height *= ratio;
}

rectangle_t *rectangle_copy(rectangle_t *rect) {
    rectangle_t *copy = rectangle_create();
    memcpy(copy, rect, sizeof(rectangle_t));
    return copy;
}

void rectangle_collision_fight(plane_t *plane, size_t left, size_t right) {
    if (left == right) return; 

    rectangle_t *lrect, *rrect;
    lrect = plane->rects[left];
    rrect = plane->rects[right]; 

    rectangle_t *winner = rectangle_compare(lrect, rrect);

    if (!winner) {
        if (!(rand() % 10)) 
            return;

        if (rand() % 2) {
            rectangle_resize_x(lrect, 0.9f);
            rectangle_resize_y(rrect, 0.9f);
        } else {
            rectangle_resize_y(lrect, 0.9f);
            rectangle_resize_x(rrect, 0.9f);
        }
    }
    
    //TODO
    /* some genetic logic may be defined here
     * for now just kill smaller one */
    rectangle_t *looser = (winner == lrect) ? rrect : lrect;

    rectangle_destroy(looser);

    return;
}

void action_nostim(plane_t *plane, size_t index) {
    rectangle_t *rect = plane->rects[index];
    if (!rect) return;
    switch (rect->actions.nostim_o) {
        case so_idle:
            rectangle_hybernate(plane, index);
            break;
        case so_random:
            rectangle_move_random(plane, index);
            break;
        case so_lrandom:
            rectangle_move_lrandom(plane, index);
            break;
        case SE_NUMBER:
            exit(1);
    }
}


void action_food(plane_t *plane, size_t index) {
    rectangle_t *rect = plane->rects[index];
    if (!rect) return;
    switch (rect->actions.food_o) {
        case fo_idle:
            rectangle_hybernate(plane, index);
            break;
        case fo_random:
            rectangle_move_random(plane, index);
            break;
        case fo_lrandom:
            rectangle_move_lrandom(plane, index);
            break;
        case fo_avoid:
            rectangle_move_avoid(plane, index);
            break;
        case fo_seek:
            rectangle_move_seek(plane, index);
            break;
        case FE_NUMBER:
            exit(1);
    }
}

void action_big(plane_t *plane, size_t index) {
    rectangle_t *rect = plane->rects[index];
    if (!rect) return;
    switch (rect->actions.food_o) {
        case bo_idle:
            rectangle_hybernate(plane, index);
            break;
        case bo_random:
            rectangle_move_random(plane, index);
            break;
        case bo_lrandom:
            rectangle_move_lrandom(plane, index);
            break;
        case bo_avoid:
            rectangle_move_avoid(plane, index);
            break;
        case bo_seek:
            rectangle_move_seek(plane, index);
            break;
        case BE_NUMBER:
            exit(1);
    }
}

void action_pray(plane_t *plane, size_t index) {
    rectangle_t *rect = plane->rects[index];
    if (!rect) return;
    switch (rect->actions.food_o) {
        case po_idle:
            rectangle_hybernate(plane, index);
            break;
        case po_random:
            rectangle_move_random(plane, index);
            break;
        case po_lrandom:
            rectangle_move_lrandom(plane, index);
            break;
        case po_avoid:
            rectangle_move_avoid(plane, index);
            break;
        case po_seek:
            rectangle_move_seek(plane, index);
            break;
        case PE_NUMBER:
            exit (1);
    }
}

void rectangle_TESTMOVE     (plane_t *plane, size_t index) { 
    rectangle_t *rect = plane->rects[index];
    rect->xspeed = 2;
    rect->yspeed = 2;
    return;
}

void rectangle_hybernate    (plane_t *plane, size_t index) { rectangle_TESTMOVE(plane, index); }
void rectangle_move_random  (plane_t *plane, size_t index) { rectangle_TESTMOVE(plane, index); }
void rectangle_move_lrandom (plane_t *plane, size_t index) { rectangle_TESTMOVE(plane, index); }
void rectangle_move_avoid   (plane_t *plane, size_t index) { rectangle_TESTMOVE(plane, index); }
void rectangle_move_seek    (plane_t *plane, size_t index) { rectangle_TESTMOVE(plane, index); }


void frame_simulate(plane_t *plane) {
    //TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    long long nsec_elapsed = (ts.tv_sec - plane->ts_curr.tv_sec) * 1000000000LL
                                    + ts.tv_nsec - plane->ts_curr.tv_nsec;


    if (nsec_elapsed < TICK_NSEC) {
        //clock_gettime(CLOCK_MONOTONIC, &plane->ts_curr);
        long long wait_nsec = TICK_NSEC - nsec_elapsed;
        ts.tv_sec  = 0;
        while (wait_nsec >= 1000000000LL) {
            ts.tv_sec++;
            wait_nsec -= 1000000000LL;
        }
        ts.tv_nsec = wait_nsec;
        nanosleep(&ts, NULL);
    }

    size_t max = plane->rect_max;
    size_t i;
    for (i = 0; i < max; i++) {
        if (!plane->rects[i]) continue;
        rectangle_act(plane, i);
    }
    clock_gettime(CLOCK_MONOTONIC, &plane->ts_curr);
}


enum actions rectangle_action_get(plane_t *plane, size_t index) {
    //FIXME, only one branch defined yet
    return a_no_stim;
}

void rectangle_act(plane_t *plane, size_t index) {
    //TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
    enum actions action = rectangle_action_get(plane, index);
    switch (action) {
        case a_no_stim:
            action_nostim(plane, index);
            break;
        case a_food:
            action_food(plane, index);
            break;
        case a_big:
            action_big(plane, index);
            break;
        case a_pray:
            action_pray(plane, index);
            break;
    }
    rectangle_simulate(plane, index);
}

int rectangle_borders_resolve(plane_t *plane, size_t index) {
    //FIXME MOVE CODE
}

void rectangle_collision_resolve(plane_t *plane, size_t index) {
    int flag_collided;
    size_t col_index = plane_check_collisions(plane, index, &flag_collided);
    if (!flag_collided) { /* if no rect collision check borders */
        if(rectangle_borders_resolve(plane, index)) /* if changed go deeper */
            rectangle_collision_resolve(plane, index);
        return; /* no rect collision and no border collision exit recursion */
    }

    /* rect collision happened, resolve by tunneling distance then go deeper */
    
    /* go deeper */
    //FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME 
    rectangle_t *r = plane->rects[index];
    rectangle_t *t = plane->rects[col_index];
    /* else if are chained to prevent double bounceback on angle collision */
    //UP
    if (r->y > t->y && r->y < t->y + t->height) {
        double overshoot = r->y - t->y - t->height; //<0 checked
        r->y -= 2*overshoot;
    } 
    //DOWN
    else if (r->y + r->height > t->y && r->y + r->height < t->y + t->height) {
        double overshoot = r->y + r->height - t->y; //>0 checked
        r->y -= 2*overshoot;
    }     
    //LEFT
    else if (r->x > t->x && r->x < t->x + t->width) {
        double overshoot = r->x - t->y - t->width; //<0 checked
        r->x -= 2*overshoot;
    }
    //RIGHT
    else if (r->x + r->width > t->x && r->x + r->width < t->x + t->width) {
        double overshoot = r->x + r->width - t->width; //>0 checked
        r->x -= 2*overshoot;
    }

    rectangle_collision_resolve(plane, index);
}

void rectangle_simulate(plane_t *plane, size_t index) {
    rectangle_t *rect = plane->rects[index];
    double nx, ny;
    nx = rect->x + (rect->xspeed) * ((long double)TICK_NSEC/(long double)1000000000LL);
    ny = rect->y + (rect->yspeed) * ((long double)TICK_NSEC/(long double)1000000000LL);
    rect->x = nx;
    rect->y = ny;

    //FIXME this function does not yet respect rectangle actions
    
    /* continue checks untill resolved, 
     * if no changes were made during iteration, unset unresolved flag */
    //FIXME move code to borders_resolve
    int unresolved = TRUE;
    while (unresolved) {
        /* TODO move to rectangle_borders_resolve */
        /* top */
        unresolved = FALSE;
        if (rect->y < 0 ) {
            rect->y = -rect->y;
            unresolved = TRUE;
        } 
        /* bot */
        else if (rect->y > plane->ysize) {
            double overflow = rect->y - plane->ysize;
            rect->y = -overflow;
            unresolved = TRUE;
        }
        /* left */
        if (rect->x < 0) {
            rect->x = -rect->x;
            unresolved = TRUE;
        }
        /* right */
        else if (rect->x + rect->width > plane->xsize) { 
            double overflow = rect->x - plane->xsize;
            rect->x = -overflow;
            unresolved = TRUE;
        }
        /* END BORDERS */    
        /* FIXME this must be handles by rectangle_collision_resolve 
        int flag_collided = false;
        size_t collided_with = plane_check_collisions(plane, index, &flag_collided);
        if (flag_collided) rectangle_collision_resolve(plane, index, collided_with);
        */
        rectangle_collision_resolve(plane, index);
    }
}

rectangle_t *rectangle_compare(rectangle_t *left, rectangle_t *right) { 
    double  lsize, rsize;
    lsize = rectangle_size(left),
    rsize = rectangle_size(right);
    double delta = MAX(lsize, rsize) / MIN(rsize, lsize); 
    if ( delta < SIZE_DIFF_TRESHOLD ) return NULL;
    return MAX(rsize, lsize) == rsize ? right : left;
}

double rectangle_size(rectangle_t *rect) { 
    return (double)(rect->width * rect->height);
}

void _rectangle_init_randomly(plane_t *plane, rectangle_t *rect) {
    rect->width     = rand() % (WIDTH_INIT_MAX - WIDTH_INIT_MIN + 1) + WIDTH_INIT_MIN;
    rect->height    = rand() % (HEIGHT_INIT_MAX - HEIGHT_INIT_MIN + 1) + HEIGHT_INIT_MIN;
    rect->x         = rand() % (int)(plane->xsize - rect->width);
    rect->y         = rand() % (int)(plane->ysize - rect->height);
    rect->energy    = 100; //FIXME has no meaning
    //TODO initialize actions randomly
}

void  plane_init(plane_t *plane, rectangle_t **rects, size_t recs_size) {
    plane->rects = calloc(sizeof(rectangle_t*), recs_size);
    plane->rect_max = recs_size;
    clock_gettime(CLOCK_MONOTONIC, &plane->ts_init);
    clock_gettime(CLOCK_MONOTONIC, &plane->ts_curr);
    if (!rects){
        /* no rects given, therefore create them accordingly to config globals
         * TODO kill intersecting ones*/
        size_t i;
        for (i = 0; i < recs_size; i++) {
            rectangle_t *rect = plane->rects[i] = rectangle_create();
            _rectangle_init_randomly(plane, rect);
            size_t j = 0;
            int flag_collided = false;
            while (j++ < RECTANGLE_INIT_MAX_RETRIES) { 
                plane_check_collisions(plane, i, &flag_collided);
                if (!flag_collided) break;
                _rectangle_init_randomly(plane, rect);        
            }
        }
        /* TODO beter initialization*/
    } else { 
        /*load existing rects*/
        exit(1); 
    }
}

plane_t *plane_create(double xsize, double ysize) {
    plane_t *plane = calloc(sizeof(plane_t), 1);
    plane->xsize = xsize;
    plane->ysize = ysize;
    return plane;
}

void plane_destroy(plane_t *plane) {
    size_t i, max;
    max = plane->rect_max;
    rectangle_t **rects = plane->rects;
    for(i = 0; i < max; i++) {
        if (!rects[i]) continue;
        free(rects[i]);
    }
    free(rects);
    free(plane);
}


/* Checks for collisions for rectangle of given index 
 * WARN assumes that index is valid 
 * this function does not check for NULL rectangles at index 
 * return size_t index of collision rectangle
 * if collision happed set flag to true if not, false and return 0 */
size_t plane_check_collisions(plane_t *plane, size_t index, int *flag_collided) {
    size_t i;
    rectangle_t *rect = plane->rects[index];
    for (i = 0; i < plane->rect_max; i++) {
        rectangle_t *curr = plane->rects[i];
        if (!curr) continue;
        if (rectangle_check_collision(curr, rect) && rect != curr) {
            *flag_collided = true;
            return i;
        }
    }
    *flag_collided = false;
    return NULL;
}

/*WARN function asumes that left and right exist and were initialized correctly*/
int rectangle_check_collision(rectangle_t *left, rectangle_t *right) {
    if (right->x > left->x && right->x < left->x + left->width){
        if (right->y > left->y && right->y < left->y + left->width)
            return 1;
        if (right->y + right->width > left->y && right->y + right->width < left->y + left->width)
            return 1;
    }
    if (right->x + right->width > left->x && right->x + right->width < left->x + left->width) {
        if(right->y > left->y && right->y < left->y + left->width)
            return 1;
        if(right->y + right->width > left->y && right->y + right->width < left->y + left->width)
            return 1;
    }
    return 0;
}
