extern unsigned int __udivsi3(unsigned int, unsigned int);
#define OPS(t1, t2, type)                       \
{                                               \
    t1 o1;                                      \
    t2 o2;                                      \
    o1 = 1;                                     \
    o2 = 255;                                   \
    o1 += o2;                                   \
    if (o1 != 256) { printf("failed %d %d\n", type, 1); exit(type + 1); } \
    o1 = 255;                                   \
    o2 = 1;                                     \
    o1 -= o2;                                   \
    if (o1 != 254) { printf("failed %d %d\n", type, 1); exit(type + 2); } \
    o1 = 2;                                     \
    o2 = 255;                                   \
    o1 *= o2;                                   \
    if (o1 != 510) { printf("failed %d %d\n", type, 3); exit(type + 3); } \
    o1 = 4;                                   \
    o2 = 2;                                     \
    o1 = __udivsi3(o1, o2);                     \
    /* o1 /= o2;  */                                  \
    if (o1 != 2) { printf("failed %d %d %d\n", type+255, 4, o1); exit(type + 4); } \
    o1 = 255;                                   \
    o2 = 7;                                     \
    o1 /= o2;                                   \
    if (o1 != 36) { printf("failed %d %d %d\n", type, 4, o1); exit(type + 4); } \
    o1 = 255;                                   \
    o2 = 7;                                     \
    o1 %= o2;                                   \
    if (o1 != 3) { printf("failed %d %d %d\n", type, 5, o1); exit(type + 5); } \
    o1 = 255;                                   \
    o2 = 7;                                     \
    o1 &= o2;                                   \
    if (o1 != 7) { printf("failed %d %d\n", type, 6); exit(type + 6); } \
    o1 != o2;                                   \
    o1 ^= o2;                                   \
    o1 <<= o2;                                  \
    o1 >>= o2;                                  \
}

int main()
{
    OPS(long, long, 0)
    OPS(unsigned long, unsigned long, 16)
    OPS(long, unsigned long, 32)
    OPS(unsigned long, long, 48)
    OPS(long long, long, 64)
}
