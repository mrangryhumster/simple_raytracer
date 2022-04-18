#pragma once
//********************************
//Place your 'config' defines for nmath lib here

//Adding some typedef for vec to store colors (see nmath_extensions.hpp for examples)
//#define NMATH_COLOR_TYPES

//This can be used to override default nmath namespace
//#define NMATH_NAMESPACE_BEGIN_OVERRIDE namespace mynamespace {
//#define NMATH_NAMESPACE_END_OVERRIDE }

//********************************
//Overrides for raytracer project

//Add color typedefs
#define NMATH_COLOR_TYPES
//Remove namespace
#define NMATH_NAMESPACE_BEGIN_OVERRIDE
#define NMATH_NAMESPACE_END_OVERRIDE
//********************************