#include <iostream>
#include <cassert>
#include <string>
#include <format>

inline uint32_t get_fvf(uint32_t dat_fvf)
{
    return (dat_fvf & 0xff0) << 4 | dat_fvf >> 8 & 0x30 | dat_fvf & 0xf;
}

inline uint32_t get_vertex_size_from_fvf(uint32_t fvf)
{
    // Just the arrays storing the size each FVF flag adds to the vertex size.
    // There might be some DX9 function I could call to get the size from the FVF.
    // But I'm not familiar with DX9 so I'll just do it like the GW source.
    constexpr uint32_t fvf_array_0[22] = {0x0, 0x8,  0x8,  0x10, 0x8,        0x10,      0x10, 0x18,
                                          0x8, 0x10, 0x10, 0x18, 0x10,       0x18,      0x18, 0x20,
                                          0x0, 0x0,  0x0,  0x1,  0xFFFFFFFF, 0xFFFFFFFF};
    constexpr uint32_t fvf_array_1[8] = {0x0, 0xC, 0xC, 0x18, 0xC, 0x18, 0x18, 0x24};
    constexpr uint32_t fvf_array_2[16] = {0x0, 0xC,  0x4, 0x10, 0xC,  0x18, 0x10, 0x1C,
                                          0x4, 0x10, 0x8, 0x14, 0x10, 0x1C, 0x14, 0x20};

    return fvf_array_0[fvf >> 0xc & 0xf] + fvf_array_0[fvf >> 8 & 0xf] + fvf_array_1[fvf >> 4 & 7] +
      fvf_array_2[fvf & 0xf];
}

inline int decode_filename(int id0, int id1) { return (id0 - 0xff00ff) + (id1 * 0xff00); }
inline void encode_filename(int& id0_out, int& id1_out, int decoded)
{
    id1_out = decoded / 0xff00;
    id0_out = (decoded % 0xff00) + 0xff00ff;
}
inline void encode_filename1(int& id0_out, int id1_in, int decoded)
{
    id0_out = (decoded - (id1_in * 0xff00)) + 0xff00ff;
}

int get_most_significant_bit_pos(uint32_t value)
{
    int bit_position = 31; // For 32-bit unsigned integer, highest bit position is 31
    if (value != 0)
    {
        while ((value >> bit_position) == 0)
        {
            bit_position--;
        }
    }
    return bit_position;
}

uint32_t FVF_to_ActualFVF(uint32_t FVF)
{
    int most_significant_bit_index;
    uint32_t ActualFVF;
    uint32_t currFlag;

    uint32_t FVF_to_ActualFVF_converter_array[] = {
      0x00000003, 0x00008006, 0x00000001, 0x00000002, 0x00000004, 0x00000010, 0x00000010, 0x00010000,
      0x00000020, 0x00020000, 0x00000008, 0x00000040, 0x00000040, 0x00040000, 0x00000000, 0x00000000};

    ActualFVF = 0;
    assert((FVF & 0x40) == 0 || (FVF & 0x34) == 0);

    if ((FVF & 0xff00) != 0)
    {
        most_significant_bit_index = get_most_significant_bit_pos(FVF & 0xff00);
        ActualFVF = (most_significant_bit_index - 7) << 8;
        FVF = FVF & 0xffff00ff;
    }
    if (FVF != 0)
    {
        const uint32_t* pArray = &FVF_to_ActualFVF_converter_array[1];
        do
        {
            currFlag = *(pArray - 1);
            assert(currFlag != 0);

            if ((currFlag & FVF) == currFlag)
            {
                FVF = FVF & ~currFlag;
                ActualFVF = ActualFVF | *pArray;
            }
            pArray = pArray + 2;
        } while (FVF != 0);
    }
    return ActualFVF;
}

int main()
{
    int FVF = get_vertex_size_from_fvf(get_fvf(0xf7));
    std::cout << FVF << "\n";
    FVF = get_vertex_size_from_fvf(get_fvf(1));
    std::cout << FVF << "\n";

    FVF = 0x15;
    std::string fvf =
      std::format("Dat_FVF: {} (0x{:x}). FVF: {} (0x{:x}). Actual_FVF: {} (0x{:x})\n", FVF, FVF, get_fvf(FVF),
                  get_fvf(FVF), FVF_to_ActualFVF(get_fvf(FVF)), FVF_to_ActualFVF(get_fvf(FVF)));
    std::cout << fvf;
    FVF = 0x17;
    fvf =
      std::format("Dat_FVF: {} (0x{:x}). FVF: {} (0x{:x}). Actual_FVF: {} (0x{:x})\n", FVF, FVF, get_fvf(FVF),
                  get_fvf(FVF), FVF_to_ActualFVF(get_fvf(FVF)), FVF_to_ActualFVF(get_fvf(FVF)));
    std::cout << fvf;
    FVF = 0x19;
    fvf =
      std::format("Dat_FVF: {} (0x{:x}). FVF: {} (0x{:x}). Actual_FVF: {} (0x{:x})\n", FVF, FVF, get_fvf(FVF),
                  get_fvf(FVF), FVF_to_ActualFVF(get_fvf(FVF)), FVF_to_ActualFVF(get_fvf(FVF)));
    std::cout << fvf;
    FVF = 0xf5;
    fvf =
      std::format("Dat_FVF: {} (0x{:x}). FVF: {} (0x{:x}). Actual_FVF: {} (0x{:x})\n", FVF, FVF, get_fvf(FVF),
                  get_fvf(FVF), FVF_to_ActualFVF(get_fvf(FVF)), FVF_to_ActualFVF(get_fvf(FVF)));
    std::cout << fvf;
    FVF = 0xf7;
    fvf =
      std::format("Dat_FVF: {} (0x{:x}). FVF: {} (0x{:x}). Actual_FVF: {} (0x{:x})\n", FVF, FVF, get_fvf(FVF),
                  get_fvf(FVF), FVF_to_ActualFVF(get_fvf(FVF)), FVF_to_ActualFVF(get_fvf(FVF)));

    std::cout << fvf;

    int id0 = 47710;
    int id1 = 0x101;
    int file_name = decode_filename(id0, id1);
    std::cout << "(" << id0 << "," << id1 << ")"
              << " -> " << file_name << "\n";

    for (int i = 0; i < 10; i++)
    {
        encode_filename1(id0, id1, file_name);
        int file_name1 = decode_filename(id0, id1);
        std::cout << "(" << std::hex << std::uppercase << id0 << "," << id1 << ")"
                  << " -> " << file_name1 << "\n";
        id1++;
    }
}
