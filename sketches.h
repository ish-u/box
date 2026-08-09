#ifndef SKETCHES_H
#define SKETCHES_H

typedef void (*SketchFn)(void);

void sketch_orbit_w(void);
void sketch_orbit_b(void);
void sketch_clock(void);

static const SketchFn SKETCHES[] = {
    sketch_clock,
    sketch_orbit_w,
    sketch_orbit_b,
};

#define TOTAL_SKETCHES (int)(sizeof(SKETCHES) / sizeof(SKETCHES[0]))

#endif