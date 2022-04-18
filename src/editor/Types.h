#ifndef RAYTRACER_EDITOR_TYPES_H
#define RAYTRACER_EDITOR_TYPES_H

// Custom types used by the Editor

template<typename T>
struct Vec2
{
	T x;
	T y;
};

typedef Vec2<double> Vec2d;
typedef Vec2<float>  Vec2f;
typedef Vec2<int>    Vec2i;

#endif // RAYTRACER_EDITOR_TYPES_H
