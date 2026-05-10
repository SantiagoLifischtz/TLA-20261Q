#ifndef LEXER_KEYWORDS_HEADER
#define LEXER_KEYWORDS_HEADER

typedef enum BuiltinDuration {
    WHOLE,
    HALF,
    QUARTER,
    EIGHTH,
    SIXTEENTH,
    THIRTYSECOND,
    SIXTYFOURTH
} BuiltinDuration;

typedef enum ModeName {
    MAJOR,
    DORIAN,
    PHRYGIAN,
    LYDIAN,
    MIXOLYDIAN,
    MINOR,
    LOCRIAN
} ModeName;

#endif
