#include <cstdint>

enum EUserCommandButtons : std::uint32_t
{
    IN_ATTACK = (1 << 0),
    IN_JUMP = (1 << 1),
    IN_DUCK = (1 << 2),
    IN_FORWARD = (1 << 3),
    IN_BACK = (1 << 4),
    IN_USE = (1 << 5),
    IN_CANCEL = (1 << 6),
    IN_LEFT = (1 << 7),
    IN_RIGHT = (1 << 8),
    IN_MOVELEFT = (1 << 9),
    IN_MOVERIGHT = (1 << 10),
    IN_SECOND_ATTACK = (1 << 11),
    IN_RUN = (1 << 12),
    IN_RELOAD = (1 << 13),
    IN_LEFT_ALT = (1 << 14),
    IN_RIGHT_ALT = (1 << 15),
    IN_SCORE = (1 << 16),
    IN_SPEED = (1 << 17),
    IN_WALK = (1 << 18),
    IN_ZOOM = (1 << 19),
    IN_FIRST_WEAPON = (1 << 20),
    IN_SECOND_WEAPON = (1 << 21),
    IN_BULLRUSH = (1 << 22),
    IN_FIRST_GRENADE = (1 << 23),
    IN_SECOND_GRENADE = (1 << 24),
    IN_MIDDLE_ATTACK = (1 << 25),
    IN_USE_OR_RELOAD = (1 << 26)
};

enum EInputHistoryBits : std::uint32_t
{
    INPUT_HISTORY_BITS_VIEWANGLES = 0x1U,
    INPUT_HISTORY_BITS_SHOOTPOSITION = 0x2U,
    INPUT_HISTORY_BITS_TARGETHEADPOSITIONCHECK = 0x4U,
    INPUT_HISTORY_BITS_TARGETABSPOSITIONCHECK = 0x8U,
    INPUT_HISTORY_BITS_TARGETANGCHECK = 0x10U,
    INPUT_HISTORY_BITS_CL_INTERP = 0x20U,
    INPUT_HISTORY_BITS_SV_INTERP0 = 0x40U,
    INPUT_HISTORY_BITS_SV_INTERP1 = 0x80U,
    INPUT_HISTORY_BITS_PLAYER_INTERP = 0x100U,
    INPUT_HISTORY_BITS_RENDERTICKCOUNT = 0x200U,
    INPUT_HISTORY_BITS_RENDERTICKFRACTION = 0x400U,
    INPUT_HISTORY_BITS_PLAYERTICKCOUNT = 0x800U,
    INPUT_HISTORY_BITS_PLAYERTICKFRACTION = 0x1000U,
    INPUT_HISTORY_BITS_FRAMENUMBER = 0x2000U,
    INPUT_HISTORY_BITS_TARGETENTINDEX = 0x4000U
};

class CBasePB
{
public:
    char pad_0000[8]; //0x0000
    std::uint32_t has_bits; //0x0004
    std::uint64_t cached_bits; //0x0012

    void set_bits (std::uint64_t bits)
    {
        cached_bits |= bits;
    }
}; //Size: 0x0012

class CInButtonState
{
public:
    char pad_0000[8]; //0x0000
    std::uint64_t value; //0x0008
    std::uint64_t value_changed; //0x0010
    std::uint64_t value_scroll; //0x0018
}; //Size: 0x0020

