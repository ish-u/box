#ifndef SKETCHES_H
#define SKETCHES_H

typedef void (*SketchFn)(void);

void sketch_orbit_w(void);
void sketch_orbit_b(void);
void sketch_bezier(void);
void sketch_clock(void);
void sketch_soloist(void);

static const SketchFn SKETCHES[] = {
    sketch_clock,
    sketch_soloist,
    sketch_orbit_w,
    sketch_orbit_b,
    sketch_bezier,
};

#define TOTAL_SKETCHES (int)(sizeof(SKETCHES) / sizeof(SKETCHES[0]))

#endif