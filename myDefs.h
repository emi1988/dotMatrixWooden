#ifndef DEFS_H
#define DEFS_H


enum enMax7219
{
    MAX7219_REG_NOOP = 0x0,
    MAX7219_REG_DIGIT0 = 0x1,
    MAX7219_REG_DIGIT1 = 0x2,
    MAX7219_REG_DIGIT2 = 0x3,
    MAX7219_REG_DIGIT3 = 0x4,
    MAX7219_REG_DIGIT4 = 0x5,
    MAX7219_REG_DIGIT5 = 0x6,
    MAX7219_REG_DIGIT6 = 0x7,
    MAX7219_REG_DIGIT7 = 0x8,
    MAX7219_REG_DECODEMODE = 0x9,
    MAX7219_REG_INTENSITY = 0xA,
    MAX7219_REG_SCANLIMIT = 0xB,
    MAX7219_REG_SHUTDOWN = 0xC,
    MAX7219_REG_DISPLAYTEST = 0xF,
};


struct stNumber3x5
{
    QString reg1;
    QString reg2;
    QString reg3;
    QString reg4;
    QString reg5;
};

#endif // DEFS_H
