STYPE
bit_reverse(STYPE value)
{
    UTYPE v = (UTYPE) value;
    UTYPE ans = 0;
    UTYPE last_bit = 0;
    UTYPE full = 1;

    while (full != 0) {
        last_bit = (v & 1);
        v >>= 1;
        ans = (ans << 1) | last_bit;
        full <<= 1;
    }

    return ans;
}
